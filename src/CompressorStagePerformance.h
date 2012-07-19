/*
 * CompressorStagePerformance.h
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSTAGEPERFORMANCE_H_
#define COMPRESSORSTAGEPERFORMANCE_H_

#include "CompressorOperatingPoint.h"

class CompressorStagePerformance {

public:
	//methods
	CompressorStagePerformance();
	CompressorStagePerformance(int stageNumber,
		double pt0,
		double pt1,
		double pt2,
		double pt3,
		double pt4,
		double tt0,
		double alpha1,
		double alpha2,
		double alpha3,
		double alpha4);
	virtual ~CompressorStagePerformance();

	//accessors (properties)
	int getStgNmbr();
	int getOptPntNmbr();
	CompressorOperatingPoint* getOptPnt();

	double getPt0();
	double getPt1();
	double getPt2();
	double getPt3();
	double getPt4();

	double getTt0();

	double getAlp1();
	double getAlp2();
	double getAlp3();
	double getAlp4();

private:
	//members
	int _stgNmbr;
	int _opPntNmbr;
	CompressorOperatingPoint* _optPntInfo;

	double _speed;

	double _pt0;
	double _pt1;
	double _pt2;
	double _pt3;
	double _pt4;
	double _tt0;

	double _alp1;
	double _alp2;
	double _alp3;
	double _alp4;

};

#endif /* COMPRESSORSTAGEPERFORMANCE_H_ */
