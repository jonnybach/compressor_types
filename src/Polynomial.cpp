/*
 * Polynomial.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */


#include <cmath>
#include "Polynomial.h"

PolynomialFit::PolynomialFit() {
	// TODO Auto-generated constructor stub

}

PolynomialFit::PolynomialFit(std::vector<double> coeffsToAdd) {

	//clear current coefficient vector
	_coeffs.clear();

	std::vector<double>::iterator it;
	for ( it = coeffsToAdd.begin() ; it < coeffsToAdd.end(); it++ ) {
		_coeffs.push_back(*it);
	}

}

PolynomialFit::~PolynomialFit() {
	// TODO Auto-generated destructor stub
}

void PolynomialFit::addCoefficient(double coeffToAdd) {
	_coeffs.push_back(coeffToAdd);
}

void PolynomialFit::replaceCoefficient(int index, double newCoeff) {
	_coeffs.at(index) = newCoeff;
}

double PolynomialFit::calculate(double x) {

	//not using indices to locate members of vector instead of iterator to gurauntee the order of items accessed
	double f = 0;
	size_t indx = 0;
	for (  indx=0; indx < _coeffs.size(); indx++ ) {
		f += _coeffs.at(indx) * pow(x, indx);
		++ indx;
	}

	return f;

}
