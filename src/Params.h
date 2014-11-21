/*
	Params.h
	Copyright 2014 Matthew V. Talluto
	This code is released under the simplified BSD license. See the LICENSE file for full
	details.

*/

#ifndef PARAMS_H
#define PARAMS_H


#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace par {
    
    std::vector<std::string> split(const std::string &s, const std::vector<char> &delim, const std::string &comment = "#");
    template<typename T>
    T str_convert(const std::string &s);
    
	template<> inline
	std::string str_convert<std::string>(const std::string &s);

    class Params
    {
        std::map<std::string, std::vector<std::string> >	data;
        const char * source;
        std::vector<char> delimiters;
        const std::string comment;
        
        void read_file();
        void get_lines(std::ifstream &file);
        
	public:
		Params(const char * f, const std::string &delim = " \t", const std::string &c = "#");
        
		template<typename T>
		std::vector<T> get_val(const std::string &key) const;
    };
    
    
    // TEMPLATE FUNCTIONS
    
    template<typename T>
    std::vector<T> Params::get_val(const std::string &key) const
    {
        std::vector<T> result;
        try {
            const std::vector<std::string> vals = data.at(key);
            for(int i = 0; i < vals.size(); i++) {
                result.push_back(str_convert<T>(vals[i]));
            }
        }
        catch(const std::out_of_range& ex) {
            std::stringstream ss;
            ss << "Warning: parameter parser tried to access unknown parameter <" << key << ">\t" << ex.what();
            throw (std::runtime_error (ss.str()));
        }
		
        return result;
    }
    
	template<typename T> inline
	T str_convert(const std::string &s)
	{
		T result;
		std::istringstream val(s);			// create stream from the string
		if(!(val >> result)) {
			std::stringstream ss;
			ss << "Cannot convert value <" << s << "> from string into requested type";
			throw( std::runtime_error (ss.str() ));
		}
		return result;
	}


	template<>
	std::string str_convert<std::string>(const std::string &s)
	{
		return s;
	}
	
	} // end namespace par


#endif /* defined(PARAMS_H) */
