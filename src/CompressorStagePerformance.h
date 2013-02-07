/*
 * CompressorStagePerformance.h
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSTAGEPERFORMANCE_H_
#define COMPRESSORSTAGEPERFORMANCE_H_

//forward declarations
class CompressorOperatingPoint;

class CompressorStagePerformance {

public:
	//methods
	CompressorStagePerformance();

	CompressorStagePerformance(int opPntNumber,
		double ptRtrPt1Abs,
		double ptRtrPt1Rel,
		double ptRtrPt2Rel,
		double ptStrPt1Rel,
		double ptStrPt2Rel,
		double ptRtrTt1Abs,
		double ptRtrTt1Rel,
		double ptRtrTt2Rel,
		double ptStrTt1Rel,
		double ptStrtt2Rel,
		double beta1,
		double beta2,
		double alpha3,
		double alpha4,
		double stagePi,
		double stageEtaAdb,
		double stageWork,
		double rotorPhi,
		double statorPhi,
		double wheelSpeed,
		double diffLiebRotor,
		double diffEqRatioRotor,
		double deHallerRotor,
		double diffLiebStator,
		double diffEqRatioStator,
		double deHallerStator,
		double stageReact,
		double rotorAspRat,
		double statorAspRat,
		double rotorNumAfs,
		double statorNumAfs
	);

	virtual ~CompressorStagePerformance();

	//accessors (properties)
	int getOptPntNmbr() const;

	double getRotorPt1Abs() const;
	double getRotorPt1Rel() const;
	double getRotorPt2Rel() const;

	double getStatorPt1() const;
	double getStatorPt2() const;

	double getRotorTt1Abs() const;
	double getRotorTt1Rel() const;
	double getRotorTt2Rel() const;

	double getStatorTt1() const;
	double getStatorTt2() const;

	double getRotorBeta1() const;
	double getRotorBeta2() const;
	double getStatorAlpha1() const;
	double getStatorAlpha2() const;

	double getRotorBeta1Metal() const;
	double getRotorBeta2Metal() const;
	double getStatorAlpha1Metal() const;
	double getStatorAlpha2Metal() const;

	double getStagePi() const;
	double getStageEtaAdiab() const;
	double getStageWork() const;
	double getRotorPhi() const;
	double getStatorPhi() const;
	double getWheelSpeed() const;

	double getDiffLiebRotor() const;
	double getDiffEqRatRotor() const;
	double getDeHallerRotor() const;

	double getDiffLiebStator() const;
	double getDiffEqRatStator() const;
	double getDeHallerStator() const;

	double getStageReaction() const;

	double getRotorAspRatio() const;
	double getStatorAspRatio() const;

	double getRotorNumAirfoils() const;
	double getStatorNumAirfoils() const;

private:
	//members
	int m_opPntNmbr;

	double m_rtrPt1Abs;
	double m_rtrPt1Rel;
	double m_rtrPt2Rel;
	double m_strPt1;
	double m_strPt2;

	double m_rtrTt1Abs;
	double m_rtrTt1Rel;
	double m_rtrTt2Rel;
	double m_strTt1;
	double m_strTt2;

	double m_beta1;
	double m_beta2;
	double m_alpha3;
	double m_alpha4;

	double m_stgPi;
	double m_stgEtaAdb;
	double m_stgWrk;
	double m_rtrPhi;
	double m_sttrPhi;
	double m_whlSpd;

	double m_diffLiebRtr;
	double m_diffEqRatRtr;
	double m_deHallrRtr;

	double m_diffLiebStr;
	double m_diffEqRatStr;
	double m_deHallrStr;

	double m_stgRct;

	double m_beta1m;
	double m_beta2m;
	double m_alpha3m;
	double m_alpha4m;

	double m_aspRatRtr;
	double m_aspRatSttr;

	double m_numAfsRtr;
	double m_numAfsSttr;

};

#endif /* COMPRESSORSTAGEPERFORMANCE_H_ */
