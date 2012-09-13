/*
 * CompressorEnumerations.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORENUMERATIONS_H_
#define COMPRESSORENUMERATIONS_H_

enum AirfoilType {
	AIRFOIL_TYPE_NONE = 0,
	AIRFOIL_TYPE_NACA_DCA = 1,
	AIRFOIL_TYPE_CDA = 2,
	AIRFOIL_TYPE_HPA = 3
};

enum AirfoilBehavior {
	AIRFOIL_BEHAVIOR_NONE = 0,
	AIRFOIL_BEHAVIOR_ROTOR = 1,
	AIRFOIL_BEHAVIOR_STATOR = 2,
	AIRFOIL_BEHAVIOR_IGV = 3,
	AIRFOIL_BEHAVIOR_OGV = 4
};

enum TipClearanceSpecification {
	TIP_CLEARANCE_NONE = 0,
	TIP_CLEARANCE_RATIO = 1,    // ratio of clearance to height
	TIP_CLEARANCE_ABSOLUTE = 2  // absolute values specified in millimeters
};

#endif /* COMPRESSORENUMERATIONS_H_ */
