/*
 * AirfoilSectionResult.h
 *
 *  Created on: May 30, 2013
 *      Author: bachm03j
 */

#ifndef AIRFOILSECTIONRESULT_H_
#define AIRFOILSECTIONRESULT_H_

class AirfoilSectionResult {

public:
    AirfoilSectionResult();

    virtual ~AirfoilSectionResult();


    //properties
    double getAlphaLe() const;
    double getAlphaTe() const;
    double getBetaLe() const;
    double getBetaTe() const;
    double getMachAbsLe() const;
    double getMachAbsTe() const;
    double getMachRelLe() const;
    double getMachRelTe() const;
    double getPressLe() const;
    double getPressTe() const;
    double getPressTotAbsLe() const;
    double getPressTotAbsTe() const;
    double getPressTotRelLe() const;
    double getPressTotRelTe() const;
    double getTempLe() const;
    double getTempRecov() const;
    double getTempTe() const;
    double getTempTotAbsLe() const;
    double getTempTotAbsTe() const;
    double getTempTotRelLe() const;
    double getTempTotRelTe() const;
    double getMassFlow() const;

    void setAlphaLe(double m_alphaLe);
    void setAlphaTe(double m_alphaTe);
    void setBetaLe(double m_betaLe);
    void setBetaTe(double m_betaTe);
    void setMachAbsLe(double m_machAbsLe);
    void setMachAbsTe(double m_machAbsTe);
    void setMachRelLe(double m_machRelLe);
    void setMachRelTe(double m_machRelTe);
    void setPressLe(double m_pressLe);
    void setPressTe(double m_pressTe);
    void setPressTotAbsLe(double m_pressTotAbsLe);
    void setPressTotAbsTe(double m_pressTotAbsTe);
    void setPressTotRelLe(double m_pressTotRelLe);
    void setPressTotRelTe(double m_pressTotRelTe);
    void setTempLe(double m_tempLe);
    void setTempRecov(double m_tempRecov);
    void setTempTe(double m_tempTe);
    void setTempTotAbsLe(double m_tempTotAbsLe);
    void setTempTotAbsTe(double m_tempTotAbsTe);
    void setTempTotRelLe(double m_tempTotRelLe);
    void setTempTotRelTe(double m_tempTotRelTe);
    void setMassFlow(double m_massFlow);


private:

    double m_massFlow;

    double m_machAbsLe;
    double m_machRelLe;

    double m_machAbsTe;
    double m_machRelTe;

    double m_pressLe;
    double m_pressTe;
    double m_pressTotAbsLe;
    double m_pressTotAbsTe;
    double m_pressTotRelLe;
    double m_pressTotRelTe;

    double m_tempLe;
    double m_tempTe;
    double m_tempTotAbsLe;
    double m_tempTotAbsTe;
    double m_tempTotRelLe;
    double m_tempTotRelTe;

    double m_tempRecov;

    double m_alphaLe;
    double m_alphaTe;

    double m_betaLe;
    double m_betaTe;

};

#endif /* AIRFOILSECTIONRESULT_H_ */
