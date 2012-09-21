/*
 * CompressorTwoDAirfoilConfiguration.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORTWODAIRFOILCONFIGURATION_H_
#define COMPRESSORTWODAIRFOILCONFIGURATION_H_

#include <string>
#include <vector>
#include "CompressorEnumerations.h"
#include "AirfoilSectionConfiguration.h"
#include "AnnulusPoint.h"

class CompressorTwoDAirfoilConfiguration {
public:
	CompressorTwoDAirfoilConfiguration();
	CompressorTwoDAirfoilConfiguration(std::string airfoilName, AirfoilBehavior airfoilBehr);
	virtual ~CompressorTwoDAirfoilConfiguration();

	void addSectionConfiguration(AirfoilSectionConfiguration newSectConfig);
	std::vector<AirfoilSectionConfiguration> getSections();
	AirfoilSectionConfiguration getSectionConfigAtIndex(int index);
	const AirfoilSectionConfiguration *getSectionNearSpanFraction(double frac);
	const AirfoilSectionConfiguration *getSectionAtMidspan();
	const AirfoilSectionConfiguration *getSectionAtHub();
	const AirfoilSectionConfiguration *getSectionAtTip();

	std::vector<AnnulusPoint> getLeSectPoints();
	std::vector<AnnulusPoint> getTeSectPoints();
	AnnulusPoint getSectLePointAtIndex(int index);
	AnnulusPoint getSectTePointAtIndex(int index);
	size_t maxSectNo();

	//methods for sail point collections,
	// note that the number of values of sail points
	// coincide with those from the STARMEP out file.
	// but the radial values are not the same as that from the OUT file
	void setLeSailPoints( std::vector<AnnulusPoint> newLePoints );
	void setTeSailPoints( std::vector<AnnulusPoint> newLePoints );
	std::vector<AnnulusPoint> getLeSailPoints();
	std::vector<AnnulusPoint> getTeSailPoints();
	AnnulusPoint getLeSailPointAtIndex(int index);
	AnnulusPoint getTeSailPointAtIndex(int index);

	AirfoilBehavior getAirfoilBehavior();
	void setAirfoiilBehavior(AirfoilBehavior newBehavior);

	AirfoilType getAirfoilType();
	void setAirfoilType(AirfoilType newAirfoilType);

	int getNumAirfoils();
	void setNumAirfoils(int numAirfoils);

	std::string getName();
	void setName(std::string newName);

	double getXnull();
	void setXnull(double newXnull);

	double getSpan();

private:

	//void updateSectionLeAndTePnts();

	std::string _name;

	std::vector<AnnulusPoint> _sailPntsLe;
	std::vector<AnnulusPoint> _sailPntsTe;

	std::vector<AirfoilSectionConfiguration> _sectConfigs;

	AirfoilBehavior _airfoilBhvr;
	AirfoilType _airfoilType;
	int _numBlades;

	//TipClearanceSpecification _tipClrSpec;
	//double _tipClearance;

	double _xNull; //meters

};

#endif /* COMPRESSORTWODAIRFOILCONFIGURATION_H_ */
