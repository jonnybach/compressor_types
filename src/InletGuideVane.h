/*
 * InletGuideVane.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef INLETGUIDEVANE_H_
#define INLETGUIDEVANE_H_

#include <vector>

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

private:

	double _anglPosn;  //positive is closed

	bool _userPtLoss;
	double _delPtLoss;

	bool _userDefR1AlphaFunc;
	PolynomialFit *_r1AlpFunc;

	//not currently used by may be a good idea to reference the same airfoil configuration that exists in the
	// stages vector so that this object and that stage is consistent
	CompressorAirfoilConfiguration *_airfoilConfig;

};

#endif /* INLETGUIDEVANE_H_ */
