/*
 * Compressor.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "Compressor.h"

#include <sstream>
#include <exception>

#include "Inlet.h"
#include "InletGuideVane.h"
#include "Diffuser.h"
#include "CompressorStage.h"
#include "CompressorAnnulus.h"
#include "LossCorrelationData.h"

Compressor::Compressor() {
	// TODO Auto-generated constructor stub
}

Compressor::~Compressor() {
	// TODO Auto-generated destructor stub
}

CompressorAnnulus *Compressor::getAnnulus() {
	if (!_annulus) {
		_annulus = new CompressorAnnulus();
	}
	return _annulus;
}

void Compressor::setAnnulus(CompressorAnnulus *newAnnulus) {
	delete(_annulus); // I think you have to destroy the original data pointed to otherwise a memory leak occurs
	_annulus = newAnnulus;
}

Inlet *Compressor::getInlet() {
	if (!_inlet) {
		_inlet = new Inlet();
	}
	return _inlet;
}

void Compressor::setInlet(Inlet *newInlet) {
	delete(_inlet); // I think you have to destroy the original data pointed to otherwise a memory leak occurs
	_inlet = newInlet;
}

InletGuideVane *Compressor::getIgv() {
	if (!_igv) {
		_igv = new InletGuideVane();
	}
	return _igv;
}

void Compressor::setIgv(InletGuideVane *newIgv) {
	delete(_igv); // I think you have to destroy the original data pointed to otherwise a memory leak occurs
	_igv = newIgv;
}

Diffuser *Compressor::getDiffuser() {
	if (!_diffuser) {
		_diffuser = new Diffuser(-9999, -9999); //set to dummy values for aid in debugging
	}
	return _diffuser;
}

void Compressor::setDiffuser(Diffuser *newDiffuser) {
	delete(_diffuser); // I think you have to destroy the original data pointed to otherwise a memory leak occurs
	_diffuser = newDiffuser;
}

CompressorOperatingPoint *Compressor::getDesignPnt() {
	return _desPnt;
}

void Compressor::setDesignPnt(CompressorOperatingPoint *newDesignPnt) {
	delete (_desPnt);
	_desPnt = newDesignPnt;
}

LossCorrelationData *Compressor::getLossCorrData() {
	if (!_lossCorrData) {
		_lossCorrData = new LossCorrelationData();
	}
	return _lossCorrData;
}

void Compressor::setLossCorrData(LossCorrelationData *newLossData) {
	delete(_lossCorrData);  // I think you have to destroy the original data pointed to otherwise a memory leak occurs
	_lossCorrData = newLossData;
}

ClearanceSpecification Compressor::getClearanceSpec() {	return _clearncSpec; }
void Compressor::setClearnaceSpec(ClearanceSpecification clearncSpec) {
	if (clearncSpec < CLEARANCE_NONE || clearncSpec > CLEARANCE_ABSOLUTE ) throw std::exception();
	_clearncSpec = clearncSpec;
}

std::vector<CompressorStage *> Compressor::getStages() {
	return _stages;
}

void Compressor::insertStageAtIndex(int stageIndex, CompressorStage *newStage ) {
	_stages.insert(_stages.begin(), newStage);
}

CompressorStage *Compressor::stageWithNumber(int stageNumber) {

	//convert number to string
	std::stringstream ssStgNmbr;
	ssStgNmbr << stageNumber;
	std::string strStgNmbr = ssStgNmbr.str();

	//search for stage by string representation of stage number
	CompressorStage *stageRef = 0;
	stageRef = this->stageWithName(strStgNmbr);
	return stageRef;

}

CompressorStage *Compressor::stageWithName(std::string stageName) {

	bool foundStage = false;
	CompressorStage *stageRef = 0;

	std::vector<CompressorStage *>::iterator it;
	for ( it=_stages.begin() ; it < _stages.end(); it++) {
		stageRef = *it;
		if ( stageRef->getStageName().find(stageName, 0 ) != std::string::npos ) {
			foundStage = true;
			break;  //end looping
		} else {
			stageRef = 0;
		}
	}

	return stageRef;

}

CompressorStage *Compressor::findOrCreateStageWithName(std::string stageName) {
	//loop through stages and compare stage name with argument.  If exists return the stage,
	// if not create a new stage and to the _stages vector and return the pointer

	bool foundStage = false;
	CompressorStage *stageRef = 0;

	std::vector<CompressorStage *>::iterator it;
	for ( it=_stages.begin() ; it < _stages.end(); it++) {
		stageRef = *it;
		if ( stageRef->getStageName().find(stageName, 0 ) != std::string::npos ) {
			foundStage = true;
			break;  //end looping
		} else {
			stageRef = 0;
		}
	}

	if ( !foundStage ) {
		stageRef = new CompressorStage(stageName);
		//add newly created stage to stages vector
		_stages.push_back(stageRef);
	}

	return stageRef;

}

void Compressor::setVgvRuleAtStage(int stageNumber, double ruleFactor) {

	//keep track of the igv in this vane schedule, therefore increase stageNumber by 1 when resizing the vector
	if ( _vaneSchedule.size() < size_t(stageNumber+1) ) {
		_vaneSchedule.resize(stageNumber+1);
	}

	_vaneSchedule.insert(_vaneSchedule.begin()+stageNumber, ruleFactor);

}

double Compressor::getVgvRuleAtStage(int stageNumber) {
	double vgvRule = _vaneSchedule.at(stageNumber);
	return vgvRule;
}
