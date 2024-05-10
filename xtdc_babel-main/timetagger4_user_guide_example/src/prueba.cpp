	// timetagger4_user_guide_example.cpp : Example application for the TimeTagger4
	#include "TimeTagger4_interface.h"
	#include "histogram.h"
	#include <stdio.h>
	#include <ctime>
	#include <string>
	#include <sstream>
	#include <stdlib.h>
	#include <stdexcept>
	#include <vector>
	#include <thread>
	#include <random>
	#include <cmath>
	#include <numeric>
	#include <fstream>
	#include <chrono>
	#include <windows.h>
	
	
	using namespace std;
	ofstream outfile;
	ofstream appendfile;
	//create the file to which the data will be written. outfile is for the continuous histogramand appendfile to store the data longterm
	stringstream ss;
	// If true the time tagger triggers a start periodically
	// The time difference of signals on channel A are measured
	// else start signal either from input or tiger is used (see below)
	// frequency of  start signal is printed and the hits are sampled
	const bool USE_CONTINUOUS_MODE = false;
	const bool USE_TIGER_START = true;	// if false, external signal must be provided on start; not applicable if continuous mode is enabled
	const bool USE_TIGER_STOPS = true; 	// if false please connect signals to some of channels A-D
	//?binsize is the resolution of the card given in picoseconds. Can be 100ps, 200ps... etc depending on the model
	//?Timestamps are given in integer multiples of the binsize and can then be converted to seconds, nanoseconds or whatever is useful
	//?groups are packets of data that arrive within a window of time. This is given by config.channel[i].start and
	//? config.channel[i].end. In continuous mode there is no end or it is set to a max value. 

	timetagger4_device * initialize_timetagger(int buffer_size, int board_id, int card_index) {
		// prepare initialization
		timetagger4_init_parameters params;

		timetagger4_get_default_init_parameters(&params);
		params.buffer_size[0] = buffer_size;				// size of the packet buffer
		params.board_id = board_id;							// value copied to "card" field of every packet, allowed range 0..255
		params.card_index = card_index;						// which of the TimeTagger4 board found in the system to be used

		int error_code;
		const char * err_message;
		timetagger4_device * device = timetagger4_init(&params, &error_code, &err_message);
		if (error_code != CRONO_OK) {
			printf("Could not init TimeTagger4 compatible board: %s\n", err_message);
			return nullptr;
		}
		timetagger4_static_info static_info;
		timetagger4_get_static_info(device, &static_info);
		bool timeTaggerNG = static_info.board_revision >= 7;
		if (USE_CONTINUOUS_MODE && !timeTaggerNG) {
			printf("Cannot use continuous mode with TimeTagger 1G/2G: %s\n", err_message);
			timetagger4_close(device);
			return nullptr;
		}

		return device;
	}

	int configure_timetagger(timetagger4_device * device,int freq, int t_end) {
		// prepare configuration
		timetagger4_static_info static_info;
		timetagger4_get_static_info(device, &static_info);
		timetagger4_configuration config;
		// fill configuration data structure with default values
		// so that the configuration is valid and only parameters
		// of interest have to be set explicitly
		timetagger4_get_default_configuration(device, &config);

		// set config of the 4 TDC channels
		for (int i = 0; i < TIMETAGGER4_TDC_CHANNEL_COUNT; i++)
		{
			// enable recording hits on TDC channel
			config.channel[i].enabled = true;

			// define range of the group
			config.channel[i].start = 0;	// range begins right after start pulse
			if (!USE_CONTINUOUS_MODE) {
				config.channel[i].stop = t_end;	// recording window stops after ~2.5 us
			//(original config.channel[i].stop = 30000;)		
			}
			else {
				config.channel[i].stop = 0x7fffffff; // trigger is independent of stops
				// set to maximal value
			}

			// measure only rising edge for tiger (positive) pulse or falling for user (negative) pulse
			config.trigger[TIMETAGGER4_TRIGGER_A + i].falling = USE_TIGER_STOPS ? 0 : 1;
			config.trigger[TIMETAGGER4_TRIGGER_A + i].rising = USE_TIGER_STOPS ? 1 : 0;
		}

		// generate an internal 25 kHz trigger, used for tiger and continuous mode
		config.auto_trigger_period = (int)(static_info.auto_trigger_ref_clock / freq);
		config.auto_trigger_random_exponent = 0;

		// setup TiGeR
		// sending a signal to the LEMO outputs (and to the TDC on the same channel)
		// requires proper 50 Ohm termination on the LEMO output to work reliably
		
		// width of the 12ns pulse in the auto_trigger clock periods
		//int pulse_width = (int) (12e-9 * static_info.auto_trigger_ref_clock);
		int pulse_width = (int) (5e-6 * static_info.auto_trigger_ref_clock);

		if (!USE_CONTINUOUS_MODE) {
			// use 200 kHz auto trigger to generate

			// generate above configured auto trigger to generate a 
			// signal with 12 ns pulse width on LEMO output Start
			config.tiger_block[0].enable = USE_TIGER_START ? 1 : 0;
			config.tiger_block[0].start = 0;
			config.tiger_block[0].stop = config.tiger_block[0].start + pulse_width;
			config.tiger_block[0].negate = 0;
			config.tiger_block[0].retrigger = 0;
			config.tiger_block[0].extend = 0;
			config.tiger_block[0].enable_lemo_output = 1;
			config.tiger_block[0].sources = TIMETAGGER4_TRIGGER_SOURCE_AUTO;
			// if TiGeR is used for triggering with positive pulses
			if (USE_TIGER_START)
				config.dc_offset[0] = TIMETAGGER4_DC_OFFSET_P_LVCMOS_18;
			else // user input expect NIM signal
				config.dc_offset[0] = TIMETAGGER4_DC_OFFSET_N_NIM;

			// start group on falling edges on the start channel 0
			config.trigger[TIMETAGGER4_TRIGGER_S].falling = USE_TIGER_START ? 0 : 1;
			config.trigger[TIMETAGGER4_TRIGGER_S].rising = USE_TIGER_START ? 1 : 0;
		} else {
			// Auto trigger is used as a start signal
			config.tdc_mode = TIMETAGGER4_TDC_MODE_CONTINUOUS;
		}

		for (int i = 1; i < TDC4_TIGER_COUNT; i++) {
			if(i==1){
				config.tiger_block[i].enable = USE_TIGER_STOPS ? 1 : 0;
				config.tiger_block[i].start =  0; //!just changed from i*100
				config.tiger_block[i].stop = config.tiger_block[i].start + pulse_width;
				config.tiger_block[i].negate = 0;
				config.tiger_block[i].retrigger = 0;
				config.tiger_block[i].extend = 0;
				config.tiger_block[i].enable_lemo_output = USE_TIGER_STOPS ? 1 : 0;
				config.tiger_block[i].sources = TIMETAGGER4_TRIGGER_SOURCE_S;
			}
			else{
				config.tiger_block[i].enable =  0;
				config.tiger_block[i].start =  100; //!just changed from i*100
				config.tiger_block[i].stop = config.tiger_block[i].start + pulse_width;
				config.tiger_block[i].negate = 0;
				config.tiger_block[i].retrigger = 0;
				config.tiger_block[i].extend = 0;
				config.tiger_block[i].enable_lemo_output = USE_TIGER_STOPS ? 1 : 0;
				config.tiger_block[i].sources = TIMETAGGER4_TRIGGER_SOURCE_S;
			}

			if (USE_TIGER_STOPS)
				config.dc_offset[i] = TIMETAGGER4_DC_OFFSET_P_LVCMOS_18;
			else // user input expect NIM signal
				config.dc_offset[i] = TIMETAGGER4_DC_OFFSET_N_NIM;		

			// this is not related to the tigers, but uses the same indexing (0 is start)
			// optionally increase input delay by 10 * 200 ps for each channel on new TT		
			// config.delay_config[i].delay = i * 10;
		}

		// write configuration to board
		return timetagger4_configure(device, &config);
	}


	void print_device_information(timetagger4_device * device, timetagger4_static_info * si, timetagger4_param_info * pi) {
		// print board information
		printf("Board Serial        : %d.%d\n", si->board_serial >> 24, si->board_serial & 0xffffff);
		printf("Board Configuration : %s\n", timetagger4_get_device_name(device));
		printf("Board Revision      : %d\n", si->board_revision);
		printf("Firmware Revision   : %d.%d\n", si->firmware_revision, si->subversion_revision);
		printf("Driver Revision     : %d.%d.%d\n", ((si->driver_revision >> 16) & 255), ((si->driver_revision >> 8) & 255), (si->driver_revision & 255));
		printf("Driver SVN Revision : %d\n", si->driver_build_revision);
		printf("\nTDC binsize         : %0.2f ps\n", pi->binsize);
	}

	double last_abs_ts_on_a = 0; //? TS last hit on channel A
	int64_t last_group_abs_time = 0;

	int64_t processPacket(volatile crono_packet *p, bool print, timetagger4_static_info *si, timetagger4_param_info *pi) {
		// do something with the data, e.g. calculate current rate
		int64_t group_abs_time = p->timestamp; // ? p points to the first hit of the group. This line extracts timestamp of first hit of current data package.
		appendfile << "New Group from process packet!!!"<<"\n";
		if (!USE_CONTINUOUS_MODE) {
			// group timestamp increments at binsize, but we see only a fraction of the packets (every update_count)
			double rate = 1e12 / ((double)(group_abs_time - last_group_abs_time) * pi->packet_binsize);
			//? this is the rate for the last group. 1e12*pi->packet_binsize is because the binsize gives a integer of the 
			//? resolution of the card, and it is given in picoseconds, thus 1e12 . the substraction is the difference 
			//?betweeen timestamp of first hit from last group and timestamp from first hit of new group. 
			if (print && last_group_abs_time > 0) {
				printf("\r%.6f kHz", rate / 1000.0);
				printf("Packet size: %d\n",  pi->packet_binsize); //! This is just for checking. Delete after using and record value. 
				// ...or print hits (not a good idea at high data rates,
				printf("Card %d - flags %d - length %d - type %d - TS %llu\n", p->card, p->flags, p->length, p->type, p->timestamp);
			}
			last_group_abs_time = group_abs_time;
		}

		int hit_count = 2 * (p->length);
		// Two hits fit into every 64 bit word. The second in the last word might be empty
		// This flag  tells us, whether the number of hits in the packet is odd
		if ((p->flags & TIMETAGGER4_PACKET_FLAG_ODD_HITS) != 0)
			hit_count -= 1;

		uint32_t* packet_data = (uint32_t*)(p->data);
		
		uint32_t rollover_count = 0; //? times the counter has overflown
		// 
		uint64_t rollover_period_bins = si->rollover_period;
		appendfile<<"rollover_period "<<rollover_period_bins<<"Rollover count " << rollover_count <<std::endl;
		appendfile<<"hitcount "<< hit_count << "Packet_binsize " << pi->packet_binsize << std::endl;
		appendfile <<"Rate " << 1e12 / ((double)(group_abs_time - last_group_abs_time) * pi->packet_binsize )<< std::endl;
		double full_ts_us=0; //!just testing
		for (int i = 0; i < hit_count; i++)
		{
			uint32_t hit = packet_data[i];
			uint32_t channel = hit & 0xf;
			// extract hit flags
			uint32_t flags = hit >> 4 & 0xf;


			if ((flags & TIMETAGGER4_HIT_FLAG_TIME_OVERFLOW) != 0) {  //? bitwise AND operation
			/*//? example:   Binary Representation of flags:              01011010
  			  //? Binary Representation of TIMETAGGER4_HIT_FLAG_TIME_OVERFLOW:  00000010
            //? Result of flags & TIMETAGGER4_HIT_FLAG_TIME_OVERFLOW:         00000010
 			//!Check if accurate.*/
				// this is a overflow of the 23/24 bit counter)
				rollover_count++;
			}
			else {
				// extract channel number (A-D)
				char channel_letter = 65 + channel;

				// extract hit timestamp 
				uint32_t ts_offset = hit >> 8 & 0xffffff; //? >> is bitwise shift operation. >> 8 shifts 8 bits to the right
				//? bits representing the time stamp start at 8 bits in hit.
				// Convert timestamp to ns, this is relative to the start of the group
				double ts_offset_us = (ts_offset + rollover_count * rollover_period_bins) * pi->binsize / 1000000.0;
				full_ts_us=ts_offset_us+group_abs_time*pi->binsize/1000000; //!just testing 24-07-2023
				//! just added	
				outfile << ("%u", full_ts_us) ;
				appendfile << ("%u", ts_offset_us) ;
				appendfile << "\n";
				outfile << "\n";
				//! just added		
				if (USE_CONTINUOUS_MODE) {
					if (channel == 0)
					{
						// compute the absolute time by adding the group time in ns
						double abs_ts_on_a = (group_abs_time * pi->packet_binsize) / 1000 + ts_offset_us;
						double diff = abs_ts_on_a - last_abs_ts_on_a;
						if (last_abs_ts_on_a > 0 && print) {
							printf("Time difference between hits on A  %.1f ns\n", diff);
						}

						last_abs_ts_on_a = abs_ts_on_a;
					}
				}
				
				else {
					if (print)
						printf("Hit  on channel %c -hit %u - flags %u -real Ts %d - offset %u (raw) / %.4f us\n", channel_letter, hit, flags,full_ts_us, ts_offset, ts_offset_us);
				}

			}
		}
		
		return group_abs_time;
	}


	void printLoadingBar(int current, int total, int barWidth = 70)
	{
		float progress = static_cast<float>(current) / total;
		int width = static_cast<int>(barWidth * progress);

		std::cout << "[";

		for (int i = 0; i < barWidth; ++i)
		{
			if (i < width) std::cout << "=";
			else std::cout << " ";
		}

		std::cout << "] " << int(progress * 100.0) << "%\r";
		std::cout.flush();
	}
	

	
	
