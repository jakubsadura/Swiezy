/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blVtkBullsEyeActor.h"

#include "vtkAxisActor2D.h"
#include "vtkCellArray.h"
#include "vtkFieldData.h"
#include "vtkCellData.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkViewport.h"
#include "vtkWindow.h"
#include "vtkLegendBoxActor.h"
#include "vtkGlyphSource2D.h"
#include "vtkProperty2D.h"
#include <vtkstd/string>
#include <vtkstd/vector>
#include "vtkSmartPointer.h"
#include "vtkScalarBarActor.h"
#include "vtkPolygon.h"
#include "vtkTextActor.h"
#include "vtkVersion.h" // Needed for VTK_*_VERSION

#include <vtkPointData.h>

#include "blLookupTables.h"
#include "blShapeUtils.h"

vtkStandardNewMacro(blVtkBullsEyeActor);

const int NUM_SEGMENTS = 17;
const int NUM_RINGS = 4;
const int NUM_SEGMENTS_PER_RING[ NUM_RINGS ] = { 1, 4, 6, 6 };
const int VTK_RING_PTS = 96;
const double RING_OFFSET[ NUM_RINGS ] = { 0, 0.5, 1, 1};

const double TITLE_SPACE = 0.1;
const double LEGEND_SPACE = 0.2;

