/*
 * AirfoilSectionConfiguration.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#include "AirfoilSectionConfiguration.h"
#include <exception>
#include <cmath>

AirfoilSectionConfiguration::AirfoilSectionConfiguration()
{
	// TODO Auto-generated constructor stub
}

AirfoilSectionConfiguration::AirfoilSectionConfiguration(double radius, double pqc, double tqc, double aqc
	, double chord, double stagrAng, double betaMtlIn, double betaMtlOut
	, double beta1, double delBeta, double avdr, double machIn, double delDevn, double delLoss )
{
	//_radius = radius;
	_pqc = pqc;
	_tqc = tqc;
	_aqc = aqc;
	_chord = chord;
	_stagrAng = stagrAng;
	_betaMin = betaMtlIn;
	_betaMout = betaMtlOut;
	_beta1 = beta1;
	_delBeta = delBeta;
	_avdr = avdr;
	_machIn = machIn;
	_delDevn = delDevn;
	_delLoss = delLoss;
}

AirfoilSectionConfiguration::AirfoilSectionConfiguration(AnnulusPoint lePoint, AnnulusPoint tePoint,
	double pqc,	double tqc,	double aqc,	double chord, double stagrAng, double betaMtlIn, double betaMtlOut,
	double beta1, double delBeta,	double avdr, double machIn,	double delDevn,	double delLoss )
{
	_lePnt = lePoint;
	_tePnt = tePoint;
	_pqc = pqc;
	_tqc = tqc;
	_aqc = aqc;
	_chord = chord;
	_stagrAng = stagrAng;
	_betaMin = betaMtlIn;
	_betaMout = betaMtlOut;
	_beta1 = beta1;
	_delBeta = delBeta;
	_avdr = avdr;
	_machIn = machIn;
	_delDevn = delDevn;
	_delLoss = delLoss;
}

AirfoilSectionConfiguration::~AirfoilSectionConfiguration()
{
	// TODO Auto-generated destructor stub
}

void AirfoilSectionConfiguration::setHpaConfig(double beta1, double delBeta
		, double tqc, double avdr, double machIn)
{
	_beta1 = beta1;
	_delBeta = delBeta;
	_tqc = tqc;
	_avdr = avdr;
	_machIn = machIn;
}

AnnulusPoint AirfoilSectionConfiguration::getLePoint() const { return _lePnt; }
void AirfoilSectionConfiguration::setLePoint( AnnulusPoint newLePnt )
{
	//delete(_lePnt);
	_lePnt = newLePnt;
}

AnnulusPoint AirfoilSectionConfiguration::getTePoint() const { return _tePnt; }
void AirfoilSectionConfiguration::setTePoint( AnnulusPoint newTePnt )
{
	//delete(_tePnt);
	_tePnt = newTePnt;
}

double AirfoilSectionConfiguration::getRadiusDelta() const {
	double r_le = _lePnt.getRadius();
	double r_te = _tePnt.getRadius();
	double delR = std::abs(r_le - r_te);
	return delR;
}

double AirfoilSectionConfiguration::getRadiusMean() const
{
	double r_le = _lePnt.getRadius();
	double r_te = _tePnt.getRadius();
	double r_mean = (r_le + r_te)/2;
	return r_mean;
}

//double AirfoilSectionConfiguration::getRadius() const { return _radius; }

double AirfoilSectionConfiguration::getPqC() const { return _pqc; }
double AirfoilSectionConfiguration::getTqC() const { return _tqc; }
double AirfoilSectionConfiguration::getAqC() const { return _aqc; }

double AirfoilSectionConfiguration::getThickness() const { return _tqc * _chord; }
double AirfoilSectionConfiguration::getChord() const { return _chord; }
double AirfoilSectionConfiguration::getAxialChord() const {
	double axChrd = std::abs( _tePnt.getX() - _lePnt.getX() );
	return axChrd;
}

double AirfoilSectionConfiguration::getStagrAng() const { return _stagrAng; }
double AirfoilSectionConfiguration::getBetaMtlIn() const { return _betaMin; }
double AirfoilSectionConfiguration::getBetaMtlOut() const { return _betaMout; }

double AirfoilSectionConfiguration::getBeta1() const { return _beta1; }
double AirfoilSectionConfiguration::getDelBeta() const { return _delBeta; }
double AirfoilSectionConfiguration::getAvdr() const { return _avdr; }
double AirfoilSectionConfiguration::getMachIn() const { return _machIn; }

double AirfoilSectionConfiguration::getDelDevn() const { return _delDevn; }
double AirfoilSectionConfiguration::getDelLoss() const { return _delLoss; }
