/*
 * CompressorTwoDAirfoilConfiguration.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#include "CompressorTwoDAirfoilConfiguration.h"
#include <exception>


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
	AirfoilSectionConfiguration configToReturn = _sectConfigs.at(index);
	return configToReturn;
}

const std::vector<AnnulusPoint > CompressorTwoDAirfoilConfiguration::getLeSectPoints()
{
	std::vector<AnnulusPoint> lePnts;
	std::vector<AirfoilSectionConfiguration>::iterator itSect;
	for ( itSect = _sectConfigs.begin(); itSect != _sectConfigs.end(); ++itSect ) {
		lePnts.push_back( (*itSect).getLePoint() );
	}
	return lePnts;
}

const std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getTeSectPoints()
{
	std::vector<AnnulusPoint> tePnts;
	std::vector<AirfoilSectionConfiguration >::iterator itSect;
	for ( itSect = _sectConfigs.begin(); itSect != _sectConfigs.end(); ++itSect ) {
		tePnts.push_back( (*itSect).getTePoint() );
	}
	return tePnts;
}

const AnnulusPoint CompressorTwoDAirfoilConfiguration::getLeSectPointAtIndex(int index)
{
	AnnulusPoint *lePnt = _sectConfigs.at(index).getLePoint();
	return lePnt;
}

const AnnulusPoint CompressorTwoDAirfoilConfiguration::getTeSectPointAtIndex(int index)
{
	AnnulusPoint *tePnt = _sectConfigs.at(index).getTePoint();
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

const std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getLeSailPoints() { return _sailPntsLe; }
const std::vector<AnnulusPoint> CompressorTwoDAirfoilConfiguration::getTeSailPoints() { return _sailPntsLe; }

const AnnulusPoint CompressorTwoDAirfoilConfiguration::getLeSailPointAtIndex(int index) { return _sailPntsLe.at(index); }
const AnnulusPoint CompressorTwoDAirfoilConfiguration::getTeSailPointAtIndex(int index) { return _sailPntsTe.at(index); }

AirfoilType CompressorTwoDAirfoilConfiguration::getAirfoilType() { return _airfoilType; }
void CompressorTwoDAirfoilConfiguration::setAirfoilType(AirfoilType newAirfoilType) { _airfoilType = newAirfoilType; }

int CompressorTwoDAirfoilConfiguration::getNumAirfoils() { return _numBlades; }
void CompressorTwoDAirfoilConfiguration::setNumAirfoils(int numAirfoils) { _numBlades = numAirfoils; }

std::string CompressorTwoDAirfoilConfiguration::getName() {	return _name; }

double CompressorTwoDAirfoilConfiguration::getXnull() { return _xNull; }
void CompressorTwoDAirfoilConfiguration::setXnull(double newXnull) { _xNull = newXnull; }