//----------------------------------------------------------------------------
// Instantiate object
blVtkBullsEyeActor::blVtkBullsEyeActor()
{
	// Actor2D positions
	PositionCoordinate->SetCoordinateSystemToNormalizedViewport();
	PositionCoordinate->SetValue(0.1,0.1);
	Position2Coordinate->SetCoordinateSystemToNormalizedViewport();
	Position2Coordinate->SetValue(0.9, 0.8);
	Position2Coordinate->SetReferenceCoordinate(NULL);

	m_Input = NULL;
	m_TitleVisibility = 1;

	m_PieceActors = new vtkActor2D* [ NUM_SEGMENTS ];
	m_PieceMappers = new vtkTextMapper* [ NUM_SEGMENTS ];
	for (vtkIdType i=0; i<NUM_SEGMENTS; i++)
	{
		m_PieceMappers[i] = vtkTextMapper::New();
		m_PieceActors[i] = vtkActor2D::New();
	}

	m_LabelTextProperty = vtkTextProperty::New();
	m_LabelTextProperty->SetFontSize(12);
	m_LabelTextProperty->SetBold(1);
	m_LabelTextProperty->SetItalic(1);
	m_LabelTextProperty->SetShadow(0);
	m_LabelTextProperty->SetFontFamilyToArial();

	m_TitleTextProperty = vtkTextProperty::New();
	m_TitleTextProperty->ShallowCopy(m_LabelTextProperty);
	m_TitleTextProperty->SetFontSize(24);
	m_TitleTextProperty->SetBold(1);
	m_TitleTextProperty->SetItalic(0);
	m_TitleTextProperty->SetShadow(1);
	m_TitleTextProperty->SetFontFamilyToArial();
	m_DataVisibility = 1;

	m_LegendVisibility = 0;

	m_EmptyPlotLut = vtkSmartPointer<vtkLookupTable>::New();
	blLookupTables lookupTables;
	m_EmptyPlotLut->DeepCopy( lookupTables.BuildLookupTable( 
		blLookupTables::LUT_TYPE_PERCEP_LIN_RAINBOW ) );
	m_vtkDefaultLut = m_EmptyPlotLut;

	m_LegendActor = vtkScalarBarActor::New();
	m_LegendActor->GetPositionCoordinate()->SetCoordinateSystemToViewport();
	m_LegendActor->GetPosition2Coordinate()->SetCoordinateSystemToViewport();
	m_LegendActor->GetPosition2Coordinate()->SetReferenceCoordinate(NULL);
	m_LegendActor->SetLookupTable( m_vtkDefaultLut.GetPointer() );

	m_GlyphSource = vtkGlyphSource2D::New();
	m_GlyphSource->SetGlyphTypeToNone();
	m_GlyphSource->DashOn();
	m_GlyphSource->FilledOff();

	m_TitleMapper = vtkTextMapper::New();
	m_TitleActor = vtkActor2D::New();
	m_TitleActor->SetMapper(m_TitleMapper);
	m_TitleActor->GetPositionCoordinate()->SetCoordinateSystemToViewport();

	// WebDataCells
	m_WebDataCells = vtkPolyData::New();
	m_WebMapperCells = vtkPolyDataMapper2D::New();
	m_WebMapperCells->SetInput(m_WebDataCells);
	m_WebMapperCells->SetScalarModeToUseCellData( );
	m_WebMapperCells->UseLookupTableScalarRangeOn( );
	m_WebMapperCells->SetLookupTable( m_vtkDefaultLut );
	m_WebActorCells = vtkActor2D::New();
	m_WebActorCells->SetMapper(m_WebMapperCells);

	// 
	m_WebDataLines = vtkPolyData::New();
	m_WebMapperLines = vtkPolyDataMapper2D::New();
	m_WebMapperLines->SetInput(m_WebDataLines);
	m_WebActorLines = vtkActor2D::New();
	m_WebActorLines->SetMapper(m_WebMapperLines);

	m_LastPosition[0] = 0;
	m_LastPosition[1] = 0;
	m_LastPosition2[0] = 0;
	m_LastPosition2[1] = 0;


	BuildCells( );

	m_P1[0] = m_P1[1] = m_P2[0] = m_P2[1] = 0.0;

	m_SegmentID = vtkSmartPointer<vtkShortArray>::New();
	m_SegmentID->SetNumberOfTuples( NUM_SEGMENTS );
	for (int i=0; i<NUM_SEGMENTS; i++)
	{
		m_SegmentID->SetTuple1( i, i + 1 );
	}

	m_TextActorVector.resize( 4 );
	m_TextActorVector[ 0 ] = vtkSmartPointer<vtkTextActor>::New();
	m_TextActorVector[ 0 ]->SetInput( "Anterior" );
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 0 ]->SetTextScaleModeToViewport();
	#endif
	m_TextActorVector[ 0 ]->SetOrientation( 0 );
	m_TextActorVector[ 0 ]->GetTextProperty()->SetJustificationToCentered();

	m_TextActorVector[ 1 ] = vtkSmartPointer<vtkTextActor>::New();
	m_TextActorVector[ 1 ]->SetInput( "Lateral" );
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 1 ]->SetTextScaleModeToViewport();
	#endif
	m_TextActorVector[ 1 ]->SetOrientation( 90 );
	m_TextActorVector[ 1 ]->GetTextProperty()->SetJustificationToCentered();

	m_TextActorVector[ 2 ] = vtkSmartPointer<vtkTextActor>::New();
	m_TextActorVector[ 2 ]->SetInput( "Inferior" );
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 2 ]->SetTextScaleModeToViewport();
	#endif
	m_TextActorVector[ 2 ]->SetOrientation( 0 );
	m_TextActorVector[ 2 ]->GetTextProperty()->SetJustificationToCentered();

	m_TextActorVector[ 3 ] = vtkSmartPointer<vtkTextActor>::New();
	m_TextActorVector[ 3 ]->SetInput( "Septal" );
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 3 ]->SetTextScaleModeToViewport();
	#endif
	m_TextActorVector[ 3 ]->SetOrientation( 90 );
	m_TextActorVector[ 3 ]->GetTextProperty()->SetJustificationToCentered();

	m_bUseInputArrayScalarRange = true;

	m_NumberOfDecimals = 2;
}

//----------------------------------------------------------------------------
blVtkBullsEyeActor::~blVtkBullsEyeActor()
{
	if ( m_Input )
	{
		m_Input->Delete();
		m_Input = NULL;
	}

	m_LabelTextProperty->Delete();
	m_TitleTextProperty->Delete();

	m_LegendActor->Delete();
	m_GlyphSource->Delete();

	m_TitleMapper->Delete();
	m_TitleMapper = NULL;
	m_TitleActor->Delete();
	m_TitleActor = NULL;

	m_WebDataCells->Delete();
	m_WebMapperCells->Delete();
	m_WebActorCells->Delete();

	m_WebDataLines->Delete();
	m_WebMapperLines->Delete();
	m_WebActorLines->Delete();

	if ( m_PieceActors )
	{
		for (int i=0; i<NUM_SEGMENTS; i++)
		{
			m_PieceMappers[i]->Delete();
			m_PieceActors[i]->Delete();
		}
		delete [] m_PieceMappers;
		m_PieceMappers = NULL;
		delete [] m_PieceActors;
		m_PieceActors = NULL;
	}

}

