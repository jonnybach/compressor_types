/*
 * CompressorEnumerations.h
 *
 *  Created on: Sep 11, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORENUMERATIONS_H_
#define COMPRESSORENUMERATIONS_H_

#include <string>

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

enum RootType {
	ROOT_TYPE_NONE = 0,
	ROOT_TYPE_TR = 1,		// t root - blades
	ROOT_TYPE_SH = 2,		// dove tail root - blades
	ROOT_TYPE_SG = 3,		// dove tail root - 1st stage blades
	ROOT_TYPE_SX = 4,		// dove tail "x-shaped" root - blades
	ROOT_TYPE_SF = 5,		// dove tail root - blades and vanes
	ROOT_TYPE_SFM = 6,		// dove tail root trapezoidal - vanes
	ROOT_TYPE_SW = 7,		// dove tail root variation - vanes
	ROOT_TYPE_SWM = 8,		// dove tail root trapezoidal variation - vanes
	ROOT_TYPE_HR = 9,		// h root - vanes
	ROOT_TYPE_VGV = 10		// vgv attachment
};

namespace sie_comp {

inline std::string getAirfoilTypeString(AirfoilType airfoilTypeValue)
{
	std::string strType;
	switch (airfoilTypeValue)
	{
	case AIRFOIL_TYPE_NACA_DCA:
		strType = "NACA/DCA";
		break;
	case AIRFOIL_TYPE_CDA:
		strType = "CDA";
		break;
	case AIRFOIL_TYPE_HPA:
		strType = "HPA";
		break;
	default:
		strType = "NOT SET";
		break;
	}
	return strType;
}

inline std::string getAirfoilBehaviorString(AirfoilBehavior airfoilBehaviorValue)
{
	std::string strType;
	switch (airfoilBehaviorValue)
	{
	case AIRFOIL_BEHAVIOR_ROTOR:
		strType = "ROTOR";
		break;
	case AIRFOIL_BEHAVIOR_STATOR:
		strType = "STATOR";
		break;
	case AIRFOIL_BEHAVIOR_IGV:
		strType = "IGV";
		break;
	case AIRFOIL_BEHAVIOR_OGV:
		strType = "OGV";
		break;
	default:
		strType = "NOT SET";
		break;
	}
	return strType;
}

inline std::string getRootTypeString(RootType rootTypeValue)
{
	std::string strType;
	switch (rootTypeValue)
	{
	case ROOT_TYPE_TR:
		strType = "TR (t-design)";
		break;
	case ROOT_TYPE_SH:
		strType = "SH (dovetail)";
		break;
	case ROOT_TYPE_SG:
		strType = "SG (dovetail)";
		break;
	case ROOT_TYPE_SX:
		strType = "SX (x shaped dovetail)";
		break;
	case ROOT_TYPE_SF:
		strType = "SF (dovetail)";
		break;
	case ROOT_TYPE_SFM:
		strType = "SFM (trapezoid dovetail)";
		break;
	case ROOT_TYPE_SW:
		strType = "SW (dovetail)";
		break;
	case ROOT_TYPE_SWM:
		strType = "SWM (trapezoid dovetail)";
		break;
	case ROOT_TYPE_HR:
		strType = "HR (h-design)";
		break;
	default:
		strType = "NOT SET";
		break;
	}
	return strType;
}

} // end sie_comp namespace

#endif /* COMPRESSORENUMERATIONS_H_ */
