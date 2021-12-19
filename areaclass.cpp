#include "pch.h"
#include <math.h>
#include "areaclass.h"
#include "vectorclass.h"

std::mt19937 generator1(SEED);
std::uniform_real_distribution<double> UNIFORMDIST1(0.0, 1.0);

Vector Parallelepiped::getrandompoint() const
{
	Vector randoms(0., dim);

	for (int i = 0; i < dim; ++i)
		randoms[i] = UNIFORMDIST1(generator1);

	return (right_limits - left_limits)*randoms + left_limits;
}

Parallelepiped Parallelepiped::intercept(const Parallelepiped &right) const
{
	for (int i = 0; i < dim; ++i)
	{
		if (right_limits[i] < right.left_limits[i] || left_limits[i] > right.right_limits[i])
			return Parallelepiped();// пустое пересечение
	}

	Vector res_lefts(dim), res_rights(dim);
	for (int i = 0; i < dim; ++i)
	{
		res_lefts[i] = std::max(left_limits[i], right.left_limits[i]);
		res_rights[i] = std::min(right_limits[i], right.right_limits[i]);
	}
	return Parallelepiped(res_lefts, res_rights);
}
Vector Parallelepiped::projectiononborder(const Vector& point) const
{
	Vector abspoint = Vector(abs(point - center));
	int imaxdiff = abspoint.getimaxcomponent();
	double maxdiff = abspoint[imaxdiff];
	double size = sizes[imaxdiff];
	return center + (point - center)*(size / maxdiff);
}