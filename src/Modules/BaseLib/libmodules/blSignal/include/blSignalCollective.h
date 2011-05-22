/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignalCollective_H
#define blSignalCollective_H

#include "BaseLibSignalWin32Header.h"
#include "blObject.h"
#include "blMacro.h"
#include "blSignal.h"
#include <vector>
#include "CILabExceptionMacros.h"

/**
This class enable grouping many signals for example for ECG

\author Jakub Lyko
\date 24 April 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalCollective : public blObject 
{
public:
	typedef blSignalCollective Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

public:
	//!
	std::string GetName() const;

	//!
	void SetName(const std::string& val);

	//!
	unsigned int GetNumberOfSignals() const;

	//!
	void SetNumberOfSignals(unsigned int val);

	//!
	blSignal::Pointer GetSignal(unsigned int pos);

	//!
	blSignal::Pointer GetSignal( const std::string &name);

	//! If nPos < size -> resize the vector
	void SetSignal(unsigned int nPos, blSignal::Pointer signal);

	//! 
	void AddSignal(blSignal::Pointer signal);

	//!
	void RemoveSignal (unsigned int pos);

	//!
	void RemoveAllSignals ();

	//!
	float GetEventSampleRate() const;

	//!
	void SetEventSampleRate(float val);

	//! Find the first annotation
	blSignalAnnotation::Pointer FindAnnotation( 
		unsigned short type,
		unsigned &signalPosition );

	//! To all channels
	void AddAnnotation( blSignalAnnotation::Pointer annot );

	//! To a single channel
	void AddAnnotation( unsigned channel, blSignalAnnotation::Pointer annot );

	//!
	void Copy( blSignalCollective::Pointer input );

protected:
	blSignalCollective( );

	~blSignalCollective( );

private:
	//!
	std::string m_name;
	
	//!
	std::vector< blSignal::Pointer > m_signals;

	//!
	float m_EventSampleRate;
};

#endif //blSignal_H
