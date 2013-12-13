/*
 * CompressorLeakage.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: bachm03j
 */

#include <exception>

#include "CompressorLeakage.h"
#include "Compressor.h"
#include "CompressorStage.h"
#include "CompressorStagePerformance.h"


CompressorLeakage::CompressorLeakage() {
    // TODO Auto-generated destructor stub
}

CompressorLeakage::CompressorLeakage(std::string stage, AirfoilBehavior afBehavior, AirfoilAxialLocation axiLocation)
{
    m_stage = stage;
    m_afBhvr = afBehavior;
    m_afAxiLoc = axiLocation;
}

CompressorLeakage::~CompressorLeakage() {
    // TODO Auto-generated destructor stub
}

void CompressorLeakage::setLeakageProperties(Compressor* compressor)
{

    //using locally stored stage name, get the compressor stage instance containing the
    //  needed data
    CompressorStage *stage = compressor->stageWithName(m_stage);
    if ( !stage ) { throw std::exception(); }

    //from the specified axial location, set the appropriate performance point properties (LE, MID, TE)
    // iterate through all operating point performance instances and create new compressor leakage
    //  performance instances, store in local performance vector

    double ptIn, psIn, ttIn, tsIn;
    double ptOut, psOut, ttOut, tsOut;
    double pt, ps, tt, ts;

    std::vector<CompressorStagePerformance> vStgPrf = stage->getOpPntPerf();
    std::vector<CompressorStagePerformance>::iterator it;
    int i = 0;
    for (it = vStgPrf.begin(); it != vStgPrf.end(); ++it, ++i) {

        if (m_afBhvr == AIRFOIL_BEHAVIOR_ROTOR) {
            ptIn = (*it).getRotorPt1Rel();
            psIn = (*it).getRotorPs1();
            ttIn = (*it).getRotorTt1Rel();
            tsIn = (*it).getRotorTs1();

            ptOut = (*it).getRotorPt2Rel();
            psOut = (*it).getRotorPs2();
            ttOut = (*it).getRotorTt2Rel();
            tsOut = (*it).getRotorTs2();

        } else if (m_afBhvr == AIRFOIL_BEHAVIOR_STATOR) {
            ptIn = (*it).getStatorPt1();
            psIn = (*it).getStatorPs1();
            ttIn = (*it).getStatorTt1();
            tsIn = (*it).getStatorTs1();

            ptOut = (*it).getStatorPt2();
            psOut = (*it).getStatorPs2();
            ttOut = (*it).getStatorTt2();
            tsOut = (*it).getStatorTs2();

        }

        // determine final properties to be included based on choice of axial location of leakage with
        //  respect to the airfoil
        pt = -9999.9;
        ps = -9999.9;
        tt = -9999.9;
        ts = -9999.9;
        if (m_afAxiLoc == AIRFOIL_AXI_LOCATION_LE) {
            pt = ptIn;
            ps = psIn;
            tt = ttIn;
            ts = tsIn;

        } else if (m_afAxiLoc == AIRFOIL_AXI_LOCATION_TE) {
            pt = ptOut;
            ps = psOut;
            tt = ttOut;
            ts = tsOut;

        } else if (m_afAxiLoc == AIRFOIL_AXI_LOCATION_MID) {
            pt = (ptIn + ptOut) / 2.0;
            ps = (psIn + psOut) / 2.0;
            tt = (ttIn + ttOut) / 2.0;
            ts = (tsIn + tsOut) / 2.0;

        } else {
            throw std::exception();
        }

        // create a new diffuser performance instance
        CompressorLeakagePerformance* newPrf = new CompressorLeakagePerformance( i+1, pt, ps, tt, ts);

        // replace the operating point performance instance at this op pnt number
        //  with the newly created diffuser performance instance
        this->addPerformancePoint(i, *newPrf);
    }

}

void CompressorLeakage::addPerformancePoint(int index, CompressorLeakagePerformance perfPntToAdd) {
    std::vector<CompressorLeakagePerformance>::iterator it;
     it = m_optPntPerf.begin();
    m_optPntPerf.insert(it+index, perfPntToAdd);
}

std::vector <CompressorLeakagePerformance> CompressorLeakage::getOpPntPerf() { return m_optPntPerf; }

CompressorLeakagePerformance CompressorLeakage::getOpPntPerfAtIndex(int index) { return m_optPntPerf.at(index); }

void CompressorLeakage::replaceOpPntPerfAtIndex(int index, CompressorLeakagePerformance newPerfPnt) {
   m_optPntPerf.at(index) = newPerfPnt;
}

std::string CompressorLeakage::getStageName() { return m_stage; }
AirfoilBehavior CompressorLeakage::getAirfoilBehavior() { return m_afBhvr; }
AirfoilAxialLocation CompressorLeakage::getAirfoilAxialLocation() { return m_afAxiLoc; }
