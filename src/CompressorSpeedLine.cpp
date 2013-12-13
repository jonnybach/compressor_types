/*
 * CompressorSpeedLine.cpp
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#include <fstream>
#include <string>
#include <exception>

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
#include "InletPerformance.h"
#include "InletGuideVanePerformance.h"
#include "CompressorLeakage.h"
#include "CompressorLeakagePerformance.h"

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

void CompressorSpeedLine::setInletPerformance(std::vector<InletPerformance> inletPerf) { m_inletPerf = inletPerf; }

void CompressorSpeedLine::setInletGuideVanePerformance(std::vector<InletGuideVanePerformance> igvPerf) { m_igvPerf = igvPerf; }

void CompressorSpeedLine::setLeakages(std::vector<CompressorLeakage> leaks) { m_leaks = leaks; }


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
    //double pRatioInterp;
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
        throw std::exception();
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

    double tmpRtrPs1[numElems];
    double tmpRtrPs2[numElems];

	double tmpStrPt1[numElems];
	double tmpStrPt2[numElems];

    double tmpStrPs1[numElems];
    double tmpStrPs2[numElems];

	double tmpRtrTt1Abs[numElems];
    double tmpRtrTt1Rel[numElems];
    double tmpRtrTt2Rel[numElems];

    double tmpStrTt1[numElems];
    double tmpStrTt2[numElems];

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

        tmpRtrPs1[i] = (*it).getRotorPs1();
        tmpRtrPs2[i] = (*it).getRotorPs2();

		tmpStrPt1[i] = (*it).getStatorPt1();
		tmpStrPt2[i] = (*it).getStatorPt2();

        tmpStrPs1[i] = (*it).getStatorPs1();
        tmpStrPs2[i] = (*it).getStatorPs2();

		tmpRtrTt1Abs[i] = (*it).getRotorTt1Abs();
		tmpRtrTt1Rel[i] = (*it).getRotorTt1Rel();
        tmpRtrTt2Rel[i] = (*it).getRotorTt2Rel();

        tmpStrTt1[i] = (*it).getStatorTt1();
        tmpStrTt2[i] = (*it).getStatorTt2();

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

	double tmpRtrPtInAbsInterp;
	double tmpRtrPtInRelInterp;
	double tmpRtrPtOutRelInterp;

	double tmpStrPtInInterp;
	double tmpStrPtOutInterp;

	double tmpRtrPsInInterp;
	double tmpRtrPsOutInterp;

	double tmpStrPsInInterp;
	double tmpStrPsOutInterp;

	double tmpRtrTtInAbsInterp;
	double tmpRtrTtInRelInterp;
    double tmpRtrTtOutRelInterp;

    double tmpStrTtInInterp;
    double tmpStrTtOutInterp;

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

		tmpRtrPtInAbsInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt1Abs, pressureRatio, 1);
		tmpRtrPtInRelInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt1Rel, pressureRatio, 1);
		tmpRtrPtOutRelInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPt2Rel, pressureRatio, 1);

		tmpStrPtInInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrPt1, pressureRatio, 1);
		tmpStrPtOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrPt2, pressureRatio, 1);

        tmpRtrPsInInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPs1, pressureRatio, 1);
        tmpRtrPsOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrPs2, pressureRatio, 1);

        tmpStrPsInInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrPs1, pressureRatio, 1);
        tmpStrPsOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrPs2, pressureRatio, 1);

		tmpRtrTtInAbsInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrTt1Abs, pressureRatio, 1);
        tmpRtrTtInRelInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrTt1Rel, pressureRatio, 1);
        tmpRtrTtOutRelInterp = LinearInterpUnsorted(numElems, pRatio, tmpRtrTt2Rel, pressureRatio, 1);

        tmpStrTtInInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrTt1, pressureRatio, 1);
        tmpStrTtOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpStrTt2, pressureRatio, 1);

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
	    throw std::exception();
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
			tmpRtrPtInAbsInterp,
			tmpRtrPtInRelInterp,
			tmpRtrPtOutRelInterp,
			tmpRtrPsInInterp,
			tmpRtrPsOutInterp,
			tmpStrPtInInterp,
			tmpStrPtOutInterp,
            tmpStrPsInInterp,
            tmpStrPsOutInterp,
			tmpRtrTtInAbsInterp,
			tmpRtrTtInRelInterp,
			tmpRtrTtOutRelInterp,
			tmpStrTtInInterp,
			tmpStrTtOutInterp,
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
	    throw std::exception();
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

	//create new diffuser performance instance, initialize with interpolated values and return
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

InletPerformance CompressorSpeedLine::getInletPerfForPressureRatio(double pressureRatio) {

    //create arrays of pressure ratio and mass flow
    int numElems = m_inletPerf.size();

    double pRatio[numElems];

    double tmpDelPtqPt[numElems];
    double tmpPtOut[numElems];

    double pRatioMin = 1e9;
    double pRatioMax = -1e9;

    int i = 0;
    int opPntNmbr = -1;
    std::vector<InletPerformance> vPrf = m_inletPerf;
    std::vector<InletPerformance>::iterator it;
    for ( it = vPrf.begin() ; it != vPrf.end(); ++it, ++i) {
        opPntNmbr = (*it).getOpPntNmbr();
        pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();

        tmpDelPtqPt[i] = (*it).getDelPtqPtScroll();
        tmpPtOut[i] = (*it).getPtOut();

        pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
        pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

    }

    double tmpDelPqPInterp;
    double tmpPtOutInterp;

    //check if pressureRatio is outside min and max bounds of pRatio array
    // if so using simple extrapolation at end points, else use gsl spline functions
    bool onlyLinrIntrp = true;
    if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
        //pressureRatio is below min pRatio from array, use linear extrapolation

        tmpDelPqPInterp = LinearInterpUnsorted(numElems, pRatio, tmpDelPtqPt, pressureRatio, 1);
        tmpPtOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpPtOut, pressureRatio, 1);

    } else {
        throw std::exception();
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

    //create new inlet performance instance, initialize with interpolated values and return
    InletPerformance newInletPerf = InletPerformance(-9999,
            tmpDelPqPInterp,
            tmpPtOutInterp);
    return newInletPerf;

}

InletGuideVanePerformance CompressorSpeedLine::getInletGuideVanePerfForPressureRatio(double pressureRatio) {

    //create arrays of pressure ratio and mass flow
    int numElems = m_igvPerf.size();

    double pRatio[numElems];

    double tmpDelPtqPt[numElems];
    double tmpPtOut[numElems];
    double tmpAlpOut[numElems];

    double pRatioMin = 1e9;
    double pRatioMax = -1e9;

    int i = 0;
    int opPntNmbr = -1;
    std::vector<InletGuideVanePerformance> vPrf = m_igvPerf;
    std::vector<InletGuideVanePerformance>::iterator it;
    for ( it = vPrf.begin() ; it != vPrf.end(); ++it, ++i) {
        opPntNmbr = (*it).getOpPntNmbr();
        pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();

        tmpDelPtqPt[i] = (*it).getDelPtqPt();
        tmpPtOut[i] = (*it).getPtOut();
        tmpAlpOut[i] = (*it).getAlpOut();

        pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
        pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

    }

    double tmpDelPqPInterp;
    double tmpPtOutInterp;
    double tmpAlpOutInterp;

    //check if pressureRatio is outside min and max bounds of pRatio array
    // if so using simple extrapolation at end points, else use gsl spline functions
    bool onlyLinrIntrp = true;
    if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
        //pressureRatio is below min pRatio from array, use linear extrapolation

        tmpDelPqPInterp = LinearInterpUnsorted(numElems, pRatio, tmpDelPtqPt, pressureRatio, 1);
        tmpPtOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpPtOut, pressureRatio, 1);
        tmpAlpOutInterp = LinearInterpUnsorted(numElems, pRatio, tmpAlpOut, pressureRatio, 1);

    } else {
        throw std::exception();
        //pressureRatio is bounded by arrays, use gsl interpolation
    }

    //create new inlet performance instance, initialize with interpolated values and return
    InletGuideVanePerformance newIgvPerf = InletGuideVanePerformance(-9999,
            tmpDelPqPInterp,
            tmpPtOutInterp,
            tmpAlpOutInterp);
    return newIgvPerf;

}

CompressorLeakagePerformance CompressorSpeedLine::getLeakagePerfForPressureRatio(int leakgIndex, double pressureRatio)
{

    //create arrays of pressure ratio and mass flow
    CompressorLeakage crntLkg = m_leaks.at(leakgIndex);

    //create arrays of pressure ratio and mass flow
    int numElems = crntLkg.getOpPntPerf().size();

    double pRatio[numElems];

    double tmpPt[numElems];
    double tmpPs[numElems];
    double tmpTt[numElems];
    double tmpTs[numElems];

    double pRatioMin = 1e9;
    double pRatioMax = -1e9;

    int i = 0;
    int opPntNmbr = -1;
    std::vector<CompressorLeakagePerformance> vPrf = crntLkg.getOpPntPerf();
    std::vector<CompressorLeakagePerformance>::iterator it;
    for ( it = vPrf.begin() ; it != vPrf.end(); ++it, ++i) {
        opPntNmbr = (*it).getOptPntNmbr();
        pRatio[i] = _opPnts[opPntNmbr-1].getPressRatio();

        tmpPt[i] = (*it).getPt();
        tmpPs[i] = (*it).getPs();
        tmpTt[i] = (*it).getTt();
        tmpTs[i] = (*it).getTs();

        pRatioMin = ((pRatio[i] < pRatioMin) ? pRatio[i] : pRatioMin);
        pRatioMax = ((pRatio[i] > pRatioMax) ? pRatio[i] : pRatioMax);

    }

    double tmpPtInterp;
    double tmpPsInterp;
    double tmpTtInterp;
    double tmpTsInterp;

    //check if pressureRatio is outside min and max bounds of pRatio array
    // if so using simple extrapolation at end points, else use gsl spline functions
    bool onlyLinrIntrp = true;
    if ( (pressureRatio > pRatioMax) || (pressureRatio < pRatioMin || onlyLinrIntrp ) ) {
        //pressureRatio is below min pRatio from array, use linear extrapolation

        tmpPtInterp = LinearInterpUnsorted(numElems, pRatio, tmpPt, pressureRatio, 1);
        tmpPsInterp = LinearInterpUnsorted(numElems, pRatio, tmpPs, pressureRatio, 1);
        tmpTtInterp = LinearInterpUnsorted(numElems, pRatio, tmpTt, pressureRatio, 1);
        tmpTsInterp = LinearInterpUnsorted(numElems, pRatio, tmpTs, pressureRatio, 1);

    } else {
        throw std::exception();
    }

    //create new diffuser performance instance, initialize with interpolated values and return
    CompressorLeakagePerformance newPerf = CompressorLeakagePerformance(-9999,
            tmpPtInterp,
            tmpPsInterp,
            tmpTtInterp,
            tmpTsInterp);

    return newPerf;

}


const CompressorOperatingPoint* CompressorSpeedLine::getOpPnt(int operatingPoint) {
	const CompressorOperatingPoint *pntToReturn = &_opPnts[operatingPoint];
	return pntToReturn;
}

CompressorStagePerformance CompressorSpeedLine::getStagePerfAtOpPnt(int operatingPoint, int stageNmbr) {
	CompressorStage crntStage = *( _stages[stageNmbr-1] );
	CompressorStagePerformance stgPerf = crntStage.getOpPntPerf().at(operatingPoint);
	return stgPerf;
}

DiffuserPerformance CompressorSpeedLine::getDiffsrPerfAtOpPnt(int operatingPoint) {
    DiffuserPerformance diffPrf = m_diffPerf.at(operatingPoint);
    return diffPrf;
}

InletPerformance CompressorSpeedLine::getInletPerfAtOpPnt(int operatingPoint) {
    InletPerformance inletPrf = m_inletPerf.at(operatingPoint);
    return inletPrf;
}

InletGuideVanePerformance CompressorSpeedLine::getInletGuideVanePerfAtOpPnt(int operatingPoint) {
    InletGuideVanePerformance igvPrf = m_igvPerf.at(operatingPoint);
    return igvPrf;
}

CompressorLeakagePerformance CompressorSpeedLine::getLeakagePerfAtOpPnt(int leakgIndex, int operatingPoint) {
    CompressorLeakage crntLeakg = m_leaks.at(leakgIndex);
    CompressorLeakagePerformance leakPerf = crntLeakg.getOpPntPerf().at(operatingPoint);
    return leakPerf;
}




double CompressorSpeedLine::getShaftSpeed() {
	return _shaftSpeed;
}


void CompressorSpeedLine::setShaftSpeed(double shaftSpeed) {
	_shaftSpeed = shaftSpeed;
}

int CompressorSpeedLine::getNumberOfLeakages() { return (int)m_leaks.size(); }
