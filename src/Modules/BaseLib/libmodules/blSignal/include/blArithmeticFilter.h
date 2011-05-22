/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blArithmeticFilter_H
#define _blArithmeticFilter_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"

#include <vector>
#include <list>

enum BLSIGNAL_OPERATION
{
	BLSIGNAL_SUBSTRACT,
	BLSIGNAL_ADD,
	BLSIGNAL_DIVIDE_K
};

/**
blSignal arithmetic operations:
- Substract

\author Xavi Planes
\date 22 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blArithmeticFilter : public blLightObject 
{
public:
	typedef blArithmeticFilter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	void SetOperation( BLSIGNAL_OPERATION op ){ m_Operation = op;}

	//!
	bool GetCopyAnnotations() const;
	void SetCopyAnnotations(bool val);

	//!
	void SetInput1( blSignal::Pointer signal );

	//!
	void SetInput2( blSignal::Pointer signal );

	//!
	void SetOutput( blSignal::Pointer signal );

	//!
	void SetK( double k );

	//!
	void Update( );

	//!
	blSignal::Pointer GetOutput( );

protected:
	//!
	blArithmeticFilter( );

	//!
	~blArithmeticFilter( );

	//!
	void ComputeSubstract( );

	//!
	void ComputeAdd( );

	//!
	void ComputeDivideK( );

private:

	//!
	blSignal::Pointer m_Input1;

	//!
	blSignal::Pointer m_Input2;

	//!
	blSignal::Pointer m_Output;

	//!
	BLSIGNAL_OPERATION m_Operation;

	//!
	bool m_copyAnnotations;

	//!
	double m_K;
};

#endif //blArithmeticFilter_H
