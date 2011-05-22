/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _coreBaseWindow_H
#define _coreBaseWindow_H

#include "gmKernelWin32Header.h"
#include "coreRenderingTree.h"
#include "coreBaseProcessor.h"
#include "coreProcessorOutputObserver.h"
#include "coreCommonDataTypes.h"
#include "coreRenderingTreeManager.h"
#include "coreBaseWindowFactory.h"
#include "blTagMap.h"

class ModuleDescription;

namespace Core
{

	namespace Widgets
	{
		class UserHelper;
		class RenderWindowBase;
		class LandmarkSelectorWidget;
		class DataEntityListBrowser;
		class ProcessorInputWidget;
		class PluginTab;
	}

/**
\brief Base Interface for all Gimias Windows. 

It has some basic functions used by all the windows, to be able to 
reuse the code. The windows needs to be registered to Gimias, in order to get
benefit from this generic interface.

- Update the active RenderingTree
- Update the active RenderWindowBase
- Store a pointer to several plugin widgets

\ingroup gmKernel
\author Xavi Planes
\date 1 Mar 2010
*/
class GMKERNEL_EXPORT BaseWindow : public Core::Object
{
public:

	typedef Core::DataHolder<blTagMap::Pointer> MetadataHolderType;

	//!
	virtual Core::BaseProcessor::Pointer GetProcessor( );

	/** Set and Get properties of the window to serialize them
	or visualize in the GUI. Is responsibility of the subclass to 
	put the properties in m_MetadataHolder
	*/
	virtual void SetProperties( blTagMap::Pointer tagMap );
	virtual void GetProperties( blTagMap::Pointer tagMap );

	//! Metadata information of this window. 
	MetadataHolderType::Pointer GetMetadataHolder() const;
	blTagMap::Pointer GetMetadata() const;

	//! 
	virtual void SetRenderingTree( RenderingTree::Pointer tree );
	virtual RenderingTree::Pointer GetRenderingTree() const;

	//! 
	Core::Widgets::UserHelper * GetHelperWidget() const;
	virtual void SetHelperWidget(Core::Widgets::UserHelper * val);

	//!
	Core::Widgets::RenderWindowBase* GetMultiRenderWindow() const;
	virtual void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

	//!
	Core::Widgets::DataEntityListBrowser* GetListBrowser() const;
	virtual void SetListBrowser(Core::Widgets::DataEntityListBrowser* val);

	//!
	Core::Widgets::ProcessorInputWidget* GetInputWidget( int num );
	virtual void SetInputWidget( int num, Core::Widgets::ProcessorInputWidget* widget );

	//!
	Core::ProcessorOutputObserver::Pointer GetProcessorOutputObserver( int num );

	//!
	Core::Widgets::PluginTab* GetPluginTab() const;
	virtual void SetPluginTab(Core::Widgets::PluginTab* val);

	/** Init observers to output processor data objects
	\param [in] enableDefaultObservers Enable ProcessorOutputObserver
	*/
	virtual void InitProcessorObservers( bool enableDefaultObservers );

	//!
	virtual int GetTimeStep() const;
	virtual void SetTimeStep( int time );
	virtual void SetTimeStepHolder(Core::IntHolderType::Pointer val);

	//!
	void SetBitmap(const char* const* data);
	const char* const* GetBitmap() const;

	//!
	std::string GetBitmapFilename() const;
	void SetBitmapFilename(std::string val);

	//!
	void Init( );

	//!
	std::string GetFactoryName() const;
	void SetFactoryName(std::string val);

	//!
	void SetModule( ModuleDescription* module );
	ModuleDescription* GetModule( );

protected:
	//!
	BaseWindow( );

	//!
	virtual ~BaseWindow( );

	//! Called when BaseWindow is initialized
	virtual void OnInit();

	//! Callback function called when input processor is new or modified
	virtual void OnModifiedInput( int num );

	//! Callback function called when output processor is new or modified
	virtual void OnModifiedOutput( int num );

	//! Callback function called when input processor is new
	virtual void OnNewInput( int num );

	//! Callback function called when output processor is new
	virtual void OnNewOutput( int num );

	//!
	virtual void OnModifiedMetadata( );

private:
	//! Current rendering tree to render objects
	RenderingTree::Pointer m_RenderingTree;

	//! User helper
	Core::Widgets::UserHelper *m_HelperWidget;

	//!
	Core::Widgets::RenderWindowBase* m_MultiRenderWindow;

	//!
	Core::Widgets::DataEntityListBrowser* m_ListBrowser;

	//!
	Core::Widgets::PluginTab* m_PluginTab;

	//!
	std::vector<Core::ProcessorOutputObserver::Pointer> m_ProcessorOutputObserverVector;

	//!
	std::map<int, Core::Widgets::ProcessorInputWidget*> m_ProcessorInputWidgetMap;

	//!
	Core::IntHolderType::Pointer m_TimeStepHolder;

	//! Bitmap in xpm format for the toolbar
	boost::any m_Bitmap_xpm;

	//! Factory name used to create this widget
	std::string m_FactoryName;

	//! Filename of the bitmap used for the toolbar
	std::string m_BitmapFilename;

	//!
	ModuleDescription* m_Module;

	//! Set of generic properties
	MetadataHolderType::Pointer m_MetadataHolder;
};

} // namespace Core

#endif // _coreBaseWindow_H

