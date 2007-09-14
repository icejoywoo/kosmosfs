//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id: properties.cc $
//
// \brief Properties implementation.
//
// Created 2004/05/05
//
// Copyright (C) 2006 Kosmix Corp.
//
// This file is part of Kosmos File System (KFS).
//
// KFS is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation under version 3 of the License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "properties.h"

using namespace KFS;

Properties::Properties()
{
    propmap = new std::map<std::string, std::string>;
}

Properties::Properties(const Properties &p)
{
    propmap = new std::map<std::string, std::string>(*(p.propmap));
}

Properties::~Properties()
{
    delete propmap;
}

int Properties::loadProperties(const char* fileName, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::ifstream input(fileName);
    std::string line;

    if(!input.is_open()) 
    {
        std::cerr <<  "Properties::loadProperties() Could not open the file:" << fileName << std::endl;
        return(-1);
    }
    loadProperties(input, delimiter, verbose, multiline);
    input.close();
    return 0;
}

int Properties::loadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::string line;

    while(ist)
    {
        getline(ist, line);                       //read one line at a time
        if  (line.find('#') == 0)
            continue;                               //ignore comments
        std::string::size_type pos =
            line.find(delimiter);                   //find the delimiter
        
        if( pos == line.npos )
            continue;                               //ignore if no delimiter is found
        std::string key = line.substr(0,pos);       // get the key
        key = removeLTSpaces(key);
        std::string value = line.substr(pos+1);     //get the value
        value = removeLTSpaces(value);

		if (multiline)
        	(*propmap)[key] += value;					// allow properties to be spread across multiple lines
        else
        	(*propmap)[key] = value;
        
        if( verbose)
            std::cout << "Loading key " << key  << " with value " << (*propmap)[key] << std::endl;
    }
    return 0;
}



void Properties::setValue(const std::string key, const std::string value) {
    (*propmap)[key] = value;
    return;
}

std::string Properties::removeLTSpaces(std::string str){

    char const* delims = " \t\r\n";

    // trim leading whitespace
    std::string::size_type  notwhite = str.find_first_not_of(delims);
    str.erase(0,notwhite);

   // trim trailing whitespace
   notwhite = str.find_last_not_of(delims);
   str.erase(notwhite+1);
   return(str);
}

std::string Properties::getValue(std::string key, std::string def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (*propmap)[key];
}
const char* Properties::getValue(std::string key, const char* def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (((*propmap)[key]).c_str());
}

int Properties::getValue(std::string key, int def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoi(((*propmap)[key]).c_str()));
}
long Properties::getValue(std::string key, long def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

long long Properties::getValue(std::string key, long long def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

uint64_t Properties::getValue(std::string key, uint64_t def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

double Properties::getValue(std::string key, double def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atof(((*propmap)[key]).c_str()));
}

void Properties::getList(std::string &outBuf, std::string linePrefix) const {
  std::map<std::string, std::string>::iterator iter;

  for (iter = propmap->begin(); iter != propmap->end(); iter++) {
    if ((*iter).first.size() > 0) {
      outBuf += linePrefix;
      outBuf += (*iter).first;
      outBuf += '=';
      outBuf += (*iter).second;
      outBuf += '\n';
    }
  }

  return;
}