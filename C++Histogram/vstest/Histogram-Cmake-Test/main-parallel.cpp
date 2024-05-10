#include "histogram.h"
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>
#include <fstream>
#include <thread>


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