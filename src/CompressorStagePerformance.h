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
		double stageReact);

	virtual ~CompressorStagePerformance();

	//accessors (properties)
	int getOptPntNmbr();

	double getRotorPt1Abs();
	void setRotorPt1Abs(double rotorPt1Abs);

	double getRotorPt1Rel();
	void setRotorPt1Rel(double rotorPt1Rel);

	double getRotorPt2Rel();
	void setRotorPt2Rel(double rotorPt2Rel);

	double getStatorPt1();
	void setStatorPt1(double PtStator1);

	double getStatorPt2();
	void setStatorPt2(double statorPt2);

	double getRotorTt1Abs();
	void setRotorTt1Abs(double rotorTt1Abs);

	double getRotorTt1Rel();
	void setRotorTt1Rel(double rotorPt1Rel);

	double getRotorTt2Rel();
	void setRotorTt2Rel(double rotorPt2Rel);

	double getStatorTt1();
	void setStatorTt1(double PtStator1);

	double getStatorTt2();
	void setStatorTt2(double statorPt2);

	double getRotorBeta1();
	double getRotorBeta2();
	double getStatorAlpha1();
	double getStatorAlpha2();

	double getStagePi();
	double getStageEtaAdiab();
	double getStageWork();
	double getRotorPhi();
	double getStatorPhi();
	double getWheelSpeed();

	double getDiffLiebRotor();
	double getDiffEqRatRotor();
	double getDeHallerRotor();

	double getDiffLiebStator();
	double getDiffEqRatStator();
	double getDeHallerStator();

	double getStageReaction();

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

};

#endif /* COMPRESSORSTAGEPERFORMANCE_H_ */
