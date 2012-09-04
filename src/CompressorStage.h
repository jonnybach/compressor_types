/*
 * CompressorStage.h
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSTAGE_H_
#define COMPRESSORSTAGE_H_

#include <string>
#include <vector>
#include "CompressorStagePerformance.h"

class CompressorAirfoilConfiguration;
class CompressorAnnulus;

class CompressorStage {
public:
	//methods
	CompressorStage();
	CompressorStage(std::string stageName);
	virtual ~CompressorStage();

	void addPerformancePoint(int index, CompressorStagePerformance perfPntToAdd);

	//accessors/properties
	std::vector <CompressorStagePerformance> getOpPntPerf();

	CompressorAirfoilConfiguration *getRotor();
	void setRotor(CompressorAirfoilConfiguration *newRotor);

	CompressorAirfoilConfiguration *getStator();
	void setStator(CompressorAirfoilConfiguration *newStator);

	std::string getStageName();

	double calcXnullRotor(CompressorAnnulus *annulus);
	double calcXnullStator(CompressorAnnulus *annulus);

private:
	//members
	std::string _stgName;
	std::vector <CompressorStagePerformance> _optPntPerf;

	CompressorAirfoilConfiguration *_rotorConfig;
	CompressorAirfoilConfiguration *_statorConfig;

	double _stgPratio;  //on same line as rotor geometry config

};

#endif /* COMPRESSORSTAGE_H_ */
