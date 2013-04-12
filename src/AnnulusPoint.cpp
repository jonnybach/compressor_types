/*
 * AnnulusPoint.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "AnnulusPoint.h"
#include <cmath>

AnnulusPoint::AnnulusPoint() {
	// TODO Auto-generated constructor stub

}

AnnulusPoint::AnnulusPoint(std::string labelIn, double Xin, double RadiusIn) {
	_label = labelIn;
	_X = Xin;
	_Y = RadiusIn;
}

AnnulusPoint::AnnulusPoint(AnnulusPoint *pntToCopy) {
	_label = pntToCopy->getLabel();
	_X = pntToCopy->getX();
	_Y = pntToCopy->getY();
	_Z = pntToCopy->getZ();
}

AnnulusPoint::~AnnulusPoint() {
	// TODO Auto-generated destructor stub
}

/*
Originally meant to overload the CartesianPoint methods but seems that static overloading doesn't work as
  initially expected, using static_cast<> in places where an AnnulusPoint object must be returned.
static AnnulusPoint *interpolatePointAtX( double X, AnnulusPoint *firstPnt, AnnulusPoint *secndPnt) {
	AnnulusPoint *interpPoint;
	interpPoint = static_cast<AnnulusPoint *>( CartesianPoint::interpolatePointAtX(X, firstPnt, secndPnt ) );
	return  interpPoint;
}

static AnnulusPoint *interpolatePointAtFrac( double distFraction, AnnulusPoint *firstPnt, AnnulusPoint *secndPnt ) {
	AnnulusPoint *interpPoint;
	interpPoint = static_cast<AnnulusPoint *>( CartesianPoint::interpolatePointAtFrac(distFraction, firstPnt, secndPnt ) );
	return  interpPoint;
}
*/

double AnnulusPoint::getX() const { return _X; }
double AnnulusPoint::getRadius() const { return _Y; }

std::string AnnulusPoint::getLabel() const { return _label; }
void AnnulusPoint::setLabel( std::string newLabel )
{
	_label = std::string( newLabel );
}

double AnnulusPoint::axialAreaTo(AnnulusPoint *otherPoint )
{

    double MyRad, OtherRad;
    MyRad = this->getRadius();
    OtherRad = otherPoint->getRadius();

    double axialArea = M_PI * ( pow(OtherRad,2) - pow(MyRad,2) );
    return axialArea;

}

double AnnulusPoint::frustumAreaTo(AnnulusPoint *otherPoint )
{

    double MyRad, OtherRad;
    MyRad = this->getRadius();
    OtherRad = otherPoint->getRadius();

    double delX = std::abs( _X - otherPoint->getX() );
    double frustArea = M_PI * (MyRad + OtherRad) * sqrt( pow( (MyRad - OtherRad),2) + pow(delX, 2) );
    return frustArea;

}

double AnnulusPoint::angleBetween(AnnulusPoint *otherPoint) {
	double ang = this->angleInXyPlane(otherPoint);
	return ang; //radians
}
