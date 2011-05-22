
#include "blBVFileReader.h"
#include "blTextUtils.h"

#include <iostream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <sys/stat.h>
#include <float.h>

#include "itksys/SystemTools.hxx"

#define FILESEP '\\'
#define VERBOSE_LEVEL 9
#define MAX_LENGTH_LABEL 40


blBVFileReader::blBVFileReader()
{
	this->m_signalCollective = blSignalCollective::New();
	m_file = NULL;
	m_lSize = 0;
	m_CurrentFilePosition = 0;
	m_FileHeader.Header = NULL;
	m_SubSamplingFactor = 1;
}
blBVFileReader::~blBVFileReader() 
{
	if ( m_FileHeader.Header != NULL )
	{
		free( m_FileHeader.Header );
	}
}

/**
*/
void blBVFileReader::SetFilename( const char* filename )
{
	m_Filename = filename;
}

/**
*/
void blBVFileReader::Update( )
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
	//  Read Header
	ReadFileHeader();

	//==============================================================
	// Decode Header 
	DecodeHeader();

	//==============================================================
	// Read data
	ReadDataRecord( );

	//==============================================================
	//Read Event Table
	ReadEventTable();

	free( m_FileHeader.Header );
	m_FileHeader.Header = NULL;

	fclose(m_file);
	m_file = NULL;

	std::cout << "Finished reading..." << std::endl;

}

/**
*/

void blBVFileReader::ReadFileHeader(){

/* open and read header file */ 
		// ifclose(hdr);
		const char *filename = m_Filename.c_str( ); // keep input file name 
		char* tmpfile = (char*)calloc(strlen(m_Filename.c_str( ))+5,1);		
		strcpy(tmpfile,m_Filename.c_str( ));
		m_FileHeader.FileName = tmpfile;
		m_FileHeader.ext = strrchr(m_FileHeader.FileName,'.')+1; 


		long count = 0;
		m_FileHeader.Header = (uint8_t*) calloc( 1, 1 );
		while (!feof(m_file)) {
			size_t bufsiz = 4096;
		    m_FileHeader.Header = (uint8_t*)realloc(m_FileHeader.Header,count+bufsiz+1);
		    count  += fread(m_FileHeader.Header+count,1,bufsiz,m_file);
		}
		m_FileHeader.Header[count]=0;
		m_FileHeader.HeadLen = count; 

		fprintf(stdout,"SOPEN(BV): header file read.\n"); 

		int seq = 0;
}

void blBVFileReader::DecodeHeader( ) 
{
	char *t;
	int currentSeq = 0;


	size_t pos;
	// skip first line with <CR><LF>
	const char EOL[] = "\r\n";		
	pos  = strcspn((char*)m_FileHeader.Header,EOL);
	pos += strspn((char*)m_FileHeader.Header+pos,EOL);
	while (pos < m_FileHeader.HeadLen ) {

		// start of line 
		t    = (char*) (m_FileHeader.Header+pos);
		pos += strcspn(t,EOL);
		m_FileHeader.Header[pos] = 0;	// line terminator
		pos += strspn((char*)m_FileHeader.Header+pos+1,EOL)+1; // skip <CR><LF> 

		// Parse lines
		if ( strncmp(t,";",1) == 0 ) 	// comments
		{
		}
		else if ( strncmp(t,"[Common Infos]",14) == 0 )
			currentSeq = 1;
		else if ( strncmp(t,"[Binary Infos]",14) == 0 )
			currentSeq = 2;
		else if ( strncmp(t,"[ASCII Infos]",13) == 0 )
			 currentSeq = 2;
		else if ( strncmp(t,"[Channel Infos]",14) == 0 )
			currentSeq = 3;
		else if (!strncmp(t,"[Coordinates]",13))
			currentSeq = 5; 
		else if (!strncmp(t,"[Comment]",9))
			currentSeq = 6; 
		else if (!strncmp(t,"[",1))
			currentSeq = 9; 
		else
		{
			switch( currentSeq )
			{
			case 1: ReadSeq1( t );break;
			case 2: 
				ReadSeq2( t );
				InitChannels( );
				break;
			case 3: ReadSeq3( t );break;
			}
		}


	}

}


