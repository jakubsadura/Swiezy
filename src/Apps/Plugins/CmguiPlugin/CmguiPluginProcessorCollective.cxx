/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginProcessorCollective.h"

// processors
#include "CmguiPluginExnodeExelemReader.h"
#include "CmguiPluginMechanicalSimulationReader.h"
#include "CmguiPluginMeshFittingReader.h"

#include "coreDataEntityReader.h"
#include "CmguiRegionImpl.h"
#include "CmguiImageImpl.h"

using namespace CmguiPlugin;

ProcessorCollective::ProcessorCollective(Cmgui* cmgui)
{
    m_fileReader = ExnodeExelemReader::New();
    m_mechanicalSimulationReader = MechanicalSimulationReader::New();
    m_meshFittingReader = MeshFittingReader::New();
    Core::IO::DataEntityReader::RegisterFormatReader(m_fileReader);
    Core::IO::DataEntityReader::RegisterFormatReader(m_mechanicalSimulationReader);
    Core::IO::DataEntityReader::RegisterFormatReader(m_meshFittingReader);

	Cmiss_region *rootRegion = cmgui->GetRootRegion();
	CmguiPlugin::CmguiRegionImpl::Factory::Pointer f;
	f = CmguiPlugin::CmguiRegionImpl::Factory::New( rootRegion );
	Core::DataEntityImplFactory::RegisterFactory( f.GetPointer() );

	CmguiPlugin::CmguiImageImpl::Factory::Pointer f1;
	f1 = CmguiPlugin::CmguiImageImpl::Factory::New( rootRegion );
	Core::DataEntityImplFactory::RegisterFactory( f1.GetPointer() );
}

ProcessorCollective::~ProcessorCollective()
{
    Core::IO::DataEntityReader::UnRegisterFormatReader(m_fileReader);
    Core::IO::DataEntityReader::UnRegisterFormatReader(m_mechanicalSimulationReader);
    Core::IO::DataEntityReader::UnRegisterFormatReader(m_meshFittingReader);
}

