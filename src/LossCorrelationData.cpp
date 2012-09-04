/*
 * LossCorrelationData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#include "LossCorrelationData.h"

LossCorrelationData::LossCorrelationData() {
	// TODO Auto-generated constructor stub

}


LossCorrelationData::LossCorrelationData(double hpaOmekor, double hpaAbkor, double cdaOmekor, double cdaDevkor
		, double cdaPmckor, double cdaDifkor, double cdaEchoke, double cdaEsurge, double cdaDslope
		, double cdaTlokor, double cdaEsukri, double cdaSmakor, double nacaOmekor
		, double nacaEchoke, double nacaEsurge) {

	_hpaOmekor = hpaOmekor;
	_hpaAbkor = hpaAbkor;

	_cdaOmekor = cdaOmekor;
	_cdaDevkor = cdaDevkor;
	_cdaPmckor = cdaPmckor;
	_cdaDifkor = cdaDifkor;
	_cdaEchoke = cdaEchoke;
	_cdaEsurge = cdaEsurge;
	_cdaDslope = cdaDslope;
	_cdaTlokor = cdaTlokor;
	_cdaEsukri = cdaEsukri;
	_cdaSmakor = cdaSmakor;

	_nacaOmekor = nacaOmekor;
	_nacaEchoke = nacaEchoke;
	_nacaEsurge = nacaEsurge;

}

LossCorrelationData::~LossCorrelationData() {
	// TODO Auto-generated destructor stub
}

double LossCorrelationData::getHpaOmekor() { return _hpaOmekor; };
double LossCorrelationData::getHpaAbkor() { return _hpaAbkor; };

double LossCorrelationData::getCdaOmekor() { return _cdaOmekor; };
double LossCorrelationData::getCdaDevkor() { return _cdaDevkor; };
double LossCorrelationData::getCdaPmckor() { return _cdaPmckor; };
double LossCorrelationData::getCdaDifkor() { return _cdaDifkor; };
double LossCorrelationData::getCdaEchoke() { return _cdaEchoke; };
double LossCorrelationData::getCdaEsurge() { return _cdaEsurge; };
double LossCorrelationData::getCdaDslope() { return _cdaDslope; };
double LossCorrelationData::getCdaTlokor() { return _cdaTlokor; };
double LossCorrelationData::getCdaEsukri() { return _cdaEsukri; };
double LossCorrelationData::getCdaSmakor() { return _cdaSmakor; };

double LossCorrelationData::getNacaOmekor() { return _nacaOmekor; };
double LossCorrelationData::getNacaEchoke() { return _nacaEchoke; };
double LossCorrelationData::getNacaEsurge() { return _nacaEsurge; };
