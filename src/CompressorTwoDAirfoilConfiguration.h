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
	AirfoilSectionConfiguration getSectionConfigAtIndex(int index);
	const std::vector<AnnulusPoint> getLeSectPoints();
	const std::vector<AnnulusPoint> getTeSectPoints();
	const AnnulusPoint getLeSectPointAtIndex(int index);
	const AnnulusPoint getTeSectPointAtIndex(int index);
	size_t maxSectNo();

	//methods for sail point collections, who's number of entries may not coincide with
	// the number of section configurations, ugh!
	void setLeSailPoints( const std::vector<AnnulusPoint> newLePoints );
	void setTeSailPoints( const std::vector<AnnulusPoint> newLePoints );
	const std::vector<AnnulusPoint> getLeSailPoints();
	const std::vector<AnnulusPoint> getTeSailPoints();
	const AnnulusPoint getLeSailPointAtIndex(int index);
	const AnnulusPoint getTeSailPointAtIndex(int index);

	AirfoilType getAirfoilType();
	void setAirfoilType(AirfoilType newAirfoilType);

	int getNumAirfoils();
	void setNumAirfoils(int numAirfoils);

	std::string getName();

	double getXnull();
	void setXnull(double newXnull);

private:

	std::string _name;

	std::vector<AnnulusPoint> _sailPntsLe;
	std::vector<AnnulusPoint> _sailPntsTe;

	std::vector<AirfoilSectionConfiguration> _sectConfigs;

	AirfoilType _airfoilType;
	AirfoilBehavior _airfoilBhvr;
	int _numBlades;

	TipClearanceSpecification _tipClrSpec;
	double _tipClearance;

	double _xNull;

};

#endif /* COMPRESSORTWODAIRFOILCONFIGURATION_H_ */
