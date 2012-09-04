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

enum ClearanceSpecification {
	CLEARANCE_NONE = 0,
	CLEARANCE_RATIO = 1,    // ratio of clearance to height
	CLEARANCE_ABSOLUTE = 2  // absolute values specified in millimeters
};

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

	ClearanceSpecification getClearanceSpec();
	void setClearnaceSpec(ClearanceSpecification clearncSpec);

	void insertStageAtIndex(int stageIndex, CompressorStage *newStage );
	std::vector<CompressorStage *> getStages();
	CompressorStage *stageWithNumber(int stageNumber);
	CompressorStage *stageWithName(std::string stageName);
	CompressorStage *findOrCreateStageWithName(std::string stageName);

	void setVgvRuleAtStage(int stageNumber, double ruleFactor);
	double getVgvRuleAtStage(int stageNumber);

private:
	CompressorAnnulus *_annulus;
	Inlet *_inlet;
	InletGuideVane *_igv;
	Diffuser *_diffuser;
	LossCorrelationData *_lossCorrData;

	CompressorOperatingPoint *_desPnt;

	std::vector<CompressorStage *> _stages;
	std::vector<CompressorOperatingPoint *> _opPnts;
	std::vector<double> _vaneSchedule;

	ClearanceSpecification _clearncSpec;

};

#endif /* COMPRESSOR_H_ */
