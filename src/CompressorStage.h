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

class CompressorStage {
public:
	//methods
	CompressorStage();
	CompressorStage(std::string stageName);
	virtual ~CompressorStage();

	void addPerformancePoint(int index, CompressorStagePerformance perfPntToAdd);

	//accessors/properties
	std::vector <CompressorStagePerformance> getOpPntPerf();

private:
	//members
	std::string _stgName;
	std::vector <CompressorStagePerformance> _optPntPerf;

	//stage geometry
	//stage input data

};

#endif /* COMPRESSORSTAGE_H_ */
