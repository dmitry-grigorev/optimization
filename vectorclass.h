#pragma once
#include <vector>
#include <iostream>
#include <valarray>
#include <limits>
using namespace std;

class Vector : public valarray<double>
{
public:
	size_t dim;
	Vector() : valarray<double>() {};
	explicit Vector(size_t dim) :dim(dim), valarray<double>(0., dim) {  };

	Vector(const double *values, size_t dim) : dim(dim), valarray<double>(values, dim) {};
	Vector(const double value, size_t dim) : dim(dim), valarray<double>(value, dim) {};

	explicit Vector(const double value) : dim(1), valarray<double>(value, 1) {};

	Vector(const valarray<double> &arr) : dim(arr.size()), valarray<double>(arr) {};
	Vector(const Vector &src) : dim(src.dim), valarray<double>(src) {};

	Vector(const Vector &&src) : dim(src.dim), valarray<double>(move(src)) {};

	Vector& operator = (const Vector &right)
	{
		valarray<double>::operator = (right);
		dim = right.dim;
		return *this;
	}

	Vector operator + (const double a)
	{
		return (*this) + Vector(a, dim);
	}

	double dot(const Vector &right) const;

	const double scalar_square() const;

	int getimaxcomponent() const
	{
		int imax = 0;
		for (int i = 1; i < dim; ++i)
			if ((*this)[i] > (*this)[imax])
				imax = i;
		return imax;
	}

	friend ostream& operator << (ostream& s, const Vector& v)
	{
		s << '(';
		for (auto vit = begin(v), vend = end(v); vit != vend; ++vit)
			s << *vit << ' ';
		s << ')';
		return s;
	}

	friend istream& operator >> (istream& s, Vector& v)
	{
		if (!v.size()) throw exception("Unavailable data access");
		for (auto vit = begin(v), vend = end(v); vit != vend; ++vit)
		{
			s >> *vit;
			while (s.fail()) {
				s.clear();
				s.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Bad entry.  Enter a NUMBER: " << endl;
				s >> *vit;
			}
		}
		return s;
	}


	~Vector()
	{
		dim = 0;
	}
};