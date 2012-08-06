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
		, double pressRatio, double delTqT, double etaAdi, double etaPoly, double wCorct
		, double wCorctOut, double wInlet, double speed) {

	_opPntNmbr = opPntNmbr;
	_tempIn = inletTemp;
	_pressIn = inletPress;
	_pressRatio = pressRatio;
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
		, double pressRatio, double delTqT, double etaAdi, double etaPoly, double wCorct, double wCorctOut
		, double wInlet, double speed) {

	_opPntNmbr = opPntNmbr;
	_tempIn = inletTemp;
	_pressIn = inletPress;
	_pressRatio = pressRatio;
	_delTqT = delTqT;
	_etaAdi = etaAdi;
	_etaPoly = etaPoly;
	_wCorct = wCorct;
	_wCorctOut = wCorctOut;
	_wInlet = wInlet;
	_speed = speed;

}

//accessors
double CompressorOperatingPoint::getWcorct() { return _wCorct; }
double CompressorOperatingPoint::getWin() { return _wInlet; }
double CompressorOperatingPoint::getEtaAdi() { return _etaAdi; }
double CompressorOperatingPoint::getPressRatio() { return _pressRatio; }
