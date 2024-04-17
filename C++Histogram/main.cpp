#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <thread>

// This code creates a histogram using SFML. The histogram data is defined in the `data` vector. The code creates a rectangle for each data point and positions it on the window. The x and y axes are also created, along with axis titles and a graph title. The code also includes a legend with a rectangle representing the data series and text describing the data series. The legend is positioned in the top right corner of the window. The code uses the Roboto-Regular.ttf font file for text rendering, which should be placed in the same directory as the code. You can adjust the font size, colors, and other parameters to customize the appearance of the histogram.

struct HistogramGraphics{
    float axisThickness = 1.0f;  // Adjust this to change the thickness of the axes
    float padding = 120.0f;  // Space around the edges of the window
    //legend size
    float legend_size_x = 400;
    float legend_size_y = 25;
    //legend position
    float legendX = 745;
    float legendY = 125;
    float maxBarHeight; // Maximum height of the bars in the histogram in pixels
    float maxPlotWidth; // Maximum width of the plot area in pixels
    float binWidthnumValue; //Numerical value of the bin width, not pixel value
    float bin_number; // Number of bins in the histogram
    float scaleFactor; // Scale factor for the histogram bars
    float MaxFreq; //value of the maximum frequency
    float MaxDataValue; //Max value of the original data
    float MinDataValue; //Min value of the original data
    std::vector<sf::RectangleShape> Histogram_bars;

    
    //Font 
    std::string font_address = "Roboto-Regular.ttf";
    sf::Text xLabel;
    sf::Text yLabel;
    //x and y axes
    sf::RectangleShape xAxis;
    sf::RectangleShape yAxis; 

    //plot title
    sf::Text title;

    //legend
    sf::RectangleShape legendItem;
    sf::RectangleShape legendDataRectangle;
    sf::Text legendText;

    //rectangle that contains everything
    sf::RectangleShape square;

    //Ticks and ticks labels

    std::vector<sf::RectangleShape> xTicks;
    std::vector<sf::Text> xTickLabels;
    std::vector<sf::RectangleShape> yTicks;
    std::vector<sf::Text> yTickLabels;


        // Load font
    sf::Font font;
    int loadFont(){
    if (!font.loadFromFile(font_address))  // Replace with the path to your font file
    {
        std::cerr << "Failed to load font from file" << std::endl;
        return -1;
    }
    else{
        font.loadFromFile(font_address);
        return 0;
    }
    }
    void updateHistogramBars(std::vector<double> data,sf::RenderWindow& window);
    void updateLegend(std::vector<double> data);
    void drawAxisTicks(sf::RenderWindow& window);
    void updateXTicks(sf::RenderWindow& window);
    void updateYTicks(sf::RenderWindow& window);
    std::vector<double> frequencies(std::vector<double> data);
};

HistogramGraphics initializeHistogramGraphics(sf::RenderWindow& window, std::vector<double> data);
std::vector<sf::RectangleShape> createBars(const std::vector<double>& freq, double width, double padding, double scaleFactor);
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Histogram");
    std::mt19937 generator;  // Mersenne Twister random number generator
    std::normal_distribution<> distribution(50, 2); // Uniform distribution between 0 and 100

    // Define histogram data
    std::vector<double> data = {50}; 
    

    std::ofstream file("data-dist-normal.txt");
    int i = 0;
    HistogramGraphics histogram_setup = initializeHistogramGraphics(window,data);
    while (window.isOpen())
    {   auto start = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(i==10){
            
            
            for (int i = 0; i < 100; i++) {
                //int value = distribution(generator);  // Generate random number
                data.push_back(distribution(generator));  // Generate random number
                //std::cout<<"Value generated " << value <<std::endl;
                file << data.back() << std::endl;  // Write data to file
            }
            histogram_setup.updateHistogramBars(data,window);
            histogram_setup.updateLegend(data);
            histogram_setup.updateXTicks(window);
            histogram_setup.updateYTicks(window);
            
            i=0;
            auto end = std::chrono::high_resolution_clock::now();  // End timer
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Generated in " << elapsed.count() << " seconds.\n";
        }
        window.clear(sf::Color::White);
        for (const auto& bar : histogram_setup.Histogram_bars)
            window.draw(bar);
        
        histogram_setup.drawAxisTicks(window);
        window.draw(histogram_setup.square);
        window.draw(histogram_setup.xAxis);
        window.draw(histogram_setup.yAxis);
        window.draw(histogram_setup.xLabel);
        window.draw(histogram_setup.yLabel);
        window.draw(histogram_setup.title);
        window.draw(histogram_setup.legendItem);
        window.draw(histogram_setup.legendText);
        window.draw(histogram_setup.legendDataRectangle);
        window.display();
        i++;
    }

    file.close();

    return 0;
}

