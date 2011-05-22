/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#pragma warning (disable : 4786) 

#include "blPDShapeWriter.h"



/** 
*	This method creates a new blPDShapeWriter
*/
//--------------------------------------------------
blPDShapeWriter::blPDShapeWriter()
//--------------------------------------------------
{
	this->SetDoublePrecision(20);	// by default
}


/** 
*	This method creates a new blPDShapeWriter
*/
//--------------------------------------------------
blPDShapeWriter::~blPDShapeWriter()
//--------------------------------------------------
{

}




// Methods inherited from the superclass
//------------------------------------------------------------

/** 
*	This method returns non-zero if the filename given is writeable. 
*/
//--------------------------------------------------
int blPDShapeWriter::CanWriteFile(const char * name)
//--------------------------------------------------
{
	return 1;
}




/** \brief write data related to this writer */
//--------------------------------------------------
void blPDShapeWriter::WriteData(std::ofstream &output)
//--------------------------------------------------
{

	// --------------------------------------------------------
	// PolyData Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("PolyData",output);
	output << std::endl;


	// --------------------------------------------------------
	// Piece Start Tag
	// --------------------------------------------------------

	unsigned int numberOfPoints = this->m_InputObject->GetNumberOfLandmarks();
	unsigned int numberOfVerts = 0;
	unsigned int numberOfStrips = 0;
	unsigned int numberOfLines = this->m_InputObject->GetNumberOfPolylines();
	unsigned int numberOfPolys = this->m_InputObject->GetNumberOfPolygons();


	std::string pieceTag = "Piece NumberOfPoints=\"";

	char buffer[10];	// auxiliary buffer for temporal storing of strings

	sprintf(buffer, "%u", numberOfPoints);
	pieceTag += buffer;

	pieceTag += "\" NumberOfVerts=\"";

	sprintf(buffer, "%u", numberOfVerts);
	pieceTag += buffer;

	pieceTag += "\" NumberOfLines=\"";
	sprintf(buffer, "%u", numberOfLines);
	pieceTag += buffer;

	pieceTag += "\" NumberOfStrips=\"";
	sprintf(buffer, "%u", numberOfStrips);
	pieceTag += buffer;

	pieceTag += "\" NumberOfPolys=\"";
	sprintf(buffer, "%u", numberOfPolys);
	pieceTag += buffer;
	pieceTag += "\"";


	WriteStartElementIndent(pieceTag, output);
	output << std::endl;


	// --------------------------------------------------------
	// Points Start Tag
	// --------------------------------------------------------

	unsigned int numberDimensions = this->m_InputObject->GetDimension();

	sprintf(buffer,"%u", numberDimensions);

	std::string pointsTag = "Points NumberOfDimensions=\"";
	pointsTag += buffer;
	pointsTag += "\"";

	WriteStartElementIndent(pointsTag, output);
	output << std::endl;


	// --------------------------------------------------------
	// DataArray Start Tag
	// --------------------------------------------------------

	/// If precision is %.20e --> Float64
	//	WriteStartElementIndent("DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\"",output);
	WriteStartElementIndent("DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\"",output);
	output << std::endl;


	// write the point coordinates...

	unsigned int numberOfDimensions = this->m_InputObject->GetDimension();
	vnl_vector<double> points(numberOfPoints * numberOfDimensions);

	this->m_InputObject->GetPoints(points);


	///// PRECISION
	int prevPrecision = output.precision();

	output.precision(this->doublePrecision);
	///// PRECISION

	unsigned i;
	int x_pos = 0;

	for (i = 0; i < numberOfPoints; i++)
	{
		// indent
		this->WriteIndentation(output);

		output << points[x_pos] << " " << points[x_pos + 1];

		if (numberOfDimensions == 3)
		{
			output << " " << points[x_pos + 2];
		}
		else
		{
			output << " 0";
		}

		output << std::endl;

		x_pos += numberOfDimensions;
	}

	///// restore PRECISION
	output.precision(prevPrecision);
	///// PRECISION

	WriteEndElementIndent("DataArray",output);
	output << std::endl;


	// --------------------------------------------------------
	// Points End Tag
	// --------------------------------------------------------

	WriteEndElementIndent("Points",output);
	output << std::endl;


	// --------------------------------------------------------
	// PointData Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("PointData",output);
	output << std::endl;


	WriteEndElementIndent("PointData",output);
	output << std::endl;


	// --------------------------------------------------------
	// CellData Start Tag
	// --------------------------------------------------------


	WriteStartElementIndent("CellData",output);
	output << std::endl;

	WriteEndElementIndent("CellData",output);
	output << std::endl;



	// --------------------------------------------------------
	// Verts Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("Verts",output);
	output << std::endl;

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"",
		output);
	output << std::endl;

	WriteEndElementIndent("DataArray", output);
	output << std::endl;

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"",
		output);
	output << std::endl;

	WriteEndElementIndent("DataArray", output);
	output << std::endl;

	WriteEndElementIndent("Verts",output);
	output << std::endl;


	// --------------------------------------------------------
	// Lines Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("Lines",output);
	output << std::endl;


	// vectors of connectivity and offset data
	std::vector<unsigned int> connectivityArray, offsetArray;

	// get the polylines from the shape
	blPDShapeInterface::ShapeCellsContainer polylines;
	this->m_InputObject->GetPolylines(polylines);

	// fill the vectors with connectivity and offset data
	this->GetConnectivityAndOffset(polylines, connectivityArray, offsetArray);

	polylines.clear();


	// connectivity
	// -----------------------

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"",
		output);
	output << std::endl;


	// write the connectivity array
	this->WriteDataArray(connectivityArray, output);


	WriteEndElementIndent("DataArray",output);
	output << std::endl;


	// offsets
	// -----------------------

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"",
		output);
	output << std::endl;


	// write the offset array
	this->WriteDataArray(offsetArray, output);


	WriteEndElementIndent("DataArray",output);
	output << std::endl;


	WriteEndElementIndent("Lines",output);
	output << std::endl;


	// --------------------------------------------------------
	// Strips Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("Strips",output);
	output << std::endl;


	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"",
		output);
	output << std::endl;

	WriteEndElementIndent("DataArray", output);
	output << std::endl;

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"",
		output);
	output << std::endl;

	WriteEndElementIndent("DataArray", output);
	output << std::endl;

	WriteEndElementIndent("Strips",output);
	output << std::endl;


	// --------------------------------------------------------
	// Polys Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("Polys",output);
	output << std::endl;

	// get the polygons
	blPDShapeInterface::ShapeCellsContainer polygons;
	this->m_InputObject->GetPolygons(polygons);

	// fill vectors with connectivity and offset data
	this->GetConnectivityAndOffset(polygons, connectivityArray, offsetArray);

	polygons.clear();


	// connectivity
	// -----------------------

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"",
		output);
	output << std::endl;


	this->WriteDataArray(connectivityArray, output);


	WriteEndElementIndent("DataArray",output);
	output << std::endl;


	// offsets
	// -----------------------	

	WriteStartElementIndent(
		"DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"",
		output);
	output << std::endl;


	this->WriteDataArray(offsetArray, output);


	WriteEndElementIndent("DataArray",output);
	output << std::endl;


	WriteEndElementIndent("Polys",output);
	output << std::endl;


	// --------------------------------------------------------
	// Piece End Tag
	// --------------------------------------------------------

	WriteEndElementIndent("Piece",output);
	output << std::endl;


	WriteEndElementIndent("PolyData",output);
	output << std::endl;

}



