#pragma once
#include <random>
#include "vectorclass.h"
#include "areaclass.h"
#include "functionclass.h"
#include <string>
using namespace std;

const double GOLDENRATIO = 1.61803398875;
const Segment UNITSEGMENT(0., 1.);


template <typename T>
struct OptMethodSolution
{
	double value;
	T argmin;
	unsigned int iter;
	vector<T> trajectory;
};

struct RBSatisftyArgs
{
	Vector &first;
	Vector &second;
	Function &function;
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
	const char *name;
protected:
	double tol;
	unsigned int maximprovenumber;
	StopCriterion() = default;
	StopCriterion(const double tol) : tol(tol) {};
	StopCriterion(const unsigned int n) : maximprovenumber(n) {};
public:
	virtual bool satisfy(const StopCriterionPars&) const = 0;
	virtual const char* getname() const = 0;
};

class GradientCrit : public StopCriterion
{
public:
	GradientCrit() = default;
	GradientCrit(const double tol) : StopCriterion(tol) {};
	bool satisfy(const StopCriterionPars& pars) const override final;
	bool satisfy(const Function& func, const Vector& vector) const { return satisfy(GradientCritPars(func, vector)); }
	const char* getname() const override { return "Gradient"; }
};

class NeighborCrit : public StopCriterion
{
public:
	NeighborCrit() = default;
	NeighborCrit(const double tol) : StopCriterion(tol) {};
	bool satisfy(const StopCriterionPars& pars) const override final;
	bool satisfy(const Vector& first, const Vector& second) const { return satisfy(NeighborCritPars(first, second)); }
	const char* getname() const override { return "Neighbor"; }
};

class LastImproveCrit : public StopCriterion
{
public:
	unsigned int count;
	LastImproveCrit(const unsigned int n): count(0), StopCriterion(n) {};
	bool satisfy(const StopCriterionPars& pars) const override final;
	bool satisfy() { return satisfy(StopCriterionPars()); }
	const char* getname() const override { return "LastImprove"; }
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

	const StopCriterion &criterion;
public:
	RibPolPars(const Vector &initial, const StopCriterion &crit) : initial(initial), criterion(crit) {};
	RibPolPars(const RibPolPars& pars) : initial(pars.initial), criterion(pars.criterion) {};
	RibPolPars(const RibPolPars&& pars) : initial(move(pars.initial)), criterion(move(pars.criterion)) {};
public:
	const Vector& getInit() const { return initial; }
	const StopCriterion& getCriterion() const { return criterion; }
};

class MDLSPars : public OptMethodPars
{
	Vector pivot;
	Vector dir;
public:
	MDLSPars(const Vector &pivot, const Vector &dir): pivot(pivot), dir(dir) {};
	MDLSPars(const MDLSPars &pars) : pivot(pars.pivot), dir(pars.dir) {};
	MDLSPars(const MDLSPars &&pars) : pivot(move(pars.pivot)), dir(move(pars.dir)){};

	const Vector& getPivot() const { return pivot; }
	const Vector& getDirection() const { return dir; }
};

class RSPars : public OptMethodPars
{
	double p;
	double alpha;
	unsigned int lastimprovenumber;
public:
	RSPars(const double p, const double alpha, const unsigned int lastimprovenumber) : p(p), alpha(alpha), lastimprovenumber(lastimprovenumber) {};
	RSPars(const RSPars &pars) :p(pars.p), alpha(pars.alpha), lastimprovenumber(pars.lastimprovenumber){};
	RSPars(const RSPars &&pars) :p(move(pars.p)), alpha(move(pars.alpha)), lastimprovenumber(move(pars.lastimprovenumber)) {};

	const double getp() const { return p; }
	const double getalpha() const { return alpha; }
	const unsigned int getlin() const { return lastimprovenumber; }
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
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const Vector &init, const StopCriterion &crit) { return optimize(func, area, RibPolPars(init, crit)); }

private:
	bool satisfy(const StopCriterion& crit, const Function &func, const Vector &point1, const Vector &point2)
	{
		if (strcmp(crit.getname(), "Gradient") == 0)
		{
			return (dynamic_cast<const GradientCrit&>(crit)).satisfy(func, point1);
		}
		return (dynamic_cast<const NeighborCrit&>(crit)).satisfy(point1, point2);
	}
public:
	~RibierePolak() {};
};


class RandomSearch : public OptimizationMethod
{
public:
	RandomSearch(const double eps, const unsigned int maxiter = 100) : OptimizationMethod(eps, maxiter) {};

	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const OptMethodPars &pars) override;
	OptMethodSolution<Vector> optimize(const Function& func, const Area &area, const double p, const double alpha, const unsigned int maximprovenumber) { return optimize(func, area, RSPars(p, alpha, maximprovenumber)); }
	~RandomSearch() {};
};