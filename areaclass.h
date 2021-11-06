#pragma once
#include "vectorclass.h"

class Area
{
	virtual bool contain(const Vector &x) = 0;
	virtual bool contain(const double x) = 0;

public:
	virtual ~Area() {};
};

class Parallelepiped : public Area
{
	const unsigned int dim;
	const Vector left_limits;
	const Vector right_limits;
public:
	Parallelepiped() = default;
	Parallelepiped(const Vector &left_limits, const Vector &right_limits, const unsigned int dim) : left_limits(left_limits), right_limits(right_limits), dim(dim) {};
	Parallelepiped(const Vector &left_limits, const Vector &right_limits) : left_limits(left_limits), right_limits(right_limits), dim(left_limits.dim) {};

	bool contain(const double x) override final { return false; }
	bool contain(const Vector &x) override
	{
		for (int i = 0; i < dim; ++i)
			if (left_limits[i] > x[i] || right_limits[i] < x[i])
				return false;
		return true;
	}

	~Parallelepiped() {};
};

class Segment : public Area
{
public:
	const double left_limit;
	const double right_limit;
	Segment(const double left_limit, const double right_limit) : left_limit(left_limit), right_limit(right_limit) {};

	bool contain(const Vector &x) override final { return false; }
	bool contain(const double x)
	{
		return left_limit < x && x < right_limit;
	}

	~Segment() {};
};

//ToDo: �� �������������� � ���������� �������, ��� ��� ��� ���������� �������������� (��������, ����� ��� ������ � ������� ��������)