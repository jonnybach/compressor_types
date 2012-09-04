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
