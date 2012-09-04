/*
 * LossCorrelationData.h
 *
 *  Created on: Aug 27, 2012
 *      Author: bachm03j
 */

#ifndef LOSSCORRELATIONDATA_H_
#define LOSSCORRELATIONDATA_H_

class LossCorrelationData {
public:
	LossCorrelationData();
	LossCorrelationData(double hpaOmekor, double hpaAbkor, double cdaOmekor, double cdaDevkor
			, double cdaPmckor, double cdaDifkor, double cdaEchoke, double cdaEsurge, double cdaDslope
			, double cdaTlokor, double cdaEsukri, double cdaSmakor, double nacaOmekor
			, double nacaEchoke, double nacaEsurge);
	virtual ~LossCorrelationData();

	double getHpaOmekor();
	double getHpaAbkor();

	double getCdaOmekor();
	double getCdaDevkor();
	double getCdaPmckor();
	double getCdaDifkor();
	double getCdaEchoke();
	double getCdaEsurge();
	double getCdaDslope();
	double getCdaTlokor();
	double getCdaEsukri();
	double getCdaSmakor();

	double getNacaOmekor();
	double getNacaEchoke();
	double getNacaEsurge();

private:
	double _hpaOmekor;
	double _hpaAbkor;

	double _cdaOmekor;
	double _cdaDevkor;
	double _cdaPmckor;
	double _cdaDifkor;
	double _cdaEchoke;
	double _cdaEsurge;
	double _cdaDslope;
	double _cdaTlokor;
	double _cdaEsukri;
	double _cdaSmakor;

	double _nacaOmekor;
	double _nacaEchoke;
	double _nacaEsurge;

};

#endif /* LOSSCORRELATIONDATA_H_ */
