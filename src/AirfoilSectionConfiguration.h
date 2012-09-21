/*
 * AirfoilSectionConfiguration.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#ifndef AIRFOILSECTIONCONFIGURATION_H_
#define AIRFOILSECTIONCONFIGURATION_H_

#include "CompressorEnumerations.h"
#include "AnnulusPoint.h"

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
		double delBeta,
		double avdr,
		double machIn,
		double delDevn,
		double delLoss
	);

	AirfoilSectionConfiguration(AnnulusPoint lePoint, AnnulusPoint tePoint,
		double pqc,
		double tqc,
		double aqc,
		double chord,
		double stagrAng,
		double betaMtlIn,
		double betaMtlOut,
		double beta1,
		double delBeta,
		double avdr,
		double machIn,
		double delDevn,
		double delLoss
	);

	virtual ~AirfoilSectionConfiguration();

	AnnulusPoint getLePoint() const;
	void setLePoint(AnnulusPoint newLePnt);

	AnnulusPoint getTePoint() const;
	void setTePoint(AnnulusPoint newTePnt);

	void setHpaConfig(double beta1, double delBeta, double tqc, double avdr, double machIn);

	double getRadiusMean() const;
	//double getRadius() const;

	double getPqC() const;
	double getTqC() const;
	double getAqC() const;

	double getThickness() const;
	double getChord() const;
	double getAxialChord() const;

	double getStagrAng() const;
	double getBetaMtlIn() const;
	double getBetaMtlOut() const;

	double getBeta1() const;
	double getDelBeta() const;
	double getAvdr() const;
	double getMachIn() const;

	double getDelDevn() const;
	double getDelLoss() const;

private:

	AnnulusPoint _lePnt; // x, Rad (meters)
	AnnulusPoint _tePnt; // x, Rad (meters)

	//double _radius;    //radial location of airfoil section

	double _pqc;		//pitch to chord
	double _tqc;		//thickness to chord
	double _aqc;		//relative CG to chord

	double _chord;		//absolute chord  (meters)

	double _stagrAng;   //stagger angle (deg)  - NOTE: output for HPA airfoils
	double _betaMin;	//in metal angle (deg)  - NOTE: output for HPA airfoils
	double _betaMout;	//out metal angle (deg) - NOTE: output for HPA airfoils

	//HPA specific parameters
	double _beta1;		  //relative inlet flow angle (deg)
	double _delBeta;	  //change in flow angle in to out (deg)
	double _avdr;		  //axial velocity density ratio
	double _machIn;		  //approach mach number

	double _delDevn;	  //Comp1D specific deviation correction adder
	double _delLoss;	  //Comp1D specific loss correction adder

};

#endif /* AIRFOILSECTIONCONFIGURATION_H_ */
