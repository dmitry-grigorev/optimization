#include "pch.h"
#include <math.h>
#include "areaclass.h"
#include "vectorclass.h"

std::mt19937 generator1(SEED);
std::uniform_real_distribution<double> UNIFORMDIST1(0.0, 1.0);

Vector Parallelepiped::getrandompoint() const
{
	Vector res(dim);

	for (int i = 0; i < dim; ++i)
		res[i] = (right_limits[i] - left_limits[i])*UNIFORMDIST1(generator1) + left_limits[i];
	return res;
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