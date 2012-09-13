/*
 * CompressorAirfoilConfiguration.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "CompressorAirfoilConfiguration.h"
#include "AnnulusPoint.h"
#include <exception>

CompressorAirfoilConfiguration::CompressorAirfoilConfiguration() {
	// TODO Auto-generated constructor stub
}

/*
CompressorAirfoilConfiguration::CompressorAirfoilConfiguration(
	AirfoilType airfoilType, int numBlades, double sqc, double tqc, double aqc, double oqs, double shapeK
	, double stagrAng, double betaMtlIn, double betaMtlOut, double tipClrToHgt
	, double delBetaM, double avdr, double machIn, double delDevn, double delLoss
	, double dsnStagePratio, double dsnAlphaExit, double bleedFrac ) {
*/
CompressorAirfoilConfiguration::CompressorAirfoilConfiguration(
		AirfoilType airfoilType, int numBlades, double sqc, double tqc, double aqc, double oqs, double shapeK
		, double stagrAng, double betaMtlIn, double betaMtlOut, double tipClrToHgt
		, double delBetaM, double avdr, double machIn, double delDevn, double delLoss
		, double bleedFrac ) {

	_airfoilType = airfoilType;
	_numBlades = numBlades;
	_sqc = sqc;
	_tqc = tqc;
	_aqc = aqc;
	_oqs = oqs;
	_shapeK = shapeK;
	_stagrAng = stagrAng;
	_betaMin = betaMtlIn;
	_betaMout = betaMtlOut;
	_tipClrToHgt = tipClrToHgt;
	_delBetaM = delBetaM;
	_avdr = avdr;
	_machIn = machIn;
	_delDevn = delDevn;
	_delLoss = delLoss;
	_bleedFrac = bleedFrac;
}

CompressorAirfoilConfiguration::~CompressorAirfoilConfiguration() {
	// TODO Auto-generated destructor stub
}

void CompressorAirfoilConfiguration::setHpaConfig(double beta1metal, double delBetaM, double tqc, double avdr, double machIn) {
	_betaMin = beta1metal;
	_delBetaM = delBetaM;
	_tqc = tqc;
	_avdr = avdr;
	_machIn = machIn;
}

AirfoilType CompressorAirfoilConfiguration::getAirfoilType() { return _airfoilType; }

int CompressorAirfoilConfiguration::getNumBlades() { return _numBlades; }

double CompressorAirfoilConfiguration::getSqc() { return _sqc; }
double CompressorAirfoilConfiguration::getTqc() { return _tqc; }
double CompressorAirfoilConfiguration::getAqc() { return _aqc; }
double CompressorAirfoilConfiguration::getOqs() { return _oqs; }

double CompressorAirfoilConfiguration::getShapeK() { return _shapeK; }

double CompressorAirfoilConfiguration::getStagrAng() { return _stagrAng; }
double CompressorAirfoilConfiguration::getBetaMtlIn() { return _betaMin; }
double CompressorAirfoilConfiguration::getBetaMtlOut() { return _betaMout; }

double CompressorAirfoilConfiguration::getTipClrToHgt() { return _tipClrToHgt; }

double CompressorAirfoilConfiguration::getDelBetaM() { return _delBetaM; }
double CompressorAirfoilConfiguration::getAvdr() { return _avdr; }
double CompressorAirfoilConfiguration::getMachIn() { return _machIn; }

double CompressorAirfoilConfiguration::getDelDevn() { return _delDevn; }
double CompressorAirfoilConfiguration::getDelLoss() { return _delLoss; }

//double CompressorAirfoilConfiguration::getDsnStagePratio() { return _stagePratio; }
//double CompressorAirfoilConfiguration::getDsnAlphaExit() { return _alphaExit; }

double CompressorAirfoilConfiguration::getBleedFrac() { return _bleedFrac; }
void CompressorAirfoilConfiguration::setBleedFrac(double bleedFraction) {
	_bleedFrac = bleedFraction;
}

AnnulusPoint *CompressorAirfoilConfiguration::getHubLe() { return _hubLe; }
AnnulusPoint *CompressorAirfoilConfiguration::getHubTe() { return _hubTe; }
AnnulusPoint *CompressorAirfoilConfiguration::getCaseLe() { return _caseLe; }
AnnulusPoint *CompressorAirfoilConfiguration::getCaseTe() { return _caseTe; }

void CompressorAirfoilConfiguration::setSailPoints( AnnulusPoint *hubLe, AnnulusPoint *hubTe, AnnulusPoint *caseLe, AnnulusPoint *caseTe ) {
	//delete(_hubLe);
	//delete(_hubTe);
	//delete(_caseLe);
	//delete(_caseTe);
	_hubLe = hubLe;
	_hubTe = hubTe;
	_caseLe = caseLe;
	_caseTe = caseTe;
}

double CompressorAirfoilConfiguration::calcXnull() {

	if ( !_hubTe || !_hubLe || !_caseTe || !_caseLe ) { throw std::exception(); }

	double xh1, xh2, xc1, xc2; //axial location of hub and casing sail points
	xh1 = _hubLe->getX();
	xh2 = _hubTe->getX();
	xc1 = _caseLe->getX();
	xc2 = _caseTe->getX();

	double xavg = (xh1 + xh2 + xc1 + xc2)/4;
	return xavg;
}
