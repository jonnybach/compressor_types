/*
 * CompressorStagePerformance.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#include <fstream>
#include "CompressorStagePerformance.h"

CompressorStagePerformance::CompressorStagePerformance() {
	// TODO Auto-generated constructor stub
}

CompressorStagePerformance::CompressorStagePerformance(int opPntNumber,
	double pt0,	double pt1,	double pt2,	double pt3,	double pt4,	double tt0,
	double alpha1,	double alpha2,	double alpha3,	double alpha4) {
	_opPntNmbr = opPntNumber;
	_pt0 = pt0;
	_pt1 = pt1;
	_pt2 = pt2;
	_pt3 = pt3;
	_pt4 = pt4;
	_tt0 = tt0;
	_alp1 = alpha1;
	_alp2 = alpha2;
	_alp3 = alpha2;
	_alp4 = alpha3 ;
}

CompressorStagePerformance::~CompressorStagePerformance() {
	// TODO Auto-generated destructor stub
	//printf("called compressor stage performance destructor\n\n");
}

//accessors
int CompressorStagePerformance::getOptPntNmbr() { return _opPntNmbr; }
double CompressorStagePerformance::getPt0() { return _pt0; }
double CompressorStagePerformance::getPt1() { return _pt1; }
double CompressorStagePerformance::getPt2() { return _pt2; }
double CompressorStagePerformance::getPt3() { return _pt3; }
double CompressorStagePerformance::getPt4() { return _pt4; }
double CompressorStagePerformance::getTt0() { return _tt0; }
double CompressorStagePerformance::getAlp1() { return _alp1; }
double CompressorStagePerformance::getAlp2() { return _alp2; }
double CompressorStagePerformance::getAlp3() { return _alp3; }
double CompressorStagePerformance::getAlp4() { return _alp4; }
