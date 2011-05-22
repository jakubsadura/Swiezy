/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blProgramOptions.h"

void blParseOptions(
					int argc, 
					char** argv, 
					const boost::program_options::options_description& desc, 
					blCommandLine& commandLine,
					bool canThrow
					)
{
	commandLine.parsedOkay = true;
	commandLine.parseMessage = "";
	try
	{
		using namespace boost::program_options;
		basic_parsed_options<char> parsed = parse_command_line(argc, argv, desc);
		store(parsed, commandLine.options);
		notify(commandLine.options); 
		commandLine.args = collect_unrecognized<char>(parsed.options, include_positional); 
	}
	catch(std::exception& e)
	{
		commandLine.parsedOkay = false;
		commandLine.parseMessage = e.what();
		if( canThrow )
			throw; // rethrow the exception
	}
	catch(...)
	{
		commandLine.parsedOkay = false;
		commandLine.parseMessage = "Unknown problem parsing the command line";
		if( canThrow )
			throw; // rethrow the exception
	}
}