// This code creates a histogram using SFML. The histogram data is defined in the `data` vector. The code creates a rectangle for each data point and positions it on the window. The x and y axes are also created, along with axis titles and a graph title. The code also includes a legend with a rectangle representing the data series and text describing the data series. The legend is positioned in the top right corner of the window. The code uses the Roboto-Regular.ttf font file for text rendering, which should be placed in the same directory as the code. You can adjust the font size, colors, and other parameters to customize the appearance of the histogram.

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Histogram");
    std::mt19937 generator;  // Mersenne Twister random number generator
    std::normal_distribution<> distribution(50, 2); // Uniform distribution between 0 and 100
    // Define histogram data
    std::vector<double> data = { 50,40 };


    std::ofstream file("data-dist-normal.txt");
    std::ofstream time_vs_size("parallel-time.txt");
    int i = 0;
    sf::Font font;
    font.loadFromFile("Roboto-Regular.ttf");
    HistogramGraphics histogram_setup = initializeHistogramGraphics(window, data, 80,font);
    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        sf::Event event;    
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (i == 10)
        {   
            for(int j=0;j<10;j++)
            {data.push_back(distribution(generator));}
              

            histogram_setup.updateValues(data,false);
            histogram_setup.updateHistogramBars(data, window);
            histogram_setup.updateLegend();
            histogram_setup.updateXTicks();
            histogram_setup.updateYTicks();
            
            //std::cout << i << std::endl;
            
            
            auto end = std::chrono::high_resolution_clock::now();  // End timer
            std::chrono::duration<double> elapsed = end - start;
            time_vs_size << elapsed.count() << "\t" << histogram_setup.HistoricalData.size() << "\n";
            
            
            i = 0;
            data.clear();
  

        }
        i++;
        window.clear(sf::Color::White);
        histogram_setup.draw(window);
        window.display();
        
    }
    for (const auto& data : histogram_setup.HistoricalData)
    {
        file << data << std::endl;
    }
    time_vs_size.close();
    file.close();

    return 0;
}

