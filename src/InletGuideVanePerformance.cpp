/*
 * InletGuideVanePerformance.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: bachm03j
 */

#include "InletGuideVanePerformance.h"

InletGuideVanePerformance::InletGuideVanePerformance(int opPntNmbr, double delPtqPt, double ptOut, double alpOut) {
	m_opPntNmbr = opPntNmbr;
	m_delPtqPt = delPtqPt;
	m_ptOut = ptOut;
	m_alpOut = alpOut;
}

InletGuideVanePerformance::~InletGuideVanePerformance() {
	// TODO Auto-generated destructor stub
}

int InletGuideVanePerformance::getOpPntNmbr() { return m_opPntNmbr; }
double InletGuideVanePerformance::getDelPtqPt() {return m_delPtqPt; }
double InletGuideVanePerformance::getPtOut() {return m_ptOut; }
double InletGuideVanePerformance::getAlpOut() {return m_alpOut; }
