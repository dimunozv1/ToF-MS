#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>

// This code creates a histogram using SFML. The histogram data is defined in the `data` vector. The code creates a rectangle for each data point and positions it on the window. The x and y axes are also created, along with axis titles and a graph title. The code also includes a legend with a rectangle representing the data series and text describing the data series. The legend is positioned in the top right corner of the window. The code uses the Roboto-Regular.ttf font file for text rendering, which should be placed in the same directory as the code. You can adjust the font size, colors, and other parameters to customize the appearance of the histogram.
std::vector<double> frequencies(std::vector<double> data,double scale_factor);
struct HistogramGraphics{
    float axisThickness = 1.0f;  // Adjust this to change the thickness of the axes
    float padding = 50.0f;  // Space around the edges of the window
    //legend size
    float legend_size_x = 400;
    float legend_size_y = 25;
    //legend position
    float legendX = 340;
    float legendY = 100;

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
};
HistogramGraphics initializeHistogramGraphics(int numBins, sf::RenderWindow& window, std::vector<double> data);
std::vector<sf::RectangleShape> createBars(const std::vector<double>& freq, double width, double padding, double scaleFactor);
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Histogram");

    // Define histogram data
    std::vector<double> data = {55, 10, 55, 10, 15, 20, 15,10,10};

   
    float padding = 50;  // Space around the edges of the window
    
    
    
    HistogramGraphics histogram_setup = initializeHistogramGraphics(5,window,data);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        std::cout << "Drawing" << std::endl;
        window.clear(sf::Color::White);
        for (const auto& bar : histogram_setup.Histogram_bars)
            window.draw(bar);
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
    }

    return 0;
}

std::vector<double> frequencies(std::vector<double> data, double scale_factor)
{       
    int bin_number = std::sqrt(data.size());

    std::vector<double> freq(bin_number, 0.0);
    //{55, 10, 55, 10, 15, 20, 15,10,10}
    float minDataValue = *std::min_element(data.begin(), data.end());
    float maxDataValue = *std::max_element(data.begin(), data.end());
    float binWidth = (maxDataValue - minDataValue) / bin_number;
    std::cout << "Min: " << minDataValue << " Max: " << maxDataValue << " Bin Width: " << binWidth << std::endl;
    for (float value : data) {
    int binIndex;
    if(value==maxDataValue)
    {
        binIndex = bin_number-1;
    }
    else
    {
        binIndex = (value - minDataValue) / binWidth;
    }
    std::cout << "Value: " << value << " Bin Index: " << binIndex << std::endl;
    freq[binIndex]+=scale_factor;
    }
    
    //rescale the frequencies
   
    return freq;
}



void HistogramGraphics::updateHistogramBars(std::vector<double> data,sf::RenderWindow& window){
    double scaleFactor = (window.getSize().y - 2 * padding)/data.size();
    std::vector<double> freq = frequencies(data,scaleFactor);
    std::vector<double> scaledFreq = freq;
    float width = (window.getSize().x - 2 * padding) / static_cast<float>(freq.size());
    std::cout << "Width: " << width << std::endl;
    Histogram_bars = std::vector<sf::RectangleShape>(freq.size());
    for (size_t i = 0; i < freq.size(); ++i)
    {   std::cout << "a " << i << " Height: " << freq[i] << std::endl;
        Histogram_bars[i].setSize(sf::Vector2f(width - 1, -freq[i]));
        std::cout << "b " << i << " Height: " << freq[i] << std::endl;
        Histogram_bars[i].setPosition(padding+i * width, window.getSize().y-padding);
        Histogram_bars[i].setFillColor(sf::Color(51,153,255));
        Histogram_bars.push_back(Histogram_bars[i]);
        std::cout << "Bar " << i << " Height: " << freq[i] << std::endl;
        
    }

}

HistogramGraphics initializeHistogramGraphics(int numBins, sf::RenderWindow& window, std::vector<double> data) {
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
    graphics.xLabel.setPosition((window.getSize().x -  2*graphics.padding) / 2-60, window.getSize().y - graphics.padding / 2 - 20);
    graphics.xLabel.setFillColor(sf::Color::Black);

    graphics.yLabel = sf::Text("Counts", graphics.font);
    graphics.yLabel.setPosition(graphics.padding / 2-graphics.axisThickness-10, window.getSize().y / 2+40);
    graphics.yLabel.setRotation(-90);
    graphics.yLabel.setFillColor(sf::Color::Black);
   
    // Create plot title
    graphics.title = sf::Text("Time of Flight", graphics.font);
    graphics.title.setPosition((window.getSize().x - 2 * graphics.padding)/2-60, graphics.padding / 2-20);
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
    
    // Create legend text
    graphics.legendText = sf::Text("Total Counts = 200; t = 20s ; Trial 1; binsize 0.491us", graphics.font);  // Replace with the name of the data series
    graphics.legendText.setCharacterSize(15);  // Adjust size as needed
    graphics.legendText.setFillColor(sf::Color::Black);
    graphics.legendText.setPosition(graphics.legendX+33 , graphics.legendY+2);  // Position the text to the right of the data rectangle

    // Create a square that contains everything
    graphics.square = sf::RectangleShape(sf::Vector2f(window.getSize().x - 2 * graphics.padding, window.getSize().y - 2 * graphics.padding));  // Set the size to the size of the window
    graphics.square.setFillColor(sf::Color::Transparent);  // Set the fill color to transparent
    graphics.square.setOutlineThickness(1);  // Set the outline thickness to 2 pixels
    graphics.square.setOutlineColor(sf::Color::Black);  // Set the outline color to white
    graphics.square.setPosition(graphics.padding, graphics.padding);  // Position the square at the top left corner of the window

    // Create rectangles for histogram
    graphics.updateHistogramBars(data,window);


    return graphics;
}