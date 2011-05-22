/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blGDFFileReader.h"
#include "blGDFUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <string.h>

using namespace std;

/**
*/
blGDFFileReader::blGDFFileReader()
{
	this->m_signalCollective = blSignalCollective::New();
	m_file = NULL;
	m_lSize = 0;
	m_CurrentFilePosition = 0;
}

/**
*/
blGDFFileReader::~blGDFFileReader() 
{
}

/**
*/
void blGDFFileReader::SetFilename( const char* filename )
{
	m_Filename = filename;
}

/**
*/
void blGDFFileReader::Update( )
{
	m_file = fopen(m_Filename.c_str(),"rb");
	fseek (m_file , 0 , SEEK_END);
	m_lSize = ftell (m_file);
	rewind (m_file);

	if (ferror(m_file)) 
	{
		printf ("Error Reading file \n");
		return;
	}

	//==============================================================
	//  Read Header 1 
	ReadHeader1();

	//==============================================================
	// Read Header 2 
	ReadHeader2();

	//==============================================================
	// Read data
	fseek(m_file, m_fileHeader.HeadLen, SEEK_SET);
	m_signalCollective = blGDFUtils::ReadDataRecord( m_file, m_CurrentFilePosition, m_fileHeader, m_chList );

	//==============================================================
	//Read Event Table
	ReadEventTable();

	//==============================================================
	//Read Free Event Names
	ReadFreeEventNames();

	fclose(m_file);
}

/**
*/
void blGDFFileReader::ReadHeader2()
{
	unsigned char* header2;
	header2 =(unsigned char*)malloc( m_fileHeader.NS * 256 );
	fread(header2, m_fileHeader.NS, 256, m_file);
	ftell(m_file);

	m_chList.resize(m_fileHeader.NS);
	m_signalCollective->SetNumberOfSignals(m_fileHeader.NS);
	size_t k;
	for (k=0; k<m_fileHeader.NS; k++)
	{
		//chList.at(k).label = header2 +(16*k );
		memcpy(m_chList.at(k).label, header2 +(16*k ), 16);
		std::cout<< " Channel " << m_chList.at(k).label <<":"<<std::endl;
		memcpy( m_chList.at(k).phydim, header2 +(6*k) +96*m_fileHeader.NS, 6);
		m_chList.at(k).phydimnew = leu16p(header2 +8*k + 102*m_fileHeader.NS);
		m_chList.at(k).PhysMin = lef64p(header2 + 8*k +104*m_fileHeader.NS);
		m_chList.at(k).PhysMax = lef64p(header2+ 8*k + 112*m_fileHeader.NS);
		m_chList.at(k).digMin = lef64p(header2 + 8*k +120*m_fileHeader.NS);
		m_chList.at(k).digMax = lef64p(header2 + 8*k +128*m_fileHeader.NS);
		m_chList.at(k).SPR = leu32p(header2+ 4*k + 216*m_fileHeader.NS);
		m_chList.at(k).GDFTYP  = leu16p(header2+ 4*k + 220*m_fileHeader.NS);
	}

	free( header2 );
}

/**
*/
void blGDFFileReader::ReadFreeEventNames( )
{
	fseek(m_file, 256*(m_fileHeader.NS+1), SEEK_SET);
	blGDFTag tag;
	fread( &tag.m_Id, 1, 1, m_file );
	char buffer[ sizeof( tag.m_Length ) ];
	fread( buffer, sizeof( buffer ), 1, m_file );
	tag.m_Length = leu32p( buffer );
	size_t maxLength = std::min( tag.m_Length, uint32_t(256-3) );
	fread( &tag.m_Value, 1, maxLength, m_file );

	// list of null-terminated strings for user-specified description of event
	// codes; last entry is identified by an additional empty string (in other
	// words, the list is terminated by two consecutive zeros \0\0) .
	size_t index = 0;
	for (size_t k1 = 0; k1< m_fileHeader.NS; k1++)
	{
		blSignal::Pointer signal;
		signal = m_signalCollective->GetSignal( k1 );

		std::vector< blSignalAnnotation::Pointer > annotationVector;
		annotationVector = signal->GetAnnotationsVector( );
		for ( size_t k2 = 0 ; k2 < annotationVector.size() ; k2++ )
		{
			blSignalAnnotation::Pointer annotation;
			annotation = annotationVector[ k2 ];
			if ( annotation->GetType() == 5 )
			{
				annotation->SetFreeName( &tag.m_Value[ index ] );
				index += annotation->GetName().size() + 1;
			}
		}
	}
}


/**
*/
void blGDFFileReader::ReadEventTable( )
{
	int64_t ETP = 
		m_fileHeader.HeadLen + 
		m_fileHeader.numRec * m_chList.at(0).SPR *  
		sizeof(unsigned short)*m_fileHeader.NS;

	if ( ETP>= m_lSize) return;
	
	unsigned long maxLength = m_lSize - m_CurrentFilePosition;
	unsigned char* eventdata = (unsigned char*)malloc(maxLength);
	size_t obj = fread(eventdata,1,maxLength, m_file);

	blGDFEventTable evenTable;
	evenTable.mode = *eventdata;
	evenTable.numOfEvt = eventdata[1] + (eventdata[2] << 8) + (eventdata[3] << 16);
	evenTable.sampleRate = lef32p( (eventdata +4) );
	m_signalCollective->SetEventSampleRate( evenTable.sampleRate );

	std::vector<blGDFEvent> evtList( evenTable.numOfEvt );
	
	for (size_t ie = 0; ie < evenTable.numOfEvt; ie++)
	{
		evtList.at(ie).position = leu32p(eventdata + 8 + ie*4);
		evtList.at(ie).type = leu16p( eventdata + 8 + 4*evenTable.numOfEvt + 2*ie);
		if (evenTable.mode == 3)
		{
			evtList.at(ie).channel = leu16p( eventdata + 8 + 6*evenTable.numOfEvt + 2*ie);
			evtList.at(ie).duration = leu32p(eventdata+ 8 +evenTable.numOfEvt*8 + 4*ie);
		}
	}

	free( eventdata );

	// Put data to blSignal
	for (size_t ie = 0; ie < evtList.size(); ie++)
	{
		blGDFEvent event = evtList.at(ie);
		
		unsigned channel = event.channel - 1;
		if ( channel < m_signalCollective->GetNumberOfSignals() )
		{
			blSignal::Pointer signal;
			signal = m_signalCollective->GetSignal( event.channel - 1 );

			blSignalAnnotation::Pointer annotation = blSignalAnnotation::New( );
			annotation->SetType( event.type );
			annotation->SetXPos( event.position );
			annotation->SetChannel( event.channel - 1 );
			annotation->SetDuration( event.duration );
			annotation->SetValueY( signal->GetValueY( annotation->GetXPos( ) ) );
			signal->AddAnnotation( annotation );
		}
	}

}

/**
*/
blSignalCollective::Pointer blGDFFileReader::GetOutput()
{
	return m_signalCollective;
}

void blGDFFileReader::ReadHeader1()
{
	// Head lenght, version?, number of data records? NS?
	unsigned char* Header1;
	Header1 =(unsigned char*)malloc(256);
	size_t bytesRead = fread(Header1, 1, 256, m_file);
	ftell(m_file);

	if ( bytesRead < 256 )
	{
		throw std::runtime_error( "File format is not GDF" );
	}

	memcpy( m_fileHeader.version ,Header1, 8);
	if ( memcmp( m_fileHeader.version, "GDF", 3 ) )
	{
		throw std::runtime_error( "File format is not GDF" );
	}
	char tmp[ 6 ];
	strncpy(tmp,(char*)Header1+3,5); tmp[5]=0;
	m_fileHeader.VERSION = float( strtod( tmp, NULL ) );
	if ( m_fileHeader.VERSION < 2.0 )
	{
		throw std::runtime_error( "GDF version is < 2.0" );
	}
	memcpy( m_fileHeader.PatientName ,Header1 + 8, 66);
	m_fileHeader.HeadLen = leu16p(Header1+184)<<8;
	m_fileHeader.NS = leu16p(Header1+252);
	m_fileHeader.Startdate = lei64p( Header1+168 );
	m_fileHeader.Dur[ 0 ] = leu32p(Header1+244);
	m_fileHeader.Dur[ 1 ] = leu32p(Header1+248);
	m_fileHeader.numRec = lei64p(Header1+236);
	// default: most data formats are little endian 
	m_fileHeader.LittleEndian = 1;

	free( Header1 );
}
