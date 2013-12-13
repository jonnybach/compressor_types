/*
 * CompressorStagePerformance.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#include "CompressorStagePerformance.h"

CompressorStagePerformance::CompressorStagePerformance() {
	// TODO Auto-generated constructor stub
}

CompressorStagePerformance::CompressorStagePerformance(int opPntNumber,
	double rotorPt1Abs, double rotorPt1Rel,	double rotorPt2Rel,
    double rotorPs1, double rotorPs2,
	double statorPt1, double statorPt2,
	double statorPs1, double statorPs2,
	double rotorTt1Abs, double rotorTt1Rel,	double rotorTt2Rel,
	double statorTt1, double statorTt2,
	double rotorBeta1,	double rotorBeta2,	double statorAlpha1, double statorAlpha2,
	double stagePi, double stageEtaAdb, double stageWork, double rotorPhi,
	double statorPhi, double wheelSpeed,
	double diffLiebRotor, double diffEqRatioRotor, double deHallerRotor,
	double diffLiebStator, double diffEqRatioStator, double deHallerStator,
	double stageReact, double rotorAspRat, double statorAspRat, int rotorNumAfs, int statorNumAfs,
	double rotorIncidence, double statorIncidence, double rotorDeviation, double statorDeviation) {

	m_opPntNmbr = opPntNumber;

	m_rtrPt1Abs = rotorPt1Abs;
	m_rtrPt1Rel = rotorPt1Rel;
	m_rtrPt2Rel = rotorPt2Rel;

	m_rtrPs1 = rotorPs1;
	m_rtrPs2 = rotorPs2;

	m_strPt1 = statorPt1;
	m_strPt2 = statorPt2;

    m_sttrPs1 = statorPs1;
    m_sttrPs2 = statorPs2;

	m_rtrTt1Abs = rotorTt1Abs;
	m_rtrTt1Rel = rotorTt1Rel;
	m_rtrTt2Rel = rotorTt2Rel;

	m_sttrTt1 = statorTt1;
	m_sttrTt2 = statorTt2;

	m_beta1 = rotorBeta1;
	m_beta2 = rotorBeta2;

	m_alpha3 = statorAlpha1;
	m_alpha4 = statorAlpha2;

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

    m_incRtr = rotorIncidence;
    m_incSttr = statorIncidence;

    m_devRtr = rotorDeviation;
    m_devSttr = statorDeviation;

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

double CompressorStagePerformance::getRotorPs1() const { return m_rtrPs1; }
double CompressorStagePerformance::getRotorPs2() const { return m_rtrPs2; }
double CompressorStagePerformance::getStatorPs1() const { return m_sttrPs1; }
double CompressorStagePerformance::getStatorPs2() const { return m_sttrPs2; }

double CompressorStagePerformance::getRotorTt1Abs() const { return m_rtrTt1Abs; }
double CompressorStagePerformance::getRotorTt1Rel() const { return m_rtrTt1Rel; }
double CompressorStagePerformance::getRotorTt2Rel() const { return m_rtrTt2Rel; }
double CompressorStagePerformance::getStatorTt1() const { return m_sttrTt1; }
double CompressorStagePerformance::getStatorTt2() const { return m_sttrTt2; }

double CompressorStagePerformance::getRotorTs1() const { return m_rtrTs1; }
double CompressorStagePerformance::getRotorTs2() const { return m_rtrTs2; }
double CompressorStagePerformance::getStatorTs1() const { return m_sttrTs1; }
double CompressorStagePerformance::getStatorTs2() const { return m_sttrTs2; }

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

double CompressorStagePerformance::getRotorIncidence() const { return m_incRtr; }
double CompressorStagePerformance::getStatorIncidence() const { return m_incSttr; }

double CompressorStagePerformance::getRotorDeviation() const { return m_devRtr; }
double CompressorStagePerformance::getStatorDeviation() const { return m_devSttr; }
