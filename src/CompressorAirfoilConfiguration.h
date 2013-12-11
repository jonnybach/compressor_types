/*
 * CompressorAirfoilConfiguration.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORAIRFOILCONFIGURATION_H_
#define COMPRESSORAIRFOILCONFIGURATION_H_

#include "CompressorEnumerations.h"

class AnnulusPoint;

class CompressorAirfoilConfiguration {
public:
	CompressorAirfoilConfiguration();

	CompressorAirfoilConfiguration(
		AirfoilType airfoilType,
		int numBlades,
		double sqc,
		double tqc,
		double aqc,
		double oqs,
		double shapeK,
		double stagrAng,
		double betaMtlIn,
		double betaMtlOut,
		double tipClrToHgt,
		double betaInAir,
		double delBetaAir,
		double avdr,
		double machIn,
		double delDevn,
		double delLoss,
		double bleedFrac
		);

	virtual ~CompressorAirfoilConfiguration();

	void setDesignedParams(double sqc, double tqc, double betaMtlIn, double betaMtlOut
			, double cambrAng, double stagrAng);
	void setHpaConfig(double betaInAir, double delBetaAir, double tqc, double avdr, double machIn);

	AirfoilType getAirfoilType();

	int getNumBlades();

	double getSqc();
	double getTqc();

	double getAqc();
	double getOqs();
	double getShapeK();

	double getCamberAng();
	double getStagrAng();

	double getBetaMtlIn();
	double getBetaMtlOut();

	double getTipClrToHgt();

	double getBetaIn();
	double getDelBetaM();
	double getAvdr();
	double getMachIn();

	double getDelDevn();
	double getDelLoss();

	//double getDsnStagePratio();
	//double getDsnAlphaExit();

	bool hasBleed();
	double getBleedFrac();
	void setBleedFrac(double bleedFraction);

	AnnulusPoint *getHubLe();
	AnnulusPoint *getHubTe();
	AnnulusPoint *getCaseLe();
	AnnulusPoint *getCaseTe();

	void setSailPoints( AnnulusPoint *hubLe, AnnulusPoint *hubTe, AnnulusPoint *caseLe, AnnulusPoint *caseTe );

	double calcXnull();

private:

	AirfoilType m_airfoilType;

	int m_numBlades;

	double m_sqc;		//pitch to chord
	double m_tqc;		//thickness to chord
	double m_aqc;		//location max thickness to chord
	double m_oqs;		//throat to pitch

	double m_shapeK;	//shape coeff.  Consult NACA SP 36.
						// Related to how similar the airfoil is to the standard NACA/DCA airfoils
						// from the correlations

	double m_camberAng; //camber angle  (deg)
	double m_stagrAng;  //stagger angle (deg)
	double m_betaMin;	//in metal angle (deg)
	double m_betaMout;	//out metal angle (deg)

	double m_tipClrToHgt; //ratio of tip clearance to height

	//HPA specific parameters
	double m_betaIn;	  //relative air inlet angle
	double m_delBeta;	  //change in air angle in to out (flow turning)
	double m_avdr;		  //axial velocity density ratio
	double m_machIn;		  //approach mach number

	double m_delDevn;	  //Comp1D specific deviation correction adder
	double m_delLoss;	  //Comp1D specific loss correction adder

	double m_bleedFrac;	  //fraction amount of inlet mass flow extracted just upstream of airfoil row
	bool m_hasBleed;      //boolean to indicate whether stage has bleed extraction just upstream

	AnnulusPoint *m_hubLe, *m_hubTe;
	AnnulusPoint *m_caseLe, *m_caseTe;

};

#endif /* COMPRESSORAIRFOILCONFIGURATION_H_ */
