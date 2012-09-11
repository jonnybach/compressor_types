/*
 * CompressorSpeedLine.cpp
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#include <fstream>

//UNCOMMENT TO USE GNU SCIENTIFIC LIBRARY SPLINE INTERPOLATION METHODS
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sys.h>
#include <gsl/gsl_spline.h>

//UNCOMMENT TO USE WILD MAGIC GEOMETRY LIBRARY INTERPOLATION METHODS
//#include "Wm5IntpAkimaNonuniform1.h"

#include "CompressorSpeedLine.h"
#include "CompressorOperatingPoint.h"
#include "CompressorStage.h"

extern "C" {
	#include "Interpolation.h"
}

CompressorSpeedLine::CompressorSpeedLine() {
	// TODO Auto-generated constructor stub
	_shaftSpeed = 10;
}

CompressorSpeedLine::~CompressorSpeedLine() {
	// TODO Auto-generated destructor stub
	//printf("called compressor speed line destructor\n\n");
}

void CompressorSpeedLine::addOperatingPoint(CompressorOperatingPoint opPntToAdd) { _opPnts.push_back(opPntToAdd); }

void CompressorSpeedLine::addStage(CompressorStage stageToAdd) { _stages.push_back(&stageToAdd); }

void CompressorSpeedLine::setStages(std::vector<CompressorStage *> stages) { _stages = stages; }

/*
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
		massFlow[i] = it->getWin();
	}

	double wInterp = LinearInterpUnsorted(numElems, pRatio, massFlow, pressureRatio, 1);
	return wInterp;

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
		pRatio[i] = (*it).getPressRatio();
		etaAdi[i] = (*it).getEtaAdi();
	}

	double etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);
	return etaAdiInterp;

}
*/

void CompressorSpeedLine::calcMassAndEta(double pressureRatio, double *wIn, double *etaAdiab) {
	//interpolates the mass flow for a given

	//create arrays of pressure ratio and mass flow
	int numElems = _opPnts.size();
	double pRatio[numElems];
	double wInlet[numElems];
	double etaAdi[numElems];

	double pRatioMin = 1e9;
	double pRatioMax = -1e9;

	int i = 0;
	std::vector<CompressorOperatingPoint>::iterator it;
	for ( it=_opPnts.begin() ; it < _opPnts.end(); it++, i++) {
		pRatio[i] = (*it).getPressRatio();
		wInlet[i] = (*it).getWin();
		etaAdi[i] = (*it).getEtaAdi();

		pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
		pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

	}

	double wInterp;
	double etaAdiInterp;

	//check if pressureRatio is outside min and max bounds of pRatio array
	// if so use linear interpolation, else use higher order interpolation
	bool onlyLinrIntrp = true;
	if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin) || onlyLinrIntrp ) {
		//pressureRatio is below min pRatio from array, use linear extrapolation

		wInterp = LinearInterpUnsorted(numElems, pRatio, wInlet, pressureRatio, 1);
		etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);

	} else {
		//pressureRatio is bounded by arrays, use gsl cubic spline interpolation or akima spline

		/*
		Wm5::IntpAkimaNonuniform1<double> wAkimIntrp( numElems, pRatio, wInlet);
		Wm5::IntpAkimaNonuniform1<double> etaAkimIntrp( numElems, pRatio, etaAdi);

		wInterp = wAkimIntrp(pressureRatio);
		etaAdiInterp = etaAkimIntrp(pressureRatio);
		*/

		//initialize a spline accelerator type
		gsl_interp_accel *acc = gsl_interp_accel_alloc();

		//initialize the w corrected spline type
		gsl_spline *wSpline = gsl_spline_alloc(gsl_interp_cspline, numElems);
		gsl_spline_init(wSpline, pRatio, wInlet, numElems);
		wInterp = gsl_spline_eval(wSpline, pressureRatio, acc);
		if ( gsl_isnan(wInterp) ) {
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

	*wIn = wInterp;
	*etaAdiab = etaAdiInterp;

}

CompressorStagePerformance CompressorSpeedLine::getStagePerfForPressureRatio(int stageNmbr, double pressureRatio) {

	//create arrays of pressure ratio and mass flow
	CompressorStage crntStage = *( _stages[stageNmbr-1] );
	int numElems = crntStage.getOpPntPerf().size();
	double pRatio[numElems];
	double tmpPt0[numElems];
	double tmpPt1[numElems];
	double tmpPt2[numElems];
	double tmpPt3[numElems];
	double tmpPt4[numElems];

	double pRatioMin = 1e9;
	double pRatioMax = -1e9;

	int i = 0;
	int opPntNmbr = -1;
	std::vector<CompressorStagePerformance> vStgPrf = crntStage.getOpPntPerf();
	std::vector<CompressorStagePerformance>::iterator it;
	for ( it = vStgPrf.begin() ; it != vStgPrf.end(); ++it, ++i) {
		opPntNmbr = (*it).getOptPntNmbr();
		pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();
		tmpPt0[i] = (*it).getPt0();
		tmpPt1[i] = (*it).getPt1();
		tmpPt2[i] = (*it).getPt2();
		tmpPt3[i] = (*it).getPt3();
		tmpPt4[i] = (*it).getPt4();

		pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
		pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

	}

	double pt0Interp;
	double pt1Interp;
	double pt2Interp;
	double pt3Interp;
	double pt4Interp;

	//check if pressureRatio is outside min and max bounds of pRatio array
	// if so using simple extrapolation at end points, else use gsl spline functions
	bool onlyLinrIntrp = true;
	if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
		//pressureRatio is below min pRatio from array, use linear extrapolation

		pt0Interp = LinearInterpUnsorted(numElems, pRatio, tmpPt0, pressureRatio, 1);
		pt1Interp = LinearInterpUnsorted(numElems, pRatio, tmpPt1, pressureRatio, 1);
		pt2Interp = LinearInterpUnsorted(numElems, pRatio, tmpPt2, pressureRatio, 1);
		pt3Interp = LinearInterpUnsorted(numElems, pRatio, tmpPt3, pressureRatio, 1);
		pt4Interp = LinearInterpUnsorted(numElems, pRatio, tmpPt4, pressureRatio, 1);

	} else {
		//pressureRatio is bounded by arrays, use gsl interpolation

		//Wm5::IntpAkimaNonuniform1<double> wAkimIntrp( numElems, pRatio, wInlet);
		//Wm5::IntpAkimaNonuniform1<double> etaAkimIntrp( numElems, pRatio, etaAdi);

		//wInterp = wAkimIntrp(pressureRatio);
		//etaAdiInterp = etaAkimIntrp(pressureRatio);

		/*
		//allocate spline and spline accelorator
		gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, numElems);
		gsl_interp_accel *acc = gsl_interp_accel_alloc();

		//pt0
		gsl_spline_init(spline, pRatio, tmpPt0, numElems);
		pt0Interp = gsl_spline_eval(spline, pressureRatio, acc);
		if ( gsl_isnan(pt0Interp) ) {
			//throw exception
		}

		//pt1
		//gsl_interp_accel_reset(acc);
		gsl_spline_init(spline, pRatio, tmpPt1, numElems);
		pt1Interp = gsl_spline_eval(spline, pressureRatio, acc);
		if ( gsl_isnan(pt1Interp) ) {
			//throw exception
		}

		//pt2
		//gsl_interp_accel_reset(acc);
		gsl_spline_init(spline, pRatio, tmpPt2, numElems);
		pt2Interp = gsl_spline_eval(spline, pressureRatio, acc);
		if ( gsl_isnan(pt2Interp) ) {
			//throw exception
		}

		//pt3
		//gsl_interp_accel_reset(acc);
		gsl_spline_init(spline, pRatio, tmpPt3, numElems);
		pt3Interp = gsl_spline_eval(spline, pressureRatio, acc);
		if ( gsl_isnan(pt3Interp) ) {
			//throw exception
		}

		//pt4
		//gsl_interp_accel_reset(acc);
		gsl_spline_init(spline, pRatio, tmpPt4, numElems);
		pt4Interp = gsl_spline_eval(spline, pressureRatio, acc);
		if ( gsl_isnan(pt4Interp) ) {
			//throw exception
		}

		//free the memory allocated for the spline and accelorator types
		gsl_spline_free(spline);
		gsl_interp_accel_free(acc);
		*/

	}

	//create new compressor stage performance type, initialize with interpolated values and return
	CompressorStagePerformance newStgPerf = CompressorStagePerformance(-9999, pt0Interp, pt1Interp, pt2Interp, pt3Interp
			, pt4Interp, 0, 0, 0, 0, 0);
	return newStgPerf;

}

double CompressorSpeedLine::getShaftSpeed() {
	return _shaftSpeed;
}

void CompressorSpeedLine::setShaftSpeed(double shaftSpeed) {
	_shaftSpeed = shaftSpeed;
}
