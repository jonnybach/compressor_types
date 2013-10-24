/*
 * CompressorOperatingPoint.cpp
 *
 *  Created on: Apr 30, 2012
 *      Author: bachm03j
 */

#include <fstream>
#include "CompressorOperatingPoint.h"

CompressorOperatingPoint::CompressorOperatingPoint() {
	// TODO Auto-generated constructor stub
}

CompressorOperatingPoint::CompressorOperatingPoint(int opPntNmbr, double inletTemp, double inletPress
		, double phi, double pressRatio, double delTqT, double etaAdi, double etaPoly, double wCorct
		, double wCorctOut, double wInlet, double speed) {

	_opPntNmbr = opPntNmbr;
	_tempIn = inletTemp;
	_pressIn = inletPress;
	_pressRatio = pressRatio;
	_phi = phi;
	_delTqT = delTqT;
	_etaAdi = etaAdi;
	_etaPoly = etaPoly;
	_wCorct = wCorct;
	_wCorctOut = wCorctOut;
	_wInlet = wInlet;
	_speed = speed;

}

CompressorOperatingPoint::~CompressorOperatingPoint() {
	// TODO Auto-generated destructor stub
	//printf("called compressor operating point destructor\n\n");
}

void CompressorOperatingPoint::setOperatingPoint(int opPntNmbr, double inletTemp, double inletPress
		, double pressRatio, double phi, double delTqT, double etaAdi, double etaPoly, double wCorct, double wCorctOut
		, double wInlet, double speed) {

	_opPntNmbr = opPntNmbr;
	_tempIn = inletTemp;
	_pressIn = inletPress;
	_pressRatio = pressRatio;
	_phi = phi;
	_delTqT = delTqT;
	_etaAdi = etaAdi;
	_etaPoly = etaPoly;
	_wCorct = wCorct;
	_wCorctOut = wCorctOut;
	_wInlet = wInlet;
	_speed = speed;

}

double CompressorOperatingPoint::getTamb() const { return _tempIn; }
double CompressorOperatingPoint::getPamb() const { return _pressIn; }
double CompressorOperatingPoint::getPhi() const { return _phi; }
double CompressorOperatingPoint::getShaftSpeed() const { return _speed; }
double CompressorOperatingPoint::getWcorct() const { return _wCorct; }
double CompressorOperatingPoint::getWcorctOut() const { return _wCorctOut; }
double CompressorOperatingPoint::getWin() const { return _wInlet; }
double CompressorOperatingPoint::getEtaAdi() const { return _etaAdi; }
double CompressorOperatingPoint::getEtaPoly() const { return _etaPoly; }
double CompressorOperatingPoint::getPressRatio() const { return _pressRatio; }
double CompressorOperatingPoint::getDelTqT() const { return _delTqT; }
