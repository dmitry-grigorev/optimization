#pragma once
#include "vectorclass.h"

class Function
{
public:
	const unsigned int dim;

	Function(const unsigned int dim) : dim(dim) {};

	virtual double expression(const Vector &x) const = 0;

	double operator ()(const Vector &x) const { return expression(x); }

	virtual const Vector gradient(const Vector &) const = 0;
	virtual ~Function() {};
};

class FunctionCrossed//класс функций, которые считают значение векторной функций с заданной опорной точкой в заданном направлении
{
	const Function &func;
	Vector pivot;
	Vector dir;
public:
	FunctionCrossed(const Function &func) : func(func) {};
	void setpivot(const Vector &pivot) { this->pivot = pivot; }
	void setdir(const Vector &dir) { this->dir = dir; }

	double operator ()(const double x) const { return func(pivot + dir * x); }


	virtual ~FunctionCrossed() {};
};