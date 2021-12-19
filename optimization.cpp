// optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "vectorclass.h"
#include "areaclass.h"
#include "functionclass.h"
#include "optimizationmethods.h"
#include <type_traits>

using namespace std;

class switchArg
{
	int min;
	int max;
public:
	int value;
	switchArg(const int min, const int max) : min(min), max(max) { };

	friend istream& operator >> (istream &s, switchArg& sa)
	{
		s >> sa.value;

		while (s.fail() || sa.value < sa.min || sa.value > sa.max) {
			s.clear();
			s.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Bad entry.  Enter an INTEGER NUMBER from: " << sa.min << " to " << sa.max << endl;
			s >> sa.value;
		}

		return s;
	}

	int operator() () { return value; }
};

class Paraboloid : public Function
{
public:
	Paraboloid() : Function(2) {};

	double expression(const Vector &x) const override
	{
		return x[0]*x[0] + x[1]*x[1];
	}

	const char* stringexpression() const override
	{
		return "x^2 + y^2";
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

	const char* stringexpression() const override
	{
		return "x^2 + y^2 + (z-4)^2";
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
		return (1- x[0])*(1 - x[0]) + 100*(x[1] - x[0] * x[0])*(x[1] - x[0] * x[0]);
	}

	const char* stringexpression() const override
	{
		return "(1-x)^2 + 100*(y-x^2)^2";
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
	Vector *left_limits = nullptr;
	Vector *right_limits = nullptr;

	int maxiters = 10000;
	int maxiterstostay = 10000;
	double prob = 0.3;
	double shrinkrate = 0.5;
	double searchradius; 
};

bool isUnit(const double a) { return a > 0 && a < 1 ; }

void inputRandomSearchParameters(DebugStruct& ds)
{
	cout << "Type probability parameter" << endl;
	cin >> ds.prob;
	while (cin.fail() || !isUnit(ds.prob)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Bad entry.  Enter a NUMBER greater than 0 and lower than 1: " << endl;
		cin >> ds.prob;
	}

	cout << "Type search radius shrink rate parameter" << endl;
	cin >> ds.shrinkrate;
	while (cin.fail() || !isUnit(ds.shrinkrate)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Bad entry.  Enter a NUMBER greater than 0 and lower than 1: " << endl;
		cin >> ds.shrinkrate;
	}
}

int main()
{
	switchArg methodtype(0, 1), criterionnumber(0, 1), functionnumber(0, 2);

	DebugStruct dbg;
	OptMethodSolution<Vector> res;
	cout << "Method type: 0 - deterministic, 1 - stochastic" << endl;
	cin >> methodtype;

	switch (methodtype())
	{
	case 0: {
		dbg.optimizer = new RibierePolak(1e-4, dbg.maxiters);

		cout << "Criterion: 0 - Gradient, 1 - Neighbor" << endl;
		cin >> criterionnumber;

		switch (criterionnumber())
		{
		case 0: {
			dbg.crit = new GradientCrit(1e-4);
		}; break;
		case 1: {
			dbg.crit = new NeighborCrit(1e-4);
		}; break;
		}

	}; break;
	case 1: {
		dbg.optimizer = new RandomSearch(dbg.maxiters);
	}; break;
	}
	
	cout << "Function: 0 - Paraboloid, 1 - 2-d Rosenbrock, 2 - 3d Paraboloid" << endl;
	cin >> functionnumber;

	switch (functionnumber())
	{
	case 0: {
		dbg.func = new Paraboloid();
	}; break;
	case 1: {
		dbg.func = new Rosenbrock();
	}; break;
	case 2: {
		dbg.func = new Paraboloid3();
	}; break;
	}

	dbg.left_limits = new Vector(dbg.func->dim);
	dbg.right_limits = new Vector(dbg.func->dim);
	dbg.init = new Vector(dbg.func->dim);

	cout << "Type left and right limits of the area: their dimensionality = " << dbg.func->dim << endl;
	cout << "left:" << endl;
	cin >> (*dbg.left_limits);
	cout << "right:" << endl;
	cin >> (*dbg.right_limits);

	cout << "Type initial point: its dimensionality = " << dbg.func->dim << endl;
	cin >> *(dbg.init);

	dbg.area = new Parallelepiped(*(dbg.left_limits), *(dbg.right_limits));

	switch(methodtype())
	{
	case 0:
	{
		res = ((RibierePolak*)dbg.optimizer)->optimize(*(dbg.func), *(dbg.area), *(dbg.init), *(dbg.crit));
	}; break;
	case 1:
	{
		inputRandomSearchParameters(dbg);

		res = ((RandomSearch*)dbg.optimizer)->optimize(*(dbg.func), *(dbg.area), dbg.prob, dbg.shrinkrate, dbg.maxiterstostay);
	}; break;
	}

	cout << dbg.func->stringexpression() << endl;
	cout << "argmin = " << res.argmin << " , min = " << res.value << " , iters = "<< res.iter << endl;

	/*
	list<Vector> &traj = res.trajectory;
	std::cout << res.argmin << std::endl << res.value << std::endl << res.iter << std::endl << std::endl;
	auto iter = traj.begin();
	auto end = traj.end();

	for (; iter != end ; ++iter)
	{
		std::cout << *iter << std::endl;
	}
	*/

	


	delete dbg.crit;
	delete dbg.func;
	delete dbg.optimizer;
	delete dbg.area;
	delete dbg.init;
	delete dbg.left_limits;
	delete dbg.right_limits;
	return 0;
}

