/*
 * StringTokenizer.h
 *
 *  Created on: May 1, 2012
 *      Author: bachm03j
 */

#ifndef STRINGTOKENIZER_H_
#define STRINGTOKENIZER_H_

#include <string>
#include <sstream>
#include <vector>

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> removeWhiteSpaceElems(std::vector<std::string> &elems);
std::vector<std::string> removeElemsOfString(std::vector<std::string> &elems, const std::string strToRemove);

#endif /* STRINGTOKENIZER_H_ */
