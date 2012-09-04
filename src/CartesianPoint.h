/*
 * CartesianPoint.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef CARTESIANPOINT_H_
#define CARTESIANPOINT_H_

enum Axis {
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2
};

enum Plane {
	PLANE_XY = 0,
	PLANE_YZ = 1,
	PLANE_XZ = 2
};

class CartesianPoint {
public:
	CartesianPoint();
	CartesianPoint(double Xin, double Yin, double Zin);
	CartesianPoint(double Xin, double Yin);
	CartesianPoint(CartesianPoint *pntToCopy);

	virtual ~CartesianPoint();

	double getX();
	double getY();
	double getZ();

   double radius();
   double axialArea();

   bool equals(CartesianPoint *pntToCompare);

   void translate(double delX, double delY, double delZ);

   static CartesianPoint *interpolatePointAtX( double X, CartesianPoint *firstPnt, CartesianPoint *secndPnt);
   static CartesianPoint *interpolatePointAtFrac( double distFraction, CartesianPoint *firstPnt, CartesianPoint *secndPnt );


protected:
	double _X;
	double _Y;
	double _Z;

};

#endif /* CARTESIANPOINT_H_ */
