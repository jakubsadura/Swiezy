/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginExnodeExelemReader_H
#define _CmguiPluginExnodeExelemReader_H

#include "CmguiPluginCmgui.h"

#include "coreBaseDataEntityReader.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"


namespace CmguiPlugin {


/**
\brief 
\author Martin Steghöfer
\date Oct 2010
\ingroup CMGUIPlugin
*/
class ExnodeExelemReader : public Core::IO::BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(ExnodeExelemReader, Core::IO::BaseDataEntityReader)

    virtual bool ReadHeader( );
    
    virtual void Update();
    
    virtual boost::any ReadSingleTimeStep(int iTimeStep, const std::string &filename );
    
protected:
    ExnodeExelemReader();
    virtual ~ExnodeExelemReader();
    
private:
    coreDeclareNoCopyConstructors(ExnodeExelemReader);
    
};

} // namespace CmguiPlugin

#endif //_CmguiPluginExnodeExelemReader_H
