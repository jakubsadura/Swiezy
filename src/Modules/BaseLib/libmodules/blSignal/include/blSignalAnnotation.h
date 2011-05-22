/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignalAnnotation_H
#define blSignalAnnotation_H

#include "BaseLibSignalWin32Header.h"
#include "blObject.h"
#include "blMacro.h"
#include <vector>
#include "CILabExceptionMacros.h"

/**
Signal annotation 

\author Xavi Planes
\date 24 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalAnnotation : public blObject 
{
public:
	typedef blSignalAnnotation Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	void Copy( blSignalAnnotation::Pointer input );

	//! Get name from blSignalEventTable
	std::string GetName();

	//! Set a free annotation (type = 0)
	void SetFreeName( const std::string &name );

	//! Return position in blSignal (position in m_xVector)
	std::size_t GetXPos() const;
	void SetXPos(std::size_t val);

	//! Return the value of m_yVector at m_XPos
	float GetValueY() const;
	void SetValueY(float val);

	//! Return duration in position of m_xVector
	std::size_t GetDuration() const;
	void SetDuration(std::size_t val);

	//!
	unsigned short GetType() const;
	void SetType(unsigned short val);

	//! If this is not set, return 0
	unsigned short GetChannel() const;
	void SetChannel(unsigned short val);

protected:
	blSignalAnnotation( );

	~blSignalAnnotation( );

private:

	//! Position in blSignal (position in m_xVector)
	std::size_t m_XPos;

	//! Value of m_yVector at m_XPos
	float m_ValueY;

	//! Duration in position of m_xVector
	std::size_t m_Duration;

	//!
	unsigned short m_Type;

	//! Sometimes this is not set
	unsigned short m_Channel;

	//!
	std::string	 m_Name;
};

#endif //blSignal_H
