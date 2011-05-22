/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPluginProcessorCollective.h"

#include "coreDataEntityReader.h"
#include "coreDataEntityWriter.h"
#include "coreSignalReader.h"
#include "coreSignalWriter.h"

svProcessorCollective::svProcessorCollective()
{

	// Signal data entity
	Core::IO::DataEntityReader::RegisterFormatReader(
		Core::IO::BaseDataEntityReader::Pointer( Core::IO::SignalReader::New() ) );

	Core::IO::DataEntityWriter::RegisterFormatWriter(
		Core::IO::BaseDataEntityWriter::Pointer( Core::IO::SignalWriter::New() ) );

}

