//============================================================================
// Name        : Comp1dTools.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "StringTokenizer.h"
#include "CompressorOperatingPoint.h"
#include "CompressorStagePerformance.h"
#include "CompressorStage.h"
#include "CompressorSpeedLine.h"
#include "StringTrimmers.h"


using namespace std;


static const string version("0.3");


//function prototype declarations
vector<CompressorSpeedLine*> readComp1dOutfile(string outfileName);
void getStagePerformance(ifstream &fileToParse, CompressorStage *stage);
void getOgvPerformance(ifstream &fileToParse, CompressorStage *ogv);
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

	bool runComp1d = false;

    //inhibit the error message from getopt by setting opterr to zero
    opterr = 0;

    while ((c = getopt(argc, argv, "e:p:" )) != -1) {
    	switch (c) {
    	case 'e':
    		//execute comp1d with specified input file
    		infileName = optarg;
    		runComp1d = true;
    		break;

    	case 'p':
    		//interpolate compressor perofrmance at specified pressure ratio
    		pvalue = optarg;
    		pRatio = atof(pvalue);
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

	if (runComp1d && failed) {
		//comp1d failed
	} else {
		speedLines = readComp1dOutfile(outfileName);
	}

	//calculate the mass flow for the given pressure ratio
	double wIn,etaAdaib;
	speedLines[0]->calcMassAndEta(pRatio, &wIn, &etaAdaib);
	CompressorStagePerformance intrpStgPerf1 = speedLines[0]->getStagePerfForPressureRatio(1, pRatio);
	CompressorStagePerformance intrpStgPerf2 = speedLines[0]->getStagePerfForPressureRatio(2, pRatio);
	CompressorStagePerformance intrpStgPerf5 = speedLines[0]->getStagePerfForPressureRatio(5, pRatio);
	CompressorStagePerformance intrpStgPerf8 = speedLines[0]->getStagePerfForPressureRatio(8, pRatio);

	cout << "Press_Ratio\tW_In\tEta_Adiab\tPt1_Sin\tPt2_Sin\tPt5_Sin\tPt8_Sin\n";
	cout << pRatio << "\t" << wIn << "\t" << etaAdaib << "\t" << intrpStgPerf1.getPt3() << "\t" << intrpStgPerf2.getPt3() << "\t" << intrpStgPerf5.getPt3() << "\t" << intrpStgPerf8.getPt3() << "\n";

	return 0;

}

vector<CompressorSpeedLine*> readComp1dOutfile(string outfileName) {
	//open output file and read speed line results
	//interpolate mass flow for user specified pressure ratio

	const string speedLineHdr("OVERALL RESULTS FOR SPEEDLINE");

	//Comp1d (v1.2) - static const string speedLinePntsHdr("POINT    TIN    PIN     WCORR     PR     DT/T   ETAADI  ETAPOLY  WCORROUT");
	const string speedLinePntsHdr("POINT     FLOW     WCORR     WCORR    DT/T     PR     ETAADI  ETAPOLY     PR     ETAADI  ETAPOLY");

	const string stageSmryHdrStg("STAGE");
	const string stageSmryHdrSmry("SUMMARY");
	const string stageSmryHdrOgv("OGV");

	vector<string> lineParts;

	CompressorSpeedLine *crntSpeedLine;
	vector<CompressorSpeedLine*> speedLines;

	CompressorOperatingPoint *tmpOpPnt;
	//vector<CompressorOperatingPoint> opPnts;

	//CompressorStage* tmpStage;
	vector<CompressorStage*> stages;

	string line;
	double crntSpeed;
	double crntTamb;

	//open a file input stream
	ifstream myfile (outfileName.data(), ifstream::in);

	if (myfile.is_open())
	{

		//read lines of the file stream
		while ( myfile.good() )
		{

			getline(myfile,line);

			if ( line.compare(0, speedLineHdr.length(), speedLineHdr) == 0 ) {
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
								, -9999
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

				//delete(newStg);

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
				//crntSpeedLine->addStage(*newStg);  //jgb - doesn't exist yet

				//delete(newStg);

			}
		} //end while (myfile.good)

		//close file stream
		myfile.close();

	}

	//delete tmpOpPnt;
	//delete crntSpeedLine;
	return speedLines;

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


