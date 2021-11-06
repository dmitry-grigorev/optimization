#include "pch.h"
#include "functionclass.h"
#include "optimizationmethods.h"
#include "areaclass.h"

bool GradientCrit::satisfy(const StopCriterionPars& pars) const
{
	const GradientCritPars &gcpars = dynamic_cast<const GradientCritPars&>(pars);
	const Function &func = gcpars.getfunc();
	const Vector &point = gcpars.getpoint();

	return (func.gradient(point)).scalar_square() < tol*tol;
}


Vector& MultiDimLinearSearch::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
{
	const MDLSPars &params = dynamic_cast<const MDLSPars&>(pars);

	const Segment &segm = dynamic_cast<const Segment&>(area);
	const Vector &pivot = params.getPivot();
	const Vector &dir = params.getDirection();

	double opt_x = segm.left_limit;
	Vector opt_point = pivot + dir * opt_x;
	double opt_value = func(opt_point);

	double curr_x = opt_x + tol;
	Vector curr_point = opt_point + dir * tol;
	double curr_value = func(curr_point);

	while (curr_x < segm.right_limit)
	{
		if (curr_value < opt_value)
		{
			opt_value = curr_value;
			opt_x = curr_x;
			opt_point = curr_point;
		}
		curr_point = curr_point + dir * tol;
		curr_x += tol;
		curr_value = func(curr_point);
	}

	optimal_x = opt_x;
	optimal_value = opt_value;

	return opt_point;
}


Vector& RibierePolak::optimize(const Function &func, const Area &area, const OptMethodPars &pars)
{
	const Parallelepiped &prlp = dynamic_cast<const Parallelepiped&>(area);
	const RibPolPars &params = dynamic_cast<const RibPolPars&>(pars);
	const Vector &init = params.getInit();

	MultiDimLinearSearch linopt(tol, 100);
	unsigned int iter = 0;
	double step = 0;
	Vector currpoint(init);
	Vector currgrad = func.gradient(init);
	Vector currdir = -currgrad ;//* -1
	Vector newgrad(currdir.dim);

	const Segment unitsegm(0, 1);

	GradientCrit criterion(tol);

	while (!criterion.satisfy(func, currpoint) && iter < maxiter)
	{
		linopt.optimize(func, unitsegm, MDLSPars(currpoint, currdir));
		step = linopt.optimal_x;

		currpoint = currpoint + currdir * step;
		newgrad = func.gradient(currpoint);
		double beta = newgrad * (newgrad - currgrad) / currgrad.scalar_square();
		currdir = -currgrad + currdir * beta;
		currgrad = newgrad;
		++iter;
	}

	optimal_point.resize(init.dim);

	optimal_point = currpoint;
	optimal_value = func(optimal_point);

	return optimal_point;
}


//ToDo: обернуть несовпадающие аргументы в объект отдельного класса, от него унаследовать два предка с разными аргументами