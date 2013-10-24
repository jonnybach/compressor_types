/*
 * ThermodynamicPoint.cpp
 *
 *  Created on: May 30, 2013
 *      Author: bachm03j
 */

#include "ThermodynamicPoint.h"

ThermodynamicPoint::ThermodynamicPoint() {
    // TODO Auto-generated constructor stub

}

ThermodynamicPoint::~ThermodynamicPoint() {
    // TODO Auto-generated destructor stub
}


//properties

std::string ThermodynamicPoint::getName() const
{
    return m_name;
}

double ThermodynamicPoint::getCp() const
{
    return m_cp;
}

double ThermodynamicPoint::getGamma() const
{
    return m_gamma;
}

double ThermodynamicPoint::getH() const
{
    return m_h;
}

double ThermodynamicPoint::getMw() const
{
    return m_mw;
}

double ThermodynamicPoint::getPr() const
{
    return m_pr;
}

double ThermodynamicPoint::getPress() const
{
    return m_press;
}

double ThermodynamicPoint::getRho() const
{
    return m_rho;
}

double ThermodynamicPoint::getS() const
{
    return m_s;
}

double ThermodynamicPoint::getTemp() const
{
    return m_temp;
}

double ThermodynamicPoint::getU() const
{
    return m_u;
}

double ThermodynamicPoint::getVisc() const
{
    return m_visc;
}

double ThermodynamicPoint::getVsonic() const
{
    return m_vsonic;
}


void ThermodynamicPoint::setCp(double m_cp)
{
    this->m_cp = m_cp;
}

void ThermodynamicPoint::setGamma(double m_gamma)
{
    this->m_gamma = m_gamma;
}

void ThermodynamicPoint::setH(double m_h)
{
    this->m_h = m_h;
}

void ThermodynamicPoint::setMw(double m_mw)
{
    this->m_mw = m_mw;
}

void ThermodynamicPoint::setName(std::string m_name)
{
    this->m_name = m_name;
}

void ThermodynamicPoint::setPr(double m_pr)
{
    this->m_pr = m_pr;
}

void ThermodynamicPoint::setPress(double m_press)
{
    this->m_press = m_press;
}

void ThermodynamicPoint::setRho(double m_rho)
{
    this->m_rho = m_rho;
}

void ThermodynamicPoint::setS(double m_s)
{
    this->m_s = m_s;
}

void ThermodynamicPoint::setTemp(double m_temp)
{
    this->m_temp = m_temp;
}

void ThermodynamicPoint::setU(double m_u)
{
    this->m_u = m_u;
}

void ThermodynamicPoint::setVisc(double m_visc)
{
    this->m_visc = m_visc;
}

void ThermodynamicPoint::setVsonic(double m_vsonic)
{
    this->m_vsonic = m_vsonic;
}

