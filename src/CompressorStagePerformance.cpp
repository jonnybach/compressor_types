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
	double stageReact ) {
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
}

CompressorStagePerformance::~CompressorStagePerformance() {
	// TODO Auto-generated destructor stub
	//printf("called compressor stage performance destructor\n\n");
}

//accessors
int CompressorStagePerformance::getOptPntNmbr() { return m_opPntNmbr; }
double CompressorStagePerformance::getRotorPt1Abs() { return m_rtrPt1Abs; }
double CompressorStagePerformance::getRotorPt1Rel() { return m_rtrPt1Rel; }
double CompressorStagePerformance::getRotorPt2Rel() { return m_rtrPt2Rel; }
double CompressorStagePerformance::getStatorPt1() { return m_strPt1; }
double CompressorStagePerformance::getStatorPt2() { return m_strPt2; }

void CompressorStagePerformance::setRotorPt1Abs(double Pt) { m_rtrPt1Abs = Pt;}
void CompressorStagePerformance::setRotorPt1Rel(double Pt) { m_rtrPt1Rel = Pt;}
void CompressorStagePerformance::setRotorPt2Rel(double Pt) { m_rtrPt2Rel = Pt;}
void CompressorStagePerformance::setStatorPt1(double Pt) { m_strPt1 = Pt;}
void CompressorStagePerformance::setStatorPt2(double Pt) { m_strPt2 = Pt;}

double CompressorStagePerformance::getRotorTt1Abs() { return m_rtrTt1Abs; }
double CompressorStagePerformance::getRotorTt1Rel() { return m_rtrTt1Rel; }
double CompressorStagePerformance::getRotorTt2Rel() { return m_rtrTt2Rel; }
double CompressorStagePerformance::getStatorTt1() { return m_strTt1; }
double CompressorStagePerformance::getStatorTt2() { return m_strTt2; }

void CompressorStagePerformance::setRotorTt1Abs(double Tt) { m_rtrTt1Abs = Tt;}
void CompressorStagePerformance::setRotorTt1Rel(double Tt) { m_rtrTt1Rel = Tt;}
void CompressorStagePerformance::setRotorTt2Rel(double Tt) { m_rtrTt2Rel = Tt;}
void CompressorStagePerformance::setStatorTt1(double Tt) { m_strTt1 = Tt;}
void CompressorStagePerformance::setStatorTt2(double Tt) { m_strTt2 = Tt;}

double CompressorStagePerformance::getRotorBeta1() { return m_beta1; }
double CompressorStagePerformance::getRotorBeta2() { return m_beta2; }
double CompressorStagePerformance::getStatorAlpha1() { return m_alpha3; }
double CompressorStagePerformance::getStatorAlpha2() { return m_alpha4; }

double CompressorStagePerformance::getStagePi() { return m_stgPi; }
double CompressorStagePerformance::getStageEtaAdiab() { return m_stgEtaAdb; }
double CompressorStagePerformance::getStageWork() { return m_stgWrk; }
double CompressorStagePerformance::getRotorPhi() { return m_rtrPhi; }
double CompressorStagePerformance::getStatorPhi() { return m_sttrPhi; }
double CompressorStagePerformance::getWheelSpeed() { return m_whlSpd; }

double CompressorStagePerformance::getDiffLiebRotor() { return m_diffLiebRtr; }
double CompressorStagePerformance::getDiffEqRatRotor() { return m_diffEqRatRtr; }
double CompressorStagePerformance::getDeHallerRotor() { return m_deHallrRtr; }

double CompressorStagePerformance::getDiffLiebStator() { return m_diffLiebStr; }
double CompressorStagePerformance::getDiffEqRatStator() { return m_diffEqRatStr; }
double CompressorStagePerformance::getDeHallerStator() { return m_deHallrStr; }

double CompressorStagePerformance::getStageReaction() { return m_stgRct; }
