/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blGDFFileWriter.h"
#include "blGDFUtils.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

/**
*/
blGDFFileWriter::blGDFFileWriter()
{
}

/**
*/
blGDFFileWriter::~blGDFFileWriter() 
{
}

/**
*/
void blGDFFileWriter::Update()
{
	FILE* file;
	file = fopen( m_Filename.c_str(), "wb"); 
	if (ferror(file)) return;


	unsigned short NS = m_SignalInput->GetNumberOfSignals();
	unsigned short HeadLen =(NS + 1 + 1);//number of blocks per 256 bytes (channels + header1 + header2)

	//! TODO put this in the signal collective and not in blsignal
	long long int NumRec  = m_SignalInput->GetSignal(0)->GetNumberOfRecords();
	unsigned long Dur_0,Dur_1; 
	Dur_0 = m_SignalInput->GetSignal(0)->GetRecordDuration().numerator;
	Dur_1 = m_SignalInput->GetSignal(0)->GetRecordDuration().dominator;
	InternalTimeType time;
	time = l_endian_u64( m_SignalInput->GetSignal(0)->GetInternalStartTime( ) );

	//==============================================================
	//Create Header1
	//std::cout<< "Writing Header 1: "<<std::endl;

	unsigned char* version =(unsigned char*)("GDF 2.11");
    fwrite(version,1,8, file);
	//std::cout<< "version: "<< version <<std::endl;
	fseek(file,168, SEEK_SET);
	fwrite(&time,1,8, file);
	fseek(file,184, SEEK_SET);
	fwrite(&HeadLen,1,2, file);
	fseek(file,236, SEEK_SET);
	fwrite(&NumRec,1,8, file);
	fseek(file,244, SEEK_SET);
	fwrite(&Dur_0,1,4, file);
	fseek(file,248, SEEK_SET);
	fwrite(&Dur_1,1,4, file);
	fseek(file,252, SEEK_SET);
	fwrite(&NS,1,2, file);

	
	//==============================================================
	//Create Header2
	fseek(file,256, SEEK_SET);
	for (size_t k = 0; k< NS; k++)
	{
		unsigned long spr = (unsigned long)( m_SignalInput->GetSignal(k)->GetYVector().size()/NumRec);
		std::string label  = m_SignalInput->GetSignal(k)->GetName();
		unsigned short gdftype = blGDFTYPCode_float64;
		double phyMin = -32000;//(double)m_SignalInput->GetSignal(k)->GetMinYValue();
		double phyMax = (double)m_SignalInput->GetSignal(k)->GetMaxYValue();
		double		cal = 1;	// gain factor
		double		off = 0;	//bias
		double digmax = (phyMax-off)/cal;
		double digmin = (phyMin-off)/cal;
		uint16_t physDimCode = 4275;  // uV

		
		fseek(file,256+ 16*k , SEEK_SET);
		fwrite(label.c_str(), 1, 16, file);
		fseek(file,256+ 8*k + 102*NS, SEEK_SET);
		fwrite(&physDimCode,1,2, file);
		fseek(file,256+ 8*k + 104*NS, SEEK_SET);
		fwrite(&phyMin,1,8, file);
		fseek(file,256+ 8*k + 112*NS, SEEK_SET);
		fwrite(&phyMax,1,8, file);
		fseek(file,256+ 8*k + 120*NS, SEEK_SET);
		fwrite(&digmin,1,8, file);
		fseek(file,256+ 8*k + 128*NS, SEEK_SET);
		fwrite(&digmax,1,8, file);
		fseek(file,256+ 4*k + 216*NS, SEEK_SET);
		fwrite(&spr,1,4, file);
		fseek(file,256+ 4*k + 220*NS, SEEK_SET);
		fwrite(&gdftype,1,4, file);
	}

	//==============================================================
	//Create Header3
	fseek(file,256*(NS+1), SEEK_SET);
	
	// Write empty values
	unsigned char* header3= (unsigned char*) malloc(256) ;
	for (size_t k3 = 0; k3< 256; k3++)
		header3[k3]=NULL;
	fwrite(header3, 1, 256, file);
	free( header3 );

	// Get free event names
	blGDFTag tag;
	tag.m_Id = 1;
	tag.m_Length = 0;

	// list of null-terminated strings for user-specified description of event
	// codes; last entry is identified by an additional empty string (in other
	// words, the list is terminated by two consecutive zeros \0\0) .
	for (size_t k1 = 0; k1< NS; k1++)
	{
		blSignal::Pointer signal;
		signal = m_SignalInput->GetSignal( k1 );

		std::vector< blSignalAnnotation::Pointer > annotationVector;
		annotationVector = signal->GetAnnotationsVector( );
		for ( size_t k2 = 0 ; k2 < annotationVector.size() ; k2++ )
		{
			blSignalAnnotation::Pointer annotation;
			annotation = annotationVector[ k2 ];
			if ( annotation->GetType() == 5 )
			{
				if ( tag.m_Length + annotation->GetName().size() + 1 + 1 < 256 )
				{
					strcpy( &tag.m_Value[ tag.m_Length ], annotation->GetName().c_str() );
					tag.m_Length += annotation->GetName().size() + 1;
				}
				else
				{
					std::cerr << "Too much free annotations. Limit is 256 char." << std::endl;
				}
			}
		}
	}
	tag.m_Value[ tag.m_Length ] = 0;
	tag.m_Length++;


	fseek(file,256*(NS+1), SEEK_SET);
	fwrite(&tag.m_Id, 1, 1, file);
	fwrite(&tag.m_Length, sizeof( tag.m_Length ), 1, file);
	fwrite(&tag.m_Value, 1, tag.m_Length, file);



	//==============================================================
	//Create data record
	fseek(file,256*(NS+2), SEEK_SET);
	size_t evenTablePosition = 256*(NS+2);
	blGDFUtils::InitializeGDFTYPMap();
	const size_t bit = blGDFUtils::m_GDFTYPMap[ blGDFTYPCode_float64 ].m_Size;
	for (size_t k1 = 0; k1< NS; k1++)
	{
		unsigned long spr =m_SignalInput->GetSignal(k1)->GetYVector().size();
		std::vector<float> yvector = m_SignalInput->GetSignal(k1)->GetYVector();
		for (size_t k2 = 0; k2 < spr; k2++)
		{	
			//int16_t value = (int16_t) yvector.at(k2);
			double value = yvector.at(k2);
			fwrite(&value , 1, bit, file);
			evenTablePosition += bit;
		}

	}

	//==============================================================
	//Create Event Table
	std::vector<blGDFEvent> evtList;
	for (size_t k1 = 0; k1< NS; k1++)
	{
		blSignal::Pointer signal;
		signal = m_SignalInput->GetSignal( k1 );

		std::vector< blSignalAnnotation::Pointer > annotationVector;
		annotationVector = signal->GetAnnotationsVector( );
		for ( size_t k2 = 0 ; k2 < annotationVector.size() ; k2++ )
		{
			blSignalAnnotation::Pointer annotation;
			annotation = annotationVector[ k2 ];

			blGDFEvent event;
			event.type = annotation->GetType( );
			event.position = uint32_t( annotation->GetXPos( ) );
			event.channel = k1 + 1;
			event.duration = uint32_t( annotation->GetDuration( ) );
			evtList.push_back( event );
		}
	}

	//==============================================================
	// Write event table
	blGDFEventTable evenTable;
	evenTable.mode = 3;
	evenTable.numOfEvt = evtList.size();
	evenTable.sampleRate = m_SignalInput->GetEventSampleRate();

	fwrite( &evenTable.mode, 1, 1, file);
	unsigned char eventdata[3];
	eventdata[ 0 ] = (unsigned char)( evenTable.numOfEvt & 255 );
	eventdata[ 1 ] = (unsigned char)( ( evenTable.numOfEvt >> 8) & 255 );
	eventdata[ 2 ] = (unsigned char)( ( evenTable.numOfEvt >> 16) & 255 );
	fwrite( eventdata, 3, 1, file);
	fwrite( &evenTable.sampleRate, 1, 4, file);

	for ( unsigned i = 0 ; i < evtList.size() ; i++ )
	{
		fseek( file, evenTablePosition + 8 + i*4, SEEK_SET);
		fwrite( &evtList.at( i ).position, 1, sizeof( evtList.at( i ).position ), file );
		//fseek( file, evenTablePosition + 8 + evenTable.numOfEvt*4 + i*4, SEEK_SET);
		fseek( file, evenTablePosition + 8 + evenTable.numOfEvt*4 + i*2, SEEK_SET);
		fwrite( &evtList.at( i ).type, 1, sizeof( evtList.at( i ).type ), file );
		if ( evenTable.mode == 3 )
		{
			fseek( file, evenTablePosition + 8 + evenTable.numOfEvt*6 + i*2, SEEK_SET);
			fwrite( &evtList.at( i ).channel, 1, sizeof( evtList.at( i ).channel ), file );
			fseek( file, evenTablePosition + 8 + evenTable.numOfEvt*8 + i*4, SEEK_SET);
			fwrite( &evtList.at( i ).duration, 1, sizeof( evtList.at( i ).duration ), file );
		}
	}


	fclose(file);

}

/**
*/
void blGDFFileWriter::SetFilename( const char* filename)
{
	m_Filename = filename;
}

/**
*/
void blGDFFileWriter::SetInput( blSignalCollective::Pointer signal)
{
	m_SignalInput = signal;
}
