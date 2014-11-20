/*
	Params.cpp
	Copyright 2014 Matthew V. Talluto
	This code is released under the simplified BSD license. See the LICENSE file for full
	details.

*/

#include "Params.h"

#include <fstream>

par::Params::Params(const char * f, const std::string &delim, const std::string &c):
source(f), comment(c)
{
	for(int i = 0; i < delim.length(); i++)
		delimiters.push_back(delim[i]);
    
	read_file();
}

void par::Params::read_file()
{
	std::ifstream inputFile (source);
	if(!inputFile.is_open()) {
		std::stringstream ss;
		ss << "Warning: problem reading from file <" << source << ">";
		throw (std::runtime_error (ss.str()));
	}
	get_lines(inputFile);
	inputFile.close();
}

void par::Params::get_lines(std::ifstream &file)
{
	while(file.good()) {
		static size_t lineno = 0;	// for reporting the line number of errors
		std::string line;
		getline(file, line);
		lineno++;
		
		std::vector<std::string> lineData;
		std::string varName;
		try {
			lineData = split(line, delimiters);
		}
		catch(...) {
			std::cerr << "error: problem parsing input, line " << lineno << std::endl;
			throw;
		}
        
		if(lineData.size() == 0) continue;		// skip lines that are empty or were commented
		varName = lineData[0];
		lineData.erase(lineData.begin());
		
		data[varName] = lineData;
	}
}

std::vector<std::string> par::split(const std::string &s, const std::vector<char> &delim, const std::string &comment)
{
	std::vector<std::string> dest;
    
	// ignore commented lines
	if( s.substr(0,comment.length()) == comment )
		return dest;
	
	std::stringstream ls(s);	// create stringstream out of input
	std::string dat;
	while(getline(ls, dat, delim.back())) {
		if(delim.size() > 1) {
			std::vector<std::string> fillDest = split(dat, std::vector<char> (delim.begin(), delim.end() - 1), comment);
			dest.insert(dest.end(), fillDest.begin(), fillDest.end());
		}
		else {
			dest.push_back(dat);
		}
	}
	
	return dest;
}
