/*=========================================================================

  Module    : Core
  File      : $RCSfile: $
  Copyright : (C)opyright CILAB
  Authors   : Juan Antonio Moya
  Modified  : $Author: $
  Purpose   : SWIG interface for wrapping core classes
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
			  $Log: $

=========================================================================*/
#ifdef WIN32
	#include <windows.i>
#endif

#ifndef SWIG_WRAPPING
#define SWIG_WRAPPING
#endif

%module Core

%{
	#include "coreKernel.h"
%}

%include "coreKernel.h"

#undef SWIG_WRAPPING

