/*
 * InletPerformance.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: bachm03j
 */

#include "InletPerformance.h"

InletPerformance::InletPerformance(int opPntNmbr, double delPtqPt_Scroll, double ptOut) {
	m_opPntNmbr = opPntNmbr;
	m_delPtqPt_Scroll = delPtqPt_Scroll;
	m_ptOut = ptOut;
}

InletPerformance::~InletPerformance() {
	// TODO Auto-generated destructor stub
}

int InletPerformance::getOpPntNmbr() { return m_opPntNmbr; }
double InletPerformance::getDelPtqPtScroll() {return m_delPtqPt_Scroll; }
double InletPerformance::getPtOut() {return m_ptOut; }
