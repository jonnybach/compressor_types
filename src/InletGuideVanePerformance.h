/*
 * InletGuideVanePerformance.h
 *
 *  Created on: Oct 31, 2012
 *      Author: bachm03j
 */

#ifndef INLETGUIDEVANEPERFORMANCE_H_
#define INLETGUIDEVANEPERFORMANCE_H_

class InletGuideVanePerformance {
public:
	InletGuideVanePerformance(int opPntNmbr, double delPtqPt, double ptOut, double alpOut);
	virtual ~InletGuideVanePerformance();

	//accessors (properties)
	int getOpPntNmbr();
	double getDelPtqPt();
	double getPtOut();
	double getAlpOut();

private:
	int m_opPntNmbr;
	double m_delPtqPt;
	double m_ptOut;
	double m_alpOut;

};

#endif /* INLETGUIDEVANEPERFORMANCE_H_ */