//----------------------------------------------------------------------------
// Plot scalar data for each input dataset.
int blVtkBullsEyeActor::RenderOverlay(vtkViewport *viewport)
{
	int renderedSomething=0;

	if ( !BuildPlot(viewport) )
	{
		return 0;
	}

	if ( m_TitleVisibility )
	{
		renderedSomething += m_TitleActor->RenderOverlay(viewport);
	}

	if ( GetInputArray() != NULL )
	{
		m_WebActorCells->SetProperty(GetProperty());
		renderedSomething += m_WebActorCells->RenderOverlay(viewport);
	}

	m_WebActorLines->SetProperty(GetProperty());
	renderedSomething += m_WebActorLines->RenderOverlay(viewport);

	if ( m_DataVisibility )
	{
		for (int i=0; i<NUM_SEGMENTS; i++)
		{
			renderedSomething += m_PieceActors[i]->RenderOverlay(viewport);
		}
	}

	if ( m_LegendVisibility )
	{
		renderedSomething += m_LegendActor->RenderOverlay(viewport);
	}

	renderedSomething += m_TextActorVector[ 0 ]->RenderOverlay( viewport );
	renderedSomething += m_TextActorVector[ 1 ]->RenderOverlay( viewport );
	renderedSomething += m_TextActorVector[ 2 ]->RenderOverlay( viewport );
	renderedSomething += m_TextActorVector[ 3 ]->RenderOverlay( viewport );

	return renderedSomething;
}

