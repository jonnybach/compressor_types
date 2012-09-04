//============================================================================
// Name        : Comp1dTools.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Comp1D toolset in C++, Ansi-style
//============================================================================

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <exception>

#include "CartesianPoint.h"
#include "CompressorAirfoilConfiguration.h"
#include "CompressorOperatingPoint.h"
#include "CompressorAnnulus.h"
#include "CompressorStagePerformance.h"
#include "CompressorStage.h"
#include "CompressorSpeedLine.h"
#include "Compressor.h"
#include "Diffuser.h"
#include "Inlet.h"
#include "InletGuideVane.h"
#include "LossCorrelationData.h"
#include "StringTrimmers.h"
#include "StringTokenizer.h"

using namespace std;


static const string version("0.4");


//functions for reading information from Comp1D
void readComp1dOutfile(string outfileName, vector<CompressorSpeedLine *> &speedLines, Compressor* &compressor);
CompressorAnnulus* getAnnulusData(ifstream &fileToParse);
void getRotorAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor);
void getStatorAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor);
void getOgvAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor);
void getVariableStatorSchedule(ifstream &fileToParse, Compressor *compressor);
void getRotorHpaConfiguration(ifstream &fileToParse, Compressor *compressor);
void getStatorHpaConfiguration(ifstream &fileToParse, Compressor *compressor);
void getLossCorrelationFactors(ifstream &fileToParse, Compressor *compressor);
void getDesignPointData(ifstream &fileToParse, Compressor *compressor);
void getStagePerformance(ifstream &fileToParse, CompressorStage *stage);
void getOgvPerformance(ifstream &fileToParse, CompressorStage *stage);
void addAnnulusPoints( Compressor *compressor );
void setStageSailPoints( Compressor *compressor);
void createIgvStage( Compressor *compressor);

//functions for writing information for STARMEP
void writeStarmepFile(string comp1dOutfile, string starmepTempFileName, string starmepTempEnterFileName, Compressor *compressor);
string createStarmepStationInfo( Compressor *compressor );
string createStarmepAnnulusInfo( AnnulusLocation annlusLoctn, Compressor *compressor );
string createStarmepInletBcInfo( Compressor *compressor);
string createStarmepAirfoilInfo(string comp1dfile );
double calcIgvXnull( CompressorAnnulus *annulus);

std::vector<std::string> getLinePartsFromIndex(std::string lineToParse, const char delimiter, size_t startIndex);
std::string getContentsOfFile(std::string fileName);

void showHelp();

//function definitions
int main(int argc, char *argv[]) {

	if (argc < 3) {
		//show help output
		showHelp();
		return 1;
	}

	int c;
    char *pvalue = NULL;
	double pRatio = 0;
	string infileName;
	string outfileName;

	string starmepTempFile;

	bool runComp1d = false;
	bool writeSmpFile = false;
	bool interpAtPratio = false;

    //inhibit the error message from getopt by setting opterr to zero
    opterr = 0;

    while ((c = getopt(argc, argv, "e:p:s:" )) != -1) {
    	switch (c) {
    	case 'e':
    		//execute comp1d with specified input file
    		infileName = optarg;
    		runComp1d = true;
    		break;

    	case 'p':
    		//interpolate compressor performance at specified pressure ratio
    		pvalue = optarg;
    		pRatio = atof(pvalue);
    		interpAtPratio = true;
    		break;

    	case 's':
    		//convert output to STARMEP input file
    		starmepTempFile = optarg;
    		writeSmpFile = true;
    		break;

    	case '?':
    		if ( optopt == 'e' || optopt == 'p' ) {
    			fprintf (stderr, "Option -%c requires an argument.\n", optopt);
    		} else if (isprint(optopt)) {
    			fprintf (stderr, "Unknown option `-%c'.\n", optopt);
    		} else {
    			fprintf (stderr,
    					"Unknown option character `\\x%x'.\n",
    					optopt);
    		}
    		return 1;

    	default:
    		abort ();

    	}
    }

    //get remaining non-option command arguments
    int index = 0;
    for (index = optind; index < argc; index++) {
    	//if (index == (argc-2)) {
    	//	//argument is input file name
    	//	infileName = argv[index];
    	//} else
    	if (index == (argc-1)) {
    		//argument is output file name
    		outfileName = argv[index];
    	}
    }

	//TODO check that files specified by user exist, if not return error

	int failed = 0;
    if ( runComp1d ) {
    	//call cmodp
    	string sysCall;
    	sysCall.assign("/swd/cmodp/cmodp");
    	sysCall.append(" ");
    	sysCall.append(infileName);
    	sysCall.append(" ");
    	sysCall.append(outfileName);
    	const char *cCall = sysCall.c_str();
    	failed = system(cCall);
    }

	//compile the speedline objects from the results
	vector<CompressorSpeedLine*> speedLines;
	Compressor *compressor = 0;

	if (runComp1d && failed) {
		//comp1d failed
		cout << "\t -- ERROR --\n\tFailure occurred during execution of Comp1d.  Could not post-process results.\n";
		return 1;
	} else {
		readComp1dOutfile(outfileName, speedLines, compressor);
	}

	//add additional annulus points for the inlet, igv, and diffuser
	addAnnulusPoints( compressor );

	//create the stage to represent the igv
	createIgvStage( compressor );

	//set annulus points to stage
	setStageSailPoints( compressor );

	if (writeSmpFile) {
		string starmepTempEnterFile = starmepTempFile;
		string oldSub("input");
		string newSub("enter");
		replaceSubstring(starmepTempEnterFile, oldSub, newSub);
		writeStarmepFile(outfileName, starmepTempFile, starmepTempEnterFile, compressor);
	}

	if ( interpAtPratio ) {

		//calculate the mass flow for the given pressure ratio
		double wIn,etaAdaib;
		speedLines[0]->calcMassAndEta(pRatio, &wIn, &etaAdaib);
		CompressorStagePerformance intrpStgPerf1 = speedLines[0]->getStagePerfForPressureRatio(1, pRatio);
		CompressorStagePerformance intrpStgPerf2 = speedLines[0]->getStagePerfForPressureRatio(2, pRatio);
		CompressorStagePerformance intrpStgPerf5 = speedLines[0]->getStagePerfForPressureRatio(5, pRatio);
		CompressorStagePerformance intrpStgPerf8 = speedLines[0]->getStagePerfForPressureRatio(8, pRatio);

		cout << "Press_Ratio\tW_In\tEta_Adiab\tPt1_Sin\tPt2_Sin\tPt5_Sin\tPt8_Sin\n";
		cout << pRatio << "\t" << wIn << "\t" << etaAdaib << "\t" << intrpStgPerf1.getPt3() << "\t" << intrpStgPerf2.getPt3() << "\t" << intrpStgPerf5.getPt3() << "\t" << intrpStgPerf8.getPt3() << "\n";

	}

	return 0;

}

