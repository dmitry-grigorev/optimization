#include "pch.h"
#include "vectorclass.h"

double Vector::dot(const Vector &right) const
{
	double res = 0;//здесь нужно обработать равенство размерностей
	for (int i = 0; i < dim; ++i)
		res += values[i] * right[i];
	return res;
}

const double Vector::scalar_square() const
{
	double res = 0;
	for (int i = 0; i < dim; ++i)
		res += values[i] * values[i];
	return res;
}

Vector Vector::sum(const Vector &right) const
{
	//здесь нужно обработать равенство размерностей
	Vector res(dim);
	for (int i = 0; i < dim; ++i)
		res[i] = values[i] + right[i];
	return res;
}

Vector Vector::subtraction(const Vector &right) const
{
	//здесь нужно обработать равенство размерностей
	Vector res(dim);
	for (int i = 0; i < dim; ++i)
		res[i] = values[i] - right[i];
	return res;
}

Vector Vector::multOnScalar(const double a) const
{
	Vector res(dim);
	for (int i = 0; i < dim; ++i)
		res[i] = a * values[i];
	return res;
}