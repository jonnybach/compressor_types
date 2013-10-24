/*
 * AirfoilSectionConfiguration.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 *
 *  This class represents the input values into STARMEP (or other meridional flow solver) that have been
 *   interpolated or re-calculated at the last solved streamline location.  This is different from the original
 *   user inputs b/c the original inputs do not necessarily lie along a the solved streamline locations but rather
 *   arbitrary locations based on geometrical requirements.
 *
 */

#ifndef AIRFOILSECTIONCONFIGURATION_H_
#define AIRFOILSECTIONCONFIGURATION_H_

#include "CompressorEnumerations.h"
#include "AirfoilSectionResult.h"
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
		double machInRel,
		double delDevn,
		double delLoss
	);

	virtual ~AirfoilSectionConfiguration();

	AirfoilSectionResult getResult() const;
	void setResult(AirfoilSectionResult newResult);

	AnnulusPoint getLePoint() const;
	void setLePoint(AnnulusPoint newLePnt);

	AnnulusPoint getTePoint() const;
	void setTePoint(AnnulusPoint newTePnt);

	void setHpaConfig(double beta1, double delBeta, double tqc, double avdr, double machIn);

	double getRadiusDelta() const;
	double getRadiusMean() const;

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
	double getMachInRel() const;

	double getDelDevn() const;
	double getDelLoss() const;

private:

	AnnulusPoint _lePnt; // x, Rad (meters)
	AnnulusPoint _tePnt; // x, Rad (meters)

	AirfoilSectionResult m_result;

	//double _radius;    //radial location of airfoil section

	//NOTE: values below represent inputs defined by the user of STARMEP.  A set of results should exists that represent the true values used from the calculation

	double _pqc;		//pitch to chord
	double _tqc;		//thickness to chord
	double _aqc;		//relative CG to chord

	double _chord;		//absolute chord  (meters)

	double _stagrAng;   //stagger angle (deg)  - NOTE: output for HPA airfoils
	double _betaMin;	//in metal angle (deg)  - NOTE: output for HPA airfoils
	double _betaMout;	//out metal angle (deg) - NOTE: output for HPA airfoils

	double _machInRel;    //inlet relative mach number

	double _beta1;		  //relative inlet flow angle (deg)
	double _delBeta;	  //change in flow angle in to out (deg)
	double _avdr;		  //axial velocity density ratio

	double _delDevn;	  //Comp1D specific deviation correction adder
	double _delLoss;	  //Comp1D specific loss correction adder

};

#endif /* AIRFOILSECTIONCONFIGURATION_H_ */
