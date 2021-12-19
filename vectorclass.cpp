#include "pch.h"
#include "vectorclass.h"

double Vector::dot(const Vector &right) const
{
	return ((*this)*right).sum();
}

const double Vector::scalar_square() const
{
	return this->dot(*this);
}