std::vector<double> HistogramGraphics::frequencies(std::vector<double> data)
{       
    bin_number = 80;//std::sqrt(data.size());

    std::vector<double> freq(bin_number, 0.0);
    //{55, 10, 55, 10, 15, 20, 15,10,10}
    MinDataValue = *std::min_element(data.begin(), data.end());
    //std::cout<<"MinDataValue = "<<MinDataValue<<std::endl;
    MaxDataValue = *std::max_element(data.begin(), data.end());
    binWidthnumValue = (MaxDataValue - MinDataValue) / bin_number;
    //std::cout<<"Bin Width = "<<binWidth<< " bin number = "<< bin_number<< std::endl;
    for (float value : data) {
        int binIndex;
        if((value - MinDataValue)/bin_number == binWidthnumValue)
        {
            binIndex = bin_number-1;
        }
        else
        {
            binIndex = (value - MinDataValue) / binWidthnumValue;
        }
        //std::cout<<"Value = "<<value<< " binIndex = "<< binIndex<< std::endl;
        //std::cout << "Bin Index = " << binIndex << std::endl;
        freq[binIndex]+=1.0;
    }
    
    //rescale the frequencies
   
    return freq;
}



void HistogramGraphics::updateHistogramBars(std::vector<double> data,sf::RenderWindow& window){
    
    std::vector<double> freq = frequencies(data);
    std::vector<double> scaledFreq = freq;
    MaxFreq = *std::max_element(freq.begin(), freq.end());
    scaleFactor = maxBarHeight / MaxFreq;
    float width = maxPlotWidth / static_cast<float>(freq.size());
    //std::cout<<"MaxFreq = "<<MaxFreq<< " scaleFactor = "<< scaleFactor<< " width = "<< width<< std::endl;
    //std::cout<< "Freq Size = " <<freq.size()<<std::endl;
    Histogram_bars = std::vector<sf::RectangleShape>(freq.size());
    for (size_t i = 0; i < freq.size(); ++i)
    {   
        Histogram_bars[i].setSize(sf::Vector2f(width - 1, -freq[i]*scaleFactor));        
        Histogram_bars[i].setPosition(padding+i * width, window.getSize().y-padding);
        Histogram_bars[i].setFillColor(sf::Color(51,153,255));
        Histogram_bars.push_back(Histogram_bars[i]);
        
        
    }

}

void HistogramGraphics::updateLegend(std::vector<double> data){
    // Create legend text
    legendText = sf::Text("Total Counts = "+std::to_string(data.size())+"; t = 20s ; Trial 1; binsize 0.491us", font);  // Replace with the name of the data series
    legendText.setCharacterSize(15);  // Adjust size as needed
    legendText.setFillColor(sf::Color::Black);
    legendText.setPosition(legendX+33 , legendY+2);  // Position the text to the right of the data rectangle
}
void HistogramGraphics::updateXTicks(sf::RenderWindow& window){
    double xNumberofTicks = bin_number;  // Set the number of ticks on the x-axis
    
    if(xNumberofTicks>13)   
    {
        xNumberofTicks = 13;
    }
    if(bin_number <= 0)
    {
        xNumberofTicks = 0;
    }
    double xTickSpacing = maxPlotWidth / xNumberofTicks;
    double InterTickSpaceInBins = bin_number/xNumberofTicks;
    xTicks.clear();
    xTickLabels.clear();
    for (int i = 0; i < xNumberofTicks; ++i) {
        std::stringstream stream;
        stream << std::scientific  << std::setprecision(1) << static_cast<float>(MinDataValue+(binWidthnumValue)/2 + i*binWidthnumValue*InterTickSpaceInBins);
        std::string xtickLabel = stream.str();
        sf::Text tickLabel(xtickLabel, font);  // Create a text label with the value of the tick
        sf::RectangleShape tick(sf::Vector2f(4, 5));  // Create a tick with width 4 and height 5
        tick.setPosition(padding + i*xTickSpacing, window.getSize().y - padding);  // Position the tick on the x-axis
        tickLabel.setPosition(padding + i*xTickSpacing-10, window.getSize().y - padding+5);  // Position the tick label below the tick
        tick.setFillColor(sf::Color::Black);  // Set the fill color to black
        tickLabel.setCharacterSize(12);  // Adjust size as needed
        tickLabel.setFillColor(sf::Color::Black);
        xTicks.push_back(tick);
        xTickLabels.push_back(tickLabel);
    }
}

void HistogramGraphics::updateYTicks(sf::RenderWindow& window) {

    double yNumberOfTicks = 10;  // Set the number of ticks on the y-axis
    double yTickSpacing = maxBarHeight / yNumberOfTicks;  // Calculate the spacing between ticks

    yTicks.clear();
    yTickLabels.clear();

    for (int i = yTicks.size(); i < yNumberOfTicks; ++i) {
        std::stringstream stream;
        stream << std::scientific  << std::setprecision(1) << static_cast<float>(i*MaxFreq/yNumberOfTicks);
        //std::cout<<"MaxFreq = "<<MaxFreq<< " yNumberOfTicks = "<< yNumberOfTicks<< " i = "<< i<< std::endl;
        std::string ytickLabel = stream.str();
        sf::Text tickLabel(ytickLabel, font);
        sf::RectangleShape tick(sf::Vector2f(5, 4));
        tick.setPosition(padding, window.getSize().y - padding - i*yTickSpacing);
        tickLabel.setPosition(padding - 50, window.getSize().y - padding - i*yTickSpacing - 10);
        tick.setFillColor(sf::Color::Black);
        tickLabel.setCharacterSize(12);
        tickLabel.setFillColor(sf::Color::Black);
        yTicks.push_back(tick);
        yTickLabels.push_back(tickLabel);
    }
}

