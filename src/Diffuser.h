/*
 * Diffuser.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef DIFFUSER_H_
#define DIFFUSER_H_

#include <string>
#include <vector>
#include "DiffuserPerformance.h"

class Diffuser {
public:
	Diffuser();
	Diffuser(double areaRatio, double calibFactor);
	virtual ~Diffuser();

	double getAratio();
	double getCalibFactor();

	void addPerformancePoint(int index, DiffuserPerformance perfPntToAdd);
	void replaceOpPntPerfAtIndex(int index, DiffuserPerformance newPerfPnt);

	std::vector <DiffuserPerformance> getOpPntPerf();
	DiffuserPerformance getOpPntPerfAtIndex(int index);


private:
	double _areaRatio;
	double _calibFactor;
	std::vector <DiffuserPerformance> _optPntPerf;

};

#endif /* DIFFUSER_H_ */
