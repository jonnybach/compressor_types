/*
 * CompressorSpeedLine.h
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSPEEDLINE_H_
#define COMPRESSORSPEEDLINE_H_

#include <vector>
#include "CompressorOperatingPoint.h"

class CompressorSpeedLine {

public:
	//methods
	CompressorSpeedLine();
	virtual ~CompressorSpeedLine();

	void addOperatingPoint(CompressorOperatingPoint opPntToAdd);
	double calcMassFlow(double pressureRatio);
	double calcEtaAdi(double pressureRatio);
	void calcMassAndEta(double pressureRatio, double *wCorrctIn, double *etaAdiab);

	//shaft speed accessors
	double getShaftSpeed();
	void setShaftSpeed(double shaftSpeed);

private:
	//members
	std::vector<CompressorOperatingPoint> _opPnts;
	double _shaftSpeed; //rpm

};

#endif /* COMPRESSORSPEEDLINE_H_ */
