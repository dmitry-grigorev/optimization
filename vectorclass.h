#pragma once
#include <vector>
#include <iostream>
#include <valarray>
#include <limits>

class Vector : public std::valarray<double>
{
public:
	size_t dim;
	Vector() : std::valarray<double>() {};
    explicit Vector(size_t dim) : std::valarray<double>(0., dim), dim(dim) {  };

    Vector(const double *values, size_t dim) : std::valarray<double>(values, dim), dim(dim) {};
    Vector(const double value, size_t dim) : std::valarray<double>(value, dim), dim(dim) {};

    explicit Vector(const double value) : std::valarray<double>(value, 1), dim(1) {};

    Vector(const valarray<double> &arr) : std::valarray<double>(arr), dim(arr.size()) {};
    Vector(const Vector &src) : std::valarray<double>(src), dim(src.dim) {};

    Vector(const Vector &&src) : valarray<double>(std::move(dynamic_cast<const std::valarray<double>&&>(src))), dim(src.dim) {};

	Vector& operator = (const Vector &right);

	Vector operator + (const double a);

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

	friend std::ostream& operator << (std::ostream& s, const Vector& v)
	{
		s << '(';
		for (auto vit = std::begin(v), vend = std::end(v); vit != vend; ++vit)
			s << *vit << ' ';
		s << ')';
		return s;
	}

	friend std::istream& operator >> (std::istream& s, Vector& v)
	{
        if (!v.size()) throw std::runtime_error("Unavailable data access");
		for (auto vit = std::begin(v), vend = std::end(v); vit != vend; ++vit)
		{
			s >> *vit;
			while (s.fail()) {
				s.clear();
				s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Bad entry.  Enter a NUMBER: " << std::endl;
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
