/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLPROGRAMOPTIONS_H
#define BLPROGRAMOPTIONS_H

#include <boost/program_options.hpp>
#include <vector>

struct blCommandLine
{
	boost::program_options::variables_map options; 
	std::vector<std::string> args;
	bool parsedOkay;
	std::string parseMessage;
};

void blParseOptions(
	int argc, 
	char** argv, 
	const boost::program_options::options_description& desc, 
	blCommandLine& commandLine,
	bool canThrow = true
);

#endif //BLPROGRAMOPTIONS_H
