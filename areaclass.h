#pragma once
#include "vectorclass.h"
#include <math.h>
#include <random>

const unsigned int SEED = 1912;

class Area
{
public:
	virtual bool contain(const Vector &x) const = 0;
	virtual ~Area() {};
};

class Parallelepiped : public Area
{
	const unsigned int dim;
protected:
	const Vector left_limits;
	const Vector right_limits;
	const Vector center;
	const Vector sizes;
public:
	Parallelepiped() : dim(0) {}; //пустое множество
	Parallelepiped(const Vector &left_limits, const Vector &right_limits, const unsigned int dim) : left_limits(left_limits), right_limits(right_limits), dim(dim), center((left_limits+right_limits)*0.5), sizes((right_limits - left_limits)*0.5) {};
	Parallelepiped(const Vector &left_limits, const Vector &right_limits) : left_limits(left_limits), right_limits(right_limits), dim(left_limits.dim), center((left_limits + right_limits)*0.5), sizes((right_limits - left_limits)*0.5) {};
	Parallelepiped(const Parallelepiped &prlp) :left_limits(prlp.left_limits), right_limits(prlp.right_limits), dim(prlp.dim), center(prlp.center), sizes(prlp.sizes) {  };
	Vector getleftlimits() const { return left_limits; }

	bool contain(const Vector &x) const override
	{
		for (int i = 0; i < dim; ++i)
			if (left_limits[i] > x[i] || right_limits[i] < x[i])
				return false;
		return true;
	}

	Parallelepiped intercept(const Parallelepiped &right) const;
	Vector getrandompoint() const;
	Vector projectiononborder(const Vector& point) const;

	~Parallelepiped() {};
};

class Segment : public Parallelepiped
{
public:
	double left_limit;
	double right_limit;
	Segment(const double left_limit, const double right_limit) : Parallelepiped(Vector(left_limit), Vector(right_limit), 1) { this->left_limit = left_limits[0]; this->right_limit = right_limits[0]; }

	bool contain(const double x) const
	{
		return left_limit < x && x < right_limit;
	}

	double getrandompoint() const { return Parallelepiped::getrandompoint()[0]; }

	~Segment() {};
};
