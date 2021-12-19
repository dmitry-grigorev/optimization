#include "pch.h"
#include "vectorclass.h"
using namespace std;

Vector& Vector::operator = (const Vector &right)
{
	valarray<double>::operator = (right);
	dim = right.dim;
	return *this;
}

Vector Vector::operator + (const double a)
{
	return Vector((*this) + valarray<double>(a, dim));
}

double Vector::dot(const Vector &right) const
{
	return ((*this)*right).sum();
}

const double Vector::scalar_square() const
{
	return this->dot(*this);
}
