/*
 * CompressorSpeedLine.h
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORSPEEDLINE_H_
#define COMPRESSORSPEEDLINE_H_

#include <vector>

//forward declarations
class CompressorOperatingPoint;
class CompressorStage;
class CompressorStagePerformance;

class CompressorSpeedLine {

public:
	//methods
	CompressorSpeedLine();
	virtual ~CompressorSpeedLine();

	void addOperatingPoint(CompressorOperatingPoint opPntToAdd);
	void addStage(CompressorStage stageToAdd);
	void setStages(std::vector<CompressorStage*> stages);

	double calcMassFlow(double pressureRatio);
	double calcEtaAdi(double pressureRatio);
	void calcMassAndEta(double pressureRatio, double *wCorrctIn, double *etaAdiab);
	CompressorStagePerformance getStagePerfForPressureRatio(int stageNmbr, double pressureRatio);

	//shaft speed accessors
	double getShaftSpeed();
	void setShaftSpeed(double shaftSpeed);

private:
	//members
	std::vector<CompressorOperatingPoint> _opPnts;
	std::vector<CompressorStage *> _stages;
	double _shaftSpeed; //rpm
	bool testIt;

};

#endif /* COMPRESSORSPEEDLINE_H_ */
