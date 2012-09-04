/*
 * InletGuideVane.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "InletGuideVane.h"
#include "CompressorAirfoilConfiguration.h"
#include "Polynomial.h"
#include <exception>

InletGuideVane::InletGuideVane() {
	// TODO Auto-generated constructor stub
}

InletGuideVane::InletGuideVane( double anglePosition
		, bool userDefPtLoss, double userDeltaPtLoss
		, bool userDefR1AlphaFunc, double r1AlphaFuncSlope, double r1AlphaFuncYintcp ) {

	_anglPosn = anglePosition;

	_userPtLoss = userDefPtLoss;
	_delPtLoss = userDeltaPtLoss;

	_userDefR1AlphaFunc = userDefR1AlphaFunc;

	std::vector<double> polyCoeffs;
	polyCoeffs.push_back(r1AlphaFuncYintcp);
	polyCoeffs.push_back(r1AlphaFuncSlope);

	PolynomialFit *r1AlpFunc = new PolynomialFit(polyCoeffs);
	_r1AlpFunc = r1AlpFunc;

}


InletGuideVane::~InletGuideVane() {
	// TODO Auto-generated destructor stub
}

double InletGuideVane::getPosition() { return _anglPosn; }

double InletGuideVane::getPtLoss() { return _delPtLoss; }

void InletGuideVane::setAlphaLaw(std::vector<double> polyCoeffs) {
	delete( _r1AlpFunc );
	_r1AlpFunc = new PolynomialFit(polyCoeffs);
}

double InletGuideVane::getR1Alpha() {
	double r1Alp = _r1AlpFunc->calculate(_anglPosn);
	return r1Alp;
}

CompressorAirfoilConfiguration *InletGuideVane::getAirfoilConfig() {
	throw std::exception(); //not fully implemented yet, just a placeholder, let developer know this
	return _airfoilConfig; }