/**
*/
blSignalCollective::Pointer blBVFileReader::GetOutput()
{
	return m_signalCollective;
}

void blBVFileReader::ReadSeq1( char *t ){

	if      (!strncmp(t,"DataFile=",9)) 
		strcpy(m_FileHeader.DataFileName,strrchr(t,'=')+1);		
	else if (!strncmp(t,"MarkerFile=",11)) {

		//char* mrkfile = (char*)calloc(strlen(m_FileHeader.FileName)+strlen(t),1);
		//		
		//if (strrchr(m_FileHeader.FileName,FILESEP)) {
		//	strcpy(mrkfile,m_FileHeader.FileName);
		//	strcpy(strrchr(mrkfile,FILESEP)+1,t+11);
		//} else  
		//	strcpy(mrkfile,t+11);
		//	
		//if (VERBOSE_LEVEL>8)
		//	fprintf(stdout,"SOPEN marker file <%s>.\n",mrkfile); 

		//HDRTYPE *hdr2 = sopen(mrkfile,"r",NULL);

		//memcpy(&hdr->EVENT,&hdr2->EVENT,sizeof(hdr2->EVENT));
		//hdr->AS.auxBUF = hdr2->AS.auxBUF;  // contains the free text annotation 
		//// do not de-allocate event table when hdr2 is deconstructed 
		//memset(&hdr2->EVENT,0,sizeof(hdr2->EVENT));
		//hdr2->AS.auxBUF = NULL;  
		//sclose(hdr2); 
		//destructHDR(hdr2);
		//free(mrkfile);
		//B4C_ERRNUM = 0; // reset error status - missing or incorrect marker file is not critical
	}
	else if (!strncmp(t,"DataFormat=BINARY",11))
		;
	else if (!strncmp(t,"DataFormat=ASCII",16)) {
		m_FileHeader.FLAG_ASCII = 1; 
		//gdftyp     = 17;
//					B4C_ERRNUM = B4C_DATATYPE_UNSUPPORTED;
//					B4C_ERRMSG = "Error SOPEN(BrainVision): ASCII-format not supported (yet).";
	}	
	else if (!strncmp(t,"DataOrientation=VECTORIZED",25))
		m_FileHeader.orientation = blBVHeader1::VEC;
	else if (!strncmp(t,"DataOrientation=MULTIPLEXED",26))
		m_FileHeader.orientation = blBVHeader1::MUL;
	else if (!strncmp(t,"DataType=TIMEDOMAIN",19))
		;
	else if (!strncmp(t,"DataType=",9)) {
		//B4C_ERRNUM = B4C_DATATYPE_UNSUPPORTED;
		//B4C_ERRMSG = "Error SOPEN(BrainVision): DataType is not TIMEDOMAIN";
	}	
	else if (!strncmp(t,"NumberOfChannels=",17)) {
		m_FileHeader.NS = atoi(t+17);
		m_signalCollective->SetNumberOfSignals( m_FileHeader.NS );
	}	
	else if (!strncmp(t,"DataPoints=",11)) {
		m_FileHeader.dataPoints = atol(t+11);
	}	
	else if (!strncmp(t,"SamplingInterval=",17)) {
		m_FileHeader.SampleRate = 1e6/atof(t+17);
		//hdr->EVENT.SampleRate = hdr->SampleRate;
	}	
}

