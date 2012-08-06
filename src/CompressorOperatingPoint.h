/*
 * CompressorOperatingPoint.h
 *
 *  Created on: Apr 30, 2012
 *      Author: bachm03j
 */

#ifndef COMPRESSOROPERATINGPOINT_H_
#define COMPRESSOROPERATINGPOINT_H_

class CompressorOperatingPoint {

public:
	//methods
	CompressorOperatingPoint();
	CompressorOperatingPoint(int opPntNmbr
			, double inletTemp
			, double inletPress
			, double pressRatio
			, double delTqT
			, double etaAdi
			, double etaPoly
			, double wCorct
			, double wCorctOut
			, double wInlet
			, double speed);
	virtual ~CompressorOperatingPoint();

	void setOperatingPoint(int opPntNmbr
			, double inletTemp
			, double inletPress
			, double pressRatio
			, double delTqT
			, double etaAdi
			, double etaPoly
			, double wCorct
			, double wCorctOut
			, double wInlet
			, double speed);

	//accessors (properties)
	double getWcorct();
	double getWin();
	double getEtaAdi();
	double getPressRatio();

private:
	//members
	int _opPntNmbr;
	double _tempIn;			//compressor inlet temperatur - kelvin
	double _pressIn;		//compressor inlet pressure - Pa
	double _pressRatio;		//pressure ration from IGV in to OGV out
	double _delTqT;			//non-dim temp change from IGV in to OGV out
	double _etaAdi;			//adiabatic efficiency from IGV in to OGV out
	double _etaPoly;		//polytropic efficiency from IGV in to OGV out
	double _wCorct;			//inlet corrected flow - kg/s
	double _wCorctOut;		//outlet corrected flow - kg/s
	double _wInlet;			//absolute mass flow at inlet - kg/s
	double _speed;			//shaft speed - RPM

};

#endif /* COMPRESSOROPERATINGPOINT_H_ */
