/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginProcessorCollective_H
#define _CmguiPluginProcessorCollective_H

#include "CmguiPluginExnodeExelemReader.h"

#include "coreBaseDataEntityReader.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"

namespace CmguiPlugin{


/**
\brief 
\author Martin Steghöfer
\date Oct 2010
\ingroup CMGUIPlugin
*/
class ProcessorCollective : public Core::SmartPointerObject
{
public:
    //!
    //coreDeclareSmartPointerClassMacro(ProcessorCollective, Core::SmartPointerObject);
    coreDeclareSmartPointerTypesMacro(ProcessorCollective, Core::SmartPointerObject)
    coreClassNameMacro(ProcessorCollective)
    coreFactorylessNewMacro1Param(ProcessorCollective, Cmgui*)
    
private:
    //! The constructor instantiates all the processors and connects them.
    ProcessorCollective(Cmgui*);
    ~ProcessorCollective();
    
private:
    Core::IO::BaseDataEntityReader::Pointer m_fileReader;
    Core::IO::BaseDataEntityReader::Pointer m_mechanicalSimulationReader;
    Core::IO::BaseDataEntityReader::Pointer m_meshFittingReader;
};

} // namespace CmguiPlugin{

#endif //_CmguiPluginProcessorCollective_H
