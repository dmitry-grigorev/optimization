// optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "vectorclass.h"
#include "areaclass.h"
#include "functionclass.h"
#include "optimizationmethods.h"

using namespace std;

class function1 : public Function
{
public:
	function1(const unsigned int dim) : Function(dim) {};

	double expression(const Vector &x) const override
	{
		return x[0]*x[0] + x[1]*x[1];
	}

	const Vector gradient(const Vector &x) const override
	{
		Vector grad(x.dim);
		grad[0] = 2 * x[0];
		grad[1] = 2 * x[1];
		return grad;
	}
};

class Rosenbrock : public Function
{
public:
	Rosenbrock(const unsigned int dim) : Function(dim) {};

	double expression(const Vector &x) const override
	{
		double a = 1 - x[0];
		double b = x[1] - x[0] * x[0];
		return a*a + 100*b*b;
	}

	const Vector gradient(const Vector &x) const override
	{
		Vector grad(x.dim);
		double a = x[1] - x[0] * x[0];
		grad[0] = 2 *(x[0] - 1 - 200*x[0]*a);
		grad[1] = 200 * a;
		return grad;
	}
};


int main()
{
	Rosenbrock func(2);
	double a[2]{ 0, 0 };
	double b[2]{ -3, -3 };
	double c[2]{ 3, 3 };

	Vector init(a, 2);
	Vector left(b, 2);
	Vector right(c, 2);
	Parallelepiped area(left, right);

	//RibierePolak optimizer(1e-3, 1000);
	//OptMethodSolution<Vector> res = optimizer.optimize(func, area, init);
	RandomSearch optimizer(1e-3, 10000);
	OptMethodSolution<Vector> res = optimizer.optimize(func, area, 0.7, 0.4,1000);
	std::cout << res.argmin << std::endl<< res.value << std::endl << res.iter << std::endl;
	return 0;
}