void HistogramGraphics::drawAxisTicks(sf::RenderWindow& window) {
        for (auto& tick : xTicks) {
            window.draw(tick);
        }
        for (auto& label : xTickLabels) {
            window.draw(label);
        }
        for (auto& tick : yTicks) {
            window.draw(tick);
        }
        for (auto& label : yTickLabels) {
            window.draw(label);
        }
    }

HistogramGraphics initializeHistogramGraphics(sf::RenderWindow& window, std::vector<double> data) {
    HistogramGraphics graphics;
    graphics.loadFont();
    //Initialize x and y axes
    graphics.xAxis.setSize(sf::Vector2f(window.getSize().x - 2 * graphics.padding, graphics.axisThickness));
    graphics.xAxis.setPosition(graphics.padding, window.getSize().y - graphics.padding);
    graphics.xAxis.setFillColor(sf::Color::Black);

    graphics.yAxis.setSize(sf::Vector2f(graphics.axisThickness, window.getSize().y - 2 * graphics.padding));
    graphics.yAxis.setPosition(graphics.padding, graphics.padding);
    graphics.yAxis.setFillColor(sf::Color::Black);
    
    // Create x and y axis titles
    graphics.xLabel = sf::Text("Time of flight (us)", graphics.font);
    graphics.xLabel.setPosition((window.getSize().x -  2*graphics.padding) / 2-50, window.getSize().y - graphics.padding / 2);
    graphics.xLabel.setFillColor(sf::Color::Black);

    graphics.yLabel = sf::Text("Counts", graphics.font);
    graphics.yLabel.setPosition(graphics.padding / 2-graphics.axisThickness-50, window.getSize().y / 2+40);
    graphics.yLabel.setRotation(-90);
    graphics.yLabel.setFillColor(sf::Color::Black);
   
    // Create plot title
    graphics.title = sf::Text("Time of Flight", graphics.font);
    graphics.title.setPosition((window.getSize().x - 2 * graphics.padding)/2-50, graphics.padding / 2-20);
    graphics.title.setFillColor(sf::Color::Black);

    // Create legend item
    graphics.legendItem = sf::RectangleShape(sf::Vector2f(graphics.legend_size_x,graphics.legend_size_y));  // Adjust size as needed
    graphics.legendItem.setFillColor(sf::Color::White);  // Set to the color of the data series
    graphics.legendItem.setOutlineThickness(1);  // Set the outline thickness to 2 pixels
    graphics.legendItem.setOutlineColor(sf::Color(220,220,220));
    graphics.legendItem.setPosition(graphics.legendX, graphics.legendY);

    // Create legend data rectangle
    graphics.legendDataRectangle = sf::RectangleShape(sf::Vector2f(20, 10));  // Adjust size as needed
    graphics.legendDataRectangle.setFillColor(sf::Color(51,153,255));  // Set to the color of the data series
    graphics.legendDataRectangle.setOutlineThickness(1);  // Set the outline thickness to 2 pixels
    graphics.legendDataRectangle.setOutlineColor(sf::Color::Black);
    graphics.legendDataRectangle.setPosition(graphics.legendX+3, graphics.legendY+7);  // Position the data rectangle below the text
    
    

    // Create a square that contains everything
    graphics.square = sf::RectangleShape(sf::Vector2f(window.getSize().x - 2 * graphics.padding, window.getSize().y - 2 * graphics.padding));  // Set the size to the size of the window
    graphics.square.setFillColor(sf::Color::Transparent);  // Set the fill color to transparent
    graphics.square.setOutlineThickness(1);  // Set the outline thickness to 2 pixels
    graphics.square.setOutlineColor(sf::Color::Black);  // Set the outline color to white
    graphics.square.setPosition(graphics.padding, graphics.padding);  // Position the square at the top left corner of the window


    graphics.maxBarHeight = window.getSize().y - 2 * graphics.padding;  // Set the maximum bar height to the height of the window minus the padding
    graphics.maxPlotWidth = window.getSize().x - 2 * graphics.padding;  // Set the maximum plot width to the width of the window minus the padding
    // Create rectangles for histogram
    graphics.updateHistogramBars(data,window);
    graphics.updateLegend(data);
    graphics.updateXTicks(window);
    graphics.updateYTicks(window);
    graphics.drawAxisTicks(window);


    return graphics;
}