/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreCreateExceptionMacros_H
#define coreCreateExceptionMacros_H

#ifdef coreCreateRuntimeExceptionClassMacro
#undef coreCreateRuntimeExceptionClassMacro
#endif

/**
\def coreCreateRuntimeExceptionClassMacro(classname, description, exportdef)
coreCreateRuntimeExceptionClassMacro is a macro for creating a Runtime 
Exception class in the Core::Exception namespace,
that behaves as a Core Exception.
\ingroup gmCommonObjects

\param classname Give a name for the exception class. 'classname' cannot 
be a variable, it is expecting a string without " "
\param description Brief description of the problem found
\param exportdef a export declaration must be given (like PLUGIN_EXPORT) 
to properly create the class with the proper export definition
*/
#define coreCreateRuntimeExceptionClassMacro(classname, description, exportdef) \
	namespace Core \
	{ \
	namespace Exceptions \
	{ \
	class exportdef classname : public RuntimeException	\
	{ \
	public: coreClassNameMacro(classname); \
			classname(const char* callingFunction) \
				: RuntimeException(callingFunction, description) \
				{ this->m_className = GetNameOfClass(); }; \
	};	\
	} \
	}


#ifdef coreCreateCriticalExceptionClassMacro
#undef coreCreateCriticalExceptionClassMacro
#endif
/**
\def coreCreateCriticalExceptionClassMacro(classname, description, exportdef)
coreCreateCriticalExceptionClassMacro is a macro for creating a Critical 
Exception class in the Core::Exception namespace,
that behaves as a Core Exception, forcing the Core to kill the application 
when catching it.
\ingroup gmCommonObjects

\param classname Give a name for the exception class. 'classname' cannot 
be a variable, it is expecting a string without " "
\param description Brief description of the problem found
\param exportdef a export declaration must be given (like PLUGIN_EXPORT) 
to properly create the class with the proper export definition
*/
#define coreCreateCriticalExceptionClassMacro(classname, description, exportdef) \
	namespace Core \
	{ \
	namespace Exceptions \
	{ \
	class exportdef classname : public CriticalException	\
	{ \
	public: coreClassNameMacro(classname); \
			classname(const char* callingFunction) \
				: CriticalException(callingFunction, description) \
				{ this->m_className = GetNameOfClass(); }; \
	};	\
	} \
	}



#ifdef coreCreateCoreRuntimeExceptionClassMacro
#undef coreCreateCoreRuntimeExceptionClassMacro
#endif
/**
\def coreCreateCoreRuntimeExceptionClassMacro(classname, description)
coreCreateRuntimeExceptionClassMacro is a macro that behaves essentially 
as coreCreateCriticalExceptionClassMacro,
using GMCOMMONOBJECTS_EXPORT as export definition, thus for creating 
Core::Exceptions within the Core library
\ingroup gmCommonObjects
*/
#define coreCreateCoreRuntimeExceptionClassMacro(classname, description) \
	coreCreateRuntimeExceptionClassMacro(classname, description, GMCOMMONOBJECTS_EXPORT)



#ifdef coreCreateCoreCriticalExceptionClassMacro
#undef coreCreateCoreCriticalExceptionClassMacro
#endif
/**
\def coreCreateCoreCriticalExceptionClassMacro(classname, description)
coreCreateRuntimeExceptionClassMacro is a macro that behaves essentially 
as coreCreateCriticalExceptionClassMacro,
using GMCOMMONOBJECTS_EXPORT as export definition, thus for creating 
Core::Exceptions within the Core library
\ingroup CoreCommonObjects
*/
#define coreCreateCoreCriticalExceptionClassMacro(classname, description) \
	coreCreateCriticalExceptionClassMacro(classname, description, GMCOMMONOBJECTS_EXPORT)



#ifdef coreCreatePluginRuntimeExceptionClassMacro
#undef coreCreatePluginRuntimeExceptionClassMacro
#endif
/**
\def coreCreatePluginRuntimeExceptionClassMacro(classname, description)
coreCreateRuntimeExceptionClassMacro is a macro that behaves essentially 
as coreCreateCriticalExceptionClassMacro,
using PLUGIN_EXPORT as export definition, thus for creating Core::Exceptions 
within a plugin library
\ingroup CoreCommonObjects
*/
#define coreCreatePluginRuntimeExceptionClassMacro(classname, description) \
	coreCreateRuntimeExceptionClassMacro(classname, description, PLUGIN_EXPORT)



#ifdef coreCreatePluginCriticalExceptionClassMacro
#undef coreCreatePluginCriticalExceptionClassMacro
#endif
/**
\def coreCreateCoreCriticalExceptionClassMacro(classname, description)
coreCreateRuntimeExceptionClassMacro is a macro that behaves essentially 
as coreCreateCriticalExceptionClassMacro,
using PLUGIN_EXPORT as export definition, thus for creating Core::Exceptions 
within a plugin library
\ingroup CoreCommonObjects
*/
#define coreCreatePluginCriticalExceptionClassMacro(classname, description) \
	coreCreateCriticalExceptionClassMacro(classname, description, PLUGIN_EXPORT)



#endif // coreCreateExceptionMacros_H
