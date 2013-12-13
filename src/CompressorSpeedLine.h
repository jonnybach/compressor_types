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
class DiffuserPerformance;
class InletPerformance;
class InletGuideVanePerformance;
class CompressorStage;
class CompressorLeakage;
class CompressorLeakagePerformance;

class CompressorSpeedLine {

public:
	//methods
	CompressorSpeedLine();
	virtual ~CompressorSpeedLine();

	void addOperatingPoint(CompressorOperatingPoint opPntToAdd);
	void addStage(CompressorStage stageToAdd);
	void setStages(std::vector<CompressorStage*> stages);
	void setDiffuserPerformance(std::vector<DiffuserPerformance> diffPerf);
	void setInletPerformance(std::vector<InletPerformance> diffPerf);
	void setInletGuideVanePerformance(std::vector<InletGuideVanePerformance> diffPerf);
	void setLeakages(std::vector<CompressorLeakage> leaks);

	//void calcMassAndEta(double pressureRatio, double *wIn, double *etaAdiab);
	CompressorStagePerformance getStagePerfForPressureRatio(int stageNmbr, double pressureRatio);
	CompressorOperatingPoint getOpPntForPressureRatio(double pressureRatio);
	DiffuserPerformance getDiffsrPerfForPressureRatio(double pressureRatio);
	InletPerformance getInletPerfForPressureRatio(double pressureRatio);
	InletGuideVanePerformance getInletGuideVanePerfForPressureRatio(double pressureRatio);
	CompressorLeakagePerformance getLeakagePerfForPressureRatio(int leakgIndex, double pressRatio);

	const CompressorOperatingPoint* getOpPnt(int opertaingPoint);
	void getPrMassAndEtaAtOpPnt(int operatingPoint, double *pr, double *wIn, double *etaAdiab);
	CompressorStagePerformance getStagePerfAtOpPnt(int operatingPoint, int stageNmbr);
	DiffuserPerformance getDiffsrPerfAtOpPnt(int operatingPoint);
	InletPerformance getInletPerfAtOpPnt(int operatingPoint);
	InletGuideVanePerformance getInletGuideVanePerfAtOpPnt(int operatingPoint);
	CompressorLeakagePerformance getLeakagePerfAtOpPnt(int leakgIndex, int operatingPoint);

	//shaft speed properties
	double getShaftSpeed();
	void setShaftSpeed(double shaftSpeed);

	int getNumberOfLeakages();

private:
	//members
	std::vector<CompressorOperatingPoint> _opPnts;
	std::vector<CompressorStage *> _stages;
	std::vector<DiffuserPerformance> m_diffPerf;
	std::vector<InletPerformance> m_inletPerf;
	std::vector<InletGuideVanePerformance> m_igvPerf;
	std::vector<CompressorLeakage> m_leaks;

	double _shaftSpeed; //rpm

};

#endif /* COMPRESSORSPEEDLINE_H_ */
