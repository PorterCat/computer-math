#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <format>
#include <fstream>
#include <cmath>

class MathFunction
{
private:
	double (*function)(double);
public:
	MathFunction(double (*func)(double)){ function = func;}
	double operator () (double x) {return function(x);}
	double derivative(double x)
	{
		double h = sqrt(std::numeric_limits<double>::epsilon());
    	return (operator()(x + h) - operator()(x - h)) / (2.0 * h);
	}
};

void half_devide_method(MathFunction func, double a, double b, double accurancy)
{
	double c;
	double first, second;
	int i = 1;
	while(std::abs(b - a)/2 > accurancy)
	{
		c = (a + b)/2;
		std::cout << i++ << ". C = " << c << std::endl;
		first = func(a) * func(c);
		second = func(c) * func(b);
		if(first < 0)
			b = c;
		else if(second < 0)
			a = c;
	}
}

void hord_method(MathFunction func, double a, double b, double accurancy)
{
	double c = a;
	double c_prev = b;
	double first, second;
	double delta = c_prev - c;
	int i = 1;

	while(std::abs(delta) > accurancy)
	{
		c_prev = c;
		c = (a * func(b) - b * func(a))/(func(b) - func(a));
		std::cout << i++ << ". C = " << c << std::endl;
		first = func(a) * func(c);
		second = func(c) * func(b);
		if(first < 0)
			b = c;
		else if(second < 0)
			a = c;
		delta = c_prev - c;
	}
}

void Newton_method(MathFunction func, double a, double b, double accurancy)
{
	double x, x_prev;
	if(func(a) > 0)
		x = a;
	else if(func(b) > 0)
		x = b;
	int i = 1;
	do
	{
		x_prev = x;
		x = x - func(x)/func.derivative(x);
		std::cout << i++ << ". X = " << x << std::endl;
	}
	while(std::abs(x_prev - x) >= accurancy);
}

int main(int argc, char* argv[])
{
	MathFunction func1 = {[](double x){return pow(x, 2) - 3;}};
	half_devide_method(func1, 1, 2, 0.0001);
	hord_method(func1, 1, 2, 0.0001);
	Newton_method(func1, 1, 2, 0.0001);
}