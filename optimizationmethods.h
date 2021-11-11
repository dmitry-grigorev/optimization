#pragma once
#include "vectorclass.h"
#include "areaclass.h"
#include "functionclass.h"
using namespace std;

const double GOLDENRATIO = 1.61803398875;
const Segment UNITSEGMENT(0, 1);


template <typename T>
struct OptMethodSolution
{
	double value;
	T argmin;
	unsigned int iter;
};

class StopCriterionPars
{
public:
	StopCriterionPars() = default;

	virtual ~StopCriterionPars() {};
};

class GradientCritPars : public StopCriterionPars
{
	const Function &func;
	Vector point;
public:
	GradientCritPars(const Function &func, const Vector &point) :  func(func), point(point) {};
	GradientCritPars(const GradientCritPars& pars) : func(pars.func), point(pars.point) {};
	const Function& getfunc() const { return func; }
	const Vector& getpoint() const { return point; }
};

class NeighborCritPars : public StopCriterionPars
{
	Vector first;
	Vector second;
public:
	NeighborCritPars(const Vector &first, const Vector &second) : first(first), second(second) {};
	NeighborCritPars(const NeighborCritPars& pars) : first(pars.first), second(pars.second) {};
	const Vector& getfirst() const { return first; }
	const Vector& getsecond() const { return second; }
};

class StopCriterion
{
protected:
	double tol;
	StopCriterion(const double tol) : tol(tol) {};
public:
	virtual bool satisfy(const StopCriterionPars&) const = 0;
};

class GradientCrit : public StopCriterion
{
public:
	GradientCrit(const double tol) : StopCriterion(tol) {};
	bool satisfy(const StopCriterionPars& pars) const override final;
	bool satisfy(const Function& func, const Vector& vector) { return satisfy(GradientCritPars(func, vector)); }
};

class NeighborCrit : public StopCriterion
{
public:
	NeighborCrit(const double tol) : StopCriterion(tol) {};
	bool satisfy(const StopCriterionPars& pars) const override final;
	bool satisfy(const Vector& first, const Vector& second) { return satisfy(NeighborCritPars(first, second)); }
};

class OptMethodPars
{
public:
	OptMethodPars() = default;

	virtual ~OptMethodPars() {};
};

class RibPolPars : public OptMethodPars
{
	Vector initial;
public:
	RibPolPars(const Vector &initial) : initial(initial) {};
	RibPolPars(const RibPolPars& pars) : initial(pars.initial) {};
	RibPolPars(const RibPolPars&& pars) : initial(move(pars.initial)) {};
public:
	const Vector& getInit() const { return initial; }
};

class MDLSPars : public OptMethodPars
{
	Vector pivot;
	Vector dir;
public:
	MDLSPars(const Vector &pivot, const Vector &dir): pivot(pivot), dir(dir) {};
	MDLSPars(const MDLSPars& pars) : pivot(pars.pivot), dir(pars.dir) {};
	MDLSPars(const MDLSPars&& pars) : pivot(move(pars.pivot)), dir(move(pars.dir)) {};
public:
	const Vector& getPivot() const { return pivot; }
	const Vector& getDirection() const { return dir; }
};

class OptimizationMethod
{
protected:
	unsigned int maxiter;
	const double tol;
public:
	double optimal_value;

	OptimizationMethod(const double eps, const unsigned int maxiter = 100) : tol(eps), maxiter(maxiter) {};
	virtual OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const OptMethodPars &pars) = 0;
	
	virtual ~OptimizationMethod() {};
};

class MultiDimLinearSearch : public OptimizationMethod
{
public:
	double optimal_x;

	MultiDimLinearSearch(const double eps, const unsigned int maxiter = 100) : OptimizationMethod(eps, maxiter) {};
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const OptMethodPars &pars) override;
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const Vector &pivot, const Vector &dir) { return optimize(func, area, MDLSPars(pivot, dir)); }

	virtual ~MultiDimLinearSearch() {};
};


class MultiDimGoldRatio : public OptimizationMethod
{
public:
	double optimal_x;

	MultiDimGoldRatio(const double eps, const unsigned int maxiter = 100) : OptimizationMethod(eps, maxiter) {};
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const OptMethodPars &pars) override;
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const Vector &pivot, const Vector &dir) { return optimize(func, area, MDLSPars(pivot, dir)); }

	virtual ~MultiDimGoldRatio() {};
};


class RibierePolak : public OptimizationMethod
{
	Vector optimal_point;
public:
	RibierePolak(const double eps, const unsigned int maxiter = 100) : OptimizationMethod(eps, maxiter) {};

	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const OptMethodPars &pars) override;
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const Vector &init) { return optimize(func, area, RibPolPars(init)); }

	~RibierePolak() {};
};