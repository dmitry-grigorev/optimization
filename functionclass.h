#pragma once
#include "vectorclass.h"

class Function
{
//	Vector lastpoint;

public:
	const unsigned int dim;

	Function(const unsigned int dim) : dim(dim) {};

	virtual double expression(const Vector &x) const = 0;

	double operator ()(const Vector &x) const { return expression(x); }

	virtual const Vector gradient(const Vector &) const = 0;
	virtual ~Function() {};
};