void readComp1dOutfile(string outfileName, vector<CompressorSpeedLine *> &speedLines, Compressor* &compressor) {

	//open output file and read speed line results
	//interpolate mass flow for user specified pressure ratio

	const string annulsSmryHdr("ANNULUS DETAILS");
	const string rotorHdr("ROTOR BLADE GEOMETRY AND STAGE PRESSURE RATIO");
	const string statorHdr("STATOR BLADE GEOMETRY AND STATOR EXIT FLOW ANGLE");
	const string ogvHdr("OUTLET GUIDE VANE GEOMETRY");
	const string bleedHdr("BLEEDS SPECIFIED AFTER STAGES");
	const string varSttrHdr("VARIABLE STATORS");
	const string rotorHpa("ROTOR HPA DESIGN POINT DATA");
	const string statorHpa("STATOR HPA DESIGN POINT DATA");
	const string tipClrTypeHdr("TIP CLEARANCE TYPE");
	const string preLossFactorHdr("BLOCKAGE FACTOR FOR ALL ROWS");
	const string diffHdr("DIFFUSER CALIBRATION FACTOR");
	const string desPntHdr("AT DESIGN POINT");
	const string speedLineHdr("OVERALL RESULTS FOR SPEEDLINE");

	//Comp1d (v1.2) - static const string speedLinePntsHdr("POINT    TIN    PIN     WCORR     PR     DT/T   ETAADI  ETAPOLY  WCORROUT");
	const string speedLinePntsHdr("POINT     FLOW     WCORR     WCORR    DT/T     PR     ETAADI  ETAPOLY     PR     ETAADI  ETAPOLY");

	const string stageSmryHdrStg("STAGE");
	const string stageSmryHdrSmry("SUMMARY");
	const string stageSmryHdrOgv("OGV");

	vector<string> lineParts;

	CompressorSpeedLine *crntSpeedLine;
	//vector<CompressorSpeedLine*> speedLines;
	speedLines.clear();

	CompressorOperatingPoint *tmpOpPnt;
	//vector<CompressorOperatingPoint> opPnts;

	//CompressorStage* tmpStage;
	vector<CompressorStage*> stages;

	//instantiate a new object and assign to the compressor pointer
	compressor = new Compressor();

	string line;
	double crntSpeed;
	double crntTamb;
	double crntPamb;
	double crntPhi;

	//open a file input stream
	ifstream myfile (outfileName.data(), ifstream::in);

	if (myfile.is_open())
	{

		//read lines of the file stream
		while ( myfile.good() )
		{

			getline(myfile,line);

			//move on to next line if empty
			if ( trim(line).empty() ) {
				continue;
			}

			if ( line.find(annulsSmryHdr, 0 ) != string::npos ) {
				//found compressor annulus data header
				CompressorAnnulus* newAnnulus = getAnnulusData(myfile);
				compressor->setAnnulus(newAnnulus);

			} else if ( line.find(rotorHdr, 0 ) != string::npos ) {
				//found blade geometry definition for all stages
				getRotorAirfoilConfiguration(myfile, compressor);

			} else if ( line.find(statorHdr, 0 ) != string::npos ) {
				//found stator geometry definition for all stages
				getStatorAirfoilConfiguration(myfile, compressor);

			} else if ( line.find(ogvHdr, 0 ) != string::npos ) {
				//found ogv geometry definition for all stages
				getOgvAirfoilConfiguration(myfile, compressor);

			} else if ( line.find(bleedHdr, 0 ) != string::npos ) {
				//found bleed definition
				// split data on current line of text and get bleed stages array
				std::vector<std::string> bleedStages = getLinePartsFromIndex( trim(line), ' ', 5);
				getline(myfile,line);
				std::vector<std::string> bleedFracs = getLinePartsFromIndex( trim(line), ' ', 6);

				CompressorStage *stage;

				//set the bleed fractions for the appropriate stage.  Notice that Comp1d requires user to specify the stage just upstream of the bleed
				// while Starmep associates bleed with downstream stage (hence the reason why the bleedStages vector value is incremented by one when finding the stage.
				int bleedStg;
				double bleedFrc;
				size_t i = 0;
				for ( i = 0; i < bleedStages.size(); i++ ) {
					bleedStg = atoi( bleedStages.at(i).c_str() ) + 1;
					bleedFrc = atof( bleedFracs.at(i).c_str() );
					stage = compressor->stageWithNumber( bleedStg );
					if ( stage != 0 ) {
						stage->getRotor()->setBleedFrac( bleedFrc );
					} else {
						throw exception();
					}
				}


			} else if ( line.find(varSttrHdr, 0 ) != string::npos ) {
				//found variable stator definition
				getVariableStatorSchedule(myfile, compressor);

			} else if ( line.find(rotorHpa, 0 ) != string::npos ) {
				//found rotor HPA definition
				getRotorHpaConfiguration(myfile, compressor);

			} else if ( line.find(statorHpa, 0 ) != string::npos ) {
				//found stator HPA definition
				getStatorHpaConfiguration(myfile, compressor);

			} else if ( line.find(tipClrTypeHdr, 0 ) != string::npos ) {
				//found tip clearance type definition
				lineParts = split(line, ' ');
				lineParts = removeWhiteSpaceElems(lineParts);
				ClearanceSpecification clrSpc = (ClearanceSpecification) atoi( lineParts[4].c_str() );
				compressor->setClearnaceSpec( clrSpc );

			} else if ( line.find(preLossFactorHdr, 0 ) != string::npos ) {
				//found line of text just before the loss correlation factor definition
				getLossCorrelationFactors(myfile, compressor);

			} else if ( line.find(diffHdr, 0 ) != string::npos ) {
				//found diffuser settings

				const char delim = '=';
				lineParts = split(line, delim);
				lineParts = removeWhiteSpaceElems(lineParts);
				double diffCalib = atof( lineParts[1].c_str() );

				getline(myfile, line);
				lineParts = split(line, delim);
				lineParts = removeWhiteSpaceElems(lineParts);
				double diffArat = atof( lineParts[1].c_str() );

				Diffuser *newDiffsr = new Diffuser(diffArat, diffCalib);
				compressor->setDiffuser(newDiffsr);

			} else if ( line.find(desPntHdr, 0 ) != string::npos ) {
				//found design point information
				getDesignPointData(myfile, compressor);

			//} else if ( line.compare(0, speedLineHdr.length(), speedLineHdr) == 0 ) {
			} else if ( line.find(speedLineHdr, 0 ) != string::npos ) {
				//found start of next speedline output

				//get the shaft speed for this speed line
				lineParts = split(line, ' ');
				lineParts = removeWhiteSpaceElems(lineParts);
				lineParts = removeElemsOfString(lineParts, "=");

				//create a new compressor speed line object and set shaft speed
				crntSpeedLine = new CompressorSpeedLine;
				crntSpeed = atof(lineParts[6].c_str());
				crntTamb = atof(lineParts[8].c_str());
				crntSpeedLine->setShaftSpeed(crntSpeed);
				crntSpeedLine->setStages(stages);

				//add crntSpeedLine to speedLines vector
				speedLines.push_back(crntSpeedLine);

			} else if ( line.compare(0, speedLinePntsHdr.length(), speedLinePntsHdr) == 0 ) {
				//else if ( line == speedLinePntsHdr ) {
				//found header for speed line point results

				//get data for each operating point on speed line
				//TODO Add error checking for cases where more than one speedline exits
				// as of 07/19/2012 Comp1D cannot write out more than one speedline
				while ( myfile.good() ) {
					getline(myfile,line);

					if ( !line.empty() ) {
						//each line contains the operating point point data along a speed line
						// get the line data
						lineParts = split(line, ' ');
						lineParts = removeWhiteSpaceElems(lineParts);

						//assign each part of the line data to the operating point
						tmpOpPnt = new CompressorOperatingPoint;
						tmpOpPnt->setOperatingPoint( atoi(lineParts[0].c_str())
								, crntTamb
								, crntPamb
								, crntPhi
								, atof(lineParts[8].c_str())
								, atof(lineParts[4].c_str())
								, atof(lineParts[9].c_str())
								, atof(lineParts[10].c_str())
								, atof(lineParts[2].c_str())
								, atof(lineParts[3].c_str())
								, atof(lineParts[1].c_str())
								, crntSpeedLine->getShaftSpeed());

						//add operating point to vector of operating points
						//opPnts.push_back(tmpOpPnt);
						crntSpeedLine->addOperatingPoint(*tmpOpPnt);

					} //end if ( !line.empty)
				} //end while (myfile.good)

			} else if (
				line.find(stageSmryHdrStg, 0 ) != string::npos
				&& line.find(stageSmryHdrSmry, 0 ) != string::npos
				) {
				// found compressor stage performance data

				// determine what stage number this section corresponds to
				lineParts = split(line, ' ');
				lineParts = removeWhiteSpaceElems(lineParts);

				//second item in lineParts vector should contain the stage number
				string tmpStgName = lineParts[1];

				//instantiate a new compressor stage object
				CompressorStage* newStg = new CompressorStage(tmpStgName);

				getStagePerformance(myfile, newStg);

				//add stage to temp stages vector
				stages.push_back(newStg);

			} else if (
				line.find(stageSmryHdrOgv, 0 ) != string::npos
				&& line.find(stageSmryHdrSmry, 0 ) != string::npos
				) {
				// found compressor OGV stage data

				string tmpStgName = "ogv";

				//instantiate a new compressor stage object
				CompressorStage* newStg = new CompressorStage(tmpStgName);

				getOgvPerformance(myfile, newStg);

				//add stage to temp stages vector
				stages.push_back(newStg);

			}
		} //end while (myfile.good)

		//close file stream
		myfile.close();

	}

}

CompressorAnnulus *getAnnulusData(ifstream &fileToParse) {

	CompressorAnnulus *newAnnulus = new CompressorAnnulus;

	string label;
	AnnulusPoint *newHubPnt;
	AnnulusPoint *newCasePnt;

	static const string annulusHdr("STAGE           X-HUB     R-HUB   X-CASING  R-CASING");
	//static const string annulusEnd("ROTOR BLADE GEOMETRY AND STAGE PRESSURE RATIO");

	bool annulusOn = false;
	bool foundAnnulusEnd = false;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundAnnulusEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/  //assume that no empty space exists once text is being parsed

		//if ( line.compare(0, annulusHdr.length(), annulusHdr) == 0 ) {
		if ( line.find(annulusHdr, 0 ) != string::npos ) {
			annulusOn = true;

		//} else if ( line.find(annulusEnd, 0 ) != string::npos ) {
		} else if ( annulusOn && line.empty() ) {
			//found end of annulus data
			foundAnnulusEnd = true;

		} else if ( annulusOn ) {
			//read next line and separate annulus coordinate points
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			//instantiate new annulus point for the hub and casing and add x and y coords
			label = lineParts[0] + " " + lineParts[1] + " " + lineParts[2];

			if ( line.find("OGV", 0 ) != string::npos || line.find("ogv", 0 ) != string::npos  ) {\
				label = lineParts[0] + " " + lineParts[1];
				newHubPnt = new AnnulusPoint(label, atof(lineParts[2].c_str()),  atof(lineParts[3].c_str()) );
				newCasePnt = new AnnulusPoint(label, atof(lineParts[4].c_str()),  atof(lineParts[5].c_str()) );
			} else {
				label = lineParts[0] + " " + lineParts[1] + " " + lineParts[2];
				newHubPnt = new AnnulusPoint(label, atof(lineParts[3].c_str()),  atof(lineParts[4].c_str()) );
				newCasePnt = new AnnulusPoint(label, atof(lineParts[5].c_str()),  atof(lineParts[6].c_str()) );
			}

			newAnnulus->addHubPoint(newHubPnt);
			newAnnulus->addCasePoint(newCasePnt);

		}

	}  //end file parsing while loop

	return newAnnulus;

}

void getRotorAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor) {

	static const string bladeHdr("STAGE   TYPE  IFORM   S/C    T/C    BETA1   A/C  SHAPEK    O/S   NOB   BETA2 STAGGER   DEVN    LOSS   TCLHR   RATIO");
	//static const string bladeEnd("STATOR BLADE GEOMETRY AND STATOR EXIT FLOW ANGLE");

	bool geomDefOn = false;
	bool foundGeomEnd = false;

	//initialize values to dummy inputs to make it easier to debug what doesn't get set in the method
	std::string stgName = "";
	AirfoilType bladeType = AirfoilType( -1 );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = -1;
	double beta2 = -9999;
	double stagr = -9999;
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stgPr = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = 0.0; //bleed input

	//stator specific variable that will only be modified by stator Comp1D output
	double stageAlphaOut = -9999;

	CompressorStage *stageRef;
	CompressorAirfoilConfiguration *newAirfoilConfig;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundGeomEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/  //assume that no empty space exists once text is being parsed

		if ( line.find(bladeHdr, 0 ) != string::npos ) {
			geomDefOn = true;

		//} else if ( line.find(bladeEnd, 0 ) != string::npos ) {
		} else if ( geomDefOn && line.empty() ) {
			//found end of annulus data
			foundGeomEnd = true;

		} else if ( geomDefOn ) {
			//read next line and separate airfoil geometry data
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			stgName = lineParts[0].c_str();
			bladeType = AirfoilType( atoi ( lineParts[1].c_str() ) );
			sqc= atof( lineParts[3].c_str() );
			tqc = atof( lineParts[4].c_str() );
			beta1 = atof( lineParts[5].c_str() );
			aqc = atof( lineParts[6].c_str() );
			shapeK = atof( lineParts[7].c_str() );
			oqs = atof( lineParts[8].c_str() );
			numBlades = atoi( lineParts[9].c_str() );
			beta2 = atof( lineParts[10].c_str() );
			stagr = atof( lineParts[11].c_str() );
			ddevn = atof( lineParts[12].c_str() );
			dloss = atof( lineParts[13].c_str() );
			tclhr = atof( lineParts[14].c_str() );
			stgPr = atof( lineParts[15].c_str() );

			//get a reference to the compressor stage to add geom parameters
			stageRef = compressor->findOrCreateStageWithName(stgName);

			newAirfoilConfig = new CompressorAirfoilConfiguration(bladeType, numBlades, sqc, tqc, aqc, oqs, shapeK
					, stagr, beta1, beta2, tclhr, delBeta, avdr, machIn, ddevn, dloss, stgPr
					, stageAlphaOut, bleedFrac);

			stageRef->setRotor(newAirfoilConfig);

		} //end if check for headers in Comp1D output file
	} //end while loop through lines of text in output file
}

void getStatorAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor) {

	static const string vaneHdr("STAGE   TYPE  IFORM   S/C    T/C    BETA1   A/C  SHAPEK    O/S   NOB   BETA2 STAGGER   DEVN    LOSS   TCLHS  ALPHA4");
	//static const string vaneEnd("OUTLET GUIDE VANE GEOMETRY");

	bool geomDefOn = false;
	bool foundGeomEnd = false;

	//initialize values to dummy inputs to make it easier to debug what doesn't get set in the method
	std::string stgName = "";
	AirfoilType bladeType = AirfoilType( -1 );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = -1;
	double beta2 = -9999;
	double stagr = -9999;
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stageAlphaOut = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = 0.0; //bleed input

	//rotor specific variable that will only be modified by rotor Comp1D output
	double stgPr = -9999;

	CompressorStage *stageRef;
	CompressorAirfoilConfiguration *newAirfoilConfig;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundGeomEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/ //assume that no empty space exists once text is being parsed

		if ( line.find(vaneHdr, 0 ) != string::npos ) {
			geomDefOn = true;

		//} else if ( line.find(vaneEnd, 0 ) != string::npos ) {
		} else if ( geomDefOn && line.empty() ) {
			//found end of annulus data
			foundGeomEnd = true;

		} else if ( geomDefOn ) {
			//read next line and separate airfoil geometry data
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			stgName = lineParts[0].c_str();
			bladeType = AirfoilType( atoi ( lineParts[1].c_str() ) );
			sqc= atof( lineParts[3].c_str() );
			tqc = atof( lineParts[4].c_str() );
			beta1 = atof( lineParts[5].c_str() );
			aqc = atof( lineParts[6].c_str() );
			shapeK = atof( lineParts[7].c_str() );
			oqs = atof( lineParts[8].c_str() );
			numBlades = atoi( lineParts[9].c_str() );
			beta2 = atof( lineParts[10].c_str() );
			stagr = atof( lineParts[11].c_str() );
			ddevn = atof( lineParts[12].c_str() );
			dloss = atof( lineParts[13].c_str() );
			tclhr = atof( lineParts[14].c_str() );
			stageAlphaOut = atof( lineParts[15].c_str() );

			//get a reference to the compressor stage to add geom parameters
			stageRef = compressor->findOrCreateStageWithName(stgName);

			newAirfoilConfig = new CompressorAirfoilConfiguration(bladeType, numBlades, sqc, tqc, aqc, oqs, shapeK
					, stagr, beta1, beta2, tclhr, delBeta, avdr, machIn, ddevn, dloss, stgPr
					, stageAlphaOut, bleedFrac);

			stageRef->setStator(newAirfoilConfig);

		} //end if check for headers in Comp1D output file
	} //end while loop through lines of text in output file

}

void getOgvAirfoilConfiguration(ifstream &fileToParse, Compressor *compressor) {

	static const string ogvHdr("TYPE  IFORM   S/C    T/C    BETA1   A/C  SHAPEK    O/S   NOB   BETA2 STAGGER   DEVN    LOSS   TCLHS");
	//static const string ogvEnd("INLET BLOCKAGE (PERCENT)");

	bool geomDefOn = false;
	bool foundGeomEnd = false;

	//initialize values to dummy inputs to make it easier to debug what doesn't get set in the method
	std::string stgName = "ogv";
	AirfoilType bladeType = AirfoilType( -1 );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = -1;
	double beta2 = -9999;
	double stagr = -9999;
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stageAlphaOut = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = 0.0; //bleed input

	//rotor specific variable that will only be modified by rotor Comp1D output
	double stgPr = -9999;

	CompressorStage *stageRef;
	CompressorAirfoilConfiguration *newAirfoilConfig;

	string line;
	vector<string> lineParts;

	int ogvCnt = 0;  //only one ogv should be specified, if this count is ever greater than 1, then it is not
	//running right

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundGeomEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/  //assume that no empty space exists once text is being parsed

		if ( line.find(ogvHdr, 0 ) != string::npos ) {
			geomDefOn = true;

		//} else if ( line.find(ogvEnd, 0 ) != string::npos ) {
		} else if ( geomDefOn && line.empty() ) {
			//found end of annulus data
			foundGeomEnd = true;

		} else if ( geomDefOn ) {
			//read next line and separate airfoil geometry data
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			bladeType = AirfoilType( atoi ( lineParts[0].c_str() ) );
			sqc= atof( lineParts[2].c_str() );
			tqc = atof( lineParts[3].c_str() );
			beta1 = atof( lineParts[4].c_str() );
			aqc = atof( lineParts[5].c_str() );
			shapeK = atof( lineParts[6].c_str() );
			oqs = atof( lineParts[7].c_str() );
			numBlades = atoi( lineParts[8].c_str() );
			beta2 = atof( lineParts[9].c_str() );
			stagr = atof( lineParts[10].c_str() );
			ddevn = atof( lineParts[11].c_str() );
			dloss = atof( lineParts[12].c_str() );
			tclhr = atof( lineParts[13].c_str() );

			//get a reference to the compressor stage to add geom parameters
			stageRef = compressor->findOrCreateStageWithName(stgName);

			newAirfoilConfig = new CompressorAirfoilConfiguration(bladeType, numBlades, sqc, tqc, aqc, oqs, shapeK
					, stagr, beta1, beta2, tclhr, delBeta, avdr, machIn, ddevn, dloss, stgPr
					, stageAlphaOut, bleedFrac);

			stageRef->setStator(newAirfoilConfig);

			ogvCnt ++;

		} //end if check for headers in Comp1D output file
	} //end while loop through lines of text in output file

	//either more than one or no ogv definition existed in the Comp1D input file
	if ( ogvCnt != 1) throw exception();

}

void getVariableStatorSchedule(ifstream &fileToParse, Compressor *compressor) {

	static const string varSttHdr("NUMBER OF VARIABLE STATORS");
	static const string vgStrt("VG");

	bool parseOn = false;
	bool foundEnd = false;

	int stageNumbr;
	double vgvFact;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundEnd ) {

		getline(fileToParse, line);

		if ( line.find(varSttHdr, 0 ) != string::npos ) {
			parseOn = true;

		//} else if ( line.find(ogvEnd, 0 ) != string::npos ) {
		} else if ( parseOn && line.empty() ) {
			//found end of annulus data
			foundEnd = true;

		} else if ( parseOn && line.find(vgStrt, 0 ) != string::npos ) {

			//read next line and separate airfoil geometry data
			lineParts = split(line, '=');
			lineParts = removeWhiteSpaceElems(lineParts);

			vgvFact = atof( lineParts[1].c_str() );

			//further parse first part of text and extract stage number
			lineParts = split(lineParts[0], '(');
			lineParts = removeWhiteSpaceElems(lineParts);

			lineParts = split(lineParts[1], ')');
			stageNumbr = atoi( lineParts[0].c_str() );

			//kind of dirty but works for v2.1 input format.  Consider cleaner code in future

			compressor->setVgvRuleAtStage(stageNumbr, vgvFact);

		}
	}
}

void getRotorHpaConfiguration(ifstream &fileToParse, Compressor *compressor) {

	static const string bladeHdr("STAGE   BETA1   DBETA    T/C     AVDR   MACH1");
	//static const string bladeEnd("STATOR BLADE GEOMETRY AND STATOR EXIT FLOW ANGLE");

	bool geomDefOn = false;
	bool foundGeomEnd = false;

	//initialize values to dummy inputs to make it easier to debug what doesn't get set in the method
	std::string stgName = "";
	AirfoilType bladeType = AirfoilType( -1 );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = -1;
	double beta2 = -9999;
	double stagr = -9999;
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stgPr = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = -9999; //bleed input

	//stator specific variable that will only be modified by stator Comp1D output
	double stageAlphaOut = -9999;

	CompressorStage *stageRef;
	//CompressorAirfoilConfiguration *newAirfoilConfig;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundGeomEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/  //assume that no empty space exists once text is being parsed

		if ( line.find(bladeHdr, 0 ) != string::npos ) {
			geomDefOn = true;

		//} else if ( line.find(bladeEnd, 0 ) != string::npos ) {
		} else if ( geomDefOn && line.empty() ) {
			//found end of annulus data
			foundGeomEnd = true;

		} else if ( geomDefOn ) {
			//read next line and separate airfoil geometry data
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			stgName = lineParts[0].c_str();
			beta1 = atof( lineParts[1].c_str() );
			delBeta = atof( lineParts[2].c_str() );
			tqc = atof( lineParts[3].c_str() );
			avdr = atof( lineParts[4].c_str() );
			machIn = atof( lineParts[5].c_str() );

			//get a reference to the compressor stage to add geom parameters
			stageRef = compressor->findOrCreateStageWithName(stgName);
			stageRef->getRotor()->setHpaConfig(beta1, delBeta, tqc, avdr, machIn);

		} //end if check for headers in Comp1D output file
	} //end while loop through lines of text in output file

}

void getStatorHpaConfiguration(ifstream &fileToParse, Compressor *compressor) {

	static const string vaneHdr("STAGE   BETA1   DBETA    T/C     AVDR   MACH1");
	//static const string vaneEnd("OUTLET GUIDE VANE GEOMETRY");

	bool geomDefOn = false;
	bool foundGeomEnd = false;

	//initialize values to dummy inputs to make it easier to debug what doesn't get set in the method
	std::string stgName = "";
	AirfoilType bladeType = AirfoilType( -1 );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = -1;
	double beta2 = -9999;
	double stagr = -9999;
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stageAlphaOut = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = -9999; //bleed input

	//rotor specific variable that will only be modified by rotor Comp1D output
	double stgPr = -9999;

	CompressorStage *stageRef;
	//CompressorAirfoilConfiguration *newAirfoilConfig;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundGeomEnd ) {

		getline(fileToParse, line);

		/*if ( trim(line).empty() ) {
			//read next line if this line contains no text
			continue;
		}*/ //assume that no empty space exists once text is being parsed

		if ( line.find(vaneHdr, 0 ) != string::npos ) {
			geomDefOn = true;

		//} else if ( line.find(vaneEnd, 0 ) != string::npos ) {
		} else if ( geomDefOn && line.empty() ) {
			//found end of annulus data
			foundGeomEnd = true;

		} else if ( geomDefOn ) {
			//read next line and separate airfoil geometry data
			lineParts = split(line, ' ');
			lineParts = removeWhiteSpaceElems(lineParts);

			stgName = lineParts[0].c_str();
			beta1 = atof( lineParts[1].c_str() );
			delBeta = atof( lineParts[2].c_str() );
			tqc = atof( lineParts[3].c_str() );
			avdr = atof( lineParts[4].c_str() );
			machIn = atof( lineParts[5].c_str() );

			//get a reference to the compressor stage to add geom parameters
			stageRef = compressor->findOrCreateStageWithName(stgName);
			stageRef->getStator()->setHpaConfig(beta1, delBeta, tqc, avdr, machIn);

		} //end if check for headers in Comp1D output file
	} //end while loop through lines of text in output file

}

