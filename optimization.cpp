// optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

class Vector
{
	double *values;
public:
	unsigned short dim;

	Vector(unsigned short dim) : dim(dim), values(new double[dim])
	{
		for (int i = 0; i < dim; ++i)
			this->values[i] = 0;
	}
	Vector(const double *values, unsigned short dim) : dim(dim), values(new double[dim])
	{
		for (int i = 0; i < dim; ++i)
			this->values[i] = values[i];
	}
	Vector(const Vector &src) : dim(src.dim), values(new double[dim])
	{
		for (int i = 0; i < dim; ++i)
			values[i] = src.values[i];
	}

	const double operator [] (const int i) const
	{
		return values[i];
	}

	double& operator [] (const int i)
	{
		return values[i];
	}

	double dot(const Vector &right)
	{
		double res = 0;//здесь нужно обработать равенство размерностей
		for (int i = 0; i < dim; ++i)
			res += values[i] * right[i];
		return res;
	}

	double operator *(const Vector &right)
	{
		return dot(right); 
	}

	Vector& sum(const Vector &right)
	{
		//здесь нужно обработать равенство размерностей
		Vector res(dim);
		for (int i = 0; i < dim; ++i)
			res[i] = values[i] + right[i];
		return res;
	}

	Vector& operator +(const Vector &right)
	{
		return sum(right);
	}

	Vector& subtraction(const Vector &right)
	{
		//здесь нужно обработать равенство размерностей
		Vector res(dim);
		for (int i = 0; i < dim; ++i)
			res[i] = values[i] - right[i];
		return res;
	}

	Vector& operator -(const Vector &right)
	{
		return subtraction(right);
	}

	~Vector()
	{
		dim = 0;
		delete[] values;
	}
};


class Function
{
protected:
	const unsigned int dim;

	Function(const unsigned int dim) : dim(dim) {};

	virtual double expression() = 0;
	virtual Vector gradient() = 0;
	virtual ~Function();
};

class function1 : public Function
{
	function1(const unsigned int dim) : Function(dim) {};

	double expression(const double *x)
	{
		return x[0]*x[0] + x[1]*x[1];
	}
	double operator ()(const double *x)
	{
		return expression(x);
	}

	Vector gradient(const Vector &x)
	{
		Vector grad(x.dim);
		grad[0] = 2 * x[0];
		grad[1] = 2 * x[1];
		return grad;
	}
};

class OptimizationMethod
{
	virtual void optimize() = 0;
};

class RibierePolak : public OptimizationMethod
{
	void optimize(const Function &func)
	{

	}
};

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source controlw
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
