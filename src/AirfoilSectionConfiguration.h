/*
 * AirfoilSectionConfiguration.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#ifndef AIRFOILSECTIONCONFIGURATION_H_
#define AIRFOILSECTIONCONFIGURATION_H_

#include "CompressorEnumerations.h"

class AnnulusPoint;

class AirfoilSectionConfiguration {
public:
	AirfoilSectionConfiguration();

	AirfoilSectionConfiguration(double radius,
		double pqc,
		double tqc,
		double aqc,
		double chord,
		double stagrAng,
		double betaMtlIn,
		double betaMtlOut,
		double beta1,
		double delBetaM,
		double avdr,
		double machIn,
		double delDevn,
		double delLoss
	);

	virtual ~AirfoilSectionConfiguration();

	AnnulusPoint *getLePoint();
	void setLePoint(AnnulusPoint *newLePnt);

	AnnulusPoint *getTePoint();
	void setTePoint(AnnulusPoint *newTePnt);

	void setHpaConfig(double beta1metal, double delBetaM, double tqc, double avdr, double machIn);

	double getRadius();

	double getPqC();
	double getTqC();
	double getAqC();

	double getThickness();
	double getChord();

	double getStagrAng();
	double getBetaMtlIn();
	double getBetaMtlOut();

	double getDelBetaM();
	double getAvdr();
	double getMachIn();

	double getDelDevn();
	double getDelLoss();

private:

	AnnulusPoint *_lePnt;
	AnnulusPoint *_tePnt;

	double _radius;    //radial location of airfoil section

	double _pqc;		//pitch to chord
	double _tqc;		//thickness to chord
	double _aqc;		//relative CG to chord

	double _chord;		//absolute chord

	double _stagrAng;   //stagger angle (deg)
	double _betaMin;	//in metal angle (deg)
	double _betaMout;	//out metal angle (deg)

	//HPA specific parameters
	double _beta1;		  //relative inlet flow angle (deg)
	double _delBetaM;	  //change in metal angle in to out
	double _avdr;		  //axial velocity density ratio
	double _machIn;		  //approach mach number

	double _delDevn;	  //Comp1D specific deviation correction adder
	double _delLoss;	  //Comp1D specific loss correction adder

};

#endif /* AIRFOILSECTIONCONFIGURATION_H_ */
