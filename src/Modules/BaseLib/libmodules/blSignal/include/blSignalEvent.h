/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blSignalEvent_H
#define _blSignalEvent_H

#include "BaseLibSignalWin32Header.h"
#include <map>
#include <string>
#include <vector>

/**
Signal event types and description

\author Xavi Planes
\date 05 August 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalEvent
{
public:
	//! Type
	unsigned int m_Type;
	//! Description
	std::string m_Description;

	blSignalEvent( unsigned int type = 0, std::string description = "" )
	{
		m_Type = type;
		m_Description = description;
	}
};

/**
Group of signal events

\author Xavi Planes
\date 22 Jan 2010
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalEventGroup
{
public:
	//!
	blSignalEventGroup( std::string code ="", std::string description ="")
	{
		m_Code = code;
		m_Description = description;
	};

	//!
	std::string GetCode( )
	{
		size_t pos = m_Code.find( '_' );
		return m_Code.substr( 0, pos );
	}

public:
	//! Code like "0x010_". Underscore means any digit
	std::string m_Code;

	//! "EEG artifacts"
	std::string m_Description;
};


/**
Signal event table

\author Xavi Planes
\date 05 August 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalEventTable
{
public:
	//!
	typedef std::map<unsigned int, blSignalEvent> EventMapType;

	//!
	static std::vector<blSignalEvent> GetAllEvents();
	//!
	static blSignalEvent GetEvent( unsigned int type );
	//!
	static void SetDirectory(std::string folder);
	//!
	static bool HasDuration( unsigned int type );

	//! Grouped events
	static int GetNumberOfGroups( );

	//! Grouped events
	static blSignalEventGroup GetGroup( int index );

	//! Grouped events
	static std::vector<blSignalEvent> GetGroupEvents( int index );

	//! Load event table from filename
	static void LoadAllEventTables( );

	//!
	static void SelectAll( );

	//!
	static void SelectEvent( unsigned int type, bool select = true );

	//!
	static bool IsEventSelected( unsigned int type );

	//!
	static void SelectGroup( int index, bool select = true );

protected:

	//! Load event table from filename. Return 1 if OK, 0 otherwise
	static bool LoadEventTable( const std::string &filename );

	//!
	static void InitGroups( );

private:
	//!
	static std::vector<std::string> m_FilePathVector;

	//!
	static std::map<unsigned int, blSignalEvent> m_EventMap;

	//!
	static std::map<unsigned int, bool> m_EventMapSelected;

	//! 
	static std::string m_Folder;

	//! 
	static std::vector<blSignalEventGroup> m_Groups;
};

#endif //_blSignalEvent_H
