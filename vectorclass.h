#pragma once
#include <vector>

class Vector
{
	std::vector<double> values;
public:
	unsigned short dim;

	Vector() = default;
	explicit Vector(unsigned short dim) :dim(dim) { values.resize(dim); }

	Vector(const double *values, unsigned short dim) : dim(dim)
	{
		this->values.resize(dim);
		for (int i = 0; i < dim; ++i)
			this->values[i] = values[i];
	}

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
		//if (dim != right.dim) throw length_error("Operands differ in dimentionality!");
		if (values.size() != right.dim)
			values.resize(right.dim);
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

	Vector subtraction(const Vector &right) const;

	Vector operator -(const Vector &right) const
	{
		return subtraction(right);
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

	~Vector()
	{
		dim = 0;
	}
};

//ToDo: ввести унарный минус, сделать умножение на скал€р коммутативным; ввести умные указатели в использование