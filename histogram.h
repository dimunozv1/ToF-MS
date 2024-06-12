#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
struct HistogramGraphics
    {
        double axisThickness = 1.0f;  // Adjust this to change the thickness of the axes
        double padding = 120.0f;  // Space around the edges of the window
        //legend size
        double legend_size_x = 400;
        double legend_size_y = 25;
        //legend position
        double legendX = 745;
        double legendY = 125;
        double maxBarHeight; // Maximum height of the bars in the histogram in pixels
        double maxPlotWidth; // Maximum width of the plot area in pixels
        double binWidthnumValue; //Numerical value of the bin width, not pixel value
        double scaleFactor; // Scale factor for the histogram bars
        double MaxFreq; //value of the maximum frequency
        double MaxDataValue; //Max value of the original data
        double MinDataValue; //Min value of the original data
        double WinSizex; //Window size in x direction in pixels
        double WinSizey; //Window size in y direction in pixels
        double bin_number = 80; // Number of bins in the histogram
        std::vector<double> HistoricalData;
        std::vector<double> freq; // Vector to store the frequencies of the data
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
        int loadFont()
        {
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
        void updateLegend();
        void drawAxisTicks(sf::RenderWindow& window);
        void updateXTicks();
        void updateYTicks();
        void updateValues(std::vector<double> data,bool firstTime = false);
        void draw(sf::RenderWindow& window);
        void frequencies(std::vector<double> data);
    };



HistogramGraphics initializeHistogramGraphics(sf::RenderWindow& window, std::vector<double> data,int nbins = 80);


void HistogramGraphics::frequencies(std::vector<double> data)
{       
    int binIndex = 0;
    
    //std::cout<<"Bin Width = "<<binWidth<< " bin number = "<< bin_number<< std::endl;
    for (double value : data) {
        binIndex = 0;
        if((value - MinDataValue)/bin_number == binWidthnumValue)
        {
            binIndex = bin_number-1;
        }
        else
        {
            binIndex = (value - MinDataValue) / binWidthnumValue;
        }
        
        freq[binIndex]+=1.0;
    }

}

void HistogramGraphics::updateValues(std::vector<double> data,bool firstTime)
{   
    double newDataMinValue = *std::min_element(data.begin(), data.end());
    double newDataMaxValue = *std::max_element(data.begin(), data.end());
    //The first time this function is called, the min and max values are set to the first data set
    //After that, the min and max values are updated if the new data set has a smaller min or a larger max
    if(firstTime)
        {
            MinDataValue = newDataMinValue;
            MaxDataValue = newDataMaxValue;
        }
    else
        {
            if(newDataMinValue < MinDataValue)
                { MinDataValue = newDataMinValue;}


            if(newDataMaxValue > MaxDataValue)
                {MaxDataValue = newDataMaxValue;}
        }
    
    binWidthnumValue = (MaxDataValue - MinDataValue) / bin_number;
    HistoricalData.insert(HistoricalData.end(), data.begin(), data.end());
    
}


void HistogramGraphics::updateHistogramBars(std::vector<double> data,sf::RenderWindow& window)
{
    frequencies(data);
    MaxFreq = *std::max_element(freq.begin(), freq.end());
    scaleFactor = maxBarHeight / MaxFreq;
    double width = maxPlotWidth / static_cast<float>(freq.size());
    Histogram_bars = std::vector<sf::RectangleShape>(bin_number);
    for (size_t i = 0; i < freq.size(); ++i)
    {   
        Histogram_bars[i].setSize(sf::Vector2f(width - 1, -freq[i]*scaleFactor));        
        Histogram_bars[i].setPosition(padding+i * width, window.getSize().y-padding);
        Histogram_bars[i].setFillColor(sf::Color(51,153,255));
        Histogram_bars.push_back(Histogram_bars[i]);
        
        
    }

}

void HistogramGraphics::updateLegend(){
    // Create legend text
    legendText = sf::Text("Total Counts = "+std::to_string(HistoricalData.size())+"; t = 20s ; Trial 1; binsize 0.491us", font);  // Replace with the name of the data series
    legendText.setCharacterSize(15);  // Adjust size as needed
    legendText.setFillColor(sf::Color::Black);
    legendText.setPosition(legendX+33 , legendY+2);  // Position the text to the right of the data rectangle
}
void HistogramGraphics::updateXTicks(){
    double xNumberofTicks = bin_number;  // Set the number of ticks on the x-axis
    
    if(xNumberofTicks>13)   
        { xNumberofTicks = 13;}

    if(bin_number <= 0)
        {xNumberofTicks = 0;}

    double xTickSpacing = maxPlotWidth / xNumberofTicks;
    double InterTickSpaceInBins = bin_number/xNumberofTicks;
    xTicks.clear();
    xTickLabels.clear();

    for (int i = 0; i < xNumberofTicks; ++i) 
    {
        std::stringstream stream; 
        //Text label for the x-axis
        stream << std::scientific  << std::setprecision(1) << static_cast<float>(MinDataValue+(binWidthnumValue)/2 + i*binWidthnumValue*InterTickSpaceInBins);
        std::string xtickLabel = stream.str();
        sf::Text tickLabel(xtickLabel, font);  // Create a text label with the value of the tick
        //Geometry and position of the tick
        sf::RectangleShape tick(sf::Vector2f(4, 5));  // Create a tick with width 4 and height 5

        tick.setPosition(padding + i*xTickSpacing, WinSizey - padding);  // Position the tick on the x-axis
        tickLabel.setPosition(padding + i*xTickSpacing-10, WinSizey - padding+5);  // Position the tick label below the tick
        tick.setFillColor(sf::Color::Black);  // Set the fill color to black
        tickLabel.setCharacterSize(12);  // Adjust size as needed
        tickLabel.setFillColor(sf::Color::Black);
        xTicks.push_back(tick);
        xTickLabels.push_back(tickLabel);
    }
}

void HistogramGraphics::updateYTicks() {

    double yNumberOfTicks = 10;  // Set the number of ticks on the y-axis
    double yTickSpacing = maxBarHeight / yNumberOfTicks;  // Calculate the spacing between ticks

    yTicks.clear();
    yTickLabels.clear();

    for (int i = yTicks.size(); i < yNumberOfTicks; ++i) 
    {
        //Text label for the y-axis
        std::stringstream stream;
        stream << std::scientific  << std::setprecision(1) << static_cast<float>(i*MaxFreq/yNumberOfTicks);
        std::string ytickLabel = stream.str();
        sf::Text tickLabel(ytickLabel, font);
        //Geometry and position of the tick
        sf::RectangleShape tick(sf::Vector2f(5, 4));
        tick.setPosition(padding, WinSizey - padding - i*yTickSpacing);
        tickLabel.setPosition(padding - 50, WinSizey - padding - i*yTickSpacing - 10);
        tick.setFillColor(sf::Color::Black);
        tickLabel.setCharacterSize(12);
        tickLabel.setFillColor(sf::Color::Black);
        yTicks.push_back(tick);
        yTickLabels.push_back(tickLabel);
    }
}

void HistogramGraphics::draw(sf::RenderWindow& window) {
        //Draw all the necessary elements
        for (const auto& bar : Histogram_bars)
            window.draw(bar);
        xLabel.setFont(font);
        yLabel.setFont(font);
        title.setFont(font);
        legendText.setFont(font);
        drawAxisTicks(window);
        window.draw(square);
        window.draw(xAxis);
        window.draw(yAxis);
        window.draw(xLabel);
        window.draw(yLabel);
        window.draw(title);
        window.draw(legendItem);
        window.draw(legendText);
        window.draw(legendDataRectangle);
        
    }
void HistogramGraphics::drawAxisTicks(sf::RenderWindow& window) {
        for (auto& tick : xTicks) 
            {window.draw(tick);}

        for (auto& label : xTickLabels) 
        {
            label.setFont(font);
            window.draw(label);}

        for (auto& tick : yTicks) 
            {window.draw(tick); }
            
        for (auto& label : yTickLabels) 
            {
            label.setFont(font);
            window.draw(label);}
    }

HistogramGraphics initializeHistogramGraphics(sf::RenderWindow& window, std::vector<double> data,int nbins, sf::Font &font) {
    // Create an instance of the HistogramGraphics struct to allow the initialization of the histogram
    HistogramGraphics graphics;

    // Load font
    graphics.font = font;

    // Set the number of bins in the histogram
    graphics.bin_number = nbins;

    //Windows size
    graphics.WinSizex = window.getSize().x;
    graphics.WinSizey = window.getSize().y;
    
    //Initialize x and y axes
    graphics.xAxis.setSize(sf::Vector2f(window.getSize().x - 2 * graphics.padding, graphics.axisThickness));
    graphics.xAxis.setPosition(graphics.padding, window.getSize().y - graphics.padding);
    graphics.xAxis.setFillColor(sf::Color::Black);

    graphics.yAxis.setSize(sf::Vector2f(graphics.axisThickness, graphics.WinSizey - 2 * graphics.padding));
    graphics.yAxis.setPosition(graphics.padding, graphics.padding);
    graphics.yAxis.setFillColor(sf::Color::Black);
    
    // Create x and y axis titles
    graphics.xLabel = sf::Text("Time of flight (us)", graphics.font);
    graphics.xLabel.setPosition((graphics.WinSizex -  2*graphics.padding) / 2-50, graphics.WinSizey - graphics.padding / 2);
    graphics.xLabel.setFillColor(sf::Color::Black);

    graphics.yLabel = sf::Text("Counts", graphics.font);
    graphics.yLabel.setPosition(graphics.padding / 2-graphics.axisThickness-50, graphics.WinSizey / 2+40);
    graphics.yLabel.setRotation(-90);
    graphics.yLabel.setFillColor(sf::Color::Black);
   
    // Create plot title
    graphics.title = sf::Text("Time of Flight", graphics.font);
    graphics.title.setPosition((graphics.WinSizex - 2 * graphics.padding)/2-30, graphics.padding / 2-20);
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
    graphics.square = sf::RectangleShape(sf::Vector2f(graphics.WinSizex - 2 * graphics.padding, graphics.WinSizey - 2 * graphics.padding));  // Set the size to the size of the window
    graphics.square.setFillColor(sf::Color::Transparent);  // Set the fill color to transparent
    graphics.square.setOutlineThickness(1);  // Set the outline thickness to 2 pixels
    graphics.square.setOutlineColor(sf::Color::Black);  // Set the outline color to white
    graphics.square.setPosition(graphics.padding, graphics.padding);  // Position the square at the top left corner of the window

    // Set the maximum bar height and plot width
    graphics.maxBarHeight = graphics.WinSizey - 2 * graphics.padding;  // Set the maximum bar height to the height of the window minus the padding
    graphics.maxPlotWidth = graphics.WinSizex - 2 * graphics.padding;  // Set the maximum plot width to the width of the window minus the padding
    
    //Vector for frequencies
    graphics.freq = std::vector<double>(graphics.bin_number,0.0); 

    // Create rectangles for histogram using the data   
    graphics.updateValues(data,true);
    graphics.updateHistogramBars(data,window);

    // Update the legend, x and y ticks with the initial data
    graphics.updateLegend();
    graphics.updateXTicks();
    graphics.updateYTicks();
    graphics.drawAxisTicks(window);

    return graphics;
}