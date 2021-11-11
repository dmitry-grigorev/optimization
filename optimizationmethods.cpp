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

bool NeighborCrit::satisfy(const StopCriterionPars& pars) const 
{
	const NeighborCritPars &ncpars = dynamic_cast<const NeighborCritPars&>(pars);
	const Vector &first = ncpars.getfirst();
	const Vector &second = ncpars.getsecond();

	return (first - second).scalar_square() < tol*tol;
}

OptMethodSolution<Vector> MultiDimLinearSearch::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
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

	return OptMethodSolution<Vector>{optimal_value, pivot + dir * opt_x, 0};
}

OptMethodSolution<Vector> MultiDimGoldRatio::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
{
	const MDLSPars &params = dynamic_cast<const MDLSPars&>(pars);

	const Segment &segm = dynamic_cast<const Segment&>(area);
	const Vector &pivot = params.getPivot();
	const Vector &dir = params.getDirection();
	const double left = segm.left_limit;
	const double right = segm.right_limit;
	const double part = 10*tol*(right - left); // шаг деления на секции


	double curr_lborder = left, curr_rborder = curr_lborder + part;
	double curr_left = curr_lborder, curr_right = curr_rborder, curr_diff;
	double curr_nleft, curr_nright;
	Vector curr_nvleft, curr_nvright;
	double curr_x, curr_v = 0, opt_value = func(pivot), opt_x = 0;
	Vector opt_point = pivot, curr_point;


	double curr_lvalue = 0, curr_rvalue = 0;

	while (curr_lborder < right)
	{
		curr_diff = curr_right - curr_left;
		curr_nleft = curr_left;
		curr_nright = curr_right;
		curr_nvleft = pivot + dir * curr_nleft;
		curr_nvright = pivot + dir * curr_nright;

		curr_lvalue = func(curr_nvleft);
		curr_rvalue = func(curr_nvright);

		while (curr_diff > tol)
		{
			if (curr_lvalue < curr_rvalue)
			{
				curr_right = curr_nright;
				curr_diff = curr_right - curr_left;

				curr_nright = curr_left + curr_diff / GOLDENRATIO;
				curr_nvright = pivot + dir * curr_nright;
				curr_rvalue = func(curr_nvright);
			}
			else
			{
				curr_left = curr_nleft;
				curr_diff = curr_right - curr_left;

				curr_nleft = curr_right - curr_diff / GOLDENRATIO;
				curr_nvleft = pivot + dir * curr_nleft;
				curr_lvalue = func(curr_nvleft);
			}
		}

		curr_x = (curr_left + curr_right) / 2;
		curr_point = pivot + dir * curr_x;
		curr_v = func(curr_point);
		if (curr_v < opt_value)
		{
			opt_value = curr_v;
			opt_point = curr_point;
			opt_x = curr_x;
		}
		curr_lborder = curr_rborder;
		curr_rborder += part;
		curr_left = curr_lborder;
		curr_right = curr_rborder;
	}
	optimal_x = opt_x;
	optimal_value = opt_value;

	return OptMethodSolution<Vector>{optimal_value, opt_point, 0};
}

OptMethodSolution<Vector> RibierePolak::optimize(const Function &func, const Area &area, const OptMethodPars &pars)//критерий вынести сюда
{
	const Parallelepiped &prlp = dynamic_cast<const Parallelepiped&>(area);
	const RibPolPars &params = dynamic_cast<const RibPolPars&>(pars);
	const Vector &init = params.getInit();

	MultiDimGoldRatio gropt(tol, maxiter);
	unsigned int iter = 0;
	double step = 0;
	Vector currpoint(init);
	Vector currgrad = func.gradient(init);
	Vector currdir = -currgrad ;
	Vector newgrad(currdir.dim);

	GradientCrit criterion(tol);

	while (!criterion.satisfy(func, currpoint) && iter < maxiter && prlp.contain(currpoint))
	{
		gropt.optimize(func, UNITSEGMENT, MDLSPars(currpoint, currdir));
		step = gropt.optimal_x;
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

	return OptMethodSolution<Vector>{optimal_value, optimal_point, iter};
}


//ToDo: обернуть несовпадающие аргументы в объект отдельного класса, от него унаследовать два предка с разными аргументами