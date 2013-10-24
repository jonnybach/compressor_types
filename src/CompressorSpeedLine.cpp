/*
 * CompressorSpeedLine.cpp
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#include <fstream>
#include <string>

//UNCOMMENT TO USE GNU SCIENTIFIC LIBRARY SPLINE INTERPOLATION METHODS
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_sys.h>
//#include <gsl/gsl_spline.h>

//UNCOMMENT TO USE WILD MAGIC GEOMETRY LIBRARY INTERPOLATION METHODS
//#include "Wm5IntpAkimaNonuniform1.h"

#include "CompressorAirfoilConfiguration.h"
#include "CompressorSpeedLine.h"
#include "CompressorOperatingPoint.h"
#include "CompressorStage.h"
#include "DiffuserPerformance.h"

extern "C" {
	#include "Interpolation.h"
}

CompressorSpeedLine::CompressorSpeedLine() {
	// TODO Auto-generated constructor stub
}

CompressorSpeedLine::~CompressorSpeedLine() {
	// TODO Auto-generated destructor stub
	//printf("called compressor speed line destructor\n\n");
}

void CompressorSpeedLine::addOperatingPoint(CompressorOperatingPoint opPntToAdd) { _opPnts.push_back(opPntToAdd); }

void CompressorSpeedLine::addStage(CompressorStage stageToAdd) { _stages.push_back(&stageToAdd); }

void CompressorSpeedLine::setStages(std::vector<CompressorStage *> stages) { _stages = stages; }

void CompressorSpeedLine::setDiffuserPerformance(std::vector<DiffuserPerformance> diffPerf) { m_diffPerf = diffPerf; }

void CompressorSpeedLine::calcMassAndEta(double pressureRatio, double *wIn, double *etaAdiab) {
	//interpolates the mass flow and efficiency for a given pressure ratio

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

//		//initialize a spline accelerator type
//		gsl_interp_accel *acc = gsl_interp_accel_alloc();
//
//		//initialize the w corrected spline type
//		gsl_spline *wSpline = gsl_spline_alloc(gsl_interp_cspline, numElems);
//		gsl_spline_init(wSpline, pRatio, wInlet, numElems);
//		wInterp = gsl_spline_eval(wSpline, pressureRatio, acc);
//		if ( gsl_isnan(wInterp) ) {
//			//throw exception
//		}
//
//		//reset the accelerator for the eta interpolation
//		gsl_interp_accel_reset(acc);
//
//		//initialize the eta spline type
//		gsl_spline *etaSpline = gsl_spline_alloc(gsl_interp_cspline, numElems);
//		gsl_spline_init(etaSpline, pRatio, etaAdi, numElems);
//		etaAdiInterp = gsl_spline_eval(etaSpline, pressureRatio, acc);
//		if ( gsl_isnan(etaAdiInterp) ) {
//			//throw exception
//		}
//
//		//free the memory allocated for the spline types
//		gsl_spline_free(wSpline);
//		gsl_spline_free(etaSpline);
//		gsl_interp_accel_free(acc);

	}

	*wIn = wInterp;
	*etaAdiab = etaAdiInterp;

}

CompressorOperatingPoint CompressorSpeedLine::getOpPntForPressureRatio(double pressureRatio) {

    //interpolates the mass flow and efficiency for a given pressure ratio

    //create arrays of pressure ratio and mass flow
    int numElems = _opPnts.size();
    double inletTemp[numElems];
    double inletPress[numElems];
    double phi[numElems];
    double delTqT[numElems];
    double pRatio[numElems];
    double wInlet[numElems];
    double wInCorr[numElems];
    double wOutCorr[numElems];
    double etaAdi[numElems];
    double etaPoly[numElems];

    double pRatioMin = 1e9;
    double pRatioMax = -1e9;

    int i = 0;
    std::vector<CompressorOperatingPoint>::iterator it;
    for ( it=_opPnts.begin() ; it < _opPnts.end(); it++, i++) {
        inletTemp[i] = (*it).getTamb();
        inletPress[i] = (*it).getPamb();
        phi[i] = (*it).getPhi();
        delTqT[i] = (*it).getDelTqT();
        pRatio[i] = (*it).getPressRatio();
        wInlet[i] = (*it).getWin();
        wInCorr[i] = (*it).getWcorct();
        wOutCorr[i] = (*it).getWcorctOut();
        etaAdi[i] = (*it).getEtaAdi();
        etaPoly[i] = (*it).getEtaPoly();

        pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
        pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

    }

    double inletTempInterp;
    double inletPressInterp;
    double phiInterp;
    double delTqTInterp;
    double pRatioInterp;
    double wInterp;
    double wInCorrInterp;
    double wOutCorrInterp;
    double etaAdiInterp;
    double etaPolyInterp;

    //check if pressureRatio is outside min and max bounds of pRatio array
    // if so use linear interpolation, else use higher order interpolation
    bool onlyLinrIntrp = true;
    if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin) || onlyLinrIntrp ) {
        //pressureRatio is below min pRatio from array, use linear extrapolation

        inletTempInterp = LinearInterpUnsorted(numElems, pRatio, inletTemp, pressureRatio, 1);
        inletPressInterp = LinearInterpUnsorted(numElems, pRatio, inletPress, pressureRatio, 1);
        phiInterp = LinearInterpUnsorted(numElems, pRatio, phi, pressureRatio, 1);
        delTqTInterp = LinearInterpUnsorted(numElems, pRatio, delTqT, pressureRatio, 1);

        wInterp = LinearInterpUnsorted(numElems, pRatio, wInlet, pressureRatio, 1);
        wInCorrInterp = LinearInterpUnsorted(numElems, pRatio, wInCorr, pressureRatio, 1);
        wOutCorrInterp = LinearInterpUnsorted(numElems, pRatio, wOutCorr, pressureRatio, 1);

        etaAdiInterp = LinearInterpUnsorted(numElems, pRatio, etaAdi, pressureRatio, 1);
        etaPolyInterp = LinearInterpUnsorted(numElems, pRatio, etaPoly, pressureRatio, 1);


    } else {
        // place holder for spline fit, but splines do not work well for extrapolation so be careful
    }

    //assign each part of the line data to the operating point
    CompressorOperatingPoint tmpOpPnt = CompressorOperatingPoint( 99
            , inletTempInterp
            , inletPressInterp
            , phiInterp
            , pressureRatio
            , delTqTInterp
            , etaAdiInterp
            , etaPolyInterp
            , wInCorrInterp
            , wOutCorrInterp
            , wInterp
            , this->getShaftSpeed());

    return tmpOpPnt;

}

CompressorStagePerformance CompressorSpeedLine::getStagePerfForPressureRatio(int stageNmbr, double pressureRatio) {

	//create arrays of pressure ratio and mass flow
	CompressorStage crntStage = *( _stages[stageNmbr-1] );
	int numElems = crntStage.getOpPntPerf().size();

	//get the number of airfoils for the stages rotor-stator.  Check if the stage is an OGV first
	int numRotorAfs = -1;
	int numStatorAfs = -1;
	std::string stgName = crntStage.getStageName();
	if (stgName.find("ogv", 0) != std::string::npos) {
		numRotorAfs = 0;
		numStatorAfs = crntStage.getStator()->getNumBlades();
	} else {
		numRotorAfs = crntStage.getRotor()->getNumBlades();
		numStatorAfs = crntStage.getStator()->getNumBlades();
	}

	double pRatio[numElems];

	double tmpStgPi[numElems];
	double tmpStgEtaAdi[numElems];
	double tmpStgWrk[numElems];
	double tmpStgRct[numElems];
	double tmpWheelSpd[numElems];

	double tmpRtrPhi[numElems];
	double tmpStrPhi[numElems];

	double tmpRtrDf[numElems];
	double tmpStrDf[numElems];

	double tmpRtrDeq[numElems];
	double tmpStrDeq[numElems];

	double tmpRtrDh[numElems];
	double tmpStrDh[numElems];

	double tmpRtrPt1Abs[numElems];
	double tmpRtrPt1Rel[numElems];
	double tmpRtrPt2Rel[numElems];

	double tmpStrPt1[numElems];
	double tmpStrPt2[numElems];

	double tmpRtrTt1Abs[numElems];

	double tmpRtrBetaIn[numElems];
	double tmpRtrBetaOut[numElems];

	double tmpStrAlpIn[numElems];
	double tmpStrAlpOut[numElems];

	double tmpRtrAR[numElems];
	double tmpStrAR[numElems];

	double tmpRtrInc[numElems];
	double tmpStrInc[numElems];

	double tmpRtrDev[numElems];
	double tmpStrDev[numElems];

	double pRatioMin = 1e9;
	double pRatioMax = -1e9;

	int i = 0;
	int opPntNmbr = -1;
	std::vector<CompressorStagePerformance> vStgPrf = crntStage.getOpPntPerf();
	std::vector<CompressorStagePerformance>::iterator it;
	for ( it = vStgPrf.begin() ; it != vStgPrf.end(); ++it, ++i) {
		opPntNmbr = (*it).getOptPntNmbr();
		pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();

		tmpStgPi[i] = (*it).getStagePi();
		tmpStgEtaAdi[i] = (*it).getStageEtaAdiab();
		tmpStgWrk[i] = (*it).getStageWork();
		tmpStgRct[i] = (*it).getStageReaction();
		tmpWheelSpd[i] = (*it).getWheelSpeed();

		tmpRtrPhi[i] = (*it).getRotorPhi();
		tmpStrPhi[i] = (*it).getRotorPhi();

		tmpRtrDf[i] = (*it).getDiffLiebRotor();
		tmpStrDf[i] = (*it).getDiffLiebStator();

		tmpRtrDeq[i] = (*it).getDiffEqRatRotor();
		tmpStrDeq[i] = (*it).getDiffEqRatStator();

		tmpRtrDh[i] = (*it).getDeHallerRotor();
		tmpStrDh[i] = (*it).getDeHallerStator();

		tmpRtrPt1Abs[i] = (*it).getRotorPt1Abs();
		tmpRtrPt1Rel[i] = (*it).getRotorPt1Rel();
		tmpRtrPt2Rel[i] = (*it).getRotorPt2Rel();

		tmpStrPt1[i] = (*it).getStatorPt1();
		tmpStrPt2[i] = (*it).getStatorPt2();

		tmpRtrTt1Abs[i] = (*it).getRotorTt1Abs();

		tmpRtrBetaIn[i] = (*it).getRotorBeta1();
		tmpRtrBetaOut[i] = (*it).getRotorBeta2();

		tmpStrAlpIn[i] = (*it).getStatorAlpha1();
		tmpStrAlpOut[i] = (*it).getStatorAlpha2();

		tmpRtrAR[i] = (*it).getRotorAspRatio();
		tmpStrAR[i] = (*it).getStatorAspRatio();

		tmpRtrInc[i] = (*it).getRotorIncidence();
		tmpStrInc[i] = (*it).getStatorIncidence();

		tmpRtrDev[i] = (*it).getRotorDeviation();
		tmpStrDev[i] = (*it).getStatorDeviation();

		pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
		pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

	}

	double tmpStgPiInterp;
	double tmpStgEtaAdiInterp;
	double tmpStgWrkInterp;
	double tmpStgRctInterp;
	double tmpWheelSpdInterp;

	double tmpRtrPhiInterp;
	double tmpStrPhiInterp;

	double tmpRtrDfInterp;
	double tmpStrDfInterp;

	double tmpRtrDeqInterp;
	double tmpStrDeqInterp;

	double tmpRtrDhInterp;
	double tmpStrDhInterp;

	double pt0Interp;
	double pt1Interp;
	double pt2Interp;
	double pt3Interp;
	double pt4Interp;

	double tmpRtrTtInAbsInterp;

	double tmpRtrBetaInInterp;
	double tmpRtrBetaOutInterp;

	double tmpStrAlpInInterp;
	double tmpStrAlpOutInterp;

	double tmpRtrARInterp;
	double tmpStrARInterp;

	double tmpRtrIncInterp;
	double tmpStrIncInterp;

	double tmpRtrDevInterp;
	double tmpStrDevInterp;

	//check if pressureRatio is outside min and max bounds of pRatio array
	// if so using simple extrapolation at end points, else use gsl spline functions
	bool onlyLinrIntrp = true;
	if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
		//pressureRatio is below min pRatio from array, use linear extrapolation

		tmpStgPiInterp = LinearInterpUnsorted(numElems, pRatio, tmpStgPi, pressureRatio, 1);
		tmpStgEtaAdiInterp = LinearInterpUnsorted(numElems, pRatio, tmpStgEtaAdi, pressureRatio, 1);
		tmpStgWrkInterp = LinearInterpUnsorted(numElems, pRatio, tmpStgWrk, pressureRatio, 1);
		tmpStgRctInterp = LinearInterpUnsorted(numElems, pRatio, tmpStgRct, pressureRatio, 1);
		tmpWheelSpdInterp = LinearInterpUnsorted(numElems, pRatio, tmpWheelSpd, pressureRatio, 1);

		tmpRtrPhiInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPhi, pressureRatio, 1);
		tmpStrPhiInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrPhi, pressureRatio, 1);

		tmpRtrDfInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrDf, pressureRatio, 1);
		tmpStrDfInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrDf, pressureRatio, 1);

		tmpRtrDeqInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrDeq, pressureRatio, 1);
		tmpStrDeqInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrDeq, pressureRatio, 1);

		tmpRtrDhInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrDh, pressureRatio, 1);
		tmpStrDhInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrDh, pressureRatio, 1);

		pt0Interp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt1Abs, pressureRatio, 1);
		pt1Interp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt1Rel, pressureRatio, 1);
		pt2Interp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt2Rel, pressureRatio, 1);
		pt3Interp = LinearInterpUnsorted(numElems, pRatio, tmpStrPt1, pressureRatio, 1);
		pt4Interp = LinearInterpUnsorted(numElems, pRatio, tmpStrPt2, pressureRatio, 1);

		tmpRtrTtInAbsInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrTt1Abs, pressureRatio, 1);

		tmpRtrBetaInInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrBetaIn, pressureRatio, 1);
		tmpRtrBetaOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrBetaOut, pressureRatio, 1);

		tmpStrAlpInInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrAlpIn, pressureRatio, 1);
		tmpStrAlpOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrAlpOut, pressureRatio, 1);

		tmpRtrARInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrAR, pressureRatio, 1);
		tmpStrARInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrAR, pressureRatio, 1);

		tmpRtrIncInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrInc, pressureRatio, 1);
		tmpStrIncInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrInc, pressureRatio, 1);

		tmpRtrDevInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrDev, pressureRatio, 1);
		tmpStrDevInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrDev, pressureRatio, 1);


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
	CompressorStagePerformance newStgPerf = CompressorStagePerformance(-9999,
			pt0Interp,
			pt1Interp,
			pt2Interp,
			pt3Interp,
			pt4Interp,
			tmpRtrTtInAbsInterp,
			-9999.9,
			-9999.9,
			-9999.9,
			-9999.9,
			tmpRtrBetaInInterp,
			tmpRtrBetaOutInterp,
			tmpStrAlpInInterp,
			tmpStrAlpOutInterp,
			tmpStgPiInterp,
			tmpStgEtaAdiInterp,
			tmpStgWrkInterp,
			tmpRtrPhiInterp,
			tmpStrPhiInterp,
			tmpWheelSpdInterp,
			tmpRtrDfInterp,
			tmpRtrDeqInterp,
			tmpRtrDhInterp,
			tmpStrDfInterp,
			tmpStrDeqInterp,
			tmpStrDhInterp,
			tmpStgRctInterp,
			tmpRtrARInterp,
			tmpStrARInterp,
			numRotorAfs,
			numStatorAfs,
			tmpRtrIncInterp,
			tmpStrIncInterp,
			tmpRtrDevInterp,
			tmpStrDevInterp);
	return newStgPerf;

}

DiffuserPerformance CompressorSpeedLine::getDiffsrPerfForPressureRatio(double pressureRatio) {

	//create arrays of pressure ratio and mass flow
	int numElems = m_diffPerf.size();

	double pRatio[numElems];

	double tmpDelPqP[numElems];

	double tmpPtIn[numElems];
	double tmpPsIn[numElems];
	double tmpTtIn[numElems];
	double tmpTsIn[numElems];
	double tmpMachIn[numElems];

	double tmpPtExit[numElems];
	double tmpPsExit[numElems];
	double tmpTtExit[numElems];
	double tmpTsExit[numElems];
	double tmpMachExit[numElems];

	double pRatioMin = 1e9;
	double pRatioMax = -1e9;

	int i = 0;
	int opPntNmbr = -1;
	std::vector<DiffuserPerformance> vPrf = m_diffPerf;
	std::vector<DiffuserPerformance>::iterator it;
	for ( it = vPrf.begin() ; it != vPrf.end(); ++it, ++i) {
		opPntNmbr = (*it).getOptPntNmbr();
		pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();

		tmpPtIn[i] = (*it).getPtIn();
		tmpPsIn[i] = (*it).getPsIn();
		tmpTtIn[i] = (*it).getTtIn();
		tmpTsIn[i] = (*it).getTsIn();
		tmpMachIn[i] = (*it).getMachIn();

		tmpPtExit[i] = (*it).getPtExit();
		tmpPsExit[i] = (*it).getPsExit();
		tmpTtExit[i] = (*it).getTtExit();
		tmpTsExit[i] = (*it).getTsExit();
		tmpMachExit[i] = (*it).getMachExit();

		tmpDelPqP[i] = (*it).getDelPqP();

		pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
		pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

	}

	double tmpDelPqPInterp;

	double tmpPtInInterp;
	double tmpPsInInterp;
	double tmpTtInInterp;
	double tmpTsInInterp;
	double tmpMachInInterp;

	double tmpPtExitInterp;
	double tmpPsExitInterp;
	double tmpTtExitInterp;
	double tmpTsExitInterp;
	double tmpMachExitInterp;

	//check if pressureRatio is outside min and max bounds of pRatio array
	// if so using simple extrapolation at end points, else use gsl spline functions
	bool onlyLinrIntrp = true;
	if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
		//pressureRatio is below min pRatio from array, use linear extrapolation

		tmpDelPqPInterp = LinearInterpUnsorted(numElems, pRatio, tmpDelPqP, pressureRatio, 1);
		tmpPtInInterp = LinearInterpUnsorted(numElems, pRatio, tmpPtIn, pressureRatio, 1);
		tmpPsInInterp = LinearInterpUnsorted(numElems, pRatio, tmpPsIn, pressureRatio, 1);
		tmpTtInInterp = LinearInterpUnsorted(numElems, pRatio, tmpTtIn, pressureRatio, 1);
		tmpTsInInterp = LinearInterpUnsorted(numElems, pRatio, tmpTsIn, pressureRatio, 1);
		tmpMachInInterp = LinearInterpUnsorted(numElems, pRatio, tmpMachIn, pressureRatio, 1);
		tmpPtExitInterp = LinearInterpUnsorted(numElems, pRatio, tmpPtExit, pressureRatio, 1);
		tmpPsExitInterp = LinearInterpUnsorted(numElems, pRatio, tmpPsExit, pressureRatio, 1);
		tmpTtExitInterp = LinearInterpUnsorted(numElems, pRatio, tmpTtExit, pressureRatio, 1);
		tmpTsExitInterp = LinearInterpUnsorted(numElems, pRatio, tmpTsExit, pressureRatio, 1);
		tmpMachExitInterp = LinearInterpUnsorted(numElems, pRatio, tmpMachExit, pressureRatio, 1);

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
	DiffuserPerformance newDiffPerf = DiffuserPerformance(-9999,
			tmpDelPqPInterp,
			tmpPtInInterp,
			tmpPsInInterp,
			tmpTtInInterp,
			tmpTsInInterp,
			tmpMachInInterp,
			tmpPtExitInterp,
			tmpPsExitInterp,
			tmpTtExitInterp,
			tmpTsExitInterp,
			tmpMachExitInterp);
	return newDiffPerf;

}

const CompressorOperatingPoint* CompressorSpeedLine::getOpPnt(int operatingPoint) {
	const CompressorOperatingPoint *pntToReturn = &_opPnts[operatingPoint];
	return pntToReturn;
}

void CompressorSpeedLine::getPrMassAndEtaAtOpPnt(int operatingPoint, double *pr, double *wIn, double *etaAdiab) {
	double pRatio = _opPnts[operatingPoint].getPressRatio();
	double wInlet = _opPnts[operatingPoint].getWin();
	double etaAdi = _opPnts[operatingPoint].getEtaAdi();
	*pr = pRatio;
	*wIn = wInlet;
	*etaAdiab = etaAdi;
}

CompressorStagePerformance CompressorSpeedLine::getStagePerfAtOpPnt(int operatingPoint, int stageNmbr) {
	//create arrays of pressure ratio and mass flow
	CompressorStage crntStage = *( _stages[stageNmbr-1] );
	CompressorStagePerformance stgPerf = crntStage.getOpPntPerf().at(operatingPoint);
	return stgPerf;
}

double CompressorSpeedLine::getShaftSpeed() {
	return _shaftSpeed;
}

void CompressorSpeedLine::setShaftSpeed(double shaftSpeed) {
	_shaftSpeed = shaftSpeed;
}
