/*
 * StringTrimmers.h
 *
 *  Created on: Jul 19, 2012
 *      Author: bachm03j
 */

#ifndef STRINGTRIMMERS_H_
#define STRINGTRIMMERS_H_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

static inline bool replaceSubstring(std::string &mainString, std::string &subString, std::string &newSubString ) {
	bool found = false;
	size_t subStrSize = subString.size();
	size_t subStrPos = mainString.find(subString);
	if (subStrPos != std::string::npos) {
		found = true;
		mainString.replace(subStrPos, subStrSize, newSubString);
	}

	return found;
}

static inline void stoupper(std::string& s) {
	std::string::iterator i = s.begin();
	std::string::iterator end = s.end();

	while (i != end) {
		*i = std::toupper((unsigned char)*i);
		++i;
	}
}

static inline void stolower(std::string& s) {
	std::string::iterator i = s.begin();
	std::string::iterator end = s.end();

	while (i != end) {
		*i = std::tolower((unsigned char)*i);
		++i;
	}
}

#endif /* STRINGTRIMMERS_H_ */
