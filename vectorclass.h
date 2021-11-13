#pragma once
#include <vector>
#include <iostream>

class Vector
{
	std::vector<double> values;
public:
	unsigned int dim;

	Vector() = default;
	explicit Vector(unsigned int dim) :dim(dim) { values.resize(dim); }

	Vector(const double *values, unsigned short dim) : dim(dim)
	{
		this->values.resize(dim);
		for (int i = 0; i < dim; ++i)
			this->values[i] = values[i];
	}

	explicit Vector(const double value) : dim(1) { values.resize(1, value); }

	Vector(const Vector &src) : dim(src.dim)
	{
		this->values.resize(src.dim);
		for (int i = 0; i < dim; ++i)
			this->values[i] = src.values[i];
	}
	
	//Vector(const Vector &&src) : dim(src.dim) { values.resize(dim); values = vector<double>(move(values)); }
	Vector(const Vector &&src) : dim(src.dim), values(move(src.values)) {};

	Vector& operator = (const Vector &right)
	{
		if (values.size() != right.dim)
		{
			values.resize(right.dim);
			dim = right.dim;
		}
		for (int i = 0; i < dim; ++i)
			values[i] = right.values[i];
		return *this;
	}

	const double operator [] (const int i) const
	{
		return values[i];
	}

	double& operator [] (const int i)
	{
		return values[i];
	}

	double dot(const Vector &right) const;

	const double scalar_square() const;

	double operator *(const Vector &right) const
	{
		return dot(right);
	}

	Vector operator - () const &
	{
		Vector res(dim);
		for (int i = 0; i < dim; ++i)
			res.values[i] = -values[i];
		return res;
	}

	Vector sum(const Vector &right) const;

	Vector operator +(const Vector &right) const
	{
		return sum(right);
	}

	Vector operator +(const double &a) const
	{
		Vector res(*this);
		for (int i = 0; i < dim; ++i)
			res[i] += a;
		return res;
	}

	Vector subtraction(const Vector &right) const;

	Vector operator -(const Vector &right) const
	{
		return subtraction(right);
	}

	Vector operator -(const double &a) const
	{
		Vector res(*this);
		for (int i = 0; i < dim; ++i)
			res[i] -= a;
		return res;
	}

	Vector multOnScalar(const double a) const;

	Vector operator *(const double a) const
	{
		return multOnScalar(a);
	}

	void resize(const int newsize)
	{
		dim = newsize;
		values.resize(dim);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
		os << "(";
		for (int i = 0; i < v.dim - 1; ++i)
			os << v.values[i] << ", ";
		os << v.values[v.dim - 1];
		os << ")";
		return os;
	}

	~Vector()
	{
		dim = 0;
	}
};

//ToDo: ввести унарный минус, сделать умножение на скал€р коммутативным; ввести умные указатели в использование