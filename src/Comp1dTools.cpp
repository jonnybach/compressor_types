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


using namespace std;


//create constants that will be used throughout cpp file
static const string speedLineHdr("OVERALL RESULTS FOR SPEEDLINE");
static const string speedLinePntsHdr("POINT    TIN    PIN     WCORR     PR     DT/T   ETAADI  ETAPOLY  WCORROUT");

static const string stageSmryHdr1("STAGE");
static const string stageSmryHdr2("SUMMARY");
static const string stageSmryDataHdr1("POINT   ALF1   ALF2   ALF3   ALF4   DEFR   DEFS     WR     WS    MNR    MNS    DFR    DFS   INCR   INCS");
static const string stageSmryDataHdr2("POINT    PS1    PS2    PS3    PS4    PT0    PT1    PT2    PT3    PT4    UM      T0    DEVR   DEVS");


//function prototype declarations
vector<CompressorSpeedLine*> readComp1dOutfile(string outfileName);
void showHelp();
vector<CompressorStagePerformance*> getPerfForStage(ifstream& fileToParse, CompressorStage* stage);

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

    //inhibit the error message from getopt by setting opterr to zero
    opterr = 0;

    while ((c = getopt(argc, argv, "p:")) != -1) {
    	switch (c) {
    	//case 'a':
    	//  aflag = 1;
    	//  break;
    	//case 'b':
    	//  bflag = 1;
    	//  break;
    	case 'p':
    		pvalue = optarg;
    		pRatio = atof(pvalue);
    		break;
    	case '?':
    		if (optopt == 'p')
    			fprintf (stderr, "Option -%c requires an argument.\n", optopt);
    		else if (isprint(optopt))
    			fprintf (stderr, "Unknown option `-%c'.\n", optopt);
    		else
    			fprintf (stderr,
    					"Unknown option character `\\x%x'.\n",
    					optopt);
    		return 1;
    	default:
    		abort ();
    	}
    }

    //get remaining non-option command arguments
    int index = 0;
    for (index = optind; index < argc; index++) {
    	if (index == (argc-2)) {
    		//argument is input file name
    		infileName = argv[index];
    	} else if (index == (argc-1)) {
    		//argument is output file name
    		outfileName = argv[index];
    	}
    }

//	int i;
//	for	(i = 0; i < argc; i++) {
//		if (strncmp(argv[i],"-p",2) == 0) {
//			pRatio = atof(argv[i]+2);
//		} else if (i==(argc-2)) {
//			//argument is input file name
//			infileName = argv[i];
//		} else if (i==(argc-1)) {
//			//argument is output file name
//			outfileName = argv[i];
//		}
//	}

	//TODO check that the infile exists, if not return error

	//call cmodp
	string sysCall;
	sysCall.assign("/swd/cmodp/cmodp");
	sysCall.append(" ");
	sysCall.append(infileName);
	sysCall.append(" ");
	sysCall.append(outfileName);
	const char *cCall = sysCall.c_str();
	int rslt = system(cCall);

	//compile the speedline objects from the results
	vector<CompressorSpeedLine*> speedLines;

	if (rslt) {
		//comp1d failed
	} else {
		speedLines = readComp1dOutfile(outfileName);
	}

	//calculate the mass flow for the given pressure ratio
	double wCrrctIn, etaAdaib;
	speedLines[0]->calcMassAndEta(pRatio, &wCrrctIn, &etaAdaib);

	cout << "Press_Ratio\tW_Crct_In\tEta_Adiab\n";
	cout << pRatio << "\t" << wCrrctIn << "\t" << etaAdaib << "\n";

	return 0;

}