void getLossCorrelationFactors(ifstream &fileToParse, Compressor *compressor) {

	double hpaOmekor = -9999;
	double hpaAbkor = -9999;

	double cdaOmekor = -9999;
	double cdaDevkor = -9999;
	double cdaPmckor = -9999;
	double cdaDifkor = -9999;
	double cdaEchoke = -9999;
	double cdaEsurge = -9999;
	double cdaDslope = -9999;
	double cdaTlokor = -9999;
	double cdaEsukri = -9999;
	double cdaSmakor = -9999;

	double nacaOmekor = -9999;
	double nacaEchoke = -9999;
	double nacaEsurge = -9999;

	string line;
	vector<string> lineParts;

	bool foundStrt = false;
	std::string strtHdr("HPA MINIMUM LOSS CORR");
	//read lines of text until start of loss correlation defintion is found
	while ( fileToParse.good() && !foundStrt ) {
		getline(fileToParse, line);
		if ( line.find(strtHdr, 0 ) != string::npos ) foundStrt = true;
	}

	if ( !foundStrt ) throw exception();  //loop through entire file without finding the appropriate header, bad.

	//each consequtive line contains loss data read and store loss factor
	//found tip clearance type definition
	const char delim = '=';

	//HPA_OMEKOR
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	hpaOmekor = atof( lineParts[1].c_str() );

	//HPA_ABKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	hpaAbkor = atof( lineParts[1].c_str() );

	//CDA_DEVKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaDevkor= atof( lineParts[1].c_str() );

	//CDA_PMCKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaPmckor= atof( lineParts[1].c_str() );

	//CDA_DIFKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaDifkor= atof( lineParts[1].c_str() );

	//CDA_ECHOKE
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaEchoke= atof( lineParts[1].c_str() );

	//CDA_ESURGE
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaEsurge= atof( lineParts[1].c_str() );

	//CDA_DSLOPE
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaDslope= atof( lineParts[1].c_str() );

	//CDA_OMEKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaOmekor= atof( lineParts[1].c_str() );

	//CDA_TLOKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaTlokor= atof( lineParts[1].c_str() );

	//CDA_ESUKRI
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaEsukri= atof( lineParts[1].c_str() );

	//CDA_SMAKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	cdaSmakor= atof( lineParts[1].c_str() );

	//NACA_ECHOKE
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	nacaEchoke= atof( lineParts[1].c_str() );

	//NACA_ESURGE
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	nacaEsurge= atof( lineParts[1].c_str() );

	//NACA_OMEKOR
	getline(fileToParse, line);
	lineParts = split(line, delim);
	lineParts = removeWhiteSpaceElems(lineParts);
	nacaOmekor= atof( lineParts[1].c_str() );

	//instantiate a new instance of the loss correlations for the compressor
	LossCorrelationData *newLossData = new LossCorrelationData( hpaOmekor, hpaAbkor, cdaOmekor, cdaDevkor
			, cdaPmckor, cdaDifkor, cdaEchoke, cdaEsurge, cdaDslope, cdaTlokor, cdaEsukri, cdaSmakor
			, nacaOmekor, nacaEchoke, nacaEsurge);

	compressor->setLossCorrData(newLossData);

}

void getDesignPointData(ifstream &fileToParse, Compressor *compressor) {

	/* SHAFT SPEED          = 3708.64
         MASS FLOW            = 547.837
         AMBIENT TEMPERATURE  = 31.41
         AMBIENT REL HUMIDITY = 25.4
         INLET FLANGE PRESSURE= 1.00770
         SCROLL CALIB. COEFF. = 1.0000
         IGV POSITION         = 5.48            POSITIVE IS CLOSED
         USE GIVEN IGV DP/Pin = 0               0 - NO   1 - YES
         IGV DELTA P/Pin      = 0.0050
         USE GIVEN R1 ALPHA   = 1               0 - NO   1 - YES
         IGV EXIT ANGLE COEFF.= 0.9726
         ROTOR 1 INLET ALPHA  = 15.62           AT THE NOMINAL IGV POSITION*/

		double shftSpeed = -9999;
		double massFlow = -9999;
		double Tamb = -9999;
		double phi = -9999;
		double psFlngIn = -9999;
		double scrollCalib = -9999;
		double igvPosn = -9999;
		bool userIgvPtLoss = -9999;
		double igvDelPtLoss = -9999;
		bool userR1AlpFunc = -9999;
		double r1AlphSlope = -9999;
		double r1AlhpYincp = -9999;

		string line;
		vector<string> lineParts;

		bool foundStrt = false;
		std::string strtHdr("SHAFT SPEED");
		//read lines of text until start of loss correlation defintion is found
		while ( fileToParse.good() && !foundStrt ) {
			getline(fileToParse, line);
			if ( line.find(strtHdr, 0 ) != string::npos ) foundStrt = true;
		}

		if ( !foundStrt ) throw exception();  //loop through entire file without finding the appropriate header, bad.

		//each consequtive line contains loss data read and store loss factor
		//found tip clearance type definition
		const char delim = '=';

		//SHAFT SPEED
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		shftSpeed = atof( lineParts[1].c_str() );

		//MASS FLOW
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		massFlow = atof( lineParts[1].c_str() );

		//AMBIENT TEMP
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		Tamb = atof( lineParts[1].c_str() );

		//PHI
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		phi = atof( lineParts[1].c_str() );

		//FLANGE INLET PRESSURE (ESSENTIALLY PAMB)
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		psFlngIn = atof( lineParts[1].c_str() );

		//SCROLL PRESSURE LOSS CALIBRATION FACTOR
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		scrollCalib = atof( lineParts[1].c_str() );

		//IGV POSITION (POSITIVE IS CLOSED AS OF COMP1D VER 2.1)
		getline(fileToParse, line);
		lineParts = split(line, ' ');  //split by white space because there is additional text after the value to parse
		lineParts = removeWhiteSpaceElems(lineParts);
		igvPosn = atof( lineParts[3].c_str() );

		//USER DEFINED IGV PT LOSS
		getline(fileToParse, line);
		lineParts = split(line, ' ');  //split by white space because there is additional text after the value to parse
		lineParts = removeWhiteSpaceElems(lineParts);
		userIgvPtLoss = atoi( lineParts[5].c_str() );

		//IGV DELTA PT LOSS
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		igvDelPtLoss = atof( lineParts[1].c_str() );

		//USER DEFINED R1 ALPHA FUNCTION VS IGV
		getline(fileToParse, line);
		lineParts = split(line, ' ');  //split by white space because there is additional text after the value to parse
		lineParts = removeWhiteSpaceElems(lineParts);
		userR1AlpFunc = atoi( lineParts[5].c_str() );

		//R1 VS IGV FUNCT SLOPE
		getline(fileToParse, line);
		lineParts = split(line, delim);
		lineParts = removeWhiteSpaceElems(lineParts);
		r1AlphSlope = atof( lineParts[1].c_str() );

		//USER DEFINED R1 ALPHA FUNCTION VS IGV
		getline(fileToParse, line);
		lineParts = split(line, ' ');  //split by white space because there is additional text after the value to parse
		lineParts = removeWhiteSpaceElems(lineParts);
		r1AlhpYincp = atof( lineParts[5].c_str() );

		//create a new operating point for the design conditions
		CompressorOperatingPoint *newDesPnt = new CompressorOperatingPoint(-1, Tamb, psFlngIn
		, phi, -9999, -9999, -9999, -9999, -9999
		, -9999, massFlow, shftSpeed);

		//create a new inlet object
		Inlet *newInlet = new Inlet(scrollCalib);

		//create a new inlet guide vane object
		InletGuideVane *newIgv = new InletGuideVane(igvPosn, userIgvPtLoss, igvDelPtLoss
				, userR1AlpFunc, r1AlphSlope, r1AlhpYincp);

		compressor->setDesignPnt(newDesPnt);
		compressor->setInlet(newInlet);
		compressor->setIgv(newIgv);

}

void getStagePerformance(ifstream &fileToParse, CompressorStage *stage) {

	//now that the start of the stage by stage performance output has been found, read all the data

	static const string stageSmryDataHdr1("POINT   ALF1   ALF2   ALF3   ALF4   DEFR   DEFS     WR     WS    MNR    MNS    DFR    DFS   INCR   INCS");
	static const string stageSmryDataHdr2("POINT    PS1    PS2    PS3    PS4    PT0    PT1    PT2    PT3    PT4    UM      T0    DEVR   DEVS");
	static const string stageSmryDataHdr3("POINT INCCHR INCMLR INCOPR INCPSR INCCHS INCMLS INCOPS INCPSS ALFCHR ALFCHS    CMR    CMS   BLK1   BLK2   BLK3   BLK4");
	static const string stageSmryDataHdr4("POINT WR-PRF WR-SHK WR-TIP WR-SEC WS-PRF WS-SHK WS-TIP WS-SEC   O/SR   O/SS  CLV2R  CLV2S   DEHR   DEHS    LG2   KOCH");
	static const string stageSmryDataHdr5("POINT   VAU1   VAU2   VAU3   VAU4  DHUSQ EDHUSQ   DT/T  R_TOT R_STAT    PTS SPRCIR SPRCIS SPRCCR SPRCCS ETAADI");

	int tmpOpPnt[100];
	double tmpPt0[100], tmpPt1[100], tmpPt2[100], tmpPt3[100], tmpPt4[100];
	double tmpTt0[100];
	double tmpAlp1[100], tmpAlp2[100], tmpAlp3[100], tmpAlp4[100];

	bool foundStageEnd = false;
	bool foundBlankLine = false;

	int lineIndx = 0;
	int maxOpPtNmbr = -1;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundStageEnd ) {
		getline(fileToParse, line);
		if ( line.compare(0, stageSmryDataHdr1.length(), stageSmryDataHdr1) == 0 ) {
			//found first section of data

			foundBlankLine = false;
			lineIndx = 0;
			while ( !foundBlankLine ) {

				getline(fileToParse, line);

				if ( !trim(line).empty() ) {

					lineParts = split(line, ' ');
					lineParts = removeWhiteSpaceElems(lineParts);

					//store operating point and alpha values for this stage
					tmpOpPnt[lineIndx] = atoi(lineParts[0].c_str());
					tmpAlp1[lineIndx] = atof(lineParts[1].c_str());
					tmpAlp2[lineIndx] = atof(lineParts[2].c_str());
					tmpAlp3[lineIndx] = atof(lineParts[3].c_str());
					tmpAlp4[lineIndx] = atof(lineParts[4].c_str());

					maxOpPtNmbr = tmpOpPnt[lineIndx];

					lineIndx++;

				} else {
					foundBlankLine = true;
				}

			}
		} else if ( line.compare(0, stageSmryDataHdr2.length(), stageSmryDataHdr2) == 0 ) {
			//found second section of data

			foundBlankLine = false;
			lineIndx = 0;
			while ( !foundBlankLine ) {

				getline(fileToParse, line);

				if ( !trim(line).empty() ) {

					lineParts = split(line, ' ');
					lineParts = removeWhiteSpaceElems(lineParts);

					//store total pressure values for this stage
					tmpPt0[lineIndx] = atof(lineParts[5].c_str());
					tmpPt1[lineIndx] = atof(lineParts[6].c_str());
					tmpPt2[lineIndx] = atof(lineParts[7].c_str());
					tmpPt3[lineIndx] = atof(lineParts[8].c_str());
					tmpPt4[lineIndx] = atof(lineParts[9].c_str());

					//store total pressure values for this stage
					tmpTt0[lineIndx] = atof(lineParts[11].c_str());

					lineIndx++;

				} else {
					foundBlankLine = true;
				}

			}

		} else if ( line.compare(0, stageSmryDataHdr3.length(), stageSmryDataHdr3) == 0 ) {
			//found third section of data
		} else if ( line.compare(0, stageSmryDataHdr4.length(), stageSmryDataHdr4) == 0 ) {
			//found forth section of data
		} else if ( line.compare(0, stageSmryDataHdr5.length(), stageSmryDataHdr5) == 0 ) {
			//found fifth section of data
			foundStageEnd = true;
		}

	} //end while ( fileToParse.good() || !foundStageEnd )

	//generate operating point performance data objects for this compressor stage
	CompressorStagePerformance* newPrf;
	for ( int i = 0; i < maxOpPtNmbr; i++) {
		newPrf = new CompressorStagePerformance( tmpOpPnt[i], tmpPt0[i], tmpPt1[i], tmpPt2[i], tmpPt3[i]
					, tmpPt4[i], tmpTt0[i], tmpAlp1[i], tmpAlp2[i], tmpAlp3[i], tmpAlp4[i] );
		stage->addPerformancePoint(i, *newPrf);
	}
	//delete(newPrf);

}

