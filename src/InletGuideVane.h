/*
 * InletGuideVane.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef INLETGUIDEVANE_H_
#define INLETGUIDEVANE_H_

#include <vector>
#include "InletGuideVanePerformance.h"

class PolynomialFit;
class CompressorAirfoilConfiguration;

class InletGuideVane {
public:
	InletGuideVane();
	InletGuideVane( double anglePosition
			, bool userDefPtLoss, double userDeltaPtLoss
			, bool userDefR1AlphaFunc, double r1AlphaFuncSlope, double r1AlphaFuncYintcp );
	virtual ~InletGuideVane();

	double getPosition();
	void setPosition(double anglePosition);

	double getPtLoss();

	void setAlphaLaw(std::vector<double> polyCoeffs);

	double getR1Alpha();

	CompressorAirfoilConfiguration *getAirfoilConfig();

	void addPerformancePoint(int opPnt, InletGuideVanePerformance perfPntToAdd);
	std::vector <InletGuideVanePerformance> getOpPntPerf();

private:

	double _anglPosn;  //positive is closed

	bool _userPtLoss;
	double _delPtLoss;

	bool _userDefR1AlphaFunc;
	PolynomialFit *_r1AlpFunc;

	//not currently used by may be a good idea to reference the same airfoil configuration that exists in the
	// stages vector so that this object and that stage is consistent
	CompressorAirfoilConfiguration *m_airfoilConfig;

	std::vector <InletGuideVanePerformance> m_opPntPerf;

};

#endif /* INLETGUIDEVANE_H_ */
