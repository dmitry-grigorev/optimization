// optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "vectorclass.h"
#include "areaclass.h"
#include "functionclass.h"
#include "optimizationmethods.h"

using namespace std;

class Paraboloid : public Function
{
public:
	Paraboloid() : Function(2) {};

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

class Paraboloid3 : public Function
{
public:
	Paraboloid3() : Function(3) {};

	double expression(const Vector &x) const override
	{
		return x[0] * x[0] + x[1] * x[1] + (x[2] - 4)*(x[2] - 4);
	}

	const Vector gradient(const Vector &x) const override
	{
		Vector grad(x.dim);
		grad[0] = 2 * x[0];
		grad[1] = 2 * x[1];
		grad[2] = 2 * (x[2] - 4);
		return grad;
	}
};

class Rosenbrock : public Function
{
public:
	Rosenbrock() : Function(2) {};

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

struct DebugStruct
{
	Function *func = nullptr;
	Area *area = nullptr;
	StopCriterion *crit = nullptr;
	OptimizationMethod *optimizer = nullptr;
	Vector *init = nullptr;
};

int main()
{
	int choice, methodchoice;
	DebugStruct dbg;
	OptMethodSolution<Vector> res;
	cout << "Method type: 0 - deterministic, 1 - stochastic" << endl;
	cin >> methodchoice;

	switch (methodchoice)
	{
	case 0: {
		dbg.optimizer = new RibierePolak(1e-3, 1000);

		cout << "Criterion: 0 - Gradient, 1 - Neighbor" << endl;
		cin >> choice;

		switch (choice)
		{
		case 0: {
			dbg.crit = new GradientCrit(1e-3);
		}; break;
		case 1: {
			dbg.crit = new NeighborCrit(1e-3);
		}; break;
		}

	}; break;
	case 1: {
		dbg.optimizer = new RandomSearch(1e-3, 10000);
	}; break;
	}
	
	cout << "Function: 0 - Paraboloid, 1 - 2-d Rosenbrock, 2 - 3d Paraboloid" << endl;
	cin >> choice;

	switch (choice)
	{
	case 0: {
		dbg.func = new Paraboloid();

		double a[2]{ -1, -1 };
		double b[2]{ -3, -3 };
		double c[2]{ 3, 3 };

		dbg.init = new Vector(a, 2);
		Vector left(b, 2);
		Vector right(c, 2);
		dbg.area = new Parallelepiped(left, right);
	}; break;
	case 1: {
		dbg.func = new Rosenbrock();

		double a[2]{ -1, -1 };
		double b[2]{ -3, -3 };
		double c[2]{ 3, 3 };

		dbg.init = new Vector(a, 2);
		Vector left(b, 2);
		Vector right(c, 2);
		dbg.area = new Parallelepiped(left, right);
	}; break;
	case 2: {
		dbg.func = new Paraboloid3();

		double a[3]{ -1, -1, -1 };
		double b[3]{ -3, -3, -3 };
		double c[3]{ 3, 3, 3 };

		dbg.init = new Vector(a, 3);
		Vector left(b, 3);
		Vector right(c, 3);
		dbg.area = new Parallelepiped(left, right);
	}; break;
	}

	

	switch(methodchoice)
	{
	case 0:
	{
		res = ((RibierePolak*)dbg.optimizer)->optimize(*(dbg.func), *(dbg.area), *(dbg.init), *(dbg.crit));
	}; break;
	case 1:
	{
		res = ((RandomSearch*)dbg.optimizer)->optimize(*(dbg.func), *(dbg.area), 0.7, 0.4, 10000);
	}; break;
	}
	vector<Vector> &traj = res.trajectory;
	std::cout << res.argmin << std::endl << res.value << std::endl << res.iter << std::endl << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << traj[i] << std::endl;
	}


	


	delete dbg.crit;
	delete dbg.func;
	delete dbg.optimizer;
	delete dbg.area;
	delete dbg.init;
	return 0;
}

