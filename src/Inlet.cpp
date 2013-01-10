/*
 * Inlet.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "Inlet.h"

Inlet::Inlet() {

}

Inlet::Inlet(double flangePressure, double scrollCalibFactor) {
	_scrollCalibFactor = scrollCalibFactor;
	_flangePressure = flangePressure;
}

Inlet::~Inlet() {
	// TODO Auto-generated destructor stub
}

void Inlet::addPerformancePoint(int opPnt, InletPerformance perfPntToAdd) {
	//increase capacity if index is greater
	std::vector<InletPerformance>::iterator it;

	 it = m_opPntPerf.begin();
	 m_opPntPerf.insert(it+opPnt, perfPntToAdd);
}

std::vector <InletPerformance> Inlet::getOpPntPerf() { return m_opPntPerf; }

double Inlet::getScrollCalibFactor() { return _scrollCalibFactor; }
double Inlet::getFlangePressure() { return _flangePressure; }
