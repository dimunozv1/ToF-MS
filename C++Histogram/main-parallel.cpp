
#include "histogram.h"
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>
#include <fstream>
#include <thread>

// This code creates a histogram using SFML. The histogram data is defined in the `data` vector. The code creates a rectangle for each data point and positions it on the window. The x and y axes are also created, along with axis titles and a graph title. The code also includes a legend with a rectangle representing the data series and text describing the data series. The legend is positioned in the top right corner of the window. The code uses the Roboto-Regular.ttf font file for text rendering, which should be placed in the same directory as the code. You can adjust the font size, colors, and other parameters to customize the appearance of the histogram.

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Histogram");
    std::mt19937 generator;  // Mersenne Twister random number generator
    std::normal_distribution<> distribution(50, 2); // Uniform distribution between 0 and 100

    // Define histogram data
    std::vector<double> data={50}; 
    

    std::ofstream file("data-dist-normal.txt");
    std::ofstream time_vs_size("parallel-time.txt");
    int i = 0;
    HistogramGraphics histogram_setup = initializeHistogramGraphics(window,data,80);
    while (window.isOpen())
    {   auto start = std::chrono::high_resolution_clock::now();
        sf::Event event;
        //std::cout <<histogram_setup.WinSizex << std::endl;
        //std::cout <<histogram_setup.WinSizey << std::endl;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if(i==10)
        {

            for (int j = 0; j < 1; j++)// Generate random number
                {data.push_back(distribution(generator));}
                      
            histogram_setup.updateValues(data);
            std::thread updateHistogramBarsThread(&HistogramGraphics::updateHistogramBars, &histogram_setup, std::ref(data), std::ref(window));
            std::thread updateLegendThread(&HistogramGraphics::updateLegend, &histogram_setup);
            std::thread updateXTicksThread(&HistogramGraphics::updateXTicks, &histogram_setup);
            std::thread updateYTicksThread(&HistogramGraphics::updateYTicks, &histogram_setup);
            updateHistogramBarsThread.join();
            updateLegendThread.join();
            updateXTicksThread.join();
            updateYTicksThread.join();
            
            i=0;
            auto end = std::chrono::high_resolution_clock::now();  // End timer
            std::chrono::duration<double> elapsed = end - start;
            time_vs_size << elapsed.count() << "\t" << histogram_setup.HistoricalData.size() << "\n";
            std::vector<double> data(10, 0.0);
        
        }
        window.clear(sf::Color::White);
        histogram_setup.draw(window);
        i++;
    }
    
    for(const auto& data : histogram_setup.HistoricalData) 
        {file << data << std::endl;}

    file.close();
    std::cout <<histogram_setup.MaxFreq << std::endl;

    return 0;
}

