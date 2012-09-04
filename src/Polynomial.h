/*
 * Polynomial.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <vector>

class PolynomialFit {
public:
	PolynomialFit();
	PolynomialFit(std::vector<double> coeffsToAdd);
	virtual ~PolynomialFit();

	//public methods
	void addCoefficient(double coeffToAdd);
	void replaceCoefficient(int index, double newCoeff);
	double calculate(double x);

private:
	std::vector<double> _coeffs;

};

#endif /* POLYNOMIALFIT_H_ */
