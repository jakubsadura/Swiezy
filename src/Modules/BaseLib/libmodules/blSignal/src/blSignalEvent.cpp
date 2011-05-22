/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blSignalEvent.h"
#include "boost/filesystem.hpp"


#include <fstream>

const std::string GDFFilePathName = "resource/GDFEventCodes.txt";
const std::string CISTIBFilePathName = "resource/CISTIBEventCodes.txt";

std::vector<std::string> blSignalEventTable::m_FilePathVector;
std::map<unsigned int, blSignalEvent> blSignalEventTable::m_EventMap;
std::string blSignalEventTable::m_Folder; 
std::vector<blSignalEventGroup> blSignalEventTable::m_Groups;
std::map<unsigned int, bool> blSignalEventTable::m_EventMapSelected;

blSignalEvent blSignalEventTable::GetEvent( unsigned int type )
{
	LoadAllEventTables( );

	static std::map<unsigned int, blSignalEvent>::iterator it;
	it = m_EventMap.find( type );
	if ( it != m_EventMap.end( ) )
	{
		return it->second;
	}
	return blSignalEvent( );
}

bool blSignalEventTable::LoadEventTable( const std::string &filename )
{
	std::ifstream file;
	std::string path;
	if ( !m_Folder.empty() )
	{
		path = m_Folder + "//" + filename;
	}
	else
	{
		path = filename;
	}

	file.open( path.c_str() );
	if ( !file.good() )
	{
		return 0;
	}

	bool fileisOk = true;
	std::string line;
	while ( fileisOk )
	{
		fileisOk = std::getline(file,line) && fileisOk;
		if ( fileisOk )
		{
			if ( line[ 0 ] == '0' )
			{
				// Read code
				size_t k;
				for (k=6; isspace(line[k]); k++) {};

				// Read code
				int code; 
				sscanf(line.c_str(),"%x",&code);
				code &= 0xffff;

				// Read description
				const char *description = (line.c_str()+k);

				m_EventMap[ code ] = blSignalEvent( code, description );
			}
		}
	}
	return 1;
}

void blSignalEventTable::LoadAllEventTables()
{
	if ( m_EventMap.size() )
	{
		return;
	}

	m_FilePathVector.push_back( GDFFilePathName );
	m_FilePathVector.push_back( CISTIBFilePathName );

	for ( unsigned i = 0 ; i < m_FilePathVector.size() ; i++ )
	{
		LoadEventTable( m_FilePathVector[ i ] );
	}

	InitGroups( );

	SelectAll();
}

/**
*/
std::vector<blSignalEvent> blSignalEventTable::GetAllEvents()
{
	//
	LoadAllEventTables();
	//
	std::map<unsigned int, blSignalEvent>::iterator it = m_EventMap.begin();
	std::vector<blSignalEvent> allEvents;
	while( it != m_EventMap.end() )
	{
		allEvents.push_back(it->second);
		it++;
	}

	return allEvents;
}

void blSignalEventTable::SetDirectory( std::string folder )
{
	m_Folder = folder;
}

bool blSignalEventTable::HasDuration( unsigned int type )
{
	// ES or ED
	if ( type == 1 || type == 2 )
	{
		return false;
	}

	return true;
}

void blSignalEventTable::InitGroups()
{
	if ( !m_Groups.empty() )
	{
		return;
	}

	m_Groups.push_back( blSignalEventGroup( "0x00__", "user specific events" ) );
	m_Groups.push_back( blSignalEventGroup( "0x010_", "EEG artifacts" ) );
	m_Groups.push_back( blSignalEventGroup( "0x011_", "EEG patterns" ) );
	m_Groups.push_back( blSignalEventGroup( "0x012_", "Stimulus for Evoked potentials" ) );
	m_Groups.push_back( blSignalEventGroup( "0x013_", "Stimulus for Steady State Evoked potentials" ) );
	m_Groups.push_back( blSignalEventGroup( "0x014_", "Response" ) );
	m_Groups.push_back( blSignalEventGroup( "0x03__", "BCI: Trigger, cues, classlabels, " ) );
	m_Groups.push_back( blSignalEventGroup( "0x040_", "Respiratory Events" ) );
	m_Groups.push_back( blSignalEventGroup( "0x041_", "Sleep stages according to Rechtschaffen&Kales and AASM'07" ) );
	m_Groups.push_back( blSignalEventGroup( "0x042_", "Sleep" ) );
	m_Groups.push_back( blSignalEventGroup( "0x043_", "Eye movements" ) );
	m_Groups.push_back( blSignalEventGroup( "0x044_", "muscle activity (for checking on EEG artifacts)" ) );
	m_Groups.push_back( blSignalEventGroup( "0x050_", "ECG events" ) );
	m_Groups.push_back( blSignalEventGroup( "0x058_", "ergometric events" ) );
	m_Groups.push_back( blSignalEventGroup( "0x100_", "neural spikes" ) );
	m_Groups.push_back( blSignalEventGroup( "0x7f__", "special codes" ) );
			  
}

int blSignalEventTable::GetNumberOfGroups()
{
	return m_Groups.size( );
}

std::vector<blSignalEvent> blSignalEventTable::GetGroupEvents( int index )
{
	std::vector<blSignalEvent> events;

	EventMapType::iterator it;
	for ( it = m_EventMap.begin() ; it != m_EventMap.end(); it++ )
	{
		char buffer[14];
		sprintf( buffer, "0x%04x", it->second.m_Type );
		std::string code = buffer;

		
		size_t pos = code.find( m_Groups[ index ].GetCode() );
		if ( pos != std::string::npos )
		{
			events.push_back( it->second );
		}
	}

	return events;
}

blSignalEventGroup blSignalEventTable::GetGroup( int index )
{
	return m_Groups[ index ];
}

void blSignalEventTable::SelectAll()
{
	EventMapType::iterator it;
	for ( it = m_EventMap.begin() ; it != m_EventMap.end(); it++ )
	{
		SelectEvent( it->second.m_Type, true );
	}
}

void blSignalEventTable::SelectEvent( unsigned int type, bool select )
{
	m_EventMapSelected[ type ] = select;
}

void blSignalEventTable::SelectGroup( int index, bool select /*= true */ )
{
	std::vector<blSignalEvent> eventTable;
	eventTable = GetGroupEvents( index );
	for ( size_t i = 0 ; i < eventTable.size() ; i++ )
	{
		SelectEvent( eventTable[ i ].m_Type, select );
	}
}

bool blSignalEventTable::IsEventSelected( unsigned int type )
{
	return m_EventMapSelected[ type ];
}