void blBVFileReader::ReadSeq2( char *t ){


	if (!strncmp(t,"BinaryFormat=IEEE_FLOAT_32",26)) {
		m_gdftyp = 16;
		m_FileHeader.digmax =  FLT_MAX;
		m_FileHeader.digmin =  FLT_MIN;
	}	
	else if (!strncmp(t,"BinaryFormat=INT_16",19)) {
		m_gdftyp =  3;
		m_FileHeader.digmax =  32767;
		m_FileHeader.digmin = -32768;
		m_FileHeader.OVERFLOWDETECTION = 1;
	}	
	else if (!strncmp(t,"BinaryFormat=UINT_16",20)) {
		m_gdftyp = 4;
		m_FileHeader.digmax = 65535;
		m_FileHeader.digmin = 0;
		m_FileHeader.OVERFLOWDETECTION = 1;
	}
	else if (!strncmp(t,"BinaryFormat",12)) {
		/*B4C_ERRNUM = B4C_DATATYPE_UNSUPPORTED;
		B4C_ERRMSG = "Error SOPEN(BrainVision): BinaryFormat=<unknown>";*/
	}
	//else if (!strncmp(t,"UseBigEndianOrder=NO",20)) {
	//	// hdr->FLAG.SWAP = (__BYTE_ORDER == __BIG_ENDIAN); 
	//	hdr->FILE.LittleEndian = 1; 
	//}	
	//else if (!strncmp(t,"UseBigEndianOrder=YES",21)) {
	//	// hdr->FLAG.SWAP = (__BYTE_ORDER == __LITTLE_ENDIAN); 
	//	hdr->FILE.LittleEndian = 0; 
	//}	
	//else if (!strncmp(t,"DecimalSymbol=",14)) {
	//	DECIMALSYMBOL = t[14];
	//}	
	//else if (!strncmp(t,"SkipLines=",10)) {
	//	SKIPLINES = atoi(t+10);
	//}	
	//else if (!strncmp(t,"SkipColumns=",12)) {
	//	SKIPCOLUMNS = atoi(t+12);
	//}
	/*else if (0) {
		B4C_ERRNUM = B4C_DATATYPE_UNSUPPORTED;
		B4C_ERRMSG = "Error SOPEN(BrainVision): BinaryFormat=<unknown>";
	
	}	*/
}

void blBVFileReader::ReadSeq3( char *t ){

	//if (VERBOSE_LEVEL==9)
	//	fprintf(stdout,"BVA: seq=%i,line=<%s>,ERR=%i\n",3,t,B4C_ERRNUM );

	if (!strncmp(t,"Ch",2)) {
		char* ptr;

		if (VERBOSE_LEVEL==9) fprintf(stdout,"%s\n",t);			

		int n = strtoul(t+2, &ptr, 10)-1;
		if ((n < 0) || (n >= m_FileHeader.NS)) {
			std::cerr << "Error SOPEN(BrainVision): invalid channel number";
			return;
		}

		// Name of channel
		std::list<std::string> words;
		std::string line( ptr+1 );
		blTextUtils::ParseLine( line, ',' ,words );

		//size_t len = std::min(strcspn(ptr+1,","),size_t(MAX_LENGTH_LABEL));
		//char label[MAX_LENGTH_LABEL];
		//strncpy(label,ptr+1,len);
		//label[len]=0;

		// Channel name
		if ( words.size() > 0 )
		{
			strncpy( (char*) m_chList.at(n).label, words.front( ).c_str( ), 16 );
			words.pop_front( );
		}

		// Reference channel name
		if ( words.size() > 0 )
		{
			words.pop_front( );
		}

		// Resolution in "Unit"
		if ( words.size() > 0 )
		{
			words.pop_front( );
		}

		// Unit
		if ( words.size() > 0 )
		{
			m_chList.at(n).units = words.front();
			words.pop_front( );
		}

		//// Read DigMax and DigMin
		//ptr += len+2;
		//ptr += strcspn(ptr,",")+1;
		//if (strlen(ptr)>0) {
		//	double tmp = atof(ptr);
		//	// Use global cal
		//	m_FileHeader.cal = tmp;
		//	//if (tmp) hdr->CHANNEL[n].Cal = tmp; 
		//	//hdr->CHANNEL[n].PhysMax = hdr->CHANNEL[n].DigMax * hdr->CHANNEL[n].Cal ;
		//	//hdr->CHANNEL[n].PhysMin = hdr->CHANNEL[n].DigMin * hdr->CHANNEL[n].Cal ;
		//}	

		//if (VERBOSE_LEVEL==9) 
		//	fprintf(stdout,"Ch%02i=%s,,%s(%f)\n",n,label,ptr,m_FileHeader.cal );			
	}	
}

