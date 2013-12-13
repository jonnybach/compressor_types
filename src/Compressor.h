/*
 * Compressor.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#include "CompressorStage.h"
#include "CompressorOperatingPoint.h"
#include "CompressorEnumerations.h"
#include "CompressorTwoDAirfoilConfiguration.h"
#include "CompressorLeakage.h"

class CompressorAnnulus;
class Inlet;
class InletGuideVane;
class Diffuser;
class LossCorrelationData;

class Compressor {
public:
	Compressor();
	virtual ~Compressor();;

	CompressorAnnulus *getAnnulus();
	void setAnnulus(CompressorAnnulus *newAnnulus);

	Inlet *getInlet();
	void setInlet(Inlet *newInlet);

	InletGuideVane *getIgv();
	void setIgv(InletGuideVane *newIgv);

	Diffuser *getDiffuser();
	void setDiffuser(Diffuser *newDiffuser);

	CompressorOperatingPoint *getDesignPnt();
	void setDesignPnt(CompressorOperatingPoint *newDesignPnt);

	LossCorrelationData *getLossCorrData();
	void setLossCorrData(LossCorrelationData *newLossCorrData);

	std::vector<CompressorLeakage> getLeakages();
	void setLeakages(std::vector<CompressorLeakage> newLeakages);

	TipClearanceSpecification getClearanceSpec();
	void setClearnaceSpec(TipClearanceSpecification clearncSpec);

	void insertStageAtIndex(int stageIndex, CompressorStage *newStage );
	std::vector<CompressorStage *> getStages();
	CompressorStage *stageWithNumber(int stageNumber);
	CompressorStage *stageWithName(std::string stageName);
	CompressorStage *findOrCreateStageWithName(std::string stageName);

	void setVgvRuleAtStage(int stageNumber, double ruleFactor);
	double getVgvRuleAtStage(int stageNumber);

	void addTwoDAirfoilConfig(CompressorTwoDAirfoilConfiguration newConfig );
	CompressorTwoDAirfoilConfiguration *getAirfoilTwoDConfigAtIndex(int index);
	CompressorTwoDAirfoilConfiguration *getAirfoilTwoDConfigWithName(std::string name);
	std::vector<CompressorTwoDAirfoilConfiguration> getAirfoilTwoDConfigs();


private:
	CompressorAnnulus *_annulus;
	Inlet *_inlet;
	InletGuideVane *_igv;
	Diffuser *_diffuser;
	LossCorrelationData *_lossCorrData;

	std::vector<CompressorLeakage> m_leaks;

	CompressorOperatingPoint *_desPnt;

	std::vector<CompressorStage *> _stages;
	std::vector<CompressorOperatingPoint *> _opPnts;
	std::vector<double> _vaneSchedule;

	TipClearanceSpecification _clearncSpec;

	std::vector<CompressorTwoDAirfoilConfiguration> _airflTwoDConfigs;

};

#endif /* COMPRESSOR_H_ */
