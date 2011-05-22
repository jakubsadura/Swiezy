/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionImpl_H
#define _dynModuleExecutionImpl_H

#include "DynLibWin32Header.h"
#include "ModuleDescription.h"
#include "blLightObject.h"
#include <list>
#include <map>

/**
Dynamic ModuleDescription execution implementation base class

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleExecutionImpl : public blLightObject
{
public:
	typedef dynModuleExecutionImpl Self;
	typedef blSmartPointer<Self> Pointer;

	/**
	Factory for dynModuleExecutionImpl
	*/
	class Factory : public blLightObject {
	public:
		typedef Factory Self;
		typedef blSmartPointer<Self> Pointer;
		
		//! Create a new dynModuleExecutionImpl instance
		virtual dynModuleExecutionImpl::Pointer NewModuleExecution( ) = 0;
	};

	//!
	ModuleDescription *GetModule() const;
	void SetModule(ModuleDescription *val);

	//!
	virtual void Update( ) = 0;

	//!
	std::string GetWorkingDirectory() const;
	void SetWorkingDirectory(std::string val);

	std::string GetTemporaryDirectory() const;
	void SetTemporaryDirectory(std::string val);

	//!
	void SetSaveScript( bool val );
	bool GetSaveScript() const;

protected:
	//!
	dynModuleExecutionImpl( );

	//!
	virtual ~dynModuleExecutionImpl( );

	//! Set all parameters to m_Filter
	void SetAllParameters( );

	//! Get all parameters from m_Filter
	void GetAllParameters( );

	//! Set a parameter value to m_Filter
	virtual void SetParameterValue( ModuleParameter *param ) = 0;

	//! Get a parameter value from m_Filter
	virtual void GetParameterValue( ModuleParameter *param ) = 0;

protected:
	//!
	ModuleDescription *m_Module;

	//! Working Directory when calling a process
	std::string m_WorkingDirectory;

	//! Temporary directory for in / out
	std::string m_TemporaryDirectory;

	//!
	std::string m_ModuleType;

	//!
	bool m_SaveScript;
};

#define defineModuleFactory( ClassName ) \
	class Factory : public dynModuleExecutionImpl::Factory \
	{ \
	public: \
		typedef Factory Self; \
		typedef blSmartPointer<Self> Pointer; \
		static dynModuleExecutionImpl::Factory::Pointer New(void) \
		{ \
			Pointer smartPtr; \
			Self* rawPtr = new Self; \
			smartPtr = rawPtr; \
			rawPtr->UnRegister(); \
			return smartPtr.GetPointer( ); \
		} \
		virtual dynModuleExecutionImpl::Pointer NewModuleExecution( ) \
		{ \
			ClassName::Pointer p = ClassName::New( ); \
			return p.GetPointer(); \
		} \
	};


#endif // _dynModuleExecutionImpl_H

