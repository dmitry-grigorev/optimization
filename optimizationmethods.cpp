#include "pch.h"
#include "functionclass.h"
#include "optimizationmethods.h"
#include "areaclass.h"

std::mt19937 generator(SEED);
std::uniform_real_distribution<double> UNIFORMDIST(0.0, 1.0);


bool GradientCrit::satisfy(const StopCriterionPars& pars) const
{
	const GradientCritPars &gcpars = dynamic_cast<const GradientCritPars&>(pars);
	const Function &func = gcpars.getfunc();
	const Vector &point = gcpars.getpoint();

	double norm = (func.gradient(point)).scalar_square();

	return norm < tol*tol;
}

bool NeighborCrit::satisfy(const StopCriterionPars& pars) const 
{
	const NeighborCritPars &ncpars = dynamic_cast<const NeighborCritPars&>(pars);
	const Vector &first = ncpars.getfirst();
	const Vector &second = ncpars.getsecond();

	double diff = (Vector(first - second)).scalar_square();

	return diff < tol*tol;
}

bool LastImproveCrit::satisfy(const StopCriterionPars& pars) const
{
	return count > maximprovenumber;
}

OptMethodSolution<Vector> MultiDimLinearSearch::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
{
	const MDLSPars &params = dynamic_cast<const MDLSPars&>(pars);

	const Segment &segm = dynamic_cast<const Segment&>(area);
	const Vector &pivot = params.getPivot();
	const Vector &dir = params.getDirection();

	FunctionCrossed funccr(func);
	funccr.setdir(dir);
	funccr.setpivot(pivot);

	double opt_x = segm.left_limit;
	Vector opt_point = pivot + dir * opt_x;
	double opt_value = func(opt_point);

	double curr_x = opt_x + tol;
	double curr_value = funccr(curr_x);

	while (curr_x < segm.right_limit)
	{
		if (curr_value < opt_value)
		{
			opt_value = curr_value;
			opt_x = curr_x;
		}
		curr_x += tol;
		curr_value = funccr(curr_x);
	}

	optimal_x = opt_x;
	optimal_value = opt_value;
	opt_point = pivot + dir * optimal_x;

	return OptMethodSolution<Vector>{optimal_value, opt_point, 0};
}

