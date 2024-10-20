#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <format>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <algorithm>

using std::vector;

struct Point
{
	double x;
	double y;
};

void renderWindow(int x_pointsNumber, int y_pointsNumber, std::vector<double> X, 
std::vector<double> Y, Point found)
{
	std::sort(X.begin(), X.end());
	std::sort(Y.begin(), Y.end());
    sf::RenderWindow window(sf::VideoMode(800, 600), "Function graph");

    const int numPoints = X.size();
	float spacingX = window.getSize().x / x_pointsNumber;
	float spacingY = window.getSize().y / y_pointsNumber;

    sf::VertexArray lines(sf::LinesStrip, numPoints);

    for (int i = 0; i < numPoints; ++i)
    {
        float x = X[i];
        float y = Y[i];

        int screenX = window.getSize().x / 2 + x * spacingX;
        int screenY = window.getSize().y / 2 - y * spacingY;

        lines[i].position = sf::Vector2f(screenX, screenY);
    }

    sf::VertexArray xAxis(sf::LinesStrip, 2);
    xAxis[0].position = sf::Vector2f(0, window.getSize().y / 2);
    xAxis[1].position = sf::Vector2f(window.getSize().x, window.getSize().y / 2);

    sf::VertexArray yAxis(sf::LinesStrip, 2);
    yAxis[0].position = sf::Vector2f(window.getSize().x / 2, 0);
    yAxis[1].position = sf::Vector2f(window.getSize().x / 2, window.getSize().y);

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/jetbrains-mono-fonts/JetBrainsMono-Bold.otf");

    sf::Text xTicks[x_pointsNumber];
    sf::Text yTicks[y_pointsNumber];

    for (int i = 0; i < x_pointsNumber; ++i)
    {
        float x = -4.0f + (8.0f * i) / (x_pointsNumber - 1);
        std::string label = std::to_string(i - x_pointsNumber/2);
        xTicks[i].setFont(font);
        xTicks[i].setString(label);
        xTicks[i].setPosition(spacingX * i, window.getSize().y / 2 + 20);
        xTicks[i].setCharacterSize(10);
        xTicks[i].setColor(sf::Color::White);
    }
    for (int i = 0; i < y_pointsNumber; ++i)
    {
        float y = -1 + (2 * i) / (y_pointsNumber - 1);
        std::string label = std::to_string(-(i - y_pointsNumber/2));
        yTicks[i].setFont(font);
        yTicks[i].setString(label);
        yTicks[i].setPosition(window.getSize().x / 2 - 30, spacingY * i);
        yTicks[i].setCharacterSize(10);
        yTicks[i].setColor(sf::Color::White);
    }

	sf::CircleShape highlightedPoint(5);
	highlightedPoint.setFillColor(sf::Color::Yellow);
	highlightedPoint.setPosition(window.getSize().x / 2 + found.x * spacingX,
	 window.getSize().y / 2 - found.y * spacingY);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.key.code == sf::Keyboard::Escape) 
				window.close();
        }

        window.clear(sf::Color::Black);
        
        window.draw(xAxis);
        window.draw(yAxis);

        for (int i = 0; i < x_pointsNumber; ++i)
            window.draw(xTicks[i]);
        for (int i = 0; i < y_pointsNumber; ++i)
            window.draw(yTicks[i]);

        window.draw(lines);
        
		window.draw(highlightedPoint);
        window.display();
    }
}

vector<Point> readFile(const std::string& filePath) 
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) 
        throw std::runtime_error("Unable to open input file");
    
	int size;
	double x, y;
	vector<Point> points;

	inFile >> size;
	inFile.ignore();
   
    for(int i = 0; i < size; ++i) 
	{
		Point point;
		inFile >> point.x;
		inFile >> point.y;
		points.push_back(point);
		inFile.ignore();
    }

    inFile.close();
    return points;
}

double round_up(double value, int decimal_places) 
{
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

double lagrange(vector<Point> points, double x)
{
	double result = 0;
	double g;
	for(int i = 0; i < points.size(); ++i)
	{
		g = points[i].y;
		for(int j = 0; j < points.size(); ++j)
			if(j != i)
				g *= (x - points[j].x)/(points[i].x - points[j].x);
		result += g;
	}
	return round_up(result, 5);
}

int main(int argc, char* argv[])
{
	vector<Point> points = readFile("input.txt");
	double x = 2.56;
	double result = lagrange(points, x); 
	std::cout << result << std::endl;

	std::vector<double> pointsX;
	std::vector<double> pointsY;
	int n = points.size();

	// float start = -10.2;
	// float end = 10.2;
	// float step = (end - start) / (n - 1);
	for(int i = 0; i < n; ++i)
	{
		pointsX.push_back(points[i].x);
		pointsY.push_back(points[i].y);
	}

	Point found {x, result};
	renderWindow(10, 10, pointsX, pointsY, found);
}