/*
 * CompressorLeakagePerformance.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: bachm03j
 */

#include "CompressorLeakagePerformance.h"

CompressorLeakagePerformance::CompressorLeakagePerformance() {
    // TODO Auto-generated constructor stub

}

CompressorLeakagePerformance::CompressorLeakagePerformance(int opPntNumber,
    double Pt,
    double Ps,
    double Tt,
    double Ts
) {
    m_opPntNmbr = opPntNumber;
    m_Pt = Pt;
    m_Ps = Ps;
    m_Tt = Tt;
    m_Ts = Ts;
}

CompressorLeakagePerformance::~CompressorLeakagePerformance() {
    // TODO Auto-generated destructor stub
}

int CompressorLeakagePerformance::getOptPntNmbr() const { return m_opPntNmbr; }
double CompressorLeakagePerformance::getPt() const { return m_Pt; }
double CompressorLeakagePerformance::getPs() const { return m_Ps; }
double CompressorLeakagePerformance::getTt() const { return m_Tt; }
double CompressorLeakagePerformance::getTs() const { return m_Ts; }
