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
		int getNumBlades,
		double sqc,
		double tqc,
		double aqc,
		double oqs,
		double shapeK,
		double stagrAng,
		double betaMtlIn,
		double betaMtlOut,
		double tipClrToHgt,
		double delBetaM,
		double avdr,
		double machIn,
		double delDevn,
		double delLoss,
		//double dsnStagePratio,
		//double dsnAlphaExit,
		double bleedFrac
		);

	virtual ~CompressorAirfoilConfiguration();

	void setHpaConfig(double beta1metal, double delBetaM, double tqc, double avdr, double machIn);

	AirfoilType getAirfoilType();

	int getNumBlades();

	double getSqc();
	double getTqc();
	double getAqc();
	double getOqs();

	double getShapeK();

	double getStagrAng();
	double getBetaMtlIn();
	double getBetaMtlOut();

	double getTipClrToHgt();

	double getDelBetaM();
	double getAvdr();
	double getMachIn();

	double getDelDevn();
	double getDelLoss();

	//double getDsnStagePratio();
	//double getDsnAlphaExit();

	double getBleedFrac();
	void setBleedFrac(double bleedFraction);

	AnnulusPoint *getHubLe();
	AnnulusPoint *getHubTe();
	AnnulusPoint *getCaseLe();
	AnnulusPoint *getCaseTe();

	void setSailPoints( AnnulusPoint *hubLe, AnnulusPoint *hubTe, AnnulusPoint *caseLe, AnnulusPoint *caseTe );

	double calcXnull();

private:

	AirfoilType _airfoilType;

	int _numBlades;

	double _sqc;		//pitch to chord
	double _tqc;		//thickness to chord
	double _aqc;		//location max thickness to chord
	double _oqs;		//throat to pitch

	double _shapeK;		//shape coeff.  Consult NACA SP 36.  I think they talk about this.
						// Related to how similar the airfoil is to the standard NACA/DCA airfoils
						// from the correlations

	double _stagrAng;   //stagger angle (deg)
	double _betaMin;	//in metal angle (deg)
	double _betaMout;	//out metal angle (deg)

	double _tipClrToHgt; //ratio of tip clearance to height

	//HPA specific parameters
	double _delBetaM;	  //change in metal angle in to out
	double _avdr;		  //axial velocity density ratio
	double _machIn;		  //approach mach number

	double _delDevn;	  //Comp1D specific deviation correction adder
	double _delLoss;	  //Comp1D specific loss correction adder

	//double _stagePratio;  //pressure ratio across stage (design parameter for blades)
	//double _alphaExit;	  //absolute exit flow angle (design parameter for vanes)

	double _bleedFrac;	  //fraction amount of inlet mass flow extracted just upstream of airfoil row

	AnnulusPoint *_hubLe, *_hubTe;
	AnnulusPoint *_caseLe, *_caseTe;

};

#endif /* COMPRESSORAIRFOILCONFIGURATION_H_ */
