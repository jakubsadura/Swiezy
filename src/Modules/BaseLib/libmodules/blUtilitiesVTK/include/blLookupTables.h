/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blLookupTables_H
#define __blLookupTables_H

#include "BaseLibVTKWin32Header.h"
#include "vtkSmartPointer.h"

#include <vector>
#include <map>

class vtkLookupTable;

/**
\brief Several color look up tables factory

Some color tables implemented in this file (except for 
_lutNonPercepLinGray, _lutPercepLinGray, and _lutWhiteToRed) have 
been taken from the homepage of Haim Levkowitz 
 (http://www.cs.uml.edu/~haim/ColorCenter/ColorCenter.htm) (March, 7th, 2008). 

 To use this class:
 \code
 vtkLookupTable *lutHue = vtkLookupTable::New();
 blLookupTables	lookupTables;
 lookupTables.BuildLutPercepLinOptimal();
 \endcode

 \note If the lut has been created, this will be reused

\ingroup baseLibVTK
 */
class BASELIBVTK_EXPORT blLookupTables
{
public:
	/**\brief types of Lookup Tables*/
	enum LUT_TYPE{
		LUT_TYPE_NON_PERCEP_LIN_GRAY,
		LUT_TYPE_PERCEP_LIN_GRAY,
		LUT_TYPE_PERCEP_LIN_RAINBOW,
		LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED,
		LUT_TYPE_PERCEP_LIN_HEATED_OBJECT,
		LUT_TYPE_PERCEP_LIN_MAGENTA,
		LUT_TYPE_OPTIMAL,
		LUT_TYPE_PERCEP_LIN_OPTIMAL,
		LUT_TYPE_BLUE_TO_CYAN,
		LUT_TYPE_BLUE_TO_YELLOW,
		LUT_TYPE_WHITE_TO_RED,
		LUT_TYPE_BLACK_WHITE, 
		LUT_TYPE_WHITE_BLACK, 
		LUT_TYPE_HOT_METAL, 
		LUT_TYPE_ICE, 
		LUT_TYPE_HOT, 
		LUT_TYPE_GE,
		LUT_TYPE_PRIMARY_COLOR,
		LUT_TYPE_16_COLORS,
		LUT_TYPE_RAINBOW_HUE,
		LUT_TYPE_RAINBOW_HUE_INVERTED,
		LUT_TYPE_RAINBOW_GE,
		LUT_TYPE_SPECTRUM_SIEMENS,
		LUT_TYPE_MITK_GRAY,
		LUT_TYPE_RED_BLUE,
		LUT_TYPE_MAX
	};

	/**\brief types of colors*/
	enum COLOR_TYPE{
		COLOR_RED, 
		COLOR_GREEN, 
		COLOR_BLUE, 
		COLOR_NONE
	};


public:
	//! constructor
	blLookupTables(void);;

	//! destructor
	~blLookupTables(void);;

	/**\brief builds a lookup table of the given type*/
	vtkLookupTable * BuildLookupTable( 
						LUT_TYPE type, 
						double exp = 0, 
						COLOR_TYPE color = COLOR_NONE );

	//!
	static std::string GetLUTName( LUT_TYPE type );

	//!
	static int GetNumberOfLuts( );

private:

	static void BuildLutNames();

	//!
	vtkLookupTable* BuildLutNonPercepLinGray();

	//!
	vtkLookupTable* BuildLutPercepLinGray(double exp);

	//! Steve Pizer, UNC Chapel Hill
	vtkLookupTable* BuildLutPercepLinRainbow();

	//! Inverted
	vtkLookupTable* BuildLutPercepLinRainbowInverted();
	
	//! Steve Pizer, UNC Chapel Hill
	vtkLookupTable* BuildLutPercepLinHeatedObject();

	//! Steve Pizer, UNC Chapel Hill
	vtkLookupTable* BuildLutPercepLinMagenta();

	//!
	vtkLookupTable* BuildLutOptimal();

	//! Haim Levkowitz, Univ of Massachusetts at Lowell
	vtkLookupTable* BuildLutPercepLinOptimal();

	//! Thrasos Papas, AT&T Bell Labs, Murray Hill, NJ
	vtkLookupTable* BuildLutBlueToCyan();

	//! Canon Research, Sydney, Australia
	vtkLookupTable* BuildLutBlueToYellow();

	vtkLookupTable* BuildLutWhiteToRed();;

	/**\brief Builds a lookup table of type BLACK AND WHITE with 256 colors*/
	vtkLookupTable * BuildLookupTableBLACKWHITE();

	/**\brief Builds a lookup table of type WHITE AND BLACK with 256 colors*/
	vtkLookupTable * BuildLookupTableWHITEBLACK();

	/**\brief Builds a lookup table of type HOT METAL with 256 colors*/
	vtkLookupTable * BuildLookupTableHOTMETAL();

	/**\brief Builds a lookup table of type ICE with 32 colors.*/
	vtkLookupTable * BuildLookupTableICE();

	/**\brief Builds a lookup table of type HOT with 32 colors.*/
	vtkLookupTable * BuildLookupTableHOT();

	/**\brief Builds a lookup table of type GE with 256 colors.*/
	vtkLookupTable * BuildLookupTableGE();

	/**\brief builds a new lookup table with a single primary color*/
	vtkLookupTable * BuildLookupTablePRIMARYCOLOR(COLOR_TYPE color);

	//! Specific for 16 colors
	vtkLookupTable * BuildLookupTable16Colors();

	//! 
	vtkLookupTable * BuildLookupTableRaibowHue();

	//!
	vtkLookupTable * BuildLookupTableRaibowHueInverted();

	//! Rainbow of General Electrics
	vtkLookupTable * BuildLookupTableRainbowGE();

	//! Spectrum of Siemens
	vtkLookupTable * BuildLookupTableSpectrumSiemens();

	//!
	vtkLookupTable * BuildLutMITKGray();

	//!
	vtkLookupTable * BuildLutRedBlue( );

private:

	vtkSmartPointer<vtkLookupTable> m_lutTable[ LUT_TYPE_MAX ];

	//! names of the LUT
	static std::map<LUT_TYPE, std::string> m_LutName;
};

#endif // __blLookupTables_H
