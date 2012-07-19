/*
 * CompressorSpeedLine.cpp
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#include <gsl/gsl_errno.h>
#include <gsl/gsl_sys.h>
#include <gsl/gsl_spline.h>
#include "CompressorSpeedLine.h"
#include <vector>

extern "C" {
#include "Interpolation.h"
}

CompressorSpeedLine::CompressorSpeedLine() {
	// TODO Auto-generated constructor stub

}

CompressorSpeedLine::~CompressorSpeedLine() {
	// TODO Auto-generated destructor stub
}

void CompressorSpeedLine::addOperatingPoint(CompressorOperatingPoint opPntToAdd) {
	_opPnts.push_back(opPntToAdd);
}

double CompressorSpeedLine::calcMassFlow(double pressureRatio) {
	//interpolates the mass flow for a given

	//create arrays of pressure ratio and mass flow
	int numElems = _opPnts.size();
	double pRatio[numElems];
	double massFlow[numElems];

	int i = 0;
	std::vector<CompressorOperatingPoint>::iterator it;
	for ( it=_opPnts.begin() ; it < _opPnts.end(); it++, i++) {
		pRatio[i] = it->getPressRatio();
		massFlow[i] = it->getWcorct();
	}

	double wCorctInterp = LinearInterpUnsorted(numElems, pRatio, massFlow, pressureRatio, 1);
	return wCorctInterp;

}

double CompressorSpeedLine::calcEtaAdi(double pressureRatio) {
	//interpolates the mass flow for a given

	//create arrays of pressure ratio and mass flow
	int numElems = _opPnts.size()-1;
	double pRatio[numElems];
	double etaAdi[numElems];

	int i = 0;
	std::vector<CompressorOperatingPoint>::iterator it;
	for ( it=_opPnts.begin() ; it < _opPnts.end(); it++, i++) {
		pRatio[i] = it->getPressRatio();
		etaAdi[i] = it->getEtaAdi();
	}

	double etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);
	return etaAdiInterp;

}

void CompressorSpeedLine::calcMassAndEta(double pressureRatio, double *wCorrctIn, double *etaAdiab) {
	//interpolates the mass flow for a given

	//create arrays of pressure ratio and mass flow
	int numElems = _opPnts.size();
	double pRatio[numElems];
	double wCrctIn[numElems];
	double etaAdi[numElems];

	double pRatioMin = 1e9;
	double pRatioMax = -1e9;

	int i = 0;
	std::vector<CompressorOperatingPoint>::iterator it;
	for ( it=_opPnts.begin() ; it < _opPnts.end(); it++, i++) {
		pRatio[i] = it->getPressRatio();
		wCrctIn[i] = it->getWcorct();
		etaAdi[i] = it->getEtaAdi();

		pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
		pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

	}

	double wCorctInterp;
	double etaAdiInterp;

	//check if pressureRatio is outside min and max bounds of pRatio array
	// if so using simple extrapolation at end points, else use gsl spline functions
	if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin) ) {
		//pressureRatio is below min pRatio from array, use linear extrapolation

		wCorctInterp = LinearInterpUnsorted(numElems, pRatio, wCrctIn, pressureRatio, 1);
		etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);

	} else {
		//pressureRatio is bounded by arrays, use gsl interpolation

		//initialize a spline accelerator type
		gsl_interp_accel *acc = gsl_interp_accel_alloc();

		//initialize the w corrected spline type
		gsl_spline *wSpline = gsl_spline_alloc(gsl_interp_cspline, numElems);
		gsl_spline_init(wSpline, pRatio, wCrctIn, numElems);
		wCorctInterp = gsl_spline_eval(wSpline, pressureRatio, acc);
		if ( gsl_isnan(wCorctInterp) ) {
			//throw exception
		}

		//reset the accelerator for the eta interpolation
		gsl_interp_accel_reset(acc);

		//initialize the eta spline type
		gsl_spline *etaSpline = gsl_spline_alloc(gsl_interp_cspline, numElems);
		gsl_spline_init(etaSpline, pRatio, etaAdi, numElems);
		etaAdiInterp = gsl_spline_eval(etaSpline, pressureRatio, acc);
		if ( gsl_isnan(etaAdiInterp) ) {
			//throw exception
		}

		//free the memory allocated for the spline types
		gsl_spline_free(wSpline);
		gsl_spline_free(etaSpline);
		gsl_interp_accel_free(acc);

	}

	//double wCorctInterp = LinearInterpUnsorted(numElems, pRatio, wCrctIn, pressureRatio, 1);
	//double etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);

	*wCorrctIn = wCorctInterp;
	*etaAdiab = etaAdiInterp;

}

double CompressorSpeedLine::getShaftSpeed() {
	return _shaftSpeed;
}

void CompressorSpeedLine::setShaftSpeed(double shaftSpeed) {
	_shaftSpeed = shaftSpeed;
}
