/*
 * InletPerformance.h
 *
 *  Created on: Oct 31, 2012
 *      Author: bachm03j
 */

#ifndef INLETPERFORMANCE_H_
#define INLETPERFORMANCE_H_

class InletPerformance {
public:
	InletPerformance(int opPntNmbr, double delPtqPt_Scroll, double ptOut);
	virtual ~InletPerformance();

	//accessors (properties)
	int getOpPntNmbr();
	double getDelPtqPtScroll();
	double getPtOut();

private:
	int m_opPntNmbr;
	double m_delPtqPt_Scroll;
	double m_ptOut;

};

#endif /* INLETPERFORMANCE_H_ */
