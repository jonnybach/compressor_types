/*
 * CompressorTwoDAirfoilConfiguration.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#include "CompressorTwoDAirfoilConfiguration.h"
#include <exception>
#include <cmath>

//ctors and dtors

CompressorTwoDAirfoilConfiguration::CompressorTwoDAirfoilConfiguration()
{
	// TODO Auto-generated constructor stub
}

CompressorTwoDAirfoilConfiguration::CompressorTwoDAirfoilConfiguration(std::string airfoilName, AirfoilBehavior airfoilBehr)
{
	_name = airfoilName;
	_airfoilBhvr = airfoilBehr;
}

CompressorTwoDAirfoilConfiguration::~CompressorTwoDAirfoilConfiguration()
{
	// TODO Auto-generated destructor stub
}


void CompressorTwoDAirfoilConfiguration::addSectionConfiguration(AirfoilSectionConfiguration newSectConfig)
{
	_sectConfigs.push_back(newSectConfig);
}

AirfoilSectionConfiguration CompressorTwoDAirfoilConfiguration::getSectionConfigAtIndex(int index)
{
	//may not be the best place to do this but update the le and te
	//  points for each section from the sail point vectors
	//this->updateSectionLeAndTePnts();

	AirfoilSectionConfiguration configToReturn = _sectConfigs.at(index);
	return configToReturn;
}

std::vector<AirfoilSectionConfiguration> CompressorTwoDAirfoilConfiguration::getSections()
{
	//may not be the best place to do this but update the le and te
	//  points for each section from the sail point vectors
	//this->updateSectionLeAndTePnts();
	return _sectConfigs;
}

const AirfoilSectionConfiguration *CompressorTwoDAirfoilConfiguration::getSectionNearSpanFraction(double frac)
{

	//may not be the best place to do this but update the le and te
	//  points for each section from the sail point vectors
	//this->updateSectionLeAndTePnts();

	//double rHub = _sectConfigs.at(0).getRadiusMean();
	//double rTip = _sectConfigs.at(_sectConfigs.size()-1).getRadiusMean();
	double rHub = (_sectConfigs.front()).getRadiusMean();
	double rTip = (_sectConfigs.back()).getRadiusMean();  //Had to enclose call to back in parentheses.

	double span = std::abs( rTip - rHub );
	double spanFrac;

	double delta = 1e12;
	double smallestDelta = delta;

	AirfoilSectionConfiguration sect;
	AirfoilSectionConfiguration *sectToReturn = 0;

	std::vector<AirfoilSectionConfiguration>::iterator itSect;
	for ( itSect = _sectConfigs.begin(); itSect != _sectConfigs.end(); ++itSect ) {
		sect = (*itSect);
		spanFrac = std::abs( (*itSect).getRadiusMean() - rHub) / span;
		delta = std::abs( spanFrac - frac );
		//smallestDelta = ( delta < smallestDelta ) ? delta : smallestDelta;
		if (delta < smallestDelta) {
			smallestDelta = delta;
			sectToReturn = &(*itSect);
		}

	}

	return sectToReturn;
}

const AirfoilSectionConfiguration *CompressorTwoDAirfoilConfiguration::getSectionAtMidspan()
{
	return this->getSectionNearSpanFraction(0.5);
}

const AirfoilSectionConfiguration *CompressorTwoDAirfoilConfiguration::getSectionAtHub()
{
	return this->getSectionNearSpanFraction(0.0);
}

const AirfoilSectionConfiguration *CompressorTwoDAirfoilConfiguration::getSectionAtTip()
{
	return this->getSectionNearSpanFraction(1.0);
}


std::vector<AnnulusPoint > CompressorTwoDAirfoilConfiguration::getLeSectPoints()
{
	std::vector<AnnulusPoint> lePnts;
	std::vector<AirfoilSectionConfiguration>::iterator itSect;
	for ( itSect = _sectConfigs.begin(); itSect != _sectConfigs.end(); ++itSect ) {
		lePnts.push_back( (*itSect).getLePoint() );
	}
	return lePnts;
}

std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getTeSectPoints()
{
	std::vector<AnnulusPoint> tePnts;
	std::vector<AirfoilSectionConfiguration >::iterator itSect;
	for ( itSect = _sectConfigs.begin(); itSect != _sectConfigs.end(); ++itSect ) {
		tePnts.push_back( (*itSect).getTePoint() );
	}
	return tePnts;
}

AnnulusPoint CompressorTwoDAirfoilConfiguration::getSectLePointAtIndex(int index)
{
	AnnulusPoint lePnt = _sectConfigs.at(index).getLePoint();
	return lePnt;
}

AnnulusPoint CompressorTwoDAirfoilConfiguration::getSectTePointAtIndex(int index)
{
	AnnulusPoint tePnt = _sectConfigs.at(index).getTePoint();
	return tePnt;
}

size_t CompressorTwoDAirfoilConfiguration::maxSectNo()
{
	size_t numSects = _sectConfigs.size();
	return numSects;
}

void CompressorTwoDAirfoilConfiguration::setLeSailPoints( const std::vector<AnnulusPoint> newLePoints ) {
	_sailPntsLe.clear();

	std::vector<AnnulusPoint>::const_iterator it;

	for ( it = newLePoints.begin(); it != newLePoints.end(); ++it ) {
		_sailPntsLe.push_back( *it );
	}
}

void CompressorTwoDAirfoilConfiguration::setTeSailPoints( const std::vector<AnnulusPoint> newTePoints ) {
	_sailPntsTe.clear();
	std::vector<AnnulusPoint>::const_iterator it;
	for ( it = newTePoints.begin(); it != newTePoints.end(); ++it ) {
		_sailPntsTe.push_back( *it );
	}
}

std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getLeSailPoints() { return _sailPntsLe; }
std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getTeSailPoints() { return _sailPntsTe; }

AnnulusPoint CompressorTwoDAirfoilConfiguration::getLeSailPointAtIndex(int index) { return _sailPntsLe.at(index); }
AnnulusPoint CompressorTwoDAirfoilConfiguration::getTeSailPointAtIndex(int index) { return _sailPntsTe.at(index); }

AirfoilBehavior CompressorTwoDAirfoilConfiguration::getAirfoilBehavior() { return _airfoilBhvr; }
void CompressorTwoDAirfoilConfiguration::setAirfoiilBehavior(AirfoilBehavior newBehavior) { _airfoilBhvr = newBehavior; }

AirfoilType CompressorTwoDAirfoilConfiguration::getAirfoilType() { return _airfoilType; }
void CompressorTwoDAirfoilConfiguration::setAirfoilType(AirfoilType newAirfoilType) { _airfoilType = newAirfoilType; }

int CompressorTwoDAirfoilConfiguration::getNumAirfoils() { return _numBlades; }
void CompressorTwoDAirfoilConfiguration::setNumAirfoils(int numAirfoils) { _numBlades = numAirfoils; }

std::string CompressorTwoDAirfoilConfiguration::getName() {	return _name; }
void CompressorTwoDAirfoilConfiguration::setName(std::string newName) { _name = newName; }

double CompressorTwoDAirfoilConfiguration::getXnull() { return _xNull; }
void CompressorTwoDAirfoilConfiguration::setXnull(double newXnull) { _xNull = newXnull; }

double CompressorTwoDAirfoilConfiguration::getRotSpeed() { return _rotSpeed; }
void CompressorTwoDAirfoilConfiguration::setRotSpeed(double newRotSpeed) { _rotSpeed = newRotSpeed; }

double CompressorTwoDAirfoilConfiguration::getSpan()
{
	double span = -9999;
	if  ( !_sectConfigs.empty() ) {
		double rHub = (*_sectConfigs.begin()).getRadiusMean();
		double rTip = (*_sectConfigs.end()).getRadiusMean();
		span = std::abs( rTip - rHub );
	}
	return span; //meters
}

double CompressorTwoDAirfoilConfiguration::getMassAveTempRecov()
{

    double massFlow;
    double num;

    double sumNum = 0;
    double sumMass = 0;

    AirfoilSectionConfiguration sect;

    std::vector<AirfoilSectionConfiguration>::iterator itSect;
    for ( size_t i = 1; i < _sectConfigs.size(); ++i ) {
        sect = _sectConfigs.at(i);
        massFlow = sect.getResult().getMassFlow();
        num = (sect.getResult().getTempRecov() + _sectConfigs.at(i-1).getResult().getTempRecov()) / 2.0 * massFlow;

        sumNum += num;
        sumMass += massFlow;

    }

    double aveTemp = sumNum/sumMass;
    return aveTemp;
}

/*
void CompressorTwoDAirfoilConfiguration::updateSectionLeAndTePnts()
{

	//check that the airfoil configurations vector contains entries, otherwise
	// leave method
	if  ( _sectConfigs.empty() ) return;

	//check that number of le and te sail points equals number of sections
	if ( _sailPntsLe.size() != _sectConfigs.size() ) { throw std::exception(); }
	if ( _sailPntsTe.size() != _sectConfigs.size() ) { throw std::exception(); }

	const std::string idLbl("ID");
	const std::string odLbl("OD");

	bool listStartsAtId;

	size_t iSect;
	std::vector<AnnulusPoint>::iterator itAp;


	// ***************  ASSIGN LE POINTS  *********************

	//check if first point in LE sail points is OD or ID
	//if ( (*_sailPntsLe.begin()).getLabel().find(idLbl) != std::string::npos ) {
	if ( _sailPntsLe.front().getRadius() < _sailPntsLe.back().getRadius() ) {
		listStartsAtId = true;
	//} else if ( (*_sailPntsLe.begin()).getLabel().find(odLbl) != std::string::npos ) {
	} else {
		listStartsAtId = false;
	//} else {
	//	throw std::exception();  //labeling is not following expected format somethings wrong
	}

	//assign each LE sail point to appropriate section config
	iSect = 0;
	for ( itAp = _sailPntsLe.begin(); itAp != _sailPntsLe.end(); ++itAp) {
		if ( listStartsAtId ) {
			(*(_sectConfigs.begin()+iSect)).setLePoint( (*itAp) );
		} else {
			(*(_sectConfigs.rbegin()+iSect)).setLePoint( (*itAp) );
		}
		iSect++;
	}


	// ***************  ASSIGN TE POINTS  *********************

	//check if first point in LE sail points is OD or ID
	iSect = 0;
	//if ( (*_sailPntsTe.begin()).getLabel().find(idLbl) != std::string::npos ) {
	AnnulusPoint frt = _sailPntsTe.front();
	AnnulusPoint bck = _sailPntsTe.back();
	if ( _sailPntsTe.front().getRadius() < _sailPntsTe.back().getRadius() ) {
		listStartsAtId = true;
	} else {
	//} else if ( (*_sailPntsTe.begin()).getLabel().find(odLbl) != std::string::npos ) {
		listStartsAtId = false;
	//} else {
	//	throw std::exception();  //labeling is not following expected format somethings wrong
	}

	//assign each TE sail point to appropriate section config
	for ( itAp = _sailPntsTe.begin(); itAp != _sailPntsTe.end(); ++itAp) {
		if ( listStartsAtId ) {
			(*(_sectConfigs.begin()+iSect)).setTePoint( (*itAp) );
		} else {
			(*(_sectConfigs.rbegin()+iSect)).setTePoint( (*itAp) );
		}
		iSect++;
	}
}
*/
