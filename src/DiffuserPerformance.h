/*
 * DiffuserPerformance.h
 *
 *  Created on: Oct 22, 2013
 *      Author: bachm03j
 */

#ifndef DIFFUSERPERFORMANCE_H_
#define DIFFUSERPERFORMANCE_H_

class DiffuserPerformance {
public:
	DiffuserPerformance();

	DiffuserPerformance(int opPntNumber,
		double delPqP,
		double PtIn,
		double PsIn,
		double TtIn,
		double TsIn,
		double machIn,
		double PtExit,
		double PsExit,
		double TtExit,
		double TsExit,
		double machExit
	);

	virtual ~DiffuserPerformance();

	int getOptPntNmbr() const;

	double getDelPqP() const;

	double getPtIn() const;
	double getPsIn() const;
	double getTtIn() const;
	double getTsIn() const;
	double getMachIn() const;

	double getPtExit() const;
	double getPsExit() const;
	double getTtExit() const;
	double getTsExit() const;
	double getMachExit() const;

private:
	double m_opPntNmbr;

	double m_delPqP;

	double m_PtIn;
	double m_PsIn;
	double m_TtIn;
	double m_TsIn;
	double m_machIn;

	double m_PtExit;
	double m_PsExit;
	double m_TtExit;
	double m_TsExit;
	double m_machExit;

};

#endif /* DIFFUSERPERFORMANCE_H_ */
