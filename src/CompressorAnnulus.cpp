/*
 * CompressorAnnulus.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "CompressorAnnulus.h"
#include <exception>

CompressorAnnulus::CompressorAnnulus() {
	// TODO Auto-generated constructor stub

}

CompressorAnnulus::~CompressorAnnulus() {
	// TODO Auto-generated destructor stub
}

void CompressorAnnulus::insertHubPointAtIndex(size_t pntIndex, AnnulusPoint * newHubPnt) {
	_hubProfile.insert( _hubProfile.begin()+pntIndex, newHubPnt);
}

void CompressorAnnulus::insertCasePointAtIndex(size_t pntIndex, AnnulusPoint * newCasePnt) {
	_caseProfile.insert( _caseProfile.begin()+pntIndex, newCasePnt);
}

void CompressorAnnulus::addHubPoint(AnnulusPoint * newHubPnt) {
	_hubProfile.push_back(newHubPnt);
}

void CompressorAnnulus::addCasePoint(AnnulusPoint * newCasePnt) {
	_caseProfile.push_back(newCasePnt);
}

std::vector<AnnulusPoint *> CompressorAnnulus::getHubProfile() { return _hubProfile; }
std::vector<AnnulusPoint *> CompressorAnnulus::getCaseProfile() { return _caseProfile; }
size_t CompressorAnnulus::getNumProfilePnts() {
	size_t numHubPnts = _hubProfile.size();
	size_t numCasePnts = _hubProfile.size();
	if ( numHubPnts != numCasePnts ) { throw std::exception(); }
	return numHubPnts;
}

AnnulusPoint* CompressorAnnulus::hubPointAtIndex(int index) {

	AnnulusPoint *hubPnt =_hubProfile.at(index);
	return hubPnt;

}

AnnulusPoint* CompressorAnnulus::casePointAtIndex(int index) {
	AnnulusPoint *casePnt =_caseProfile.at(index);
	return casePnt;
}
