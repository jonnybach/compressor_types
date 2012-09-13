/*
 * AirfoilSectionConfiguration.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#include "AirfoilSectionConfiguration.h"
#include "AnnulusPoint.h"
#include <exception>

AirfoilSectionConfiguration::AirfoilSectionConfiguration()
{
	// TODO Auto-generated constructor stub
}

AirfoilSectionConfiguration::AirfoilSectionConfiguration(double radius, double pqc, double tqc, double aqc
	, double chord, double stagrAng, double betaMtlIn, double betaMtlOut
	, double beta1, double delBetaM, double avdr, double machIn, double delDevn, double delLoss )
{
	_radius = radius;
	_pqc = pqc;
	_tqc = tqc;
	_aqc = aqc;
	_chord = chord;
	_stagrAng = stagrAng;
	_betaMin = betaMtlIn;
	_betaMout = betaMtlOut;
	_beta1 = beta1;
	_delBetaM = delBetaM;
	_avdr = avdr;
	_machIn = machIn;
	_delDevn = delDevn;
	_delLoss = delLoss;
}

AirfoilSectionConfiguration::~AirfoilSectionConfiguration()
{
	// TODO Auto-generated destructor stub
}

void AirfoilSectionConfiguration::setHpaConfig(double beta1metal, double delBetaM
		, double tqc, double avdr, double machIn)
{
	_betaMin = beta1metal;
	_delBetaM = delBetaM;
	_tqc = tqc;
	_avdr = avdr;
	_machIn = machIn;
}

AnnulusPoint *AirfoilSectionConfiguration::getLePoint() { return _lePnt; }
void AirfoilSectionConfiguration::setLePoint( AnnulusPoint *newLePnt )
{
	delete(_lePnt);
	_lePnt = newLePnt;
}

AnnulusPoint *AirfoilSectionConfiguration::getTePoint() { return _tePnt; }
void AirfoilSectionConfiguration::setTePoint( AnnulusPoint *newTePnt )
{
	delete(_tePnt);
	_tePnt = newTePnt;
}

double AirfoilSectionConfiguration::getRadius() { return _radius; }

double AirfoilSectionConfiguration::getPqC() { return _pqc; }
double AirfoilSectionConfiguration::getTqC() { return _tqc; }
double AirfoilSectionConfiguration::getAqC() { return _aqc; }

double AirfoilSectionConfiguration::getThickness() { return _tqc * _chord; }
double AirfoilSectionConfiguration::getChord() { return _chord; }

double AirfoilSectionConfiguration::getStagrAng() { return _stagrAng; }
double AirfoilSectionConfiguration::getBetaMtlIn() { return _betaMin; }
double AirfoilSectionConfiguration::getBetaMtlOut() { return _betaMout; }

double AirfoilSectionConfiguration::getDelBetaM() { return _delBetaM; }
double AirfoilSectionConfiguration::getAvdr() { return _avdr; }
double AirfoilSectionConfiguration::getMachIn() { return _machIn; }

double AirfoilSectionConfiguration::getDelDevn() { return _delDevn; }
double AirfoilSectionConfiguration::getDelLoss() { return _delLoss; }