OptMethodSolution<Vector> MultiDimGoldRatio::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
{
	const MDLSPars &params = dynamic_cast<const MDLSPars&>(pars);

	const Segment &segm = dynamic_cast<const Segment&>(area);
	const Vector &pivot = params.getPivot();
	const Vector &dir = params.getDirection();

	FunctionCrossed funccr(func);
	funccr.setdir(dir);
	funccr.setpivot(pivot);

	const double left = segm.left_limit;
	const double right = segm.right_limit;
	const double part = tol; 

	double curr_lborder = left, curr_rborder = curr_lborder + part;
	double curr_left = curr_lborder, curr_right = curr_rborder, curr_diff;
	double curr_nleft, curr_nright;
	double curr_x, curr_v = 0, opt_value = func(pivot), opt_x = 0;
	Vector opt_point = pivot, curr_point;


	double curr_lvalue = 0, curr_rvalue = 0;

	while (curr_lborder < right)
	{
		curr_diff = curr_right - curr_left;
		curr_nleft = curr_left;
		curr_nright = curr_right;

		curr_lvalue = funccr(curr_nleft);
		curr_rvalue = funccr(curr_nright);

		while (curr_diff > tol)
		{
			if (curr_lvalue < curr_rvalue)
			{
				curr_right = curr_nright;
				curr_diff = curr_right - curr_left;

				curr_nright = curr_left + curr_diff / GOLDENRATIO;
				curr_rvalue = funccr(curr_nright);
			}
			else
			{
				curr_left = curr_nleft;
				curr_diff = curr_right - curr_left;

				curr_nleft = curr_right - curr_diff / GOLDENRATIO;
				curr_lvalue = funccr(curr_nleft);
			}
		}

		curr_x = (curr_left + curr_right) / 2;
		curr_v = funccr(curr_x);
		if (curr_v < opt_value)
		{
			opt_value = curr_v;
			opt_x = curr_x;
		}
		curr_lborder = curr_rborder;
		curr_rborder += part;
		curr_left = curr_lborder;
		curr_right = curr_rborder;
	}
	optimal_x = opt_x;
	optimal_value = opt_value;
	opt_point = pivot + dir * optimal_x;

	return OptMethodSolution<Vector>{optimal_value, opt_point, 0};
}
//метод сопряжённых градиентов Рибьера-Полака с возвратом в область в случае выхода за неё
OptMethodSolution<Vector> RibierePolak::optimize(const Function &func, const Area &area, const OptMethodPars &pars)
{
	const Parallelepiped &prlp = dynamic_cast<const Parallelepiped&>(area);
	const RibPolPars &params = dynamic_cast<const RibPolPars&>(pars);
	const Vector &init = params.getInit();
	const StopCriterion &crit = params.getCriterion();

	MultiDimGoldRatio gropt(tol, maxiter);
	unsigned int iter = 0;
	double step = 0;
	double beta;
	Vector currpoint(init);
	Vector prevpoint(init + 1.);
	Vector currgrad = func.gradient(init);
	Vector currdir = -currgrad ;
	Vector newgrad(currdir.dim);

	list<Vector> trajectory(1, init);

	while (!satisfy(crit, func, currpoint, prevpoint) && iter < maxiter && prlp.contain(currpoint))
	{
		gropt.optimize(func, UNITSEGMENT, MDLSPars(currpoint, currdir));
		step = gropt.optimal_x;
		prevpoint = currpoint;
		currpoint = currpoint + currdir * step;
		newgrad = func.gradient(currpoint);
		beta = newgrad.dot(newgrad - currgrad) / currgrad.scalar_square();
		currdir = -newgrad + currdir * beta;
		currgrad = newgrad;

		trajectory.push_back(currpoint);

		++iter;
	}

	if (!prlp.contain(currpoint))
	{
		currpoint = prlp.projectiononborder(currpoint);
		trajectory.push_back(currpoint);
	}
	optimal_point.resize(init.dim);

	optimal_point = currpoint;
	optimal_value = func(optimal_point);

	return OptMethodSolution<Vector>{optimal_value, optimal_point, iter, trajectory};
}

OptMethodSolution<Vector> RandomSearch::optimize(const Function& func, const Area &area, const OptMethodPars &pars)
{
	const Parallelepiped &prlp = dynamic_cast<const Parallelepiped&>(area);
	const RSPars &params = dynamic_cast<const RSPars&>(pars);
	const double p = params.getp();
	const double alpha = params.getalpha();
	const unsigned int lastimprovenumber = params.getlin();

	Vector optpoint = prlp.getrandompoint();
	Vector currpoint(optpoint.dim);
	double optvalue = func(optpoint), currvalue, beta;
	double delta = 1;
	unsigned int iter = 0;
	
	LastImproveCrit crit(lastimprovenumber);

	list<Vector> trajectory(1, optpoint);

	while (!crit.satisfy() && iter < maxiter)
	{
		beta = UNIFORMDIST(generator);
		if (beta < p)
		{
			currpoint = prlp.getrandompoint();
		}
		else
		{
			currpoint = prlp.intercept(Parallelepiped(optpoint - delta, optpoint + delta)).getrandompoint();
		}
		currvalue = func(currpoint);

		if (currvalue < optvalue)
		{
			optpoint = currpoint;
			optvalue = currvalue;
			if (beta > p)
				delta *= alpha;
			else
				delta = 1;
			crit.count = 0;
			trajectory.push_back(currpoint);
		}
		else
			++crit.count;
		++iter;
	}
	optimal_value = optvalue;
	return OptMethodSolution<Vector>{optvalue, optpoint, iter, trajectory};
}
