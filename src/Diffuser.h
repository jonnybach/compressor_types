/*
 * Diffuser.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef DIFFUSER_H_
#define DIFFUSER_H_

class Diffuser {
public:
	Diffuser();
	Diffuser(double areaRatio, double calibFactor);
	virtual ~Diffuser();

	double getAratio();
	double getCalibFactor();

private:
	double _areaRatio;
	double _calibFactor;

};

#endif /* DIFFUSER_H_ */
