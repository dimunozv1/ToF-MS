#include "timetagger4_interface.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>

#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>

//keeping the cosole from closing directly after program run
using namespace std;

/*void wait()
{
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();
}*/
// end of console closing modification

typedef unsigned int uint32;
//typedef unsigned __int64 uint64;
typedef unsigned long long uint64;

//int main(int argc, char* argv[])
int main()
{
	printf("Program started\n");
	printf("cronologic timetagger4_user_guide_example using driver: %s\n", timetagger4_get_driver_revision_str());

	// prepare initialization
	timetagger4_init_parameters params;

	timetagger4_get_default_init_parameters(&params);
	params.buffer_size[0] = 8 * 1024 * 1024; // use 8 MByte as packet buffer
	params.board_id = 0; // value copied to "card" field of every packet, allowed range 0..255
	params.card_index = 0; // initialize first TimeTagger4 board found in the system

	// initialize card
	/*int error_code;
	const char* err_message;
	timetagger4_device* device = timetagger4_init(&params, &error_code, &err_message);
	*/
	/*
	if (error_code != CRONO_OK)
	{
		printf("Could not init TimeTagger4 compatible board: %s\n", err_message);
		//cout << error_code << endl;
		Sleep(10000);
		return 1;
	}

	// prepare configuration
	timetagger4_configuration config;

	// fill configuration data structure with default values
	// so that the configuration is valid and only parameters
	// of interest have to be set explicitly
	timetagger4_get_default_configuration(device, &config);

	// set config of the 4 TDC channels
	for (int i = 0; i < TIMETAGGER4_TDC_CHANNEL_COUNT; i++)
	{
		// enable recording hits on channel
		config.channel[i].enabled = true;

		// define range of the group
		config.channel[i].start = 0; // range begins right after start pulse
		config.channel[i].stop = 50000; // recording window stops after~20 us
		//(original config.channel[i].stop = 30000;)

		// measure only falling edge
		config.trigger[i + 1].falling = 1;
		config.trigger[i + 1].rising = 1;
	}

	// start group on falling edges on the start channel (external Trigger? ? ? )
	config.trigger[0].falling = 1; // enable packet generation on falling edge of start pulse
	config.trigger[0].rising = 0; // disable packet generation on rising edge of start pulse

	// generate an internal 200 kHz trigger
	config.auto_trigger_period = 1250;
	config.auto_trigger_random_exponent = 0;

	// setup TiGeR
	// sending a signal to the LEMO outputs (and to the TDC on the same channel)
	// requires proper 50 Ohm termination on the LEMO output to work reliably

	// use 200 kHz auto trigger to generate
	// a 200 kHz signal with 12 ns pulse width on LEMO output Start
	config.tiger_block[0].enable = 1;
	config.tiger_block[0].start = 0;
	config.tiger_block[0].stop = config.tiger_block[0].start + 2;
	config.tiger_block[0].negate = 0;
	config.tiger_block[0].retrigger = 0;
	config.tiger_block[0].extend = 0;
	config.tiger_block[0].enable_lemo_output = 1;
	config.tiger_block[0].sources = TIMETAGGER4_TRIGGER_SOURCE_AUTO;

	// if TiGeR is used for triggering with positive pulses
	if (config.tiger_block[0].enable == 1)
		config.dc_offset[0] = TIMETAGGER4_DC_OFFSET_P_LVCMOS_18;
	else
		config.dc_offset[0] = TIMETAGGER4_DC_OFFSET_N_NIM;


	// write configuration to board
	int status = timetagger4_configure(device, &config);
	if (status != CRONO_OK)
	{
		printf("Could not configure TimeTagger4: %s\n", err_message);
		return error_code;
	}

	// print board information
	timetagger4_static_info staticinfo;
	timetagger4_get_static_info(device, &staticinfo);
	printf("Board Serial : %d.%d\n", staticinfo.board_serial >> 24,
		staticinfo.board_serial & 0xffffff);
	printf("Board Configuration : TimeTagger4-%iG\n", staticinfo.chip_id);
	printf("Board Revision : %d\n", staticinfo.board_revision);
	printf("Firmware Revision : %d.%d\n", staticinfo.firmware_revision, staticinfo.subversion_revision);
	printf("Driver Revision : %d.%d.%d.%d\n", ((staticinfo.driver_revision >> 24) & 255), ((staticinfo.driver_revision >> 16) & 255), ((staticinfo.driver_revision >> 8) & 255), (staticinfo.driver_revision & 255));

	timetagger4_param_info parinfo;
	timetagger4_get_param_info(device, &parinfo);
	printf("\nTDC binsize : %0.2f ps\n", parinfo.binsize);

	// configure readout behaviour
	timetagger4_read_in read_config;
	// automatically acknowledge all data as processed
	// on the next call to timetagger4_read()
	// old packet pointers are invalid after calling timetagger4_read()
	read_config.acknowledge_last_read = 1;

	// structure with packet pointers for read data
	timetagger4_read_out read_data;

	// start data capture
	status = timetagger4_start_capture(device);
	if (status != CRONO_OK) {
		printf("Could not start capturing %s", timetagger4_get_last_error_message(device));
		timetagger4_close(device);
		return status;
	}

	// start timing generator
	timetagger4_start_tiger(device);

	// some book keeping
	int packet_count = 0;
	// int empty_packets = 0; //Those are not used at the moment, so to focus on the real errors I commented them out
	// int packets_with_errors = 0;
	// bool last_read_no_data = false;


	//_int64 GroupAbsTime = 0;
	//_int64 GroupAbsTime_old = 0;
	long long GroupAbsTime = 0;
	long long GroupAbsTime_old = 0;
	// 1=every single laser pulse is measured, 10=every 10th laser pulse is measured
	int UpdateCount = 1;
	//After how many read packages will the program create a new .txt. file to put the data in
	int RenewTime = 200;
	//double binsize = get_binsize(device);

	//definition of variable to get current unix time in millisecond
	unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//create and open the file to which the data will be written. outfile is for the continuous histogramand appendfile to store the data longterm
	ofstream outfile;
	ofstream appendfile;
	outfile.open("C:\\Users\\Administrator\\Documents\\Visual\ Studio\ 2019\\zhiyuan\\time_tagger\\test\\" + std::to_string(now) + ".txt");
	appendfile.open("C:\\Users\\Administrator\\Documents\\Visual\ Studio\ 2019\\zhiyuan\\time_tagger\\test\\RawTOFDataAppend\\" + std::to_string(now) + ".txt");
	stringstream ss;



	printf("Reading packets:\n");
	// read 10000 packets
	//Will later on be changed to a continuusly running loop
	while (packet_count < 10000)
	{
		// get pointers to acquired packets
		status = timetagger4_read(device, &read_config, &read_data);
		if (status != CRONO_OK)
		{
			Sleep(100);
			printf(" - No data! -\n");
			//break;
		}
		else
		{
			// iterate over all packets received with the last read
			volatile crono_packet* p = read_data.first_packet;
			while (p <= read_data.last_packet)
			{

				// Do something with the data e.g. print hits (not a good idea at high data rates,
				// so let�s do that only for every 1000th packet...)
				// print packet information
				if (packet_count % UpdateCount == 0) {
					//Things that could be printed:
					//printf("Card %d - Flags %d - Length %d - Type %d - TS %llu\n", p->card, p->flags, p->length, p->type, p->timestamp);

					int hit_count = 2 * (p->length);  //DIFF
					if ((p->flags & 0x1) == 1)
						hit_count -= 1;
					uint32* packet_data = (uint32*)(p->data);
					appendfile << "[";
					for (int i = 0; i < hit_count; i++)
					{
						uint32* hit = (packet_data + i);


						// extract hit timestamp
						int ts_offset = (*hit >> 8 & 0xffffff); //DIFF

						// TDC bin size is 500 ps. Convert timestamp to ns.
						double ts_offset_ns = ts_offset;  //DIFF
						ts_offset_ns *= parinfo.binsize / 1000.0; //DIFF
						outfile << ("%f", ts_offset_ns) << ",";
						appendfile << ("%f", ts_offset_ns) << ", ";
					}
					appendfile << "none]\n";

					//start writing the data to stream in a new file with a new name, so python knows which file
					//belongs to which time and there won�t be problems with python and C++ writing / reading in the same file
					if (packet_count % RenewTime == 0) {
						outfile.close();
						//gets the current Unix time in ms
						now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						std::cout << now << std::endl;
						outfile.open(".\\TOFDatafiles\\" + std::to_string(now) + ".txt");
					}

				}
				// go to next packet

				p = crono_next_packet(p);
				packet_count++;
			}
		}
	}
	//close the file to which the data is written
	appendfile.close();
	outfile.close();

	// shut down packet generation and DMA transfers
	timetagger4_stop_capture(device);

	// deactivate timetagger4
	timetagger4_close(device);

	printf("press Enter to close program");
	wait(); // Waiting for Enter */
	return 0;

}
