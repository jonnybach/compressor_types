/*
 * DiffuserPerformance.cpp
 *
 *  Created on: Oct 22, 2013
 *      Author: bachm03j
 */

#include "DiffuserPerformance.h"

DiffuserPerformance::DiffuserPerformance() {
	// TODO Auto-generated constructor stub
}

DiffuserPerformance::DiffuserPerformance(int opPntNumber,
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
) {
	m_opPntNmbr = opPntNumber;
	m_delPqP = delPqP,
	m_PtIn = PtIn;
	m_PsIn = PsIn;
	m_TtIn = TtIn;
	m_TsIn = TsIn;
	m_machIn = machIn;
	m_PtExit = PtExit;
	m_PsExit = PsExit;
	m_TtExit = TtIn;
	m_TsExit = TsExit;
	m_machExit = machExit;
}

DiffuserPerformance::~DiffuserPerformance() {
	// TODO Auto-generated destructor stub
}

int DiffuserPerformance::getOptPntNmbr() const { return m_opPntNmbr; }

double DiffuserPerformance::getDelPqP() const { return m_delPqP; }

double DiffuserPerformance::getPtIn() const { return m_PtIn; }
double DiffuserPerformance::getPsIn() const { return m_PsIn; }
double DiffuserPerformance::getTtIn() const { return m_TtIn; }
double DiffuserPerformance::getTsIn() const { return m_TsIn; }
double DiffuserPerformance::getMachIn() const { return m_machIn; }

double DiffuserPerformance::getPtExit() const { return m_PtExit; }
double DiffuserPerformance::getPsExit() const { return m_PsExit; }
double DiffuserPerformance::getTtExit() const { return m_TtExit; }
double DiffuserPerformance::getTsExit() const { return m_TsExit; }
double DiffuserPerformance::getMachExit() const { return m_machExit; }
