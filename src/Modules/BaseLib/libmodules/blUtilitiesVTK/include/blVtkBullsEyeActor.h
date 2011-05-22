/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVtkBullsEyeActor_h
#define __blVtkBullsEyeActor_h

#include "BaseLibVTKWin32Header.h"

#include "vtkActor2D.h"
#include "vtkSmartPointer.h"

#include <vector>
#include <vtkShortArray.h>

class vtkAxisActor2D;
class vtkDataObject;
class vtkPolyData;
class vtkPolyDataMapper2D;
class vtkTextMapper;
class vtkTextProperty;
class vtkLegendBoxActor;
class vtkGlyphSource2D;
class vtkAxisLabelArray;
class vtkAxisRanges;
class vtkPieceLabelArray;
class blVtkSegment;
class vtkScalarBarActor;
class vtkShortArray;
class vtkTextActor;
class vtkScalarsToColors;

/**
\brief Create a bull's eye plot

 If you dont provide Input, the 17 number are printed inside each segment
 without legend.

 If you provide Input with scalar data, the scalar data is printed inside
 each segment and color scalar bar is also shown

You'll probably also want to specify the position of the plot be setting the Position and
 Position2 instance variables, which define a rectangle in which the plot
 lies. There are also many other instance
 variables that control the look of the plot includes its title and legend.

 Set the text property/attributes of the title and the labels through the 
 vtkTextProperty objects associated with these components.

 \note Field data can contain non-numeric arrays (i.e. arrays not subclasses of
 vtkDataArray). Such arrays are skipped.

 \sa vtkParallelCoordinatesActor vtkXYPlotActor2D

 \todo The size of the font is not adjusted when the width is increased
*/
class BASELIBVTK_EXPORT blVtkBullsEyeActor : public vtkActor2D
{
public:
	//! Standard methods for type information and printing.
	vtkTypeMacro(blVtkBullsEyeActor,vtkActor2D);
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Instantiate this class.
	static blVtkBullsEyeActor *New();

	//! Set the input to the actor.
	virtual void SetInput(vtkDataObject*);

	//! Set array name to show data
	void SetArrayName( const char* arrayName );

	//! Get the input data object to this actor.
	vtkDataObject* GetInput();

	//! Enable/Disable the display of a plot title.
	bool GetTitleVisibility() const;
	void SetTitleVisibility(bool val);

	//! Set/Get the title of the spider plot.
	const char* GetTitle() const;
	void SetTitle(const char * val);

	//! Set/Get the title text property.
	vtkTextProperty* GetTitleTextProperty() const;

	//! Enable/Disable the display data. 
	bool GetDataVisibility() const;
	void SetDataVisibility(bool val);

	/** Enable/Disable the creation of a legend. If on, the legend labels will
	 be created automatically unless the per plot legend symbol has been
	 set.
	 */
	bool GetLegendVisibility() const;
	void SetLegendVisibility(bool val);

	/** Enable/Disable the creation of a legend. If on, the legend labels will
	 be created automatically unless the per plot legend symbol has been
	 set.
	 */
	bool GetLabelVisibility() const;
	void SetLabelVisibility(bool val);

	//! Label properties
	vtkTextProperty * GetLabelTextProperty() const;

	/** Retrieve handles to the legend box. This is useful if you would like 
	 to manually control the legend appearance.
	 */
	vtkScalarBarActor * GetLegendActor() const { return m_LegendActor; }

	//! Draw the plot.
	int RenderOverlay(vtkViewport*);
	int RenderOpaqueGeometry(vtkViewport*);
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport* ) {return 0;}

	//! Does this prop have some translucent polygonal geometry?
	virtual int HasTranslucentPolygonalGeometry();

	/** Release any graphics resources that are being consumed by this actor.
	 The parameter window could be used to determine which graphic
	 resources to release.
	 */
	void ReleaseGraphicsResources(vtkWindow *);

	//! Use the range of the input array
	void UseInputArrayScalarRange( bool val );

	vtkScalarsToColors* GetVtkDefaultLut() const;
	void SetVtkDefaultLut(vtkScalarsToColors* val);

	//! Set number of decimals when printing the values of each cell
	void SetNumberOfDecimals( long numberOfDecimals );

protected:
	blVtkBullsEyeActor();
	~blVtkBullsEyeActor();

	//!
	vtkDataArray *GetInputArray( );

private:

	//! Rebuild all axes, rings, legend and title
	int PlacePlot(vtkViewport *viewport, int *size);

	//! Update plot
	int BuildPlot(vtkViewport*);

	//!
	void ComputeAxisAndRings( );

	//!
	void ComputeLegendPosition( );

	//!
	void BuildTitle( vtkViewport *viewport );

	//!
	void ComputePlotSize( vtkViewport *viewport );

	//! 
	int PlaceData( vtkViewport *viewport );

	//!
	void BuildCells( );

	//! Not implemented.
	blVtkBullsEyeActor(const blVtkBullsEyeActor&);

	//! Not implemented.
	void operator=(const blVtkBullsEyeActor&);

	//! 
	void PlaceAnnotations( vtkViewport *viewport );

private:
	//! List of data sets to plot
	vtkDataObject *m_Input;
	
	//! Should I see the title?
	bool m_TitleVisibility;

	//! The title string
	std::string m_Title;

	vtkTextProperty *m_TitleTextProperty; 

	bool m_DataVisibility;

	vtkTextProperty *m_LabelTextProperty;

	bool m_LegendVisibility;

	bool m_LabelVisibility;

	vtkScalarBarActor *m_LegendActor;

	vtkGlyphSource2D *m_GlyphSource;

	vtkTextMapper    *m_TitleMapper;

	vtkActor2D       *m_TitleActor;

	//! The web of the spider plot
	vtkPolyData         *m_WebDataCells;

	vtkPolyDataMapper2D *m_WebMapperCells;

	vtkActor2D          *m_WebActorCells;

	vtkPolyData         *m_WebDataLines;

	vtkPolyDataMapper2D *m_WebMapperLines;

	vtkActor2D          *m_WebActorLines;

	vtkTimeStamp  m_BuildTime;

	double m_Center[3];

	double m_Radius;

	int   m_LastPosition[2];

	int   m_LastPosition2[2];

	//! The location of the corners of the box
	double m_P1[3];

	//! The location of the corners of the box
	double m_P2[3];

	//! Size of the plot without legend and title
	double m_PlotSize[ 2 ];

	//! Data mappers
	vtkTextMapper    **m_PieceMappers;

	//! Data mappers
	vtkActor2D       **m_PieceActors;

	//!
	vtkSmartPointer<vtkShortArray> m_SegmentID;

	//!
	vtkSmartPointer<vtkScalarsToColors> m_vtkDefaultLut;

	//!
	vtkSmartPointer<vtkLookupTable> m_EmptyPlotLut;

	//! 
	std::string m_strArrayName;

	//!
	bool m_bUseInputArrayScalarRange;

	//!
	std::vector< vtkSmartPointer<vtkTextActor> > m_TextActorVector;

	//!
	long m_NumberOfDecimals;
};


#endif

