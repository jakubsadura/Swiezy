/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blPDShapeReader_H
#define __blPDShapeReader_H

#include <itkXMLFile.h>

#include "blPDShape2D.h"
#include "blPDShape3D.h"
#include "blStringTokenizer.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#ifdef WIN32
#define STRING_NOCASE_COMPARE(x,y) strcmpi(x,y)
#else
#define STRING_NOCASE_COMPARE(x,y) strcasecmp(x,y)
#endif

//#define DEBUG_MESSAGES_PDSHAPE_READER


// Constant values indicating the position of some attributes 
// in the VTK XML file.

// Well, stupid idiots, the XML fields are not always at the same position
// const unsigned char PIECE_POS_ATTR_NUMBEROFPOINTS = 1;
// const unsigned char PIECE_POS_ATTR_NUMBEROFLINES = 5;
// const unsigned char PIECE_POS_ATTR_NUMBEROFPOLYS = 9;



const unsigned char DATAARRAY_POS_ATTR_CONNECTIVITY = 3;

const unsigned char DATAARRAY_POS_ATTR_OFFSET = 3;


// const unsigned char POINTS_POS_ATTR_NUMBEROFDIMENSIONS = 0;
//const unsigned char POINTS_POS_VAL_NUMBEROFDIMENSIONS = 1;
// 



/**
 * \brief This class reads from a shape file and 
 *	creates a new blPDShape object.
 * \ingroup blPDShape
 *
 * This class creates a new blPDShape based on the data specified in
 * a VTK file in XML format that contains a PolyData dataset.
 */
class blPDShapeReader: public itk::XMLReaderBase
{

    public:		
		
		// ----------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blPDShapeReader Self;
		typedef blSmartPointer<Self> Pointer;


		typedef std::vector<unsigned int> PointsIdsVector;


		// ----------------------------------------------
		// Methods
		//-----------------------------------------------------

		blNewMacro(Self);

		/** \brief return the output of this object */
		blPDShapeInterface * GetOutput() const
			{  return this->pdShape.GetPointer(); };

		/** \brief reads from the specified file and creates the blPDShape */
		void Update();
		
	protected:
		/** \brief Constructor */
		blPDShapeReader();

		/** \brief Destructor */
		virtual ~blPDShapeReader();


    private:        			
		blPDShapeReader(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		
		
		// ----------------------------------------------
		// Typedefs
		// ----------------------------------------------



		// ----------------------------------------------
		// Member variables
		// ----------------------------------------------

		blPDShapeInterface::Pointer pdShape;	
			//!< blPDShape to be built	


		// blags indicating where the parse is currently
		bool inDataArray;
		bool inDataArrayConnectivity;
		bool inDataArrayOffset;

		bool inPoints;
		bool inLines;
		bool inPolys;

		bool numberOfDimension_found;



		// Data obtained from the file
		// ----------------------------------------------

		unsigned int numberDimensions;

		unsigned int numberPoints;
		unsigned int numberLines;
		unsigned int numberPolys;


		// intermediate store of point coords
		std::vector<double> points;

		// intermediate store of lines points ids and offsets
		PointsIdsVector linesPointsIds;
		PointsIdsVector linesPointsOffsets;

		// intermediate store of polygons points ids and offsets
		PointsIdsVector polysPointsIds;
		PointsIdsVector polysPointsOffsets;


		blStringTokenizer tokenizer;

		// ----------------------------------------------
		// Methods
		// ----------------------------------------------

		/** To find a string inside atts */
		unsigned int findStringInXmlAtts (const char* theString, const char **atts);

		/**\brief initialize all the member variables before a new reading*/
		void Initialize();


		// Methods inherited from superclass
		// ----------------------------------------------
		
		/** determine whether a file can be opened and read */
		virtual int CanReadFile(const char* name);


		/** Callback function -- called from XML parser with start-of-element
		* information.
		*/
		virtual void StartElement(const char * name,const char **atts);


		/** Callback function -- called from XML parser when ending tag
		* encountered
		*/
		virtual void EndElement(const char *name);


		/** Callback function -- called from XML parser with the character data
		* for an XML element
		*/
		virtual void CharacterDataHandler(const char *inData, int inLength);






		void BuildCells(const PointsIdsVector & offsets, 
						const PointsIdsVector & ids, 
						blPDShape2D::ShapeCellsContainer & cells);

		void BuildPolygons(blPDShape2D::ShapeCellsContainer & polygons);

		void BuildPolylines(blPDShape2D::ShapeCellsContainer & polylines);

};

#endif