void addAnnulusPoints( Compressor *compressor ) {

	//get le and te points for rotor 1
	AnnulusPoint *r1HubLe, *r1HubTe;
	AnnulusPoint *r1CaseLe, *r1CaseTe;

	r1HubLe = compressor->getAnnulus()->hubPointAtIndex(0);
	r1HubTe = compressor->getAnnulus()->hubPointAtIndex(1);

	r1CaseLe = compressor->getAnnulus()->casePointAtIndex(0);
	r1CaseTe = compressor->getAnnulus()->casePointAtIndex(1);

	double delR1Case = r1CaseTe->getX() - r1CaseLe->getX();
	double delR1Hub = r1HubTe->getX() - r1HubLe->getX();

	AnnulusPoint *igvHubTe = new AnnulusPoint("igv_te", r1HubLe->getX(), r1HubLe->getRadius() );
	AnnulusPoint *igvCaseTe = new AnnulusPoint("igv_te",  r1CaseLe->getX(), r1CaseLe->getRadius());

	igvHubTe->translate(-delR1Case, 0, 0);
	igvCaseTe->translate(-delR1Case, 0, 0);

	AnnulusPoint *igvHubLe = new AnnulusPoint("igv_le", igvHubTe->getX(), igvHubTe->getRadius() );
	AnnulusPoint *igvCaseLe = new AnnulusPoint("igv_le",  igvCaseTe->getX(), igvCaseTe->getRadius());

	//assume that IGV sail point distances are inverted from R1
	igvHubLe->translate(-delR1Case, 0, 0);
	igvCaseLe->translate(-delR1Hub, 0, 0);

	//add an additional set of points upstream to define the inlet
	AnnulusPoint *inletHub = new AnnulusPoint("inlet_01", igvHubLe->getX(), igvHubLe->getRadius() );
	AnnulusPoint *inletCase = new AnnulusPoint("inlet_01", igvCaseLe->getX(), igvCaseLe->getRadius() );

	//translate both inlet points in x direction by rotor 1 hub lenght and case inlet point in radial direction by small offset
	double delR = 0.1 * delR1Case;
	inletHub->translate(-delR1Case, 0, 0);
	inletCase->translate(-delR1Case, delR, 0);

	//igv and inlet points to annulus
	compressor->getAnnulus()->insertHubPointAtIndex(0, igvHubTe);
	compressor->getAnnulus()->insertCasePointAtIndex(0, igvCaseTe);

	compressor->getAnnulus()->insertHubPointAtIndex(0, igvHubLe);
	compressor->getAnnulus()->insertCasePointAtIndex(0, igvCaseLe);

	compressor->getAnnulus()->insertHubPointAtIndex(0, inletHub);
	compressor->getAnnulus()->insertCasePointAtIndex(0, inletCase);


	//add points for compressor exit diffuser

	//get trailing edge ogv annulus points
	size_t lastIndx = compressor->getAnnulus()->getHubProfile().size();
	AnnulusPoint *ogvHubTe = compressor->getAnnulus()->hubPointAtIndex(lastIndx-1);
	AnnulusPoint *ogvCaseTe = compressor->getAnnulus()->casePointAtIndex(lastIndx-1);

	//create diffuser inlet annulus points just downstream of the OGV exit
	AnnulusPoint *diffInHub, *diffInCase;
	diffInHub = new AnnulusPoint("diff_in_hub", ogvHubTe->getX(), ogvHubTe->getRadius() );
	diffInCase = new AnnulusPoint("diff_in_case", ogvCaseTe->getX(), ogvCaseTe->getRadius() );

	double delX = 5.0;  // (mm) offset inlet of diffuser from ogv exit by arbitrary amount for now
	diffInHub->translate(delX, 0, 0);
	diffInCase->translate(delX, 0, 0);

	double diffAratio = compressor->getDiffuser()->getAratio();
	double diffInAreaAxial = diffInHub->axialAreaTo(diffInCase);
	double diffExitArea = diffInAreaAxial * diffAratio;

	//in order to calculate the diffuser exit coordinates, assume that inner annulus radius decreases by arbitrary amount for now
	double rExitHub = diffInHub->getRadius() * 0.95;
	//knowing area and hub radial value, calculate case trailing edge radius
	double rExitCase = sqrt( diffExitArea / M_PI + pow(rExitHub, 2) );

	//calculate x location from assumed diffusion angle for casing
	double diffCaseAng = 7.5 * M_PI / 180.0;
	double diffLength = ( rExitCase - diffInHub->getRadius() ) / tan( diffCaseAng ) ;
	double xDiffExit = diffInHub->getX() + diffLength;

	//create trailing edge diffuser points
	AnnulusPoint *diffExitHub = new AnnulusPoint("diff_exit_hub", xDiffExit, rExitHub);
	AnnulusPoint *diffExitCase = new AnnulusPoint("diff_exit_case", xDiffExit, rExitCase);

	//interpolate a new diffuser point halfway downstream
	CartesianPoint *newCpPnt;
	newCpPnt = CartesianPoint::interpolatePointAtFrac( 0.5, ogvHubTe, diffExitHub );
	AnnulusPoint *diffMidHub = new AnnulusPoint( "diff_mid_hub", newCpPnt->getX(), newCpPnt->getY() );
	newCpPnt = CartesianPoint::interpolatePointAtFrac( 0.5, ogvCaseTe, diffExitCase);
	AnnulusPoint *diffMidCase = new AnnulusPoint( "diff_mid_case", newCpPnt->getX(), newCpPnt->getY() );

	//add diffuser points
	compressor->getAnnulus()->addHubPoint(diffInHub);
	compressor->getAnnulus()->addCasePoint(diffInCase);

	compressor->getAnnulus()->addHubPoint(diffMidHub);
	compressor->getAnnulus()->addCasePoint(diffMidCase);

	compressor->getAnnulus()->addHubPoint(diffExitHub);
	compressor->getAnnulus()->addCasePoint(diffExitCase);

}

void setStageSailPoints( Compressor *compressor) {

	//loop through by stage
	// for each stage number find corresponding annulus points
	// if stage is 0, igv

	/*
	//TODO: bad implementation, come up with a better way to index stage numbers
	// since some stages are not named by number like OGV
	int pntIndxLe, pntIndxTe;
	if (( _stgName.find("IGV") != std::string::npos || _stgName.find("igv") != std::string::npos )) {
		throw std::exception();
		pntIndxLe = 0; //this won't be right if inlet points are added
		pntIndxTe = 1;
	} else if ( _stgName.find("OGV") != std::string::npos || _stgName.find("ogv") != std::string::npos ) {
		pntIndxLe = annulus->getHubProfile().size()-1;
		pntIndxTe = annulus->getHubProfile().size()-2;
	} else {
		int stgNumb = atoi( _stgName.c_str() );
		pntIndxLe = (stgNumb - 1) * 4 + 2;
		pntIndxTe = (stgNumb - 1) * 4 + 3;
	}*/

	//find annulus point index that contains the igv labeling
	size_t indxIgvAnnls;
	std::string pntLabel;
	std::string igvLabel = "igv";
	for ( size_t iAnnls = 0; iAnnls <= (compressor->getAnnulus()->getNumProfilePnts()-1); ++iAnnls ) {
		pntLabel = compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getLabel();
		if ( pntLabel.find(igvLabel, 0 ) != string::npos ) {
			indxIgvAnnls = iAnnls;
			break;
		}
	}

	vector<CompressorStage *>stages = compressor->getStages();
	size_t numStages = stages.size();;
	size_t iLe = indxIgvAnnls; //initialize the leading edge point reference to the start of the igv
	AnnulusPoint *hubLe, *hubTe, *caseLe, *caseTe;
	for ( size_t iStg = 0; iStg < numStages; ++iStg ) {

		//rotor airfoil definition does not exist for the igv and ogv stages
		// check stage number and do not call methods on rotor airfoil object if igv or ogv
		if ( iStg != 0 && iStg != (numStages-1) ) {
			//rotor airfoil configuration only exists for the turbomachine stages and not the IGV or OGV
			hubLe = compressor->getAnnulus()->hubPointAtIndex(iLe);
			hubTe = compressor->getAnnulus()->hubPointAtIndex(iLe+1);
			caseLe = compressor->getAnnulus()->casePointAtIndex(iLe);
			caseTe = compressor->getAnnulus()->casePointAtIndex(iLe+1);
			stages.at(iStg)->getRotor()->setSailPoints(hubLe, hubTe, caseLe, caseTe);
			iLe += 2; //increment iLe by 2
		}

		//set stator sail points for the current stage
		hubLe = compressor->getAnnulus()->hubPointAtIndex(iLe);
		hubTe = compressor->getAnnulus()->hubPointAtIndex(iLe+1);
		caseLe = compressor->getAnnulus()->casePointAtIndex(iLe);
		caseTe = compressor->getAnnulus()->casePointAtIndex(iLe+1);
		stages.at(iStg)->getStator()->setSailPoints(hubLe, hubTe, caseLe, caseTe);
		iLe += 2; //increment iLe by 2

	}

}