vector<CompressorSpeedLine*> readComp1dOutfile(string outfileName) {
	//open output file and read speed line results
	//interpolate mass flow for user specified pressure ratio

	vector<string> lineParts;

	CompressorSpeedLine *crntSpeedLine;
	CompressorOperatingPoint *tmpOpPnt;

	vector<CompressorSpeedLine*> speedLines;
	//vector<CompressorOperatingPoint> opPnts;

	string line;
	double crntSpeed;

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
				crntSpeedLine->setShaftSpeed(crntSpeed);

				//add crntSpeedLine to speedLines vector
				speedLines.push_back(crntSpeedLine);

			} else if ( line.compare(0, speedLinePntsHdr.length(), speedLinePntsHdr) == 0 ) {
				//else if ( line == speedLinePntsHdr ) {
				//found header for speed line point results

				//get data for each operating point on speed line
				//TODO Add error checking for cases where more than one speedline exits
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
								, atof(lineParts[1].c_str())
								, atof(lineParts[2].c_str())
								, atof(lineParts[4].c_str())
								, atof(lineParts[5].c_str())
								, atof(lineParts[6].c_str())
								, atof(lineParts[7].c_str())
								, atof(lineParts[3].c_str())
								, atof(lineParts[8].c_str())
								, crntSpeedLine->getShaftSpeed());

						//add operating point to vector of operating points
						//opPnts.push_back(tmpOpPnt);
						crntSpeedLine->addOperatingPoint(*tmpOpPnt);
					} //end if ( !line.empty)
				} //end while (myfile.good)
			/*} else if (
				line.find(stageSmryHdr1, 0 ) != string::npos
				&& line.find(stageSmryHdr2, 0 ) != string::npos
				) {
				// found stage data

				// determine what stage number this section corresponds to
				lineParts = split(line, ' ');
				lineParts = removeWhiteSpaceElems(lineParts);

				//second item in lineParts vector should contain the stage number
				string tmpStgName = lineParts[1];

				//instantiate a new compressor stage object
				CompressorStage* newStg = new CompressorStage(tmpStgName);

				vector<CompressorStagePerformance*> x = getPerfForStage(myfile, newStg);
			*/
			}
		} //end while (myfile.good)

		//close file stream
		myfile.close();

	}

	delete tmpOpPnt;
	delete crntSpeedLine;
	return speedLines;

}

vector<CompressorStagePerformance*> getPerfForStage(ifstream& fileToParse, CompressorStage* stage) {

	double tmpPt0, tmpPt1, tmpPt2, tmpPt3, tmpPt4;
	double tmpTt0;
	double tmpAlp1, tmpAlp2, tmpAlp3, tmpAlp4;

	bool foundStageEnd = false;

	//loop through lines of text in the stage data section and find the pressure and temperature data
	string line;
	while ( fileToParse.good() || !foundStageEnd ) {
		getline(fileToParse, line);
		if (
			line.find(stageSmryHdr1, 0 ) != string::npos
			&& line.find(stageSmryHdr2, 0 ) != string::npos
			) {
			//found end of current stage data
			//create and return compressor stage data

			foundStageEnd = true;

		} else if ( line.compare(0, stageSmryDataHdr1.length(), stageSmryDataHdr1) ) {
			//found line of text containing alpha data


		} else if ( line.compare(0, stageSmryDataHdr2.length(), stageSmryDataHdr2) ) {
			//found line of data containing press and temp data


		}

	} //end while (myfile.good)

	//CompressorStagePerformance* newCmpStg = new CompressorStagePerformance(stgNmbr, tmpPt0, tmpPt1, tmpPt2, tmpPt3, tmpPt4
	//												, tmpTt0, tmpAlp1, tmpAlp2, tmpAlp3, tmpAlp4);
	//return newCmpStg;

}

void showHelp() {
	printf("\nComp1D Tools - ERROR in argument syntax\n"
			"\tProgram must be called as follows:\n"
			"\tcomp1dtools [Options] \"input file\" \"output file\"\n"
			"\t    1) \"input file\" is name of Comp1D input file\n"
			"\t    2) \"output file\" is name of the output file to write results to\n"
			"\t    Options) currently the only option is -p <PRESSURE RATIO>  which allows Comp1d tools to \n"
			"\t             determine the mass flow associated with a specified pressure ratio\n");
}


