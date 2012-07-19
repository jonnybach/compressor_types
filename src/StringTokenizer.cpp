/*
 * StringTokenizer.cpp
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#include "StringTokenizer.h"

using std::vector;
using std::string;
using std::stringstream;

vector<string>& split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s,stringstream::in);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    return split(s, delim, elems);
}

vector<string> removeWhiteSpaceElems(vector<string> &elems) {

	vector<string> newElems;

	vector<string>::iterator it;
	for ( it=elems.begin() ; it < elems.end(); it++ ) {
		if (*it != "") {
			newElems.push_back(*it);
		}
	}

	return newElems;

}

vector<string> removeElemsOfString(vector<string> &elems, const string strToRemove) {
	vector<string> newElems;

	vector<string>::iterator it;
	for ( it=elems.begin() ; it < elems.end(); it++ ) {
		if (*it != strToRemove) {
			newElems.push_back(*it);
		}
	}

	return newElems;
}
