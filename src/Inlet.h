/*
 * Inlet.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef INLET_H_
#define INLET_H_

#include <vector>
#include "InletPerformance.h"

class Inlet {
public:
	Inlet();
	Inlet(double flangePressure, double scrollCalibFactor);
	virtual ~Inlet();

	double getScrollCalibFactor();
	double getFlangePressure();

	void addPerformancePoint(int opPnt, InletPerformance perfPntToAdd);
	std::vector <InletPerformance> getOpPntPerf();

private:
	double _scrollCalibFactor;
	double _flangePressure;

	std::vector <InletPerformance> m_opPntPerf;

};

#endif /* INLET_H_ */
