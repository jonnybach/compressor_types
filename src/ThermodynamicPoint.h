/*
 * ThermodynamicPoint.h
 *
 *  Created on: May 30, 2013
 *      Author: bachm03j
 */

#ifndef THERMODYNAMICPOINT_H_
#define THERMODYNAMICPOINT_H_

#include <string>

class ThermodynamicPoint {

public:
    ThermodynamicPoint();
    virtual ~ThermodynamicPoint();

    std::string getName() const;

    double getCp() const;
    double getGamma() const;
    double getH() const;
    double getMw() const;
    double getPr() const;
    double getPress() const;
    double getRho() const;
    double getS() const;
    double getTemp() const;
    double getU() const;
    double getVisc() const;
    double getVsonic() const;

    void setCp(double m_cp);
    void setGamma(double m_gamma);
    void setH(double m_h);
    void setMw(double m_mw);
    void setName(std::string m_name);
    void setPr(double m_pr);
    void setPress(double m_press);
    void setRho(double m_rho);
    void setS(double m_s);
    void setTemp(double m_temp);
    void setU(double m_u);
    void setVisc(double m_visc);
    void setVsonic(double m_vsonic);

private:

    std::string m_name;
    double m_rho; // kg/m^3
    double m_press; // bar
    double m_temp; // k
    double m_h; // kj/kg
    double m_s; // kj/kg-K
    double m_u; // kj/kg
    double m_mw; // kg/kg-kmol
    double m_gamma; // -
    double m_cp; // kj/kg-k
    double m_vsonic; // m/s
    double m_visc; // kg/m-s
    double m_pr;

};

#endif /* THERMODYNAMICPOINT_H_ */
