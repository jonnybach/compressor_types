/*
 * Diffuser.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "Diffuser.h"

Diffuser::Diffuser() {
	// TODO Auto-generated constructor stub

}

Diffuser::Diffuser(double areaRatio, double calibFactor) {
	_areaRatio = areaRatio;
	_calibFactor = calibFactor;
}

Diffuser::~Diffuser() {
	// TODO Auto-generated destructor stub
}

//properties
double Diffuser::getAratio() { return _areaRatio; }
double Diffuser::getCalibFactor() { return _calibFactor; }

void Diffuser::addPerformancePoint(int index, DiffuserPerformance perfPntToAdd) {
	//increase capacity if index is greater
	std::vector<DiffuserPerformance>::iterator it;

	 it = _optPntPerf.begin();
	_optPntPerf.insert(it+index, perfPntToAdd);

}

std::vector <DiffuserPerformance> Diffuser::getOpPntPerf() {	return _optPntPerf; }

DiffuserPerformance Diffuser::getOpPntPerfAtIndex(int index) {
	return _optPntPerf.at(index);
}

void Diffuser::replaceOpPntPerfAtIndex(int index, DiffuserPerformance newPerfPnt) {
	_optPntPerf.at(index) = newPerfPnt;
}
