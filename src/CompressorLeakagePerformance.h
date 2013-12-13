/*
 * CompressorLeakagePerformance.h
 *
 *  Created on: Dec 12, 2013
 *      Author: bachm03j
 */

#ifndef COMPRESSORLEAKAGEPERFORMANCE_H_
#define COMPRESSORLEAKAGEPERFORMANCE_H_

class CompressorLeakagePerformance {
public:

    CompressorLeakagePerformance();

    CompressorLeakagePerformance(int opPntNumber,
            double Pt,
            double Ps,
            double Tt,
            double Ts
            );

    virtual ~CompressorLeakagePerformance();

    int getOptPntNmbr() const;

    double getPt() const;
    double getPs() const;
    double getTt() const;
    double getTs() const;

private:
    int m_opPntNmbr;
    double m_Pt;
    double m_Ps;
    double m_Tt;
    double m_Ts;

};

#endif /* COMPRESSORLEAKAGEPERFORMANCE_H_ */