/** 
*	Write the XML file, based on the Input Object 
*/
//--------------------------------------------------
int blPDShapeWriter::WriteFile()
//--------------------------------------------------
{

	// sanity checks
	if(m_InputObject == 0)
	{
		std::cerr << "No Shape to Write" << std::endl;
		return 1;
	}

	if(m_Filename.length() == 0)
	{
		std::cerr << "No filename given" << std::endl;
		return 1;
	}

	// define the output stream
	std::ofstream output(m_Filename.c_str());

	if(output.fail())
	{

		std::cerr << "Can't Create " << this->GetFilename() << std::endl;
		return 1;
	}

	this->WriteStartElement("?xml version=\"1.0\"?",output);
	output << std::endl;

	// --------------------------------------------------------
	// VTKFile Start Tag
	// --------------------------------------------------------

	WriteStartElementIndent("VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\" compressor=\"vtkZLibDataCompressor\"",output);
	output << std::endl;


	this->WriteData(output);


	WriteEndElementIndent("VTKFile",output);
	output << std::endl;


	return 0;
}





/** 
*	Writes an array of unsigned int elements in the given output stream.
*
*	It writes 6 elements per line, separated by one white space
*
*	\param dataArray array of elements
*	\param output output stream
*/
//--------------------------------------------------
void blPDShapeWriter::WriteDataArray(std::vector<unsigned int> dataArray,
									 std::ofstream & output)
									 //--------------------------------------------------
{

	// create iterators 	
	std::vector<unsigned int>::const_iterator dataIter, dataIterEnd;
	dataIter = dataArray.begin();
	dataIterEnd = dataArray.end();


	// max 6 cols per line
	int maxColsPerLine = 6;
	int colsCounter = 0;


	while (dataIter != dataIterEnd)
	{			
		if (colsCounter == 0)
		{
			// first a indentation
			this->WriteIndentation(output);
		}

		output << (unsigned int) *dataIter << " ";
		colsCounter++;

		if (colsCounter == maxColsPerLine)
		{
			// new line and reset cols counter
			output << std::endl;			
			colsCounter = 0;
		}

		dataIter++;	// update iterator
	}

	// last new line
	if (colsCounter != 0) 
	{
		output << std::endl;
	}	
}



/** 
*	Returns 2 arrays with connectivity and offsets for the given array of cells.
*
*	\param cellContainer vector of cells
*	\param connectivity vector of connectivity values
*	\param offsets vector of offsets values
*/
//--------------------------------------------------
void blPDShapeWriter::GetConnectivityAndOffset(
	const blPDShapeInterface::ShapeCellsContainer & cellContainer,
	std::vector<unsigned int> & connectivity, std::vector<unsigned int> & offsets)
	//--------------------------------------------------
{
	// clear arrays
	connectivity.clear();
	offsets.clear();


	// create cell iterators
	blPDShapeInterface::ShapeCellsContainer::const_iterator cellIter, cellIterEnd;
	cellIter = cellContainer.begin();
	cellIterEnd = cellContainer.end();

	// value of last offset 
	unsigned int lastOffset = 0;


	while (cellIter != cellIterEnd)
	{
		blPDShapeInterface::PointIdsContainer pointIds = *cellIter;
		blPDShapeInterface::PointIdsContainer::iterator pointIdIter, pointIdIterEnd;

		pointIdIter = pointIds.begin();
		pointIdIterEnd = pointIds.end();

		unsigned int numberPointIds = pointIds.size();

		lastOffset += numberPointIds;

		// add a new offset
		offsets.push_back(lastOffset);


		// update offsets string
		while (pointIdIter != pointIdIterEnd)
		{
			connectivity.push_back(*pointIdIter);

			pointIdIter++;	// update iterator
		}

		cellIter++;	// update iterator
	}



}
