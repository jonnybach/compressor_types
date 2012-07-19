/*
 * CompressorStage.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#include "CompressorStage.h"

CompressorStage::CompressorStage() {
	// TODO Auto-generated constructor stub
}

CompressorStage::CompressorStage(std::string stageName) {
	_stgName = stageName;
}

CompressorStage::~CompressorStage() {
	// TODO Auto-generated destructor stub
}

void CompressorStage::addPerformancePoint(int index, CompressorStagePerformance* perfPntToAdd) {
	//increase capacity if index is greater
	std::vector<CompressorStagePerformance*>::iterator it;

	 it = _optPntPerf.begin();
	_optPntPerf.insert(it+index, perfPntToAdd);

}
