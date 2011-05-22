/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseWindowFactory_H
#define _coreBaseWindowFactory_H

#include "coreObject.h"
#include "coreBaseWindow.h"
#include "coreWindowConfig.h"
#include <typeinfo>
#include "ModuleDescription.h"

namespace Core
{

/**
\brief Base Factory for BaseWindow

\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
class GMKERNEL_EXPORT BaseWindowFactory : public Core::SmartPointerObject
{
public:
	coreClassNameMacro(BaseWindowFactory);
	coreDeclareSmartPointerTypesMacro(
		BaseWindowFactory, 
		Core::SmartPointerObject);

	//!
	void SetParent( wxWindow* parent );
	wxWindow* GetParent() const;

	//!
	void SetWindowId( int id );
	int GetWindowId( ) const;

	//!
	std::string GetWindowName() const;
	void SetWindowName(std::string val);

	//!
	wxPoint GetPosition() const;
	void SetPosition(wxPoint val);

	//!
	wxSize GetSize() const;
	void SetSize(wxSize val);

	//!
	std::string GetBitmapFilename() const;
	void SetBitmapFilename(std::string val);

	//! The name will be used to identify the factory
	virtual std::string GetName() const = 0;

	//!
	virtual Core::BaseWindow* CreateWindow( ) = 0;

	//!
	ModuleDescription &GetModule();
	void SetModule(ModuleDescription val);

protected:
	//!
	BaseWindowFactory( );

protected:
	//!
	wxWindow* m_Parent;
	//!
	int m_WindowId;
	//!
	std::string m_WindowName;
	//!
	wxPoint m_Position;
	//!
	wxSize m_Size;
	//
	std::string m_BitmapFilename;
	//!
	ModuleDescription m_Module;
};

/**
\brief Base Factory for BaseWindow

\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
class GMKERNEL_EXPORT BaseWindowReaderFactory : public BaseWindowFactory
{
public:
	coreClassNameMacro(BaseWindowReaderFactory);
	coreDeclareSmartPointerTypesMacro(BaseWindowReaderFactory, BaseWindowFactory);

	//! Check if this reader is matching the type and ext
	virtual bool CheckType( Core::DataEntityType type, const std::string &ext );

protected:
	//!
	BaseWindowReaderFactory( );
protected:
	//!
	Core::DataEntityType m_DataEntitytype;
	//!
	std::string m_Extension;
};


} // namespace Core

/**
\brief NewBase() function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsNewBase(className) \
	static BaseWindowFactory::Pointer NewBase(void) \
		{ \
		Pointer p(New()); \
		return p.GetPointer(); \
		}

/**
\brief CreateWindowCommon( ) function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsCreateWindowCommon() \
		window->SetFactoryName( GetName( ) );\
		window->SetBitmapFilename( GetBitmapFilename( ) ); \
		window->SetModule( &GetModule( ) ); \
		if ( !GetWindowName( ).empty( ) ){ \
			wxWindow* windowBase = window; \
			windowBase->SetName( GetWindowName( ) ); \
		} \
		return window; \
	}

/**
\brief CreateWindow( ) function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsCreateWindow(className) \
	virtual Core::BaseWindow* CreateWindow( ) \
	{ \
		className* window = new className( GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );\
		coreCommonFactoryFunctionsCreateWindowCommon( )

/**
\brief CreateWindow() function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsCreateWindow1param(className) \
	virtual Core::BaseWindow* CreateWindow( ) \
	{ \
		className* window = new className( m_p1, GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );\
		coreCommonFactoryFunctionsCreateWindowCommon( )

/**
\brief CreateWindow() function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsCreateWindow2param(className) \
	virtual Core::BaseWindow* CreateWindow( ) \
	{ \
		className* window = new className( m_p1, m_p2, GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );\
		coreCommonFactoryFunctionsCreateWindowCommon( )


/**
\brief NewBase() function
\ingroup gmKernel
\author Xavi Planes
\date 8 Oct 2010
*/
#define coreCommonFactoryFunctionsGetName(className) \
	std::string GetName() const \
		{ \
		return typeid( className ).name( ); \
		}


/**
\brief Common factory functions definitions
\note For template classes like SimpleProcessingWidget, we need to use
typeid, to be able to identify each class uniquelly
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreCommonFactoryFunctions(className) \
		coreCommonFactoryFunctionsNewBase( className ) \
		coreCommonFactoryFunctionsCreateWindow( className ) \
		coreCommonFactoryFunctionsGetName( className )

/**
\brief Create a BaseWindowFactory derived class
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreDefineBaseWindowFactory(className) \
	class Factory : public Core::BaseWindowFactory \
	{ \
	public:  \
		coreDeclareSmartPointerTypesMacro(Factory,BaseWindowFactory) \
		coreFactorylessNewMacro(Factory) \
		coreClassNameMacro(Factory)\
		coreCommonFactoryFunctions( className )\
	};

/**
\brief Create a BaseWindowFactory derived class
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreDefineBaseWindowFactory1param(className,param1) \
	class Factory : public Core::BaseWindowFactory \
	{ \
	public:  \
		coreDeclareSmartPointerTypesMacro(Factory,BaseWindowFactory) \
		coreFactorylessNewMacro1Param( Factory, param1 ) \
		coreClassNameMacro(Factory)\
		coreCommonFactoryFunctionsCreateWindow1param( className ) \
		coreCommonFactoryFunctionsGetName( className ) \
    private: \
        param1 m_p1; \
        Factory(param1 p1) { \
            m_p1 = p1; \
        } \
	};

/**
\brief Create a BaseWindowFactory derived class
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreDefineBaseWindowFactory2param(className,param1,param2) \
	class Factory : public Core::BaseWindowFactory \
	{ \
	public:  \
		coreDeclareSmartPointerTypesMacro(Factory,BaseWindowFactory) \
		coreFactorylessNewMacro2Param( Factory, param1, param2 ) \
		coreClassNameMacro(Factory)\
		coreCommonFactoryFunctionsCreateWindow2param( className ) \
		coreCommonFactoryFunctionsGetName( className ) \
    private: \
        param1 m_p1; \
        param2 m_p2; \
        Factory(param1 p1, param2 p2) { \
            m_p1 = p1; \
            m_p2 = p2; \
        } \
	};


/**
\brief Create a BaseWindowFactory derived class for reader dialogs
- dataEntityType is the DataEntityType that this dialog can manage
- ext is the extension that this dialog can manage
\ingroup gmKernel
\author Xavi Planes
\date 21 June 2010
*/
#define coreDefineBaseWindowReaderFactory(className, dataEntityType, ext) \
	class Factory : public Core::BaseWindowReaderFactory \
	{ \
	public:  \
		coreDeclareSmartPointerTypesMacro(Factory,BaseWindowReaderFactory) \
		coreFactorylessNewMacro(Factory) \
		coreClassNameMacro(Factory)\
		coreCommonFactoryFunctions( className )\
	protected:\
		Factory( ){\
			m_DataEntitytype = dataEntityType;\
			m_Extension = ext;\
		}\
	}; \


#endif // coreBaseWindowFactory_H
