/*
 * Inlet.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "Inlet.h"

Inlet::Inlet() {
	// TODO Auto-generated constructor stub

}

Inlet::Inlet(double calibFactor) {
	_calibFactor = calibFactor;
}

Inlet::~Inlet() {
	// TODO Auto-generated destructor stub
}

double Inlet::getCalibFactor() { return _calibFactor; }