void createIgvStage( Compressor *compressor) {

	//create a stage placeholder for the IGV to store the number of blades
	// and igv position.  This will help add consistency to the object graph
	// but be sure tot check

	AirfoilType bladeType = AirfoilType( compressor->getStages().at(0)->getRotor()->getAirfoilType() );
	double sqc = -9999;
	double tqc = -9999;
	double beta1 = -9999;
	double aqc = -9999;
	double shapeK = -9999;
	double oqs = -9999;
	int numBlades = compressor->getStages().at(0)->getRotor()->getNumBlades() * 2; //assume IGV stage contains twice as many airfoils as rotor 1
	double beta2 = -9999;
	double stagr =  compressor->getIgv()->getPosition();
	double ddevn = -9999;
	double dloss = -9999;
	double tclhr = -9999;
	double stageAlphaOut = -9999;

	//initialize dummy inputs that will not be set in this method but go into the CompressorAirfoilConfig class constructor
	double machIn = -9999;    //HPA
	double avdr = -9999;      //HPA
	double delBeta = -9999;   //HPA
	double bleedFrac = 0.0; //bleed input

	//rotor specific variable that will only be modified by rotor Comp1D output
	double stgPr = -9999;

	//get a reference to the compressor stage to add geom parameters
	CompressorStage *stageRef = new CompressorStage("igv");

	CompressorAirfoilConfiguration *newAirfoilConfig = new CompressorAirfoilConfiguration(bladeType, numBlades, sqc, tqc, aqc, oqs, shapeK
			, stagr, beta1, beta2, tclhr, delBeta, avdr, machIn, ddevn, dloss, stgPr
			, stageAlphaOut, bleedFrac);

	stageRef->setStator(newAirfoilConfig);
	stageRef->setRotor(0);  //set pointer to null for rotor
	compressor->insertStageAtIndex(0, stageRef);

}

void getOgvPerformance(ifstream &fileToParse, CompressorStage *stage) {

	static const string stageSmryDataHdr1("POINT   ALF3   ALF4   DEFS     WS    MNS    DFS   INCS");
	static const string stageSmryDataHdr2("POINT    PS3    PS4    PT3    PT4    T0    DEVS");
	static const string stageSmryDataHdr3("POINT INCCHS INCMLS INCOPS INCPSS   BLK3   BLK4");
	static const string stageSmryDataHdr4("POINT WS-PRF WS-SHK WS-TIP WS-SEC   O/SS");

	int tmpOpPnt[100];
	double tmpPt0[100], tmpPt1[100], tmpPt2[100], tmpPt3[100], tmpPt4[100];
	double tmpTt0[100];
	double tmpAlp1[100], tmpAlp2[100], tmpAlp3[100], tmpAlp4[100];

	bool foundStageEnd = false;
	bool foundBlankLine = false;

	int lineIndx = 0;
	int maxOpPtNmbr = -1;

	string line;
	vector<string> lineParts;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	while ( fileToParse.good() && !foundStageEnd ) {
		getline(fileToParse, line);
		if ( line.compare(0, stageSmryDataHdr1.length(), stageSmryDataHdr1) == 0 ) {
			//found first section of data

			foundBlankLine = false;
			lineIndx = 0;
			while ( !foundBlankLine ) {

				getline(fileToParse, line);

				if ( !trim(line).empty() ) {

					lineParts = split(line, ' ');
					lineParts = removeWhiteSpaceElems(lineParts);

					//store operating point and alpha values for this stage
					tmpOpPnt[lineIndx] = atoi(lineParts[0].c_str());
					tmpAlp1[lineIndx] = -9999; //alpha 1 doesn't exist for ogv
					tmpAlp2[lineIndx] = -9999; //alpha 2 doesn't exist for ogv
					tmpAlp3[lineIndx] = atof(lineParts[1].c_str());
					tmpAlp4[lineIndx] = atof(lineParts[2].c_str());

					maxOpPtNmbr = tmpOpPnt[lineIndx];

					lineIndx++;

				} else {
					foundBlankLine = true;
				}

			}
		} else if ( line.compare(0, stageSmryDataHdr2.length(), stageSmryDataHdr2) == 0 ) {
			//found second section of data

			foundBlankLine = false;
			lineIndx = 0;
			while ( !foundBlankLine ) {

				getline(fileToParse, line);

				if ( !trim(line).empty() ) {

					lineParts = split(line, ' ');
					lineParts = removeWhiteSpaceElems(lineParts);

					//store total pressure values for this stage
					tmpPt0[lineIndx] = -9999;
					tmpPt1[lineIndx] = -9999;
					tmpPt2[lineIndx] = -9999;
					tmpPt3[lineIndx] = atof(lineParts[3].c_str());
					tmpPt4[lineIndx] = atof(lineParts[4].c_str());

					//store total pressure values for this stage
					tmpTt0[lineIndx] = atof(lineParts[5].c_str());

					lineIndx++;

				} else {
					foundBlankLine = true;
				}

			}

		} else if ( line.compare(0, stageSmryDataHdr3.length(), stageSmryDataHdr3) == 0 ) {
			//found third section of data
		} else if ( line.compare(0, stageSmryDataHdr4.length(), stageSmryDataHdr4) == 0 ) {
			//found forth section of data
			foundStageEnd = true;
		}
	} //end while ( fileToParse.good() || !foundStageEnd )

	//generate operating point performance data objects for this compressor stage
	CompressorStagePerformance* newPrf;
	for ( int i = 0; i < maxOpPtNmbr; i++) {
		newPrf = new CompressorStagePerformance( tmpOpPnt[i], tmpPt0[i], tmpPt1[i], tmpPt2[i], tmpPt3[i]
					, tmpPt4[i], tmpTt0[i], tmpAlp1[i], tmpAlp2[i], tmpAlp3[i], tmpAlp4[i] );
		stage->addPerformancePoint(i, *newPrf);
	}
	//delete(newPrf);

}

void writeStarmepFile(string comp1dOutfile, string starmepTempFileName, string starmepTempEnterFileName, Compressor *compressor) {

	//read entire contents of template starmep input file
	string smpText = getContentsOfFile(starmepTempFileName);

	//read entire contents of template starmep enter file
	string smpEntrText = getContentsOfFile(starmepTempEnterFileName);

	//declare a string stream to use for formating other datatypes
	stringstream tempStrm;
	string keyword;


	// {COMPRESSOR_TYPE}
	string cmpType("HarmC Compressor");
	keyword.assign("{COMPRESSOR_TYPE}");
	replaceSubstring( smpText, keyword, cmpType );


	// {PROJECT}
	string project("Comp1d to Starmep Converter");
	keyword.assign("{PROJECT}");
	replaceSubstring( smpText, keyword, project );


	// {DATE}
	//   store the current time and date
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	string date_time( asctime (timeinfo) );
	date_time.erase(date_time.end()-1);
	keyword.assign("{DATE}");

	replaceSubstring( smpText, keyword, date_time );


	// {COMMENT1}  {COMMENT2}  {COMMENT3}
	string cmt1( "This file was created using comp1dtools v" + version );
	string cmt2( "dummy text 2" );
	string cmt3( "dummy text 3" );
	keyword.assign("{COMMENT1}");
	replaceSubstring( smpText, keyword, cmt1 );
	keyword.assign("{COMMENT2}");
	replaceSubstring( smpText, keyword, cmt2 );
	keyword.assign("{COMMENT3}");
	replaceSubstring( smpText, keyword, cmt3 );


	// {STRM_LINES}
	//   TODO: number of streamlines will need to come from the configuration file later
	//int numStrmLines = 17;
	string strStrmLines("17");
	keyword.assign("{STRM_LINES}");
	replaceSubstring( smpText, keyword, strStrmLines );


	// {TAMB}
	tempStrm << compressor->getDesignPnt()->getTamb();
	string Tamb = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{TAMB}");
	replaceSubstring( smpText, keyword, Tamb );
	replaceSubstring( smpEntrText, keyword, Tamb );


	// {PAMB}
	tempStrm << compressor->getDesignPnt()->getPamb();
	string Pamb = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{PAMB}");
	replaceSubstring( smpText, keyword, Pamb );


	// {PHI}
	tempStrm << compressor->getDesignPnt()->getPhi();
	string phi = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{PHI}");
	replaceSubstring( smpText, keyword, phi );


	// {DEL_IGV}
	tempStrm << compressor->getIgv()->getPosition();
	string igvPos = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{DEL_IGV}");
	replaceSubstring( smpText, keyword, igvPos );
	replaceSubstring( smpEntrText, keyword, igvPos );


	// {K_VGV0}
	string kVgv0("1.0");
	keyword.assign("{K_VGV0}");
	replaceSubstring( smpText, keyword, kVgv0 );


	// {K_VGV1}
	tempStrm << compressor->getVgvRuleAtStage(1);
	string kVgv1 = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{K_VGV1}");
	replaceSubstring( smpText, keyword, kVgv1 );


	// {K_VGV2}
	tempStrm << compressor->getVgvRuleAtStage(2);
	string kVgv2 = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{K_VGV2}");
	replaceSubstring( smpText, keyword, kVgv2 );


	// {K_VGV3}
	tempStrm << compressor->getVgvRuleAtStage(3);
	string kVgv3 = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{K_VGV3}");
	replaceSubstring( smpText, keyword, kVgv3 );


	// {ECHOKE_NCA}
	tempStrm << compressor->getLossCorrData()->getNacaEchoke();
	string nacaEchoke = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ECHOKE_NCA}");
	replaceSubstring( smpText, keyword, nacaEchoke );


	// {ESURGE_NCA}
	tempStrm << compressor->getLossCorrData()->getNacaEsurge();
	string nacaEsurge = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ESURGE_NCA}");
	replaceSubstring( smpText, keyword, nacaEsurge );


	// {OMEKOR_NCA}
	tempStrm << compressor->getLossCorrData()->getNacaOmekor();
	string nacaOmekor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{OMEKOR_NCA}");
	replaceSubstring( smpText, keyword, nacaOmekor );


	// {OMEKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaOmekor();
	string cdaOmekor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{OMEKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaOmekor );


	// {DEVKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaDevkor();
	string cdaDevkor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{DEVKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaDevkor );


	// {PMCKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaPmckor();
	string cdaPmckor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{PMCKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaPmckor );


	// {DIFKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaDifkor();
	string cdaDifkor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{DIFKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaDifkor );


	// {ECHOKE_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaEchoke();
	string cdaEchoke = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ECHOKE_CDA}");
	replaceSubstring( smpText, keyword, cdaEchoke );


	// {ESURGE_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaEsurge();
	string cdaEsurge = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ESURGE_CDA}");
	replaceSubstring( smpText, keyword, cdaEsurge );


	// {DSLOPE_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaDslope();
	string cdaDslope = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{DSLOPE_CDA}");
	replaceSubstring( smpText, keyword, cdaDslope );


	// {TLOKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaTlokor();
	string cdaTlokor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{TLOKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaTlokor );


	// {ESUKRI_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaEsukri();
	string cdaEsukri = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ESUKRI_CDA}");
	replaceSubstring( smpText, keyword, cdaEsukri );


	// {SMAKOR_CDA}
	tempStrm << compressor->getLossCorrData()->getCdaSmakor();
	string cdaSmakor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{SMAKOR_CDA}");
	replaceSubstring( smpText, keyword, cdaSmakor );


	// {OMEKOR_HPA}
	tempStrm << compressor->getLossCorrData()->getHpaOmekor();
	string hpaOmekor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{OMEKOR_HPA}");
	replaceSubstring( smpText, keyword, hpaOmekor );


	// {ABKOR_HPA}
	tempStrm << compressor->getLossCorrData()->getHpaAbkor();
	string hpaAbkor = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{ABKOR_HPA}");
	replaceSubstring( smpText, keyword, hpaAbkor );


	// {DIFF_AR}
	tempStrm << compressor->getDiffuser()->getAratio();
	string diffAratio = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{DIFF_AR}");
	replaceSubstring( smpText, keyword, diffAratio );


	// {MASSFLOW}
	tempStrm << compressor->getDesignPnt()->getWin();
	string strMass = tempStrm.str();
	tempStrm.str(std::string()); //clear the stream
	keyword.assign("{MASSFLOW}");
	replaceSubstring( smpText, keyword, strMass );
	replaceSubstring( smpEntrText, keyword, strMass );


	// {SPEEDFRAC}
	tempStrm << ( compressor->getDesignPnt()->getShaftSpeed()/3600.0 );
	string strSpeedFrac = tempStrm.str();
	keyword.assign("{SPEEDFRAC}");
	replaceSubstring( smpText, keyword, strSpeedFrac );
	replaceSubstring( smpEntrText, keyword, strSpeedFrac );


	// {STATION_INFO}
	string stationText = createStarmepStationInfo( compressor );
	keyword.assign("{STATION_INFO}\n");
	replaceSubstring( smpText, keyword, stationText );


	// {ANNULUS_HUB}
	string annlsHub = createStarmepAnnulusInfo( ANNULUS_LOCATION_HUB ,compressor);
	keyword.assign("{ANNULUS_HUB}\n");
	replaceSubstring( smpText, keyword, annlsHub );


	// {ANNULUS_CASE}
	string annlsCase = createStarmepAnnulusInfo( ANNULUS_LOCATION_CASE, compressor);
	keyword.assign("{ANNULUS_CASE}\n");
	replaceSubstring( smpText, keyword, annlsCase );


	// {INLET_BC_PROFILE}
	string inletBcCase = createStarmepInletBcInfo( compressor);
	keyword.assign("{INLET_BC_PROFILE}\n");
	replaceSubstring( smpText, keyword, inletBcCase );


	// {AIRFOIL_CONFIG}
	string airfoilProfiles = createStarmepAirfoilInfo( comp1dOutfile );
	keyword.assign("{AIRFOIL_CONFIG}");
	replaceSubstring( smpText, keyword, airfoilProfiles );


	//write string contents to STARMEP input file
	string newSmpFile("NewSmpFile.dat");
	ofstream smpFile( newSmpFile.c_str() );
	smpFile << smpText;
	smpFile.close();


	// {SMP_INFILE}
	keyword.assign("{SMP_INFILE}");
	replaceSubstring( smpEntrText, keyword, newSmpFile );


	//write string contents to STARMEP enter file
	string newSmpEntrFile("enter.dat");
	ofstream smpEntrFile( newSmpEntrFile.c_str() );
	smpEntrFile << smpEntrText;
	smpEntrFile.close();


}