void blBVFileReader::InitChannels()
{

	// bpb
	blGDFUtils::InitializeGDFTYPMap();
	m_FileHeader.bpb = m_FileHeader.NS*blGDFUtils::m_GDFTYPMap[ m_gdftyp ].m_Size;
	//m_FileHeader.bpb+= ( 4 );			

	//if (VERBOSE_LEVEL==9) fprintf(stdout,"BVA210,%i,%i\n",pos,hdr->HeadLen);

	/* open data file */ 
	FILE *fDataFile;
	std::string filenameDataFile;
	filenameDataFile = itksys::SystemTools::GetFilenamePath( m_FileHeader.FileName ) + 
		"/" + m_FileHeader.DataFileName;

	if (m_FileHeader.FLAG_ASCII) fDataFile = fopen(filenameDataFile.c_str( ),"rt");
	else 	        fDataFile = fopen(filenameDataFile.c_str( ),"rb");
		
	//if (VERBOSE_LEVEL==9) fprintf(stdout,"BVA210,%i,%i\n",pos,hdr->HeadLen);

	size_t npts=0;
	if (!npts) {
		struct stat FileBuf;
		stat(filenameDataFile.c_str( ),&FileBuf);
		npts = FileBuf.st_size/m_FileHeader.bpb;
	}	


	//if (VERBOSE_LEVEL==9) fprintf(stdout,"BVA210,%i,%i,ERR=%i\n",pos,hdr->HeadLen,B4C_ERRNUM);

	if (m_FileHeader.orientation == blBVHeader1::VEC) {
		m_FileHeader.SPR = npts;
		m_FileHeader.numRec= 1;
		m_FileHeader.Dur[ 0 ] = npts;
		m_FileHeader.Dur[ 1 ] = m_FileHeader.SampleRate;
	} else {
		m_FileHeader.SPR = 1;
		m_FileHeader.numRec= npts;
		m_FileHeader.Dur[ 0 ] = 1;
		m_FileHeader.Dur[ 1 ] = m_FileHeader.SampleRate;
	}

	//if (VERBOSE_LEVEL==9) fprintf(stdout,"BVA210,%i,%i\n",pos,hdr->HeadLen);

	m_chList.resize(m_FileHeader.NS);
	m_signalCollective->SetNumberOfSignals(m_FileHeader.NS);
	size_t k;
	for (k=0; k<m_FileHeader.NS; k++)
	{
		m_chList.at(k).label[ 0 ] = 0;
		m_chList.at(k).GDFTYP = m_gdftyp;
		m_chList.at(k).SPR = m_FileHeader.SPR;
		m_chList.at(k).PhysMax = m_FileHeader.physmax;
		m_chList.at(k).PhysMin = m_FileHeader.physmin;
		m_chList.at(k).digMax = m_FileHeader.digmax;
		m_chList.at(k).digMin = m_FileHeader.digmin;
	}


}

