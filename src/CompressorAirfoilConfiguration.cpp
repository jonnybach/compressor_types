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
		, double betaInAir, double delBetaAir, double avdr, double machIn, double delDevn, double delLoss
		, double bleedFrac ) {

	m_airfoilType = airfoilType;
	m_numBlades = numBlades;
	m_sqc = sqc;
	m_tqc = tqc;
	m_aqc = aqc;
	m_oqs = oqs;
	m_shapeK = shapeK;
	m_stagrAng = stagrAng;
	m_betaMin = betaMtlIn;
	m_betaMout = betaMtlOut;
	m_tipClrToHgt = tipClrToHgt;

	m_betaIn = betaInAir;
	m_delBeta = delBetaAir;
	m_avdr = avdr;
	m_machIn = machIn;
	m_delDevn = delDevn;
	m_delLoss = delLoss;

	//m_bleedFrac = bleedFrac;
	//if (bleedFrac != 0.0) {
		//using property so that the hasBleed flag is also set to true
		this->setBleedFrac(bleedFrac);
	//}
}

CompressorAirfoilConfiguration::~CompressorAirfoilConfiguration() {
	// TODO Auto-generated destructor stub
}

void CompressorAirfoilConfiguration::setDesignedParams(double sqc, double tqc, double betaMtlIn, double betaMtlOut, double cambrAng, double stagrAng) {
	m_sqc = sqc;
	m_tqc = tqc;
	m_betaMin = betaMtlIn;
	m_betaMout = betaMtlOut;
	m_camberAng = cambrAng;
	m_stagrAng = stagrAng;
}

void CompressorAirfoilConfiguration::setHpaConfig(double betaInAir, double delBetaAir, double tqc, double avdr, double machIn) {
	m_betaIn = betaInAir;
	m_delBeta = delBetaAir;
	m_tqc = tqc;
	m_avdr = avdr;
	m_machIn = machIn;
}

AirfoilType CompressorAirfoilConfiguration::getAirfoilType() { return m_airfoilType; }

int CompressorAirfoilConfiguration::getNumBlades() { return m_numBlades; }

double CompressorAirfoilConfiguration::getSqc() { return m_sqc; }
double CompressorAirfoilConfiguration::getTqc() { return m_tqc; }
double CompressorAirfoilConfiguration::getAqc() { return m_aqc; }
double CompressorAirfoilConfiguration::getOqs() { return m_oqs; }

double CompressorAirfoilConfiguration::getShapeK() { return m_shapeK; }

double CompressorAirfoilConfiguration::getStagrAng() { return m_stagrAng; }
double CompressorAirfoilConfiguration::getBetaMtlIn() { return m_betaMin; }
double CompressorAirfoilConfiguration::getBetaMtlOut() { return m_betaMout; }

double CompressorAirfoilConfiguration::getTipClrToHgt() { return m_tipClrToHgt; }

double CompressorAirfoilConfiguration::getBetaIn() { return m_betaIn; }
double CompressorAirfoilConfiguration::getDelBetaM() { return m_delBeta; }
double CompressorAirfoilConfiguration::getAvdr() { return m_avdr; }
double CompressorAirfoilConfiguration::getMachIn() { return m_machIn; }

double CompressorAirfoilConfiguration::getDelDevn() { return m_delDevn; }
double CompressorAirfoilConfiguration::getDelLoss() { return m_delLoss; }

//double CompressorAirfoilConfiguration::getDsnStagePratio() { return _stagePratio; }
//double CompressorAirfoilConfiguration::getDsnAlphaExit() { return _alphaExit; }

bool CompressorAirfoilConfiguration::hasBleed() { return m_hasBleed; }
double CompressorAirfoilConfiguration::getBleedFrac() { return m_bleedFrac; }
void CompressorAirfoilConfiguration::setBleedFrac(double bleedFraction) {
	if (bleedFraction != 0.0) {
		m_bleedFrac = bleedFraction;
		m_hasBleed = true;
	} else {
		m_bleedFrac = 0.0;
		m_hasBleed = false;
	}
}

AnnulusPoint *CompressorAirfoilConfiguration::getHubLe() { return m_hubLe; }
AnnulusPoint *CompressorAirfoilConfiguration::getHubTe() { return m_hubTe; }
AnnulusPoint *CompressorAirfoilConfiguration::getCaseLe() { return m_caseLe; }
AnnulusPoint *CompressorAirfoilConfiguration::getCaseTe() { return m_caseTe; }

void CompressorAirfoilConfiguration::setSailPoints( AnnulusPoint *hubLe, AnnulusPoint *hubTe, AnnulusPoint *caseLe, AnnulusPoint *caseTe ) {
	//delete(_hubLe);
	//delete(_hubTe);
	//delete(_caseLe);
	//delete(_caseTe);
	m_hubLe = hubLe;
	m_hubTe = hubTe;
	m_caseLe = caseLe;
	m_caseTe = caseTe;
}

double CompressorAirfoilConfiguration::calcXnull() {

	if ( !m_hubTe || !m_hubLe || !m_caseTe || !m_caseLe ) { throw std::exception(); }

	double xh1, xh2, xc1, xc2; //axial location of hub and casing sail points
	xh1 = m_hubLe->getX();
	xh2 = m_hubTe->getX();
	xc1 = m_caseLe->getX();
	xc2 = m_caseTe->getX();

	double xavg = (xh1 + xh2 + xc1 + xc2)/4;
	return xavg;
}