string createStarmepStationInfo(Compressor *compressor) {

	char *fmtNwork   = " %-3i";
	char *fmtIarc    = "  %-2i";
	char *fmtNzsch   = " %-3i";
	char *fmtNloss   = "  %-2i";
	char *fmtNterp   = "  %-2i";
	char *fmtNmach   = "  %-2i";
	char *fmtNwref   = "  %-2i";
	char *fmtNlref   = "  %-2i";
	char *fmtIform   = "  %-2i";
	char *fmtIftran  = "  %-2i";
	char *fmtWdfac   = " %-5.2f";
	char *fmtShapeK  = " %-5.2f";
	char *fmtSpeed   = " %7.1f";
	char *fmtXnull   = " %7.4f";
	char *fmtXhub    = " %-8.4f";
	char *fmtXcase   = " %-9.4f";
	char *fmtBldHub  = " %-6.2f";
	char *fmtBldCase = " %-6.4f";
	char *fmtStnName = "  %s";

	char nWork[10];
	char aflType[10];
	char numAirfoils[10];
	char nLoss[10];
	char nTerp[10];
	char nMach[10];
	char nWref[10];
	char nLref[10];
	char iForm[10];
	char iFtran[10];
	char wDfac[10];
	char shapeK[10];
	char speed[10];
	char noSpeed[10];
	char xNull[10];
	char xHub[10];
	char xCase[10];
	char bldHub[10];
	char bldCase[10];
	char stnName[10];

	string stationText = "";
	stringstream stationStream;
	stationStream.str( string() );


	// ------------- INLET PLANES --------------
	//WRITE OUT STATION INFO FOR INLET PLANES TO EVALUATE
	std::string pntLabel;
	std::string igvLabel = "igv";
	size_t iAnnls = 0;
	bool foundIgv = false;
	while ( !foundIgv && iAnnls < (compressor->getAnnulus()->getNumProfilePnts()) ) {
		pntLabel = compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getLabel();
		if ( pntLabel.find(igvLabel, 0 ) != string::npos ) {
			foundIgv = true;
		} else if ( !foundIgv ) {
			sprintf( nWork, fmtNwork, -1 );
			sprintf( aflType, "    ", -1 );
			sprintf( numAirfoils, "    ", -1);
			sprintf( nLoss, "    ", 0);
			sprintf( nTerp, fmtNterp, 0);
			sprintf( nMach, fmtNmach, 0);
			sprintf( nWref, "    ", 0);
			sprintf( nLref, "    ", 1);
			sprintf( iForm, "    ", 1);
			sprintf( iFtran, "    ", 0);
			sprintf( wDfac, "      ", 1.00);
			sprintf( shapeK, "      ", 0.50);
			sprintf( speed, "        ", 0.0 );
			sprintf( xNull, "        ", 0.0 );
			sprintf( xHub, fmtXhub, compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getX()/1000.0 );
			sprintf( xCase, fmtXcase, compressor->getAnnulus()->casePointAtIndex(iAnnls)->getX()/1000.0 );
			sprintf( bldHub, fmtBldHub, 0.0);
			sprintf( bldCase, fmtBldCase, 0.0);
			stoupper(pntLabel);
			sprintf( stnName, fmtStnName, pntLabel.c_str() );

			//write station info
			//"-1            0   0                                                 "
			stationStream << nWork
					<< aflType
					<< numAirfoils
					<< nLoss
					<< nTerp
					<< nMach
					<< nWref
					<< nLref
					<< iForm
					<< iFtran
					<< wDfac
					<< shapeK
					<< speed
					<< xNull
					<< xHub
					<< xCase
					<< bldHub
					<< bldCase
					<< stnName
					<< "\n";
		}
		iAnnls += 1;
	}

	// ------------- TURBOMACHINE PLANES --------------
	//WRITE OUT STATION INFO FOR STAGES
	//generate c strings that will be held constant for all stations
	/*sprintf( nWork, fmtNwork, -1 );
	sprintf( aflType, fmtIarc, -1 );
	sprintf( numAirfoils, fmtNzsch, -1);
	sprintf( nLoss, fmtNloss, 0);
	sprintf( nTerp, fmtNterp, 0);
	sprintf( nMach, fmtNmach, 0);
	sprintf( nWref, fmtNwref, 0);
	sprintf( nLref, fmtNlref, 1);
	sprintf( iForm, fmtIform, 1);
	sprintf( iFtran, fmtIftran, 0);
	sprintf( wDfac, fmtWdfac, 1.00);
	sprintf( shapeK, fmtShapeK , 0.50);
	sprintf( speed, fmtSpeed, 0.0 );
	sprintf( xNull, fmtXnull, 0.0 );
	sprintf( xHub, fmtXhub, compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getX()/1000.0 );
	sprintf( xCase, fmtXcase, compressor->getAnnulus()->casePointAtIndex(iAnnls)->getX()/1000.0 );
	sprintf( bldHub, fmtBldHub, 0.0);
	sprintf( bldCase, fmtBldCase, 0.0);*/

	sprintf( nWork, fmtNwork, 7);
	sprintf( nLoss, fmtNloss, 0);
	sprintf( nTerp, fmtNterp, 0);
	sprintf( nMach, fmtNmach, 0);
	sprintf( nWref, fmtNwref, 0);
	sprintf( nLref, fmtNlref, 1);
	sprintf( iForm, fmtIform, 1);
	sprintf( iFtran, fmtIftran, 0);
	sprintf( wDfac, fmtWdfac, 1.00);
	sprintf( shapeK, fmtShapeK, 0.50);
	sprintf( xHub, "         ", 0.0);  //xHub, "%-8.2f", 0.0);
	sprintf( xCase, "          ", 0.0); //xCase, "%-9.2f", 0.0);
	sprintf( speed, fmtSpeed, compressor->getDesignPnt()->getShaftSpeed() );
	sprintf( noSpeed, fmtSpeed, 0.0 );
	sprintf( bldHub, fmtBldHub, 0.0);

	CompressorStage *stage;
	CompressorAirfoilConfiguration *rotor;
	CompressorAirfoilConfiguration *stator;

	size_t stgNum;
	for ( stgNum = 0; stgNum < compressor->getStages().size(); stgNum++)
	{

		stage = compressor->getStages()[stgNum];

		if ( stgNum != 0 && stgNum != compressor->getStages().size() - 1 ) {

			// these stages represent true turbomachine stages and not the IGV or OGV
			// write rotor info

			rotor = stage->getRotor();

			sprintf( aflType, fmtIarc, rotor->getAirfoilType() );
			sprintf( numAirfoils, fmtNzsch, rotor->getNumBlades());
			sprintf( xNull, fmtXnull, stage->calcXnullRotor( compressor->getAnnulus() )/1000.0 );
			sprintf( bldCase, fmtBldCase, rotor->getBleedFrac() );
			sprintf( stnName, "%s", stage->getStageName().c_str() );

			//write leading edge station info
			stationStream << " 0                                                                                      0.0    0.0   "
					<< "  LE_R" << stnName
					<< "\n";

			//write trailing edge station info
			stationStream << nWork
					<< aflType
					<< numAirfoils
					<< nLoss
					<< nTerp
					<< nMach
					<< nWref
					<< nLref
					<< iForm
					<< iFtran
					<< wDfac
					<< shapeK
					<< speed
					<< xNull
					<< xHub
					<< xCase
					<< bldHub
					<< bldCase
					<< "  TE_R" << stnName
					<< "\n";

		}

		//PRINT STATOR LE AND TE INFO
		stator = stage->getStator();

		sprintf( aflType, fmtIarc, stator->getAirfoilType() );
		sprintf( numAirfoils, fmtNzsch, stator->getNumBlades());
		sprintf( xNull, fmtXnull, stage->calcXnullStator( compressor->getAnnulus() )/1000.0 );
		sprintf( bldCase, fmtBldCase, stator->getBleedFrac() );

		if ( stgNum != 0 && stgNum != ( compressor->getStages().size() - 1 ) ) {
			sprintf( stnName, "S%s", stage->getStageName().c_str() );
		} else {
			string tmpStgName( stage->getStageName() );
			stoupper(tmpStgName);
			sprintf( stnName, "%s", tmpStgName.c_str() );
		}

		//write leading edge station info
		stationStream << " 0                                                                                      0.0    0.0   "
				<< "  LE_" << stnName
				<< "\n";

		//write trailing edge station info
		stationStream << nWork
				<< aflType
				<< numAirfoils
				<< nLoss
				<< nTerp
				<< nMach
				<< nWref
				<< nLref
				<< iForm
				<< iFtran
				<< wDfac
				<< shapeK
				<< noSpeed
				<< xNull
				<< xHub
				<< xCase
				<< bldHub
				<< bldCase
				<< "  TE_" << stnName
				<< "\n";

	} // end for each stage loop



	// ------------- DIFFUSER PLANES --------------
	//WRITE OUT STATION INFO FOR DIFFUSER PLANES TO EVALUATE
	std::string pntLabelDiff;
	std::string diffLabel = "diff";
	iAnnls = 0;
	bool foundDiff = false;
	while ( iAnnls < compressor->getAnnulus()->getNumProfilePnts() ) {
		pntLabelDiff = compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getLabel();
		if ( pntLabelDiff.find(diffLabel, 0 ) != string::npos ) {
			foundDiff = true;
		} else {
			foundDiff = false;
		}

		if ( foundDiff ) {
			sprintf( nWork, fmtNwork, -1 );
			sprintf( aflType, "    ", -1 );
			sprintf( numAirfoils, "    ", -1);
			sprintf( nLoss, "    ", 0);
			sprintf( nTerp, fmtNterp, 0);
			sprintf( nMach, fmtNmach, 0);
			sprintf( nWref, "    ", 0);
			sprintf( nLref, "    ", 1);
			sprintf( iForm, "    ", 1);
			sprintf( iFtran, "    ", 0);
			sprintf( wDfac, "      ", 1.00);
			sprintf( shapeK, "      ", 0.50);
			sprintf( speed, "        ", 0.0 );
			sprintf( xNull, "        ", 0.0 );
			sprintf( xHub, fmtXhub, compressor->getAnnulus()->hubPointAtIndex(iAnnls)->getX()/1000.0 );
			sprintf( xCase, fmtXcase, compressor->getAnnulus()->casePointAtIndex(iAnnls)->getX()/1000.0 );
			sprintf( bldHub, fmtBldHub, 0.0);
			sprintf( bldCase, fmtBldCase, 0.0);
			stoupper(pntLabel);
			sprintf( stnName, "  DIFF_%2.2i", iAnnls+1 );

			//write station info
			stationStream << nWork
					<< aflType
					<< numAirfoils
					<< nLoss
					<< nTerp
					<< nMach
					<< nWref
					<< nLref
					<< iForm
					<< iFtran
					<< wDfac
					<< shapeK
					<< speed
					<< xNull
					<< xHub
					<< xCase
					<< bldHub
					<< bldCase
					<< stnName
					<< "\n";
		}
		iAnnls += 1;
	}

	stationText = stationStream.str();
	return stationText;

}