void blBVFileReader::ReadDataRecord()
{
	FILE *fDataFile;
	std::string filenameDataFile;
	filenameDataFile = itksys::SystemTools::GetFilenamePath( m_FileHeader.FileName ) + 
		"/" + m_FileHeader.DataFileName;

	if (m_FileHeader.FLAG_ASCII) fDataFile = fopen(filenameDataFile.c_str( ),"rt");
	else 	        fDataFile = fopen(filenameDataFile.c_str( ),"rb");

	m_FileHeader.ROW_BASED_CHANNELS = 1;
	m_signalCollective = blGDFUtils::ReadDataRecord( 
		fDataFile, 
		m_CurrentFilePosition, 
		m_FileHeader, 
		m_chList,
		m_SubSamplingFactor );

	fclose( fDataFile );

	for ( unsigned i = 0 ; i < m_signalCollective->GetNumberOfSignals() ; i++ )
	{
		m_signalCollective->GetSignal( i )->SetYUnit( m_chList.at(i).units );
		m_signalCollective->GetSignal( i )->SetXUnit( "seg." );
	}

}

void blBVFileReader::ReadEventTable( )
{
	std::string filenameMarkerFile;
	filenameMarkerFile = itksys::SystemTools::GetFilenamePath( m_FileHeader.FileName ) + 
		"/" + itksys::SystemTools::GetFilenameWithoutLastExtension(m_FileHeader.FileName) +".vmrk";

	std::ifstream signalFile;
	signalFile.open ( filenameMarkerFile.c_str() );

	std::cout << "Reading annotations..." << std::endl;

	if (signalFile.good())
	{
		bool fileisOk = true;
		std::string line;
		while ( fileisOk )
		{
	
			fileisOk = std::getline(signalFile,line) && fileisOk;
			std::list<std::string> list; 
			blTextUtils::ParseLine(line, ',', list);

			if ( list.size() == 0 )
			{
				continue;
			}

			// Type
			list.pop_front();
			if ( list.size() > 3 &&
				strcmp(list.front().c_str(), "R128" )  == 0 )
			{	
				// Description
				list.pop_front();

				blSignalAnnotation::Pointer annot = blSignalAnnotation::New();

				// Position in data points
				int markerPosition = atoi(list.front().c_str());
				list.pop_front();
				annot->SetXPos( markerPosition );

				// Size in data points
				int duration = atoi(list.front().c_str());
				list.pop_front();
				annot->SetDuration( duration );

				// Channel number (0 = marker is related to all channels)
				int channel = atoi(list.front().c_str());
				list.pop_front();

				// Random type
				annot->SetType(10);

				if ( channel == 0 )
				{
					//! \todo: Only temporal
					//m_signalCollective->AddAnnotation( annot );
					m_signalCollective->AddAnnotation( 0, annot );
				}
				else
				{
					m_signalCollective->AddAnnotation( channel - 1, annot );
				}

			}
		}
	}
	signalFile.close();

}

void blBVFileReader::ReadHeader()
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
	//  Read Header
	ReadFileHeader();

	//==============================================================
	// Decode Header 
	DecodeHeader();

	fclose(m_file);

	std::cout << "Finished reading header..." << std::endl;
}

void blBVFileReader::GetHeaderTags( blTagMap::Pointer tagMap )
{
	const size_t totalSamples = m_FileHeader.NS*m_FileHeader.numRec * m_FileHeader.SPR;
	tagMap->AddTag( "TotalSamples", totalSamples );
	tagMap->AddTag( "NumberOfSignals", size_t( m_FileHeader.NS ) );
	tagMap->AddTag( "NumberOfDataRecords", size_t( m_FileHeader.numRec ) );
	tagMap->AddTag( "SamplesPerRecord", size_t( m_FileHeader.SPR ) );
	tagMap->AddTag( "SampleRate", float( m_FileHeader.SampleRate ) );
	const size_t bit = blGDFUtils::m_GDFTYPMap[ m_chList.at(0).GDFTYP ].m_Size;
	tagMap->AddTag( "SampleSize", bit );
	tagMap->AddTag( "FileSize (Mb)", bit * totalSamples / 1024 / 1024 );
	
}

void blBVFileReader::SetHeaderTags( blTagMap::Pointer tagMap )
{
	blTag::Pointer tag;
	m_SubSamplingFactor = 1;
	tag = tagMap->FindTagByName( "SampleRateFactor" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( m_SubSamplingFactor );
	}
}
