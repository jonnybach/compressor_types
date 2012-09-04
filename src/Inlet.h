/*
 * Inlet.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef INLET_H_
#define INLET_H_

class Inlet {
public:
	Inlet();
	Inlet(double calibFactor);
	virtual ~Inlet();

	double getCalibFactor();

private:
	double _calibFactor;

};

#endif /* INLET_H_ */
