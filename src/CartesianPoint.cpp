/*
 * CartesianPoint.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "CartesianPoint.h"
#include <cmath>

CartesianPoint::CartesianPoint() {
	_X = 0.0;
	_Y = 0.0;
	_Z = 0.0;
}

CartesianPoint::CartesianPoint(double Xin, double Yin, double Zin) {
	_X = Xin;
	_Y = Yin;
	_Z = Zin;
}

CartesianPoint::CartesianPoint(double Xin, double Yin) {
	_X = Xin;
	_Y = Yin;
	_Z = 0.0;
}

CartesianPoint::CartesianPoint(CartesianPoint *pntToCopy) {
	_X = pntToCopy->getX();
	_Y = pntToCopy->getY();
	_Z = pntToCopy->getZ();
}

CartesianPoint::~CartesianPoint() {
	// TODO Auto-generated destructor stub
}

double CartesianPoint::getX() { return _X; }
double CartesianPoint::getY() { return _Y; }
double CartesianPoint::getZ() { return _Z; }

double CartesianPoint::radius() {
	return ( std::sqrt( pow(_Y, 2) + pow(_Z, 2) ) );
}

double CartesianPoint::axialArea() {
   return ( M_PI * pow(this->radius(), 2) );
}

bool CartesianPoint::equals(CartesianPoint *pntToCompare) {

    double Tol = 0.00000005;

    bool Xeql = false;
    bool Yeql = false;
    bool Zeql = false;


    if ( std::abs( _X - pntToCompare->getX() ) < Tol ) {
        Xeql = true;
    }

    if ( std::abs( _Y - pntToCompare->getY() ) < Tol ) {
        Yeql = true;
    }

    if ( std::abs( _Z - pntToCompare->getZ() ) < Tol ) {
        Zeql = true;
    }

    return (Xeql && Yeql && Zeql);

}

CartesianPoint *CartesianPoint::interpolatePointAtX( double X, CartesianPoint *firstPnt, CartesianPoint *secndPnt ) {

    double X1, X2;
    double Y1, Y2, Yintrp;
    double Z1, Z2, Zintrp;
    double m;

    X1 = firstPnt->getX();
    X2 = secndPnt->getX();

    m = (X - X1) / (X2 - X1);

    Y1 = firstPnt->getY();
    Y2 = secndPnt->getY();
    Z1 = firstPnt->getZ();
    Z2 = secndPnt->getZ();

    Yintrp = Y1 + m * (Y2 - Y1);
    Zintrp = Z1 + m * (Z2 - Z1);

    CartesianPoint *intrpPnt = new CartesianPoint(X, Yintrp, Zintrp);
    return intrpPnt;

}

CartesianPoint *CartesianPoint::interpolatePointAtFrac( double distFraction, CartesianPoint *firstPnt, CartesianPoint *secndPnt ) {

    double X1, X2, Xintrp;
    double Y1, Y2, Yintrp;
    double Z1, Z2, Zintrp;
    double m;

    m = distFraction;

    X1 = firstPnt->getX();
    X2 = secndPnt->getX();
    Y1 = firstPnt->getY();
    Y2 = secndPnt->getY();
    Z1 = firstPnt->getZ();
    Z2 = secndPnt->getZ();

    Xintrp = X1 + m * (X2 - X1);
    Yintrp = Y1 + m * (Y2 - Y1);
    Zintrp = Z1 + m * (Z2 - Z1);

    CartesianPoint *intrpPnt = new CartesianPoint(Xintrp, Yintrp, Zintrp);
    return intrpPnt;

}

void CartesianPoint::translate(double delX, double delY, double delZ) {
    _X += delX;
    _Y += delY;
    _Z += delZ;
}



/*Public Function DistanceTo(ByVal OtherPoint As CartesianPoint) As Double
    Dim Dist As Double = Sqrt((OtherPoint.X ^ 2 - Me.mX ^ 2) + (OtherPoint.Y ^ 2 - Me.mY ^ 2) + (OtherPoint.Z ^ 2 - Me.mZ ^ 2))
    Return Dist
End Function*/
