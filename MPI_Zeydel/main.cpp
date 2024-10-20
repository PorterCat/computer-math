#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <format>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <algorithm>

void printMatrix(double** mat, int size)
{
    for(int i = 0; i < size - 1; ++i)
    {
        for(int j = 0; j < size; ++j)
            std::cout << std::setw(10) << mat[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

double** readFile(const std::string& filePath, int& rows, int& cols) 
{
    double** mat;
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) 
        throw std::runtime_error("Unable to open input file");
    

    inFile >> cols;
    rows = cols - 1;
    mat = new double*[rows];
    
    for(int i = 0; i < rows; ++i) {
        mat[i] = new double[cols];
        for(int j = 0; j < cols; ++j) {
            inFile >> mat[i][j];
        }
        inFile.ignore();
    }

    inFile.close();
    return mat;
}

void normalize(double** matrix, int size)
{
	double devisor;
	for(int i = 0; i < size - 1; ++i)
	{
		devisor = matrix[i][i];
		for(int j = 0; j < size; ++j)
			matrix[i][j] /= devisor;
	}
}

double* mpi_method(double** mat, int size, int iterations)
{
	double* x = new double[size - 1];
	double sum = 0;

	for(int iteration = 0; iteration < iterations; ++iteration)
	{
		double* new_x = new double[size - 1];
		for(int i = 0; i < size - 1; ++i)
		{
			sum = 0;
			for(int j = 0; j < size; ++j)
				if(i != j)
					sum += x[j] * mat[i][j];
			new_x[i] = mat[i][size-1] - sum;	
		}
		delete[] x;
		x = new_x;
		std::cout << "Iteration: " << iteration + 1 << std::endl << '\t';
		for(int i = 0; i < size - 1; ++i)
			std::cout << x[i] << ' ';
		std::cout << '\n';
	}
	return x;
}

double* zeydel_method(double** mat, int size, double accuracy)
{
	int iteration = 0;
	double* x = new double[size - 1];
	double sum = 0;
	double diff = 0;

	while(true)
	{
		double* new_x = new double[size - 1];
		for(int i = 0; i < size - 1; ++i)
			new_x[i] = x[i];

		for(int i = 0; i < size - 1; ++i)
		{
			sum = mat[i][size-1];
			for(int j = 0; j < size-1; ++j)
				if(i != j)
					sum -= mat[i][j] * new_x[j];
			new_x[i] = sum;
		}

		diff = std::abs(new_x[0] - x[0]);
		for(int i = 1; i < size - 1; ++i)
			if(std::abs(new_x[i] - x[i]) > diff)
				diff = std::abs(new_x[i] - x[i]);

		delete[] x;
		x = new_x;
		iteration++;

		std::cout << "Iteration: " << iteration + 1 << std::endl << '\t';
		for(int i = 0; i < size - 1; ++i)
			std::cout << x[i] << ' ';
		std::cout << '\n';

		if(diff < accuracy)
			break;
	}
	return x;
}

bool isIteratable(double** mat, int size) //Iteration condition
{
	double sum = 0;
    for(int i = 0; i < size - 1; ++i)
	{
		sum = 0;
		for(int j = 0; j < size - 1; ++j)
			if(i != j)
				sum += std::abs(mat[i][j]);
		if(std::abs(mat[i][i]) <= sum)
			return false;
	}
	return true;
}

void renderWindow(int x_pointsNumber, int y_pointsNumber, std::vector<float> X)
{
	std::sort(X.begin(), X.end());
    sf::RenderWindow window(sf::VideoMode(800, 600), "Function graph");

    const int numPoints = X.size();
	float spacingX = window.getSize().x / x_pointsNumber;
	float spacingY = window.getSize().y / y_pointsNumber;

    sf::VertexArray lines(sf::LinesStrip, numPoints);

    for (int i = 0; i < numPoints; ++i)
    {
        float x = X[i];
        float y = pow(x, 2);

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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
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
        
        window.display();
    }
}

int main(int argc, char* argv[])
{
	int cols, rows;
    double** mat;

    mat = readFile("input.txt", rows, cols);

	normalize(mat, cols);
	printMatrix(mat, cols);

	double* arr1;
	double* arr2;

	if(isIteratable(mat, cols))
		arr1 = mpi_method(mat, cols, 1000);
	else
		std::cout << "Matrix is not iretable" << std::endl;

	if(isIteratable(mat, cols))
		arr2 = zeydel_method(mat, cols, 0.0001);
	else
		std::cout << "Matrix is not iretable" << std::endl;

	std::cout << "\nresult\n" << std::endl;

	for(int i = 0; i < cols - 1; ++i)
	{
		std::cout << arr1[i] << ' ';
	}
	std::cout << std::endl;
	for(int i = 0; i < cols - 1; ++i)
	{
		std::cout << arr2[i] << ' ';
	}
	std::cout << std::endl;

	int n = 1000;

	std::vector<float> points;

	float start = -10.2;
	float end = 10.2;
	float step = (end - start) / (n - 1);
	for(int i = 0; i < n; ++i)
		points.push_back(start + i * step);
	renderWindow(10, 10, points);

    return 0;
}