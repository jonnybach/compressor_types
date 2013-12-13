/*
 * CompressorLeakage.h
 *
 *  Created on: Dec 12, 2013
 *      Author: bachm03j
 */

#ifndef COMPRESSORLEAKAGE_H_
#define COMPRESSORLEAKAGE_H_

#include <string>
#include <vector>
#include "CompressorLeakagePerformance.h"
#include "CompressorEnumerations.h"

//forward declarations
class Compressor;

class CompressorLeakage {
public:
    CompressorLeakage();
    CompressorLeakage(std::string stage, AirfoilBehavior afBehavior, AirfoilAxialLocation axiLocation);
    virtual ~CompressorLeakage();

    void setLeakageProperties(Compressor* compressor);

    void addPerformancePoint(int index, CompressorLeakagePerformance perfPntToAdd);
    void replaceOpPntPerfAtIndex(int index, CompressorLeakagePerformance newPerfPnt);

    CompressorLeakagePerformance getLeakPerfForPressureRatio(double pressRatio, const double &pRatio);

    std::vector <CompressorLeakagePerformance> getOpPntPerf();
    CompressorLeakagePerformance getOpPntPerfAtIndex(int index);

    std::string getStageName();
    AirfoilBehavior getAirfoilBehavior();
    AirfoilAxialLocation getAirfoilAxialLocation();

private:
    std::string m_stage;
    AirfoilBehavior m_afBhvr;
    AirfoilAxialLocation m_afAxiLoc;
    std::vector <CompressorLeakagePerformance> m_optPntPerf;

};

#endif /* COMPRESSORLEAKAGE_H_ */
