/*
 * CompressorStage.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#include "CompressorStage.h"
#include "CompressorAnnulus.h"
#include "CompressorAirfoilConfiguration.h"
#include "StringTrimmers.h"

#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>

CompressorStage::CompressorStage() {
	// TODO Auto-generated constructor stub
}

CompressorStage::CompressorStage(std::string stageName) {
	_stgName = stageName;
}

CompressorStage::~CompressorStage() {
	// TODO Auto-generated destructor stub
	//printf("called compressor stage destructor\n\n");
}

void CompressorStage::addPerformancePoint(int index, CompressorStagePerformance perfPntToAdd) {
	//increase capacity if index is greater
	std::vector<CompressorStagePerformance>::iterator it;

	 it = _optPntPerf.begin();
	_optPntPerf.insert(it+index, perfPntToAdd);

}

//accessor methods
std::vector <CompressorStagePerformance> CompressorStage::getOpPntPerf() {	return _optPntPerf; }

CompressorAirfoilConfiguration *CompressorStage::getRotor() { return _rotorConfig; }

void CompressorStage::setRotor(CompressorAirfoilConfiguration *newRotor) {
	_rotorConfig = newRotor;
}

CompressorAirfoilConfiguration *CompressorStage::getStator() { return _statorConfig; }

void CompressorStage::setStator(CompressorAirfoilConfiguration *newStator) {
	_statorConfig = newStator;
}

std::string CompressorStage::getStageName() {
	std::string rtrnStageName;
	char cStgName[3];
	int intStgRep = atoi( _stgName.c_str() );
	if ( intStgRep != 0 ) {
		sprintf( cStgName, "%2.2i", intStgRep );
		rtrnStageName = std::string(cStgName);
	} else {
		rtrnStageName = _stgName;
	}
	return rtrnStageName;
}

double CompressorStage::calcXnullRotor(CompressorAnnulus *annulus) {

	return _rotorConfig->calcXnull();

	/*
	int stgNumb = atoi( _stgName.c_str() );
	int pntIndxLe, pntIndxTe;

	pntIndxLe = (stgNumb - 1) * 4;
	pntIndxTe = (stgNumb - 1) * 4 + 1;

	double xh1, xh2, xc1, xc2; //axial location of hub and casing sail points
	xh1 = annulus->hubPointAtIndex( pntIndxLe )->getX();
	xh2 = annulus->hubPointAtIndex( pntIndxTe )->getX();
	xc1 = annulus->casePointAtIndex( pntIndxLe )->getX();
	xc2 = annulus->casePointAtIndex( pntIndxTe )->getX();

	double xavg = (xh1 + xh2 + xc1 + xc2)/4;
	return xavg;
	 */

}

double CompressorStage::calcXnullStator(CompressorAnnulus *annulus) {

	return _statorConfig->calcXnull();

	/*
	//TODO: bad implementation, come up with a better way to index stage numbers
	// since some stages are not named by number like OGV
	int pntIndxLe, pntIndxTe;
	if (( _stgName.find("IGV") != std::string::npos || _stgName.find("igv") != std::string::npos )) {
		throw std::exception();
		pntIndxLe = 0; //this won't be right if inlet points are added
		pntIndxTe = 1;
	} else if ( _stgName.find("OGV") != std::string::npos || _stgName.find("ogv") != std::string::npos ) {
		pntIndxLe = annulus->getHubProfile().size()-1;
		pntIndxTe = annulus->getHubProfile().size()-2;
	} else {
		int stgNumb = atoi( _stgName.c_str() );
		pntIndxLe = (stgNumb - 1) * 4 + 2;
		pntIndxTe = (stgNumb - 1) * 4 + 3;
	}


	double xh1, xh2, xc1, xc2; //axial location of hub and casing sail points
	xh1 = annulus->hubPointAtIndex( pntIndxLe )->getX();
	xh2 = annulus->hubPointAtIndex( pntIndxTe )->getX();
	xc1 = annulus->casePointAtIndex( pntIndxLe )->getX();
	xc2 = annulus->casePointAtIndex( pntIndxTe )->getX();

	double xavg = (xh1 + xh2 + xc1 + xc2)/4;
	return xavg;
	*/
}