//----------------------------------------------------------------------------
// Plot scalar data for each input dataset.
int blVtkBullsEyeActor::RenderOpaqueGeometry(vtkViewport *viewport)
{
	int renderedSomething=0;

	if ( !BuildPlot(viewport) )
	{
		return 0;
	}

	if ( m_TitleVisibility )
	{
		renderedSomething += m_TitleActor->RenderOpaqueGeometry(viewport);
	}

	if ( GetInputArray() != NULL )
	{
		m_WebActorCells->SetProperty(GetProperty());
		renderedSomething += m_WebActorCells->RenderOpaqueGeometry(viewport);
	}

	m_WebActorLines->SetProperty(GetProperty());
	renderedSomething += m_WebActorLines->RenderOverlay(viewport);

	if ( m_DataVisibility )
	{
		for (int i=0; i<NUM_SEGMENTS; i++)
		{
			renderedSomething += m_PieceActors[i]->RenderOpaqueGeometry(viewport);
		}
	}

	if ( m_LegendVisibility )
	{
		renderedSomething += m_LegendActor->RenderOpaqueGeometry(viewport);
	}

	renderedSomething += m_TextActorVector[ 0 ]->RenderOpaqueGeometry( viewport );
	renderedSomething += m_TextActorVector[ 1 ]->RenderOpaqueGeometry( viewport );
	renderedSomething += m_TextActorVector[ 2 ]->RenderOpaqueGeometry( viewport );
	renderedSomething += m_TextActorVector[ 3 ]->RenderOpaqueGeometry( viewport );

	return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int blVtkBullsEyeActor::HasTranslucentPolygonalGeometry()
{
	return 0;
}

//-----------------------------------------------------------------------------  
int blVtkBullsEyeActor::BuildPlot(vtkViewport *viewport)
{
	// Initialize
	vtkDebugMacro(<<"Building spider plot");

	// Make sure input is up to date, and that the data is the correct shape to
	// plot.
	if (!m_TitleTextProperty)
	{
		vtkErrorMacro(<<"Need title text property to render plot");
		return 0;
	}
	if (!m_LabelTextProperty)
	{
		vtkErrorMacro(<<"Need label text property to render plot");
		return 0;
	}

	// Viewport change may not require rebuild
	int positionsHaveChanged = 0;
	if (viewport->GetMTime() > m_BuildTime || 
		(viewport->GetVTKWindow() && 
		viewport->GetVTKWindow()->GetMTime() > m_BuildTime))
	{
	int *lastPosition = 
		PositionCoordinate->GetComputedViewportValue(viewport);
	int *lastPosition2 =
		Position2Coordinate->GetComputedViewportValue(viewport);
	if (lastPosition[0] != m_LastPosition[0] ||
		lastPosition[1] != m_LastPosition[1] ||
		lastPosition2[0] != m_LastPosition2[0] ||
		lastPosition2[1] != m_LastPosition2[1] )
		{
		m_LastPosition[0] = lastPosition[0];
		m_LastPosition[1] = lastPosition[1];
		m_LastPosition2[0] = lastPosition2[0];
		m_LastPosition2[1] = lastPosition2[1];
		positionsHaveChanged = 1;
		}
	}

	// Check modified time to see whether we have to rebuild.
	unsigned long int inputBuildTime = m_BuildTime;
	if ( m_Input != NULL )
	{
		m_Input->Update();
		inputBuildTime = m_Input->GetMTime();
	}

	if (positionsHaveChanged ||
		GetMTime() > m_BuildTime ||
		inputBuildTime > m_BuildTime ||
		m_LabelTextProperty->GetMTime() > m_BuildTime ||
		m_TitleTextProperty->GetMTime() > m_BuildTime)
	{
		vtkDebugMacro(<<"Rebuilding plot");

		// Build axes
		int *size = viewport->GetSize();
		if (!PlacePlot(viewport, size))
			{
			return 0;
			}

		m_BuildTime.Modified();
	} // If need to rebuild the plot

	return 1;
}

//----------------------------------------------------------------------------
static inline int blVtkBullsEyeActorGetComponent(
	vtkFieldData* field,
	vtkIdType tuple, 
	int component, 
	double* val)
{
	int array_comp;
	int array_index = field->GetArrayContainingComponent(component, array_comp);
	if (array_index < 0)
	{
		return 0;
	}
	vtkDataArray* da = field->GetArray(array_index);
	if (!da)
	{
		// non-numeric array.
		return 0;
	}
	*val = da->GetComponent(tuple, array_comp);
	return 1;
}

//----------------------------------------------------------------------------
int blVtkBullsEyeActor::PlacePlot(vtkViewport *viewport, int* vtkNotUsed(size))
{
	// Add scalar array to WebDataCells to paint colors
	m_WebDataCells->GetCellData()->SetScalars( GetInputArray() );

	ComputePlotSize( viewport );

	ComputeAxisAndRings( );
	
	ComputeLegendPosition();

	BuildTitle( viewport );

	PlaceData( viewport );

	PlaceAnnotations( viewport );

	return 1;
}

//----------------------------------------------------------------------------
// Release any graphics resources that are being consumed by this actor.
// The parameter window could be used to determine which graphic
// resources to release.
void blVtkBullsEyeActor::ReleaseGraphicsResources(vtkWindow *win)
{
	m_TitleActor->ReleaseGraphicsResources(win);
	m_LegendActor->ReleaseGraphicsResources(win);
	m_WebActorCells->ReleaseGraphicsResources(win);
	m_WebActorLines->ReleaseGraphicsResources(win);
	for (int i=0; m_PieceActors && i<NUM_SEGMENTS; i++)
	{
		m_PieceActors[i]->ReleaseGraphicsResources(win);
	}
}

//----------------------------------------------------------------------------
void blVtkBullsEyeActor::PrintSelf(ostream& os, vtkIndent indent)
{
	Superclass::PrintSelf(os,indent);

	os << indent << "Input: " << m_Input << "\n";

	os << indent << "Independent Variables: ";

	os << indent << "Title Visibility: " 
		<< (m_TitleVisibility ? "On\n" : "Off\n");

	os << indent << "Title: " << m_Title.c_str() << "\n";

	if (m_TitleTextProperty)
	{
		os << indent << "Title Text Property:\n";
		m_TitleTextProperty->PrintSelf(os,indent.GetNextIndent());
	}
	else
	{
		os << indent << "Title Text Property: (none)\n";
	}

	os << indent << "Label Visibility: " 
		<< (m_DataVisibility ? "On\n" : "Off\n");

	if (m_LabelTextProperty)
	{
		os << indent << "Label Text Property:\n";
		m_LabelTextProperty->PrintSelf(os,indent.GetNextIndent());
	}
	else
	{
		os << indent << "Label Text Property: (none)\n";
	}

	os << indent << "Legend Visibility: " 
		<< (m_LegendVisibility ? "On\n" : "Off\n");

	os << indent << "Legend Actor: " 
		<< m_LegendActor << "\n";
	m_LegendActor->PrintSelf(os, indent.GetNextIndent());

}

void blVtkBullsEyeActor::ComputeAxisAndRings()
{
	vtkIdType i, j;

	// Now generate the web points
	vtkPoints *webPtsCells = m_WebDataCells->GetPoints();
	vtkPoints *webPtsLines = m_WebDataLines->GetPoints();

	// Ring has VTK_RING_PTS + 1 points
	double currentAlpha;
	vtkIdType pointID = 0;
	double x[3]; 
	x[2] = 0.0;

	// RINGS
	double  deltaRadius = m_Radius / NUM_RINGS;
	currentAlpha = 2.0*vtkMath::Pi()/VTK_RING_PTS;
	for (i=0; i<NUM_RINGS + 1; i++)
	{
		double currentRadius;
		currentRadius = static_cast<double>(i) * deltaRadius;
		for (j=0; j<VTK_RING_PTS; j++)
		{
			x[0] = m_Center[0] + currentRadius*cos(j*currentAlpha);
			x[1] = m_Center[1] + currentRadius*sin(j*currentAlpha);
			webPtsCells->SetPoint( pointID, x );
			webPtsLines->SetPoint( pointID, x );
			pointID++;
		}
	}
}

void blVtkBullsEyeActor::ComputeLegendPosition()
{
	// Set VTK range
	double range[2];
	if ( m_bUseInputArrayScalarRange && GetInputArray() )
	{
		GetInputArray()->GetRange( range );
		m_vtkDefaultLut->SetRange( range );
	}
	else if ( GetInputArray() == NULL )
	{
		range[ 0 ] = 0;
		range[ 1 ] = 1;
		m_vtkDefaultLut = m_EmptyPlotLut;
		m_vtkDefaultLut->SetRange( range );
	}

	// Display the legend
	if ( m_LegendVisibility )
	{
		m_LegendActor->GetPositionCoordinate()->SetValue(
			m_P1[0] + 0.85*(m_P2[0]-m_P1[0]),m_P1[1] + 0.20*(m_P2[1]-m_P1[1]));
		m_LegendActor->GetPosition2Coordinate()->SetValue(
			m_P2[0], m_P1[1] + 0.80*(m_P2[1]-m_P1[1]));
	}

}

void blVtkBullsEyeActor::BuildTitle( vtkViewport *viewport )
{
	m_TitleMapper->SetInput(m_Title.c_str());
	if (m_TitleTextProperty->GetMTime() > m_BuildTime)
	{
		// Shallow copy here since the justification is changed but we still
		// want to allow actors to share the same text property, and in that case
		// specifically allow the title and label text prop to be the same.
		m_TitleMapper->GetTextProperty()->ShallowCopy(
			m_TitleTextProperty);
		m_TitleMapper->GetTextProperty()->SetJustificationToCentered();
	}

	// We could do some caching here, but hey, that's just the title
	int tsize[2];
	tsize[0] = static_cast<int>(0.25*m_PlotSize[ 0 ]);
	tsize[1] = static_cast<int>(0.15*m_PlotSize[ 1 ]);
	m_TitleMapper->SetConstrainedFontSize(viewport, tsize[0], tsize[1]);

	m_TitleActor->GetPositionCoordinate()->
		SetValue(m_Center[0],m_Center[1]+m_Radius+tsize[1]);
	m_TitleActor->SetProperty(GetProperty());
}

void blVtkBullsEyeActor::ComputePlotSize( vtkViewport *viewport )
{
	// Get the location of the corners of the box
	double *p1 = PositionCoordinate->GetComputedDoubleViewportValue(viewport);
	double *p2 = Position2Coordinate->GetComputedDoubleViewportValue(viewport);
	m_P1[0] = (p1[0] < p2[0] ? p1[0] : p2[0]);
	m_P1[1] = (p1[1] < p2[1] ? p1[1] : p2[1]);
	m_P2[0] = (p1[0] > p2[0] ? p1[0] : p2[0]);
	m_P2[1] = (p1[1] > p2[1] ? p1[1] : p2[1]);

	// Determine the center of the spider plot. Leave room for the title and the legend
	double titleSpace=0.0, legendSpace=0.0;
	if ( m_TitleVisibility )
	{
		titleSpace = TITLE_SPACE;
	}
	if ( m_LegendVisibility )
	{
		legendSpace = LEGEND_SPACE;
	}

	m_PlotSize[ 0 ] = m_P2[0] - legendSpace*(m_P2[0]-m_P1[0]) - m_P1[0];
	m_PlotSize[ 1 ] = m_P2[1] - titleSpace*(m_P2[1]-m_P1[1]) - m_P1[1];

	m_Center[0] = m_P1[0] + m_PlotSize[ 0 ]/2.0;
	m_Center[1] = m_P1[1] + m_PlotSize[ 1 ]/2.0;
	m_Center[2] = 0.0;
	m_Radius = (m_PlotSize[ 0 ] < m_PlotSize[ 1 ] ? m_PlotSize[ 0 ] : m_PlotSize[ 1 ]);
	m_Radius /= 2.0;
}

int blVtkBullsEyeActor::PlaceData( vtkViewport *viewport )
{
	if ( !m_DataVisibility )
	{
		return 0;
	}

	vtkDataArray *array;
	if ( GetInputArray() )
	{
		array = GetInputArray();
	}
	else
	{
		array = m_SegmentID.GetPointer();
	}

	vtkIdType numRows;
	vtkIdType numTuples;
	numTuples = array->GetNumberOfTuples();
	numRows = numTuples;
	if ( numTuples != NUM_SEGMENTS )
	{
		return 0;
	}

	char label[1024];
	int minFontSize=1000, fontSize, tsize[2];

	// Now generate the web points
	vtkPoints *webPts = m_WebDataCells->GetPoints();
	vtkCellArray *webPolys = m_WebDataCells->GetPolys( );
	webPolys->InitTraversal( );
	vtkIdType npts;
	vtkIdType *pIds;
	int cellID = 0;

	double average_position[ NUM_SEGMENTS ][ 3 ] = {0,0,0};
	while ( webPolys->GetNextCell( npts, pIds ) != 0 )
	{
		// Compute mean of the points of the segment
		double *point_position;
		average_position[ cellID ][ 0 ] = 0;
		average_position[ cellID ][ 1 ] = 0;
		average_position[ cellID ][ 2 ] = 0;
		point_position = webPts->GetPoint( pIds[ npts * 25 / 100 ] );
		average_position[ cellID ][ 0 ] += point_position[ 0 ];
		average_position[ cellID ][ 1 ] += point_position[ 1 ];
		average_position[ cellID ][ 2 ] += point_position[ 2 ];
		point_position = webPts->GetPoint( pIds[ npts * 75 / 100 ] );
		average_position[ cellID ][ 0 ] += point_position[ 0 ];
		average_position[ cellID ][ 1 ] += point_position[ 1 ];
		average_position[ cellID ][ 2 ] += point_position[ 2 ];
		average_position[ cellID ][ 0 ] /= 2;
		average_position[ cellID ][ 1 ] /= 2;
		average_position[ cellID ][ 2 ] /= 2;

		// 
		if ( array == m_SegmentID.GetPointer() )
		{
			sprintf( label, "%d", int( array->GetComponent( cellID, 0 ) ) );
		}
		else
		{
			char bufferFormat[ 32 ];
			strcpy( bufferFormat, "%.2f" );
			bufferFormat[ 2 ] = m_NumberOfDecimals + '0';
			sprintf( label, bufferFormat, array->GetComponent( cellID, 0 ) );
			//double *color;
			//color = LegendActor->GetEntryColor( cellID );
			//LabelTextProperty->SetColor( color );
		}


		m_PieceMappers[ cellID ]->SetInput( label );
		m_PieceMappers[ cellID ]->GetTextProperty()->
			ShallowCopy(m_LabelTextProperty);
		tsize[0] = static_cast<int>( 0.1 * m_PlotSize[ 0 ] );
		tsize[1] = static_cast<int>( 0.1 * m_PlotSize[ 1 ] );
		fontSize = m_PieceMappers[ cellID ]->SetConstrainedFontSize(
			viewport, tsize[0], tsize[1]);
		minFontSize = (fontSize < minFontSize ? fontSize : minFontSize);

		m_PieceActors[ cellID ]->SetMapper(m_PieceMappers[ cellID ]);
		m_PieceActors[ cellID ]->GetPositionCoordinate()->
			SetCoordinateSystemToViewport();
		m_PieceActors[ cellID ]->SetPosition( average_position[ cellID ] );

		cellID++;
	}


	//Now reset font sizes to the same value
	for (int cellID=0; cellID<NUM_SEGMENTS; cellID++)
	{
		m_PieceMappers[ cellID ]->GetTextProperty()->SetFontSize(minFontSize);
		m_PieceMappers[ cellID ]->GetTextProperty()->SetJustificationToCentered();
		m_PieceMappers[ cellID ]->GetTextProperty()->SetVerticalJustificationToCentered( );
	}


	return 1;
}

void blVtkBullsEyeActor::BuildCells()
{
	// Now generate the web points
	m_WebDataCells->Initialize();
	// Axis lines
	vtkSmartPointer<vtkPoints> webPts = vtkSmartPointer<vtkPoints>::New();

	// Points
	int numTotalPoints = VTK_RING_PTS * ( NUM_RINGS + 1 );
	webPts->SetNumberOfPoints( numTotalPoints );
	m_WebDataCells->SetPoints(webPts);

	// Initialize polys (to paint cells)
	vtkSmartPointer<vtkCellArray> webPolys = vtkSmartPointer<vtkCellArray>::New();
	vtkIdType pointID = 0;
	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
	for ( int iRingID = NUM_RINGS - 1; iRingID >= 0; iRingID--)
	{
		// For each ring segment
		for ( int iRingSegment=0; iRingSegment < NUM_SEGMENTS_PER_RING[ iRingID ]; iRingSegment++)
		{
			polygon->GetPointIds( )->Reset( );

			int numRingSegmentPoints = VTK_RING_PTS / NUM_SEGMENTS_PER_RING[ iRingID ];
			int iOffsetPoints = RING_OFFSET[ iRingID ] * numRingSegmentPoints;
			int pointIDInternalRingStart = iRingID * VTK_RING_PTS;
			int pointIDExternalRingStart = ( iRingID + 1 ) * VTK_RING_PTS;
			int iSegmentPoint = 0;

			// Internal ring
			if ( NUM_SEGMENTS_PER_RING[ iRingID ] > 1 )
			{
				for ( int iPoint = 0 ; iPoint < numRingSegmentPoints + 1; iPoint++ )
				{
					pointID = pointIDInternalRingStart + 
						( numRingSegmentPoints * iRingSegment + iPoint + iOffsetPoints ) % VTK_RING_PTS;
					polygon->GetPointIds( )->InsertNextId( pointID );
				}
			}

			// External ring
			for ( int iPoint = numRingSegmentPoints ; iPoint >= 0; iPoint-- )
			{
				pointID = pointIDExternalRingStart + 
					( numRingSegmentPoints * iRingSegment + iPoint + iOffsetPoints ) % VTK_RING_PTS;
				polygon->GetPointIds( )->InsertNextId( pointID );
			}

			// End point = start point
			//polygon->GetPointIds( )->InsertNextId( polygon->GetPointIds( )->GetId( 0 ) );

			// Insert next cell
			webPolys->InsertNextCell( polygon->GetPointIds());
		}
	}

	// Polys
	m_WebDataCells->SetPolys(webPolys);

	// Line polydata
	vtkSmartPointer<vtkPoints> webPtsOfLines = vtkSmartPointer<vtkPoints>::New();

	// Points
	webPtsOfLines->DeepCopy( webPts );
	m_WebDataLines->SetPoints(webPtsOfLines);

	vtkSmartPointer<vtkCellArray> webLines = vtkSmartPointer<vtkCellArray>::New();
	webLines->DeepCopy( webPolys );
	m_WebDataLines->SetLines( webLines );

}

void blVtkBullsEyeActor::SetInput( vtkDataObject* input )
{
	vtkDebugMacro(
		<< GetClassName() << " (" << this 
		<< "): setting " << "Input to " << input );
	
	if (m_Input != input)
	{
		vtkDataObject* tempSGMacroVar = m_Input;

		// Remove all arrays from PointData of WebDataCells 
		m_WebDataCells->GetPointData()->Initialize();

		m_Input = input;
		if (m_Input != NULL) 
		{ 
			m_Input->Register(this); 
		}
		if (tempSGMacroVar != NULL)
		{
			tempSGMacroVar->UnRegister(this);
		}
		Modified();
	}
}

vtkDataObject* blVtkBullsEyeActor::GetInput()
{
	return m_Input;
}

bool blVtkBullsEyeActor::GetTitleVisibility() const
{
	return m_TitleVisibility;
}

void blVtkBullsEyeActor::SetTitleVisibility( bool val )
{
	m_TitleVisibility = val;
}

const char * blVtkBullsEyeActor::GetTitle() const
{
	return m_Title.c_str();
}

void blVtkBullsEyeActor::SetTitle( const char * val )
{
	m_Title = val;
}

bool blVtkBullsEyeActor::GetDataVisibility() const
{
	return m_DataVisibility;
}

void blVtkBullsEyeActor::SetDataVisibility( bool val )
{
	m_DataVisibility = val;
}

bool blVtkBullsEyeActor::GetLegendVisibility() const
{
	return m_LegendVisibility;
}

void blVtkBullsEyeActor::SetLegendVisibility( bool val )
{
	m_LegendVisibility = val;
}

bool blVtkBullsEyeActor::GetLabelVisibility() const
{
	return m_LabelVisibility;
}

void blVtkBullsEyeActor::SetLabelVisibility( bool val )
{
	m_LabelVisibility = val;
}

vtkTextProperty * blVtkBullsEyeActor::GetTitleTextProperty() const
{
	return m_TitleTextProperty;
}

vtkTextProperty * blVtkBullsEyeActor::GetLabelTextProperty() const
{
	return m_LabelTextProperty;
}

void blVtkBullsEyeActor::SetArrayName( const char* arrayName )
{
	m_strArrayName = arrayName;
}

vtkDataArray * blVtkBullsEyeActor::GetInputArray()
{
	vtkDataArray *dataArray = NULL;

	if ( GetInput( ) != NULL && 
		GetInput( )->GetFieldData() != NULL && 
		GetInput( )->GetFieldData()->GetNumberOfArrays() > 0 && 
		GetInput( )->GetFieldData()->GetArray( 0 ) )
	{
		if ( m_strArrayName.empty() )
		{
			dataArray = GetInput( )->GetFieldData()->GetArray( 0 );
		}
		else
		{
			dataArray = GetInput( )->GetFieldData()->GetArray( m_strArrayName.c_str() );
		}

		// If number of values is 0 -> return NULL
		if ( dataArray->GetNumberOfTuples( ) == 0 )
		{
			dataArray = NULL;
		}
	}

	return dataArray;
}

void blVtkBullsEyeActor::UseInputArrayScalarRange( bool val )
{
	m_bUseInputArrayScalarRange = val;
}

void blVtkBullsEyeActor::PlaceAnnotations( vtkViewport *viewport )
{
	float fOffestX = 0.02 * m_PlotSize[ 0 ];
	float fOffestY = 0.02 * m_PlotSize[ 1 ];

	m_TextActorVector[ 0 ]->SetPosition( m_Center[ 0 ], m_Center[ 1 ] + m_Radius + fOffestY );

	int fontSize2 = 0;
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 1 ]->ComputeScaledFont( viewport );
		fontSize2 = m_TextActorVector[ 1 ]->GetScaledTextProperty()->GetFontSize();
	#endif
	m_TextActorVector[ 1 ]->SetPosition( m_Center[ 0 ] + m_Radius + fontSize2 + fOffestX, m_Center[ 1 ] );

	int fontSize1 = 0;
	#if VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2
		m_TextActorVector[ 2 ]->ComputeScaledFont( viewport );
		fontSize1 = m_TextActorVector[ 2 ]->GetScaledTextProperty()->GetFontSize();
	#endif
	m_TextActorVector[ 2 ]->SetPosition( m_Center[ 0 ], m_Center[ 1 ] - m_Radius - fontSize1 - fOffestY );

	m_TextActorVector[ 3 ]->SetPosition( m_Center[ 0 ] - m_Radius - fOffestX, m_Center[ 1 ] );

}

vtkScalarsToColors* blVtkBullsEyeActor::GetVtkDefaultLut() const
{
	return m_vtkDefaultLut;
}

void blVtkBullsEyeActor::SetVtkDefaultLut( vtkScalarsToColors* val )
{
	m_vtkDefaultLut = val;
	m_LegendActor->SetLookupTable( m_vtkDefaultLut );
	m_WebMapperCells->SetLookupTable( m_vtkDefaultLut );
}

void blVtkBullsEyeActor::SetNumberOfDecimals( long numberOfDecimals )
{
	m_NumberOfDecimals = numberOfDecimals;
}

