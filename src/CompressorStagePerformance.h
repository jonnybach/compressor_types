/*
 * CompressorStagePerformance.h
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSTAGEPERFORMANCE_H_
#define COMPRESSORSTAGEPERFORMANCE_H_

class CompressorStagePerformance {

public:
	//methods
	CompressorStagePerformance();

	CompressorStagePerformance(int opPntNumber,
	    double rotorPt1Abs,
		double rotorPt1Rel,
		double rotorPt2Rel,
		double rotorPs1,
		double rotorPs2,
		double statorPt1,
		double statorPt2,
        double statorPs1,
        double statorPs2,
		double rotorTt1Abs,
		double rotorTt1Rel,
		double rotorTt2Rel,
		double statorTt1,
		double statorTt2,
		double rotorBeta1,
		double rotorBeta2,
		double statorAlpha1,
		double statorAlpha2,
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
		int rotorNumAfs,
		int statorNumAfs,
		double rotorIncidence,
		double statorIncidence,
		double rotorDeviation,
		double statorDeviation
	);

	virtual ~CompressorStagePerformance();

	//accessors (properties)
	int getOptPntNmbr() const;

	double getRotorPt1Abs() const;
	double getRotorPt1Rel() const;
	double getRotorPt2Rel() const;

	double getStatorPt1() const;
	double getStatorPt2() const;

	double getRotorPs1() const;
	double getRotorPs2() const;

	double getStatorPs1() const;
	double getStatorPs2() const;

	double getRotorTt1Abs() const;
	double getRotorTt1Rel() const;
	double getRotorTt2Rel() const;

	double getStatorTt1() const;
	double getStatorTt2() const;

	double getRotorTs1() const;
	double getRotorTs2() const;
	double getStatorTs1() const;
	double getStatorTs2() const;

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

    double getRotorIncidence() const;
    double getStatorIncidence() const;

    double getRotorDeviation() const;
    double getStatorDeviation() const;

private:
	//members
	int m_opPntNmbr;

	double m_rtrPt1Abs;
	double m_rtrPt1Rel;
	double m_rtrPt2Rel;
	double m_strPt1;
	double m_strPt2;

	double m_rtrPs1;
	double m_rtrPs2;
	double m_sttrPs1;
	double m_sttrPs2;

	double m_rtrTt1Abs;
	double m_rtrTt1Rel;
	double m_rtrTt2Rel;
	double m_strTt1;
	double m_strTt2;

    double m_rtrTs1;
    double m_rtrTs2;
    double m_sttrTs1;
    double m_sttrTs2;

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

	double m_incRtr;  //rotor incidence angle (deg)
	double m_incSttr; //stator incidence angle (deg)

	double m_devRtr; //rotor deviation angle (deg)
	double m_devSttr; //stator deviation angle (deg)

};

#endif /* COMPRESSORSTAGEPERFORMANCE_H_ */
