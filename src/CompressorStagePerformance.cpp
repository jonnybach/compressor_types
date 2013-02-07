/*
 * CompressorStagePerformance.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#include <fstream>
#include "CompressorStagePerformance.h"

CompressorStagePerformance::CompressorStagePerformance() {
	// TODO Auto-generated constructor stub
}

CompressorStagePerformance::CompressorStagePerformance(int opPntNumber,
	double ptRtrPt1Abs, double ptRtrPt1Rel,	double ptRtrPt2Rel, double ptStrPt1, double ptStrPt2,
	double ttRtrTt1Abs, double ptRtrTt1Rel,	double ptRtrTt2Rel, double ptStrTt1, double ptStrTt2,
	double betaRtr1,	double betaRtr2,	double alphaStr1,	double alphaStr2,
	double stagePi, double stageEtaAdb, double stageWork, double rotorPhi,
	double statorPhi, double wheelSpeed,
	double diffLiebRotor, double diffEqRatioRotor, double deHallerRotor,
	double diffLiebStator, double diffEqRatioStator, double deHallerStator,
	double stageReact, double rotorAspRat, double statorAspRat, double rotorNumAfs, double statorNumAfs) {
	m_opPntNmbr = opPntNumber;
	m_rtrPt1Abs = ptRtrPt1Abs;
	m_rtrPt1Rel = ptRtrPt1Rel;
	m_rtrPt2Rel = ptRtrPt2Rel;
	m_strPt1 = ptStrPt1;
	m_strPt2 = ptStrPt2;
	m_rtrTt1Abs = ttRtrTt1Abs;
	m_rtrTt1Rel = ptRtrTt1Rel;
	m_rtrTt2Rel = ptRtrTt2Rel;
	m_strTt1 = ptStrTt1;
	m_strTt2 = ptStrTt2;
	m_beta1 = betaRtr1;
	m_beta2 = betaRtr2;
	m_alpha3 = alphaStr1;
	m_alpha4 = alphaStr2;
	m_stgPi = stagePi;
	m_stgEtaAdb = stageEtaAdb;
	m_stgWrk = stageWork;
	m_rtrPhi = rotorPhi;
	m_sttrPhi = statorPhi;
	m_whlSpd = wheelSpeed;
	m_diffLiebRtr = diffLiebRotor;
	m_diffEqRatRtr = diffEqRatioRotor;
	m_deHallrRtr = deHallerRotor;
	m_diffLiebStr = diffLiebStator;
	m_diffEqRatStr = diffEqRatioStator;
	m_deHallrStr = deHallerStator;
	m_stgRct = stageReact;
	m_aspRatRtr = rotorAspRat;
	m_aspRatSttr = statorAspRat;
	m_numAfsRtr = rotorNumAfs;
	m_numAfsSttr = statorNumAfs;
}

CompressorStagePerformance::~CompressorStagePerformance() {
	// TODO Auto-generated destructor stub
	//printf("called compressor stage performance destructor\n\n");
}

//accessors
int CompressorStagePerformance::getOptPntNmbr() const { return m_opPntNmbr; }
double CompressorStagePerformance::getRotorPt1Abs() const { return m_rtrPt1Abs; }
double CompressorStagePerformance::getRotorPt1Rel() const { return m_rtrPt1Rel; }
double CompressorStagePerformance::getRotorPt2Rel() const { return m_rtrPt2Rel; }
double CompressorStagePerformance::getStatorPt1() const { return m_strPt1; }
double CompressorStagePerformance::getStatorPt2() const { return m_strPt2; }

double CompressorStagePerformance::getRotorTt1Abs() const { return m_rtrTt1Abs; }
double CompressorStagePerformance::getRotorTt1Rel() const { return m_rtrTt1Rel; }
double CompressorStagePerformance::getRotorTt2Rel() const { return m_rtrTt2Rel; }
double CompressorStagePerformance::getStatorTt1() const { return m_strTt1; }
double CompressorStagePerformance::getStatorTt2() const { return m_strTt2; }

double CompressorStagePerformance::getRotorBeta1() const { return m_beta1; }
double CompressorStagePerformance::getRotorBeta2() const { return m_beta2; }
double CompressorStagePerformance::getStatorAlpha1() const { return m_alpha3; }
double CompressorStagePerformance::getStatorAlpha2() const { return m_alpha4; }

double CompressorStagePerformance::getRotorBeta1Metal() const { return m_beta1m; }
double CompressorStagePerformance::getRotorBeta2Metal() const { return m_beta2m; }
double CompressorStagePerformance::getStatorAlpha1Metal() const { return m_alpha3m; }
double CompressorStagePerformance::getStatorAlpha2Metal() const { return m_alpha4m; }

double CompressorStagePerformance::getStagePi() const { return m_stgPi; }
double CompressorStagePerformance::getStageEtaAdiab() const { return m_stgEtaAdb; }
double CompressorStagePerformance::getStageWork() const { return m_stgWrk; }
double CompressorStagePerformance::getRotorPhi() const { return m_rtrPhi; }
double CompressorStagePerformance::getStatorPhi() const { return m_sttrPhi; }
double CompressorStagePerformance::getWheelSpeed() const { return m_whlSpd; }

double CompressorStagePerformance::getDiffLiebRotor() const { return m_diffLiebRtr; }
double CompressorStagePerformance::getDiffEqRatRotor() const { return m_diffEqRatRtr; }
double CompressorStagePerformance::getDeHallerRotor() const { return m_deHallrRtr; }

double CompressorStagePerformance::getDiffLiebStator() const { return m_diffLiebStr; }
double CompressorStagePerformance::getDiffEqRatStator() const { return m_diffEqRatStr; }
double CompressorStagePerformance::getDeHallerStator() const { return m_deHallrStr; }

double CompressorStagePerformance::getStageReaction() const { return m_stgRct; }

double CompressorStagePerformance::getRotorAspRatio() const { return m_aspRatRtr; }
double CompressorStagePerformance::getStatorAspRatio() const { return m_aspRatSttr; }

double CompressorStagePerformance::getRotorNumAirfoils() const {return m_numAfsRtr; }
double CompressorStagePerformance::getStatorNumAirfoils() const {return m_numAfsSttr; }
