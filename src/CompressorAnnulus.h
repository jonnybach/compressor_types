/*
 * CompressorAnnulus.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSORANNULUS_H_
#define COMPRESSORANNULUS_H_

enum AnnulusLocation {
	ANNULUS_LOCATION_NONE = 0,
	ANNULUS_LOCATION_HUB = 1,
	ANNULUS_LOCATION_CASE = 2
};

#include <vector>
#include "AnnulusPoint.h"

class CompressorAnnulus {
public:
	CompressorAnnulus();
	virtual ~CompressorAnnulus();

	void insertHubPointAtIndex(size_t pntIndex, AnnulusPoint * newHubPnt);
	void insertCasePointAtIndex(size_t pntIndex, AnnulusPoint * newCasePnt);

	void addHubPoint(AnnulusPoint * newHubPnt);
	void addCasePoint(AnnulusPoint * newCasePnt);

	std::vector<AnnulusPoint *> getHubProfile();
	std::vector<AnnulusPoint *> getCaseProfile();
	size_t getNumProfilePnts();

	AnnulusPoint*  hubPointAtIndex(int index);
	AnnulusPoint*  casePointAtIndex(int index);

private:
	std::vector<AnnulusPoint *> _hubProfile;
	std::vector<AnnulusPoint *> _caseProfile;

};

#endif /* COMPRESSORANNULUS_H_ */