string createStarmepAnnulusInfo( AnnulusLocation annlusLoctn, Compressor *compressor ) {

	//get the annulus object and the vector of points for either the hub or the casing
	CompressorAnnulus *annls = compressor->getAnnulus();
	std::vector<AnnulusPoint *> annlsPnts;
	if ( annlusLoctn == ANNULUS_LOCATION_HUB ) {
		annlsPnts = annls->getHubProfile();
	} else if ( annlusLoctn == ANNULUS_LOCATION_CASE ) {
		annlsPnts = annls->getCaseProfile();
	} else {
		throw exception();
	}

	char x[10];
	char rad[10];

	string annlsText;
	stringstream annlsStream;
	annlsStream.str( string() ) ;

	vector<AnnulusPoint *>::iterator itPnt;
	for(itPnt = annlsPnts.begin();
		itPnt != annlsPnts.end();
		itPnt++)
	{

		sprintf( x, "%-8.4f", (*itPnt)->getX()/1000.0);
		sprintf( rad, "%-8.4f", (*itPnt)->getRadius()/1000.0);

		//write annulus info
		annlsStream << "    "
					<< x
					<< "    "
					<< rad
					<< "\n";

	}

	annlsText = annlsStream.str();
	return annlsText;

}

string createStarmepInletBcInfo( Compressor *compressor ) {

	char radH[10], radM[10], radC[10];
	char ttot[10];
	char ptot[10];
	char flwAng[10];

	string bcText;
	stringstream bcStream;
	bcStream.str( string() ) ;

	//get Pamb and Tamb from design point info
	double tamb = compressor->getDesignPnt()->getTamb();
	double pamb = compressor->getDesignPnt()->getPamb();
	double flowAngle = 0.0;

	//get inlet hub and casing points and calculate midpoint
	AnnulusPoint *inletHubPnt = compressor->getAnnulus()->getHubProfile().at(0);
	AnnulusPoint *inletCasePnt = compressor->getAnnulus()->getCaseProfile().at(0);
	CartesianPoint *newCpPnt =  CartesianPoint::interpolatePointAtFrac( 0.5, inletHubPnt, inletCasePnt );
	AnnulusPoint *inletMidPnt = new AnnulusPoint( "inlet_01_midspan", newCpPnt->getX(),  newCpPnt->getY() );

	double radHub = compressor->getAnnulus()->getHubProfile().at(0)->getRadius();
	double radCase = compressor->getAnnulus()->getCaseProfile().at(0)->getRadius();
	double radMid = inletMidPnt->getRadius();


	sprintf( radH, "%-8.4f", radHub/1000.0);
	sprintf( radM, "%-8.4f", radMid/1000.0);
	sprintf( radC, "%-8.4f", radCase/1000.0);
	sprintf( ttot, "  %-8.4f", tamb);
	sprintf( ptot, "  %-8.4f", pamb);
	sprintf( flwAng, "  %-8.4f", flowAngle );

	bcStream << "     " << radH << ttot << pamb << flwAng << "\n";
	bcStream << "     " << radM << ttot << pamb << flwAng << "\n";
	bcStream << "     " << radC << ttot << pamb << flwAng << "\n";

	bcText = bcStream.str();
	return bcText;
}

string createStarmepAirfoilInfo( string comp1dfile ) {
	string sysCall("comp1d2starmep.py " + comp1dfile);
	system( sysCall.c_str() );
	string profileFileName("starmep.geometry");
	string profileData = getContentsOfFile(profileFileName);
	return profileData;
}

double calcIgvXnull( CompressorAnnulus *annulus) {

	double xh1, xh2, xc1, xc2; //axial location of hub and casing sail points for rotor 1
	xh1 = annulus->hubPointAtIndex( 0 )->getX();
	xh2 = annulus->hubPointAtIndex( 1 )->getX();
	xc1 = annulus->casePointAtIndex( 0 )->getX();
	xc2 = annulus->casePointAtIndex( 1 )->getX();

	double delXh = xh2 - xh1;
	double delXc = xc2- xc1;

	double xNullIgvH = xh1 - delXh - delXc/2;
	double xNullIgvC = xc1 - delXc - delXh/2;

	double xavg = (xNullIgvH + xNullIgvC)/2;
	xavg = (3.25*(xh1 + xc1) - 1.25*(xh2 + xc2))/4;
	return xavg;

}

vector<string> getLinePartsFromIndex(string lineToParse, const char delimiter, size_t startIndex) {

	vector<string> lineParts;
	vector<string> partsToReturn;

	//split line of text by specified delimination character
	lineParts = split(lineToParse, delimiter);
	lineParts = removeWhiteSpaceElems(lineParts);

	//erase all parts up to specified starting index
	partsToReturn = lineParts;
	partsToReturn.erase(partsToReturn.begin(), partsToReturn.begin()+startIndex );  //remove all parts just before the starting index

	return partsToReturn;

}

std::string getContentsOfFile(std::string fileName) {

	ifstream file;
	int	size;
	char* inBuf;
	std::string fileContents;

	file.open(fileName.c_str(), ios_base::binary);

	if ( file.is_open() ) {

		//get file size
		file.seekg(0,ios::end);
		size = file.tellg();
		file.seekg(0,ios::beg);

		inBuf = new char[size];

		file.read(inBuf,size);
		file.close();

		fileContents.assign( inBuf, size );
	}

	//cout << fileContents;
	return fileContents;

}

void showHelp() {
	printf("\nComp1D Tools (v%s) - ERROR in argument syntax\n\n"
			"Program must be called as follows:\n"
			"\n"
			"comp1dtools [Options] \RESULTS FILE>\n"
			"\n"
			"Required)\n"
			"\t<RESULTS FILE>      : name of the Comp1D results file to be parsed.\n"
			"\n"
			"Options)\n"
			"\t-e <INPUT FILE>     : runs Comp1d analysis mode using the specified input file <INPUT FILE> \n"
			"\t                      and writes results to <RESULTS FILE>.  After running Comp1D \n"
			"\t                      parses the results file for further operations.\n"
			"\n"
			"\t-p <PRESSURE RATIO> : allows Comp1d tools to interpolate the compressor performance\n"
			"\t                      associated with the specified pressure ratio <PRESSURE RATIO>.\n"
			"\t                      Will only run correctly when a series of points are run along a speedline.\n"
			"\n"
			, version.c_str());
}


