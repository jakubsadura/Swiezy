/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
/**
* @file viewer.cpp 
* @brief Example DICOM data viewer using itk, vtk and dcmapi loading library and displaying using vtk (vtkImageViewer2).
*/
/**
* \defgroup viewer Viewer
* \ingroup apps
*/
// boost
#include <boost/filesystem.hpp>

// VTK

// Core
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
// Window
#include <vtkImageViewer2.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLegendScaleActor.h>
#include <vtkScalarBarActor.h>
// Interaction
#include <vtkCommand.h>
#include <vtkInteractorObserver.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkImageActor.h>
#include <vtkPointPicker.h>
#include <vtkCallbackCommand.h>
// IO
#include <vtkStructuredPointsWriter.h>
// dicom
#include "vtkDicomRead.h"

// ITK
#include "itkDicomRead.h"

// dcmAPI 
#include "dcmapiDicomRead.h"

/**
* \defgroup utils Utils
* \ingroup viewer
*/

//@{ 


/**
* Converts a number to a string.
* @param value The number to convert.
* @return The string representation of the input.
*/
template< class T >
std::string toString( const T& value )
{
    std::ostringstream oss;
    oss << value;
    return oss.str(); 
}

/**
* Get some pixel information from an image.
* @param pImageData The image data from which to get the pixel info.
* @param index The index at which to get the pixel info.
* @return String with pixel info.
*/
template< class T >
std::string getPixelInfo( vtkSmartPointer<vtkImageData> pImageData, int index, double* point )
{
    std::string text;
    // handle different number of scalar components
    switch (pImageData->GetNumberOfScalarComponents())
    {
        case 1:
            {
                T* pPix = (T*)pImageData->GetScalarPointer();
                T usPix = pPix[index];
                text = "Pixel val: [" + toString(usPix) 
                    + "] at index [" + toString(index) 
                    + "], coordinates(" + toString(point[0]) + "," + toString(point[1]) + ")";
            }
            break;
        
        case 3:
            {
                T* pPix = (T*)pImageData->GetScalarPointer();
                T usPixR = pPix[index * 3 + 0];
                T usPixG = pPix[index * 3 + 1];
                T usPixB = pPix[index * 3 + 2];
                text = "Pixel val: [" + toString(usPixR) + "," + toString(usPixG) + "," + toString(usPixB)
                    + "] at index [" +  toString(index) 
                    + "], coordinates(" +  toString(point[0]) + "," + toString(point[1]) + ")";
            }
            break;

        default:
            text = "Unsupported number of scalar components [" + toString(pImageData->GetNumberOfScalarComponents()) + "] for pixel picker.";
            break;
    }
    return text;
}

/**
* Store image info: slice and time point number.
* Done here cause I could not find an event to listen to in the imageViewer for the slice.
*/
class ImageInfo
{
public:
    /**
    * Constructor.
    * @param imageViewer The image viewer.
    * @param textActor The text actor to update.
    */
    ImageInfo(
        vtkImageViewer2* imageViewer, 
        vtkTextActor* textActor,
        const std::vector< vtkSmartPointer<vtkImageData> >& imageData )
     : m_imageViewer(imageViewer), m_textActor(textActor), m_imageData(imageData),
      m_hasSliceChanged(false), m_hasTimeChanged(false),
      m_sliceNumber(0), m_timePointNumber(0)
    {}
    /**
    * Set the slice number.
    * @param sliceNumber The slice number.
    */
    void SetSliceNumber( unsigned int sliceNumber )
    { 
        const unsigned int sliceMax = m_imageViewer->GetSliceMax();
        const unsigned int sliceMin = m_imageViewer->GetSliceMin();
        unsigned int newSliceNumber = sliceNumber;
        if( sliceNumber > sliceMax )
        {
            newSliceNumber = sliceMax;
        }
        else if( sliceNumber < sliceMin )
        {
            newSliceNumber = sliceMin;
        }
        if( newSliceNumber != m_sliceNumber )
        { 
            m_hasSliceChanged = true;
            m_sliceNumber = newSliceNumber;
        }
    }
    /**
    * Get the slice number.
    * @return sliceNumber The slice number.
    */
    unsigned int GetSliceNumber() const { return m_sliceNumber; }
    /**
    * Set the time point number.
    * @param timePointNumber The time point number.
    */
    void SetTimePointNumber( unsigned int timePointNumber )
    {
        const unsigned int timeMax = m_imageData.size()-1;
        unsigned int newTimePointNumber = timePointNumber;
        if( newTimePointNumber < 0 )
        {
            newTimePointNumber = 0;
        }
        else if( newTimePointNumber > timeMax )
        {
            newTimePointNumber = timeMax;
        }
        if( newTimePointNumber != m_timePointNumber )
        {
            m_hasTimeChanged = true;
            m_timePointNumber = newTimePointNumber;
        }
    }
    /**
    * Get the time point number.
    * @return timePointNumber The time point number.
    */
    unsigned int GetTimePointNumber() const { return m_timePointNumber; }
    /**
    * Update viewer and text actor.
    */
    void Update() 
    {
        if( m_hasSliceChanged )
        {
            // change viewer slice
            m_imageViewer->SetSlice( m_sliceNumber );
            // update actor
            UpdateTextActor();
            // render
            m_imageViewer->Render();
            // update flag
            m_hasSliceChanged = false;
        }
        if( m_hasTimeChanged )
        {
            // change time point
            m_imageViewer->SetInput( m_imageData[m_timePointNumber] );
            // update actor
            UpdateTextActor();
            // render
            m_imageViewer->Render();
            // update flag
            m_hasTimeChanged = false;
        }
    }
    /**
    * Get the image data.
    */
    std::vector< vtkSmartPointer<vtkImageData> > GetImageData() const { return m_imageData; }
private:
    // image viewer
    vtkImageViewer2* m_imageViewer;
    // text actor
    vtkTextActor* m_textActor;
    // image data
    std::vector< vtkSmartPointer<vtkImageData> > m_imageData;
    // flag to know if the slice has changed
    bool m_hasSliceChanged;
    // flag to know if the time has changed
    bool m_hasTimeChanged;
    // slice number
    unsigned int m_sliceNumber;
    // time point number
    unsigned int m_timePointNumber;
    /**
    * Update text actor.
    */
    void UpdateTextActor()
    {
        // update text actor
        std::string text = "Slice: " + toString(m_imageViewer->GetSlice()) + "/" + toString(m_imageViewer->GetSliceMax());
        if( m_imageData.size() > 1 )
        {
            text += "\nTime point: " + toString(m_timePointNumber+1) + "/" + toString(m_imageData.size());
        }
        m_textActor->SetInput(text.c_str());
    }
}; // class ImageInfo

//@}

/**
* \defgroup obs Observers
* \ingroup viewer
*/

//@{ 


/**
* Observer on the mouse wheel. Moves slices when scrolling mouse wheel.
*/
class ObserverMouseWheel : public vtkCommand
{
public:
    /**
    * Constructor.
    * @param imageViewer The iamge viewer.
    * @param textActor The text actor to update.
    */
    ObserverMouseWheel(vtkImageViewer2* imageViewer, const ImageInfo& imageInfo)
     : m_imageViewer(imageViewer), m_imageInfo(imageInfo) {}
    // New
    static ObserverMouseWheel* New(vtkImageViewer2* imageViewer, const ImageInfo& imageInfo)
    { return new ObserverMouseWheel(imageViewer, imageInfo); }
protected:
    // Desctructor
    ~ObserverMouseWheel() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void*)
    {
    	// set the viewer slice
    	unsigned int newSlice = m_imageViewer->GetSlice();
    	switch(eventId) 
    	{
	        case vtkCommand::MouseWheelForwardEvent:
		        newSlice += 1;
		        break;
	        case vtkCommand::MouseWheelBackwardEvent:
		        if(newSlice != 0) newSlice -= 1;
		        break;
	        default:
	            std::cerr << "Unknown event." << std::endl;
		        break;
        }
        m_imageInfo.SetSliceNumber(newSlice);
        m_imageInfo.Update();
    }
    // pointer to the viewer
    vtkImageViewer2* m_imageViewer;
    // image info
    ImageInfo m_imageInfo;
    
}; // class ObserverMouseWheel

/**
* Observer on the mouse move. Gives the pixel value to the text actor.
*/
class ObserverMouseMove : public vtkCommand
{
public:
    // Constructor
    ObserverMouseMove(vtkImageViewer2* imageViewer,
        vtkRenderWindowInteractor* windowInteractor,
        vtkPointPicker* pointPicker,
        vtkTextActor* textActor)
     : m_imageViewer(imageViewer), m_windowInteractor(windowInteractor), m_pointPicker(pointPicker), m_textActor(textActor) {}
    // New
    static ObserverMouseMove *New(vtkImageViewer2* imageViewer,
        vtkRenderWindowInteractor* windowInteractor,
        vtkPointPicker* pointPicker,
        vtkTextActor* textActor) 
    { return new ObserverMouseMove(imageViewer, windowInteractor, pointPicker, textActor); }
protected:
    // Desctructor
    ~ObserverMouseMove() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void*)
    {
        // Do the pick. It will return a non-zero value if we intersected the image.
        if (!m_pointPicker->Pick(m_windowInteractor->GetEventPosition()[0], 
                             m_windowInteractor->GetEventPosition()[1], 
                             0,  // always zero.
                             m_imageViewer->GetRenderer())) 
        {
            m_textActor->SetInput("Mouse is outside of image extent.");
            m_windowInteractor->Render();
            return;
        }

        // Get a shortcut to the pixel data.
        vtkImageData* pImageData = m_imageViewer->GetInput();

        // Get the mapped position of the mouse using the picker.
        double mousePosition[3];
        m_pointPicker->GetMapperPosition(mousePosition);
        // Image spacing
        double spacing[3];
        pImageData->GetSpacing(spacing);
        // Image origin
        double origin[3];
        pImageData->GetOrigin(origin);
        
        // Calculate the point position from the mousePosition
        double point[3];
        point[0] = mousePosition[0];
        point[1] = mousePosition[1];
        point[2] = origin[2] + m_imageViewer->GetSlice() * spacing[2];

        // Get the volume index within the entire volume now.
        const int index = pImageData->FindPoint(point);

        // We have to handle different scalar types.
        std::string text;
        switch( pImageData->GetScalarType() )
        {
            case VTK_CHAR:
                text = getPixelInfo<char>(pImageData, index, point);
                break;
            case VTK_UNSIGNED_CHAR:
                text = getPixelInfo<unsigned char>(pImageData, index, point);
                break;
            case VTK_SHORT:
                text = getPixelInfo<short>(pImageData, index, point);
                break;
            case VTK_UNSIGNED_SHORT:
                text = getPixelInfo<unsigned short>(pImageData, index, point);
                break;
            case VTK_INT:
                text = getPixelInfo<int>(pImageData, index, point);
                break;
            case VTK_UNSIGNED_INT:
                text = getPixelInfo<unsigned int>(pImageData, index, point);
                break;
            case VTK_LONG:
                text = getPixelInfo<long>(pImageData, index, point);
                break;
            case VTK_UNSIGNED_LONG:
                text = getPixelInfo<unsigned long>(pImageData, index, point);
                break;
            case VTK_FLOAT:
                text = getPixelInfo<float>(pImageData, index, point);
                break;
            case VTK_DOUBLE:
                text = getPixelInfo<double>(pImageData, index, point);
                break;
            default:
                std::cerr << "[ERROR] Unknown scalar type." << std::endl;
                break;
        }
        
        // Update the info text with pixel coordinates/value if requested.
        if (m_textActor) {
            m_textActor->SetInput(text.c_str());
            m_windowInteractor->Render();
        }
    }
    // pointer to the image viewer
    vtkImageViewer2* m_imageViewer;
    // pointer to the window interactor
    vtkRenderWindowInteractor* m_windowInteractor;
    // pointer to the point picker
    vtkPointPicker* m_pointPicker;
    // pointer to the text actor
    vtkTextActor* m_textActor;
    
}; // class ObserverMouseMove
 
// Window postition flag.
enum WindowPositionFlag {
    POS_UPPER_LEFT,
    POS_UPPER_RIGHT,
    POS_LOWER_RIGHT,
    POS_LOWER_LEFT
}; // enum

/**
* Observer on the window. Updates text actor positions.
*/
class ObserverWindowModified : public vtkCommand
{
public:
    /**
    * Constructor.
    * @param textActor The text actor to update.
    * @param positionFlag The position flag (see WindowPositionFlag).
    */
    ObserverWindowModified(vtkTextActor* textActor, int positionFlag)
     : m_textActor(textActor), m_positionFlag(positionFlag), m_marginX(10), m_marginY(10) {}
    // New
    static ObserverWindowModified* New(vtkTextActor* textActor, int positionFlag)
    { return new ObserverWindowModified(textActor, positionFlag); }
protected:
    // Desctructor
    ~ObserverWindowModified() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void*)
    {
        vtkRenderWindow* window = static_cast<vtkRenderWindow*>(caller);
        int* windowSize = window->GetSize();
        //std::cout << "Size: " << windowSize[0] << " " << windowSize[1] << std::endl;
        switch (m_positionFlag)
        {
            case POS_UPPER_LEFT:
                m_textActor->GetTextProperty()->SetVerticalJustificationToTop();
                m_textActor->SetDisplayPosition(m_marginX, windowSize[1] - m_marginY);
                break;
            case POS_UPPER_RIGHT:
                m_textActor->GetTextProperty()->SetVerticalJustificationToTop();
                m_textActor->GetTextProperty()->SetJustificationToRight();
                m_textActor->SetDisplayPosition(windowSize[0] - m_marginX, windowSize[1] - m_marginY);
                break;
            case POS_LOWER_LEFT:
                m_textActor->SetDisplayPosition(m_marginX, m_marginY);
                break;
            case POS_LOWER_RIGHT:
                m_textActor->GetTextProperty()->SetJustificationToRight();
                m_textActor->SetDisplayPosition(windowSize[0] - m_marginX, m_marginY);
                break;
            default:
                std::cerr << "Unknown window position." << std::endl;
                break;
        }
    }
    // pointer to the text actor
    vtkTextActor* m_textActor;
    // position flag in the window
    int m_positionFlag;
    // X margin
    int m_marginX;
    // Y margin
    int m_marginY;
    
}; // class ObserverWindowModified

/**
* Observer on the window/level. Updates text actor values.
*/
class ObserverWindowLevel : public vtkCommand
{
public:
    /**
    * Constructor.
    * @param imageViewer The image viewer.
    * @param textActor The text actor to update.
    */
    ObserverWindowLevel(vtkImageViewer2* imageViewer, vtkTextActor* textActor)
     : m_imageViewer(imageViewer), m_textActor(textActor) {}
    // New
    static ObserverWindowLevel* New(vtkImageViewer2* imageViewer, vtkTextActor* textActor)
    { return new ObserverWindowLevel(imageViewer, textActor); }
protected:
    // Desctructor
    ~ObserverWindowLevel() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void*)
    {
        const std::string text = "W: " + toString((int)m_imageViewer->GetColorWindow()) 
            + ", L: " + toString((int)m_imageViewer->GetColorLevel());
        m_textActor->SetInput(text.c_str());
        // render
        m_imageViewer->Render();
    }
    // pointer to the image viewer
    vtkImageViewer2* m_imageViewer;
    // pointer to the text actor
    vtkTextActor* m_textActor;
    
}; // class ObserverWindowLevel

/**
* Observer on pressed keys.
*/
class ObserverPressKey : public vtkCommand
{
public:
    /**
    * Constructor.
    * @param imageViewer The image viewer.
    * @param textActors The text actors (t: display on/off).
    * @param imageInfo The image info (move slice and time point).
    */
    ObserverPressKey(vtkImageViewer2* imageViewer, 
        const std::vector< vtkSmartPointer<vtkTextActor> >& textActors,
        const ImageInfo& imageInfo)
     : m_imageViewer(imageViewer), m_textActors(textActors), m_imageInfo(imageInfo),
       m_isScalarBarShown(false), m_canScalarBarBeShown(false), 
       m_isLegendScaleShown(false), m_areTextActorsShown(true)
     {
        // lut
        m_lut = vtkSmartPointer<vtkLookupTable>::New();
        // scalar bar
        m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        m_scalarBar->SetWidth(0.05);
        m_scalarBar->SetHeight(0.9);
        m_scalarBar->GetPositionCoordinate()->SetValue(0.90, 0.10);
        // scale legend            
        m_legendScaleActor = vtkSmartPointer<vtkLegendScaleActor>::New();
     }
    // New
    static ObserverPressKey* New(vtkImageViewer2* imageViewer, 
        const std::vector< vtkSmartPointer<vtkTextActor> >& textActors,
        const ImageInfo& imageInfo)
    { return new ObserverPressKey(imageViewer, textActors, imageInfo); }
protected:
    // Desctructor
    ~ObserverPressKey() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void*)
    {
        vtkSmartPointer<vtkRenderWindowInteractor> windowInteactor =
            vtkRenderWindowInteractor::SafeDownCast( caller );
        const std::string keySim( windowInteactor->GetKeySym() );
        const bool isCrtl = windowInteactor->GetControlKey() == 0 ? false : true;
        // background color
        if (keySim == "b")
        {
            double bg[3];
            m_imageViewer->GetRenderer()->GetBackground(bg);
            if( bg[0] == 0.5 && bg[1] == 0.5 && bg[2] == 0.5 )
            {
                m_imageViewer->GetRenderer()->SetBackground(0,0,0);
            }
            else if( bg[0] == 0 && bg[1] == 0 && bg[2] == 0 )
            {
                m_imageViewer->GetRenderer()->SetBackground(0.5,0.5,0.5);
            }
            m_imageViewer->Render();
        }
        // help: list of shortcuts and mouse moves
        else if (keySim == "h")
        {
            std::cout << std::endl;
            std::cout << "Keyboard shortcut list:" << std::endl;
            std::cout << "-----------------------" << std::endl;
            std::cout << " [b]: turn backgroud color black/grey" << std::endl;
            std::cout << " [e]: exit (same as q)" << std::endl; //extern
            std::cout << " [h]: help" << std::endl;
            std::cout << " [i]: turn interpolation on/off" << std::endl;
            std::cout << " [l]: display/hide scale legend" << std::endl;
            std::cout << " [q]: quit (same as e)" << std::endl; //extern
            std::cout << " [r]: reset window/level" << std::endl; //extern
            std::cout << " [SHIFT+r]: reset zoom" << std::endl; //extern
            std::cout << " [s]: display/hide scalar bar" << std::endl;
            std::cout << " [CRTL+s]: save image(s) as VTK" << std::endl;
            std::cout << " [t]: display/hide text" << std::endl;
            std::cout << " [Arrow Up]: increment slice" << std::endl;
            std::cout << " [Arrow Down]: decrement slice" << std::endl;
            std::cout << " [Arrow Prior (PgUp)]: increment*10 slice" << std::endl;
            std::cout << " [Arrow Next (PgDn)]: decrement*10 slice" << std::endl;
            std::cout << " [Arrow Right]: increment timepoint" << std::endl;
            std::cout << " [Arrow Left]: decrement timepoint" << std::endl;
            std::cout << " [1]: default color map" << std::endl;
            std::cout << " [2]: value color map" << std::endl;
            std::cout << " [3]: pink???, press again to reset" << std::endl; //extern
            std::cout << " [4]: hue color map (full circle)" << std::endl;
            std::cout << " [5]: hue color map (half circle)" << std::endl;
            std::cout << " [6]: saturation color map" << std::endl;
            std::cout << " [7]: value color map on CT range" << std::endl;
            std::cout << std::endl;
            std::cout << "Mouse moves:" << std::endl;
            std::cout << "------------" << std::endl;
            std::cout << " [left + move horizontal]: change window width" << std::endl;
            std::cout << " [left + move vertical]: change window level" << std::endl;
            std::cout << " [middle scroll]: change slice" << std::endl;
            std::cout << " [middle + move]: move image" << std::endl;
            std::cout << " [right + move up]: zoom forward" << std::endl;
            std::cout << " [right + move down]: zoom backward" << std::endl;
        }
        // interpolation
        else if (keySim == "i")
        {
            if (m_imageViewer->GetImageActor()->GetInterpolate() == 1 )
            {
                m_imageViewer->GetImageActor()->InterpolateOff();
            }
            else
            {
                m_imageViewer->GetImageActor()->InterpolateOn();
            }
            m_imageViewer->Render();
        }
        // display legend scale
        else if (keySim == "l")
        {
            if( !m_isLegendScaleShown )
            {
                // add it to the renderer
                m_imageViewer->GetRenderer()->AddViewProp(m_legendScaleActor);
                // update flag
                m_isLegendScaleShown = true;
                // render
                m_imageViewer->Render();
            }
            else
            {
                // remove it from the renderer
                m_imageViewer->GetRenderer()->RemoveViewProp(m_legendScaleActor);
                // update flag
                m_isLegendScaleShown = false;
                // render
                m_imageViewer->Render();
            }
        }
        // display scalar bar
        else if (keySim == "s" && !isCrtl)
        {
            if( m_lut )
            {
                if( m_canScalarBarBeShown )
                {
                    if( !m_isScalarBarShown )
                    {
                        m_scalarBar->SetLookupTable(m_lut);
                        // add it to the renderer
                        m_imageViewer->GetRenderer()->AddViewProp(m_scalarBar);
                        // update flag
                        m_isScalarBarShown = true;
                    }
                    else
                    {
                        // remove it from the renderer
                        m_imageViewer->GetRenderer()->RemoveViewProp(m_scalarBar);
                        // update flag
                        m_isScalarBarShown = false;
                    }
                }
            }
            else
            {
                std::cerr << "No lut created yet..." << std::endl;
            }
        }
        // save
        else if (keySim == "s" && isCrtl)
        {
            const std::string filenameRoot = "dcm";
            std::string filename;
            std::string number;
            // writer
            vtkSmartPointer< vtkStructuredPointsWriter > writer = 
                vtkSmartPointer< vtkStructuredPointsWriter >::New();
            writer->SetFileTypeToBinary();
            // Observe file loading progress
            vtk::ObserverFileIO* observeProgress = vtk::ObserverFileIO::New();
            observeProgress->SetIsLoading(false);
            //writer->AddObserver(vtkCommand::StartEvent, observeProgress); // sent twice?
            writer->AddObserver(vtkCommand::ProgressEvent, observeProgress);
            writer->AddObserver(vtkCommand::EndEvent, observeProgress); // sent twice?
            // Observe errors and warnings while loading the images
            vtk::ObserverEvent* observeError = vtk::ObserverEvent::New();
            writer->AddObserver(vtkCommand::ErrorEvent, observeError);
            vtk::ObserverEvent* observeWarning = vtk::ObserverEvent::New();
            writer->AddObserver(vtkCommand::WarningEvent, observeWarning);
            for( unsigned int i = 0; i < m_imageInfo.GetImageData().size(); ++i )
            {
                // file name
                number = "_";
                if( i < 10 ) number += "00";
                else if( i < 100 ) number += "0";
                number += toString(i);
                filename = filenameRoot + number + ".vtk";
                std::cout << "[vtk] Saving to: " << filename << std::endl;
                writer->SetFileName( filename.c_str() );
                // image data
                writer->SetInput( m_imageInfo.GetImageData()[i] );
                // save
                try
                {
                    writer->Update();
                }
                catch( std::exception& err )
                {
                    std::cerr << "Exception caught!" << std::endl;
                    std::cerr << err.what() << std::endl;
                }
            }
            // clean
            observeProgress->Delete();
            observeError->Delete();
            observeWarning->Delete();
        }
        // display text actors
        else if (keySim == "t")
        {
            if( !m_areTextActorsShown )
            {
                // add it to the renderer
                for( unsigned int i = 0; i < m_textActors.size(); ++i )
                {
                    m_imageViewer->GetRenderer()->AddViewProp(m_textActors[i]);
                }
                // update flag
                m_areTextActorsShown = true;
                // render
                m_imageViewer->Render();
            }
            else
            {
                // remove it from the renderer
                for( unsigned int i = 0; i < m_textActors.size(); ++i )
                {
                    m_imageViewer->GetRenderer()->RemoveViewProp(m_textActors[i]);
                }
                // update flag
                m_areTextActorsShown = false;
                // render
                m_imageViewer->Render();
            }
        }
        // timepoint change
        else if( keySim == "Left" || keySim == "Right" )
        {
            unsigned int newTimePoint = m_imageInfo.GetTimePointNumber();
            if (keySim == "Left")
            {
                if( newTimePoint != 0 ) newTimePoint -= 1;
            }
            else if (keySim == "Right")
            {
                newTimePoint += 1;
            }
            // this will verify if the new value is valid
            m_imageInfo.SetTimePointNumber(newTimePoint);
            m_imageInfo.Update();
        }
        // slice change
        else if( keySim == "Up" || keySim == "Down" || keySim == "Prior" || keySim == "Next" )
        {
            unsigned int newSlice = m_imageInfo.GetSliceNumber();
            if (keySim == "Up")
            {
                newSlice += 1;
            }
            else if (keySim == "Down")
            {
                if( newSlice != 0 ) newSlice -= 1;
            }
            else if (keySim == "Prior")
            {
                newSlice += 10;
            }
            else if (keySim == "Next")
            {
                if( newSlice >= 10 ) newSlice -= 10;
                else newSlice = 0;
            }
            // this will verify if the new value is valid
            m_imageInfo.SetSliceNumber(newSlice);
            m_imageInfo.Update();
        }
        // color maps
        else if( keySim == "1" || keySim == "2" || keySim == "4" || keySim == "5" || keySim == "6" || keySim == "7" )
        {
            // color map: default
            if (keySim == "1")
            {
                // update scalar bar
                if( m_isScalarBarShown )
                {
                    // remove it from the renderer
                    m_imageViewer->GetRenderer()->RemoveViewProp(m_scalarBar);
                    // update flag
                    m_isScalarBarShown = false;
                }
                // update flag
                m_canScalarBarBeShown = false;
                // set null lut
                m_imageViewer->GetWindowLevel()->SetLookupTable(NULL);
                m_imageViewer->Render();
            }
            // others
            else
            {    
                // update flag
                m_canScalarBarBeShown = true;
                // Get a shortcut to the pixel data.
                double bounds[2];
                m_imageViewer->GetInput()->GetScalarRange(bounds);

                // setup lut
                m_lut = vtkSmartPointer<vtkLookupTable>::New();
                m_lut->SetTableRange(bounds[0], bounds[1]);
                m_lut->SetNumberOfTableValues(256);
                    
                // color map: value lut
                if (keySim == "2")
                {
                    m_lut->SetHueRange(0, 0);
                    m_lut->SetSaturationRange(0, 0);
                    m_lut->SetValueRange(0, 1);
                }
                // color map: hue lut (rainbow)
                else if (keySim == "4")
                {
                    m_lut->SetHueRange(0, 0.667);
                    m_lut->SetSaturationRange(1, 1);
                    m_lut->SetValueRange(1, 1);
                }
                // color map: hue lut (invert rainbow)
                else if (keySim == "5")
                {
                    m_lut->SetHueRange(0.667, 1);
                    m_lut->SetSaturationRange(1, 1);
                    m_lut->SetValueRange(1, 1);
                }
                // color map: saturation lut
                else if (keySim == "6")
                {
                    m_lut->SetHueRange(.6, .6);
                    m_lut->SetSaturationRange(0, 1);
                    m_lut->SetValueRange(1, 1);
                } 
                // color map: CT range
                else if (keySim == "7")
                {
                    m_lut->SetTableRange( 0, 5120 ); //-1024.0, 4096.0
                    m_lut->SetHueRange(0, 0);
                    m_lut->SetSaturationRange(0, 0);
                    m_lut->SetValueRange(0, 1);
                } 
                
                // build the lut
                m_lut->Build();
                // update scalar bar
                if( m_isScalarBarShown )
                {
                    m_scalarBar->SetLookupTable(m_lut);
                    // add it to the renderer
                    m_imageViewer->GetRenderer()->AddViewProp(m_scalarBar);
                }
                // set viewer lut
                m_imageViewer->GetWindowLevel()->SetLookupTable(m_lut);
                m_imageViewer->Render();
            }
        }   
    }
    // pointer to the image viewer
    vtkImageViewer2* m_imageViewer;
    // list of text actors
    std::vector< vtkSmartPointer<vtkTextActor> > m_textActors;
    // image info
    ImageInfo m_imageInfo;
    
    // lookup table
    vtkSmartPointer<vtkLookupTable> m_lut;
    // scalar bar
    vtkSmartPointer<vtkScalarBarActor> m_scalarBar;
    // flag to know if the scalar bar is displayed
    bool m_isScalarBarShown;
    // flag to know if the lut supports scalar bar
    bool m_canScalarBarBeShown;
    // legend scale
    vtkSmartPointer<vtkLegendScaleActor> m_legendScaleActor;
    // flag to know if the legend scale is displayed
    bool m_isLegendScaleShown;
    // flag to know if text actors are displayed
    bool m_areTextActorsShown;
    
}; // class ObserverPressKey


//@}

 
/**
* Simple dicom image viewer.
* @param argv[1] The path to the DICOM data.
* @param argv[2] The DICOM reader.
*
* @ingroup viewer
*/
int main(int argc, char* argv[])
{
    // process command line
    if( argc < 3 || argc > 4 )
    {
        std::cerr << "Usage: " << argv[0] << " PATH READER OPTION" << std::endl;
        std::cerr << "  PATH: the path to the DICOM data." << std::endl;
        std::cerr << "  READER: the DICOM reader:" << std::endl;
        std::cerr << "   - 'vtk' (file or folder)" << std::endl;
        std::cerr << "   - 'itk' (file or folder)" << std::endl;
        std::cerr << "   - 'dcmapi' (file, folder or DICOMDIR)." << std::endl;
        std::cerr << "  OPTION: string option for dcmapi DICOM reading:" << std::endl;
        std::cerr << "   - 'quironct': Quiron CT, using a truncated UID as time separator" << std::endl;
        std::cerr << "   - 'quironmr': Quiron MR, using the phase number as time separator" << std::endl;
        std::cerr << "   - 'quironmr2': Quiron MR2, using the acquisition time as time separator" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string inputStr = argv[1];
    const std::string reader = argv[2];
    dcmapi::ReadOption option = dcmapi::DEFAULT;
    if( argc > 3 && reader == "dcmapi")
    {
        const std::string option_str = argv[3];
        if( option_str == "quironct")
        {
            option = dcmapi::QUIRON_CT;
        }
        else if( option_str == "quironmr")
        {  
            option = dcmapi::QUIRON_MR;
        }
        else if( option_str == "quironmr2")
        {  
            option = dcmapi::QUIRON_MR2;
        }
    }
    
    // -----------------
    // Load
    // -----------------
    std::vector< vtkSmartPointer<vtkImageData> > imageData;
    const boost::filesystem::path inputPath(inputStr);
    
    // exit if path does not exist
    if( !boost::filesystem::exists(inputPath) )
    { 
        std::cerr << "[ERROR] Input file/path does not exist: " << inputStr << std::endl;
        return EXIT_FAILURE;
    }

    // vtk
    if( reader == "vtk" )
    {
        imageData = vtk::ReadDicom(inputPath);
    }
    // itk
    else if( reader == "itk" )
    {
	    // get the file list
	    std::vector<std::string> fileList = itk::GetDicomFilesList(inputStr);
	    // use GDCMImageIO to obtain pizel type from the first file of the list
	    itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
	    gdcmImageIO->SetFileName(fileList[0]);
	    gdcmImageIO->ReadImageInformation();
        itk::ImageIOBase::IOComponentType pixelType = gdcmImageIO->GetComponentType();
        // read the data
    	imageData = itk::ReadDicomFiles(fileList, pixelType);
    }
    // dcmAPI
    else if( reader == "dcmapi" )
    {
	    // get the file list
	    std::vector< std::vector<std::string> > fileList = dcmapi::GetDicomFilesList(inputStr, option);
	    // use GDCMImageIO to obtain pizel type from the first file of the list
	    itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
	    gdcmImageIO->SetFileName(fileList[0][0]);
	    gdcmImageIO->ReadImageInformation();
        itk::ImageIOBase::IOComponentType pixelType = gdcmImageIO->GetComponentType();
        // read the data
        const bool reorient = false;
        std::string protocol;
        gdcmImageIO->GetValueFromTag("0018|1030", protocol);
        const bool isTaggedMR = (protocol.find("3DTAG") != std::string::npos);
        imageData = dcmapi::ReadDicomFiles(fileList, reorient, isTaggedMR, pixelType);
    }
    else
    {
        std::cerr << "Unknow DICOM reader: " << reader << std::endl;
        return EXIT_FAILURE;
    }

    // check loaded data
    if( imageData.size() == 0 )
    {
        std::cerr << "Problem while reading data, pointer is NULL. " << std::endl;
        return EXIT_FAILURE;
    }
    
    // info on the data (displayed on top left)
    std::string info = "Path: " + inputStr;
    int dimensions[3];
    imageData[0]->GetDimensions(dimensions);
    info += "\nDimensions: " + toString(dimensions[0]) + ", " + toString(dimensions[1]) + ", " + toString(dimensions[2]);
    double spacing[3];
    imageData[0]->GetSpacing(spacing);
    info += "\nSpacing: " + toString(spacing[0]) + ", " + toString(spacing[1]) + ", " + toString(spacing[2]);
    double origin[3];
    imageData[0]->GetOrigin(origin);
    info += "\nOrigin: " + toString(origin[0]) + ", " + toString(origin[1]) + ", " + toString(origin[2]);
    
    // -----------------
    // View
    // -----------------
  
    // create point picker (to get pixel value)
    vtkSmartPointer<vtkPointPicker> pointPicker = 
        vtkSmartPointer<vtkPointPicker>::New();
    // set the point picker
    pointPicker->SetTolerance(1e-6); // otherwise strange behaviour at borders
    
    // create the window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor = 
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    // set the window interactor
    windowInteractor->SetPicker(pointPicker);

    // create the viewer
    vtkSmartPointer<vtkImageViewer2> imageViewer = 
        vtkSmartPointer<vtkImageViewer2>::New();
    // set the viewer
    imageViewer->SetInput(imageData[0]);
    imageViewer->SetupInteractor(windowInteractor);
    const int windowSizeX = 640;
    const int windowSizeY = 480;
    imageViewer->GetRenderWindow()->SetSize(windowSizeX, windowSizeY);
    
    // Text actors
    
    // create the info text actor
    vtkSmartPointer<vtkTextActor> infoTextActor = 
        vtkSmartPointer<vtkTextActor>::New();
    // set the text mapper
    infoTextActor->SetInput(info.c_str());
    infoTextActor->GetTextProperty()->BoldOn();
    // add it to the renderer
    imageViewer->GetRenderer()->AddActor2D(infoTextActor);

    // create the slice text actor
    vtkSmartPointer<vtkTextActor> sliceTextActor = 
        vtkSmartPointer<vtkTextActor>::New();
    // set the text mapper
    sliceTextActor->GetTextProperty()->BoldOn();
    // add it to the renderer
    imageViewer->GetRenderer()->AddActor2D(sliceTextActor);

    // create the pixel value text actor
    vtkSmartPointer<vtkTextActor> pixelTextActor = 
        vtkSmartPointer<vtkTextActor>::New();
    // set the text mapper
    pixelTextActor->GetTextProperty()->BoldOn();
    // add it to the renderer
    imageViewer->GetRenderer()->AddActor2D(pixelTextActor);

    // create the window/level text actor
    vtkSmartPointer<vtkTextActor> wlTextActor = 
        vtkSmartPointer<vtkTextActor>::New();
    // set the text mapper
    wlTextActor->GetTextProperty()->BoldOn();
    // add it to the renderer
    imageViewer->GetRenderer()->AddActor2D(wlTextActor);
	
    // set initial slice to middle
    const int sliceMin = imageViewer->GetSliceMin();
    const int sliceMax = imageViewer->GetSliceMax();
    const int sliceNumber = (sliceMax-sliceMin) / 2;
    ImageInfo imageInfo( imageViewer, sliceTextActor, imageData );
    imageInfo.SetSliceNumber( sliceNumber );
    imageInfo.SetTimePointNumber( 0 );

	// Observers
	
	// Observe Mouse Wheel Forward Event
    ObserverMouseWheel* observeMouseWheelForward = ObserverMouseWheel::New(imageViewer, imageInfo);
	windowInteractor->GetInteractorStyle()->AddObserver(vtkCommand::MouseWheelForwardEvent, observeMouseWheelForward);
    observeMouseWheelForward->Delete();
    // Observe Mouse Wheel Backward Event
	ObserverMouseWheel* observeMouseWheelBackward = ObserverMouseWheel::New(imageViewer, imageInfo);
	windowInteractor->GetInteractorStyle()->AddObserver(vtkCommand::MouseWheelBackwardEvent, observeMouseWheelBackward);
    observeMouseWheelBackward->Delete();
    // Observe Mouse Move
    ObserverMouseMove* observeMouseMove = ObserverMouseMove::New(imageViewer, windowInteractor, pointPicker, pixelTextActor);
    windowInteractor->AddObserver(vtkCommand::MouseMoveEvent, observeMouseMove);
    observeMouseMove->Delete();
    // Observe Window Size for the info text position
	ObserverWindowModified* observeWindowModified1 = ObserverWindowModified::New(infoTextActor, POS_UPPER_LEFT);
    imageViewer->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, observeWindowModified1);
    observeWindowModified1->Delete();
    // Observe Window Size for the slice text position
	ObserverWindowModified* observeWindowModified2 = ObserverWindowModified::New(sliceTextActor, POS_LOWER_RIGHT);
    imageViewer->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, observeWindowModified2);
    observeWindowModified2->Delete();
    // Observe Window Size for the pixel value text position
	ObserverWindowModified* observeWindowModified3 = ObserverWindowModified::New(pixelTextActor, POS_LOWER_LEFT);
    imageViewer->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, observeWindowModified3);
    observeWindowModified3->Delete();
    // Observe Window Size for the window/level text position
	ObserverWindowModified* observeWindowModified4 = ObserverWindowModified::New(wlTextActor, POS_UPPER_RIGHT);
    imageViewer->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, observeWindowModified4);
    observeWindowModified4->Delete();
    // Observe Window/Level
    ObserverWindowLevel* observeWindowLevel = ObserverWindowLevel::New(imageViewer, wlTextActor);
    windowInteractor->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, observeWindowLevel);
    windowInteractor->GetInteractorStyle()->AddObserver(vtkCommand::ResetWindowLevelEvent, observeWindowLevel);
    observeWindowLevel->Delete();
    // Observe Pressed Keys
    std::vector< vtkSmartPointer<vtkTextActor> > actors;
    actors.push_back(infoTextActor);
    actors.push_back(sliceTextActor);
    actors.push_back(pixelTextActor);
    actors.push_back(wlTextActor);
    ObserverPressKey* observePressKey = ObserverPressKey::New(imageViewer, actors, imageInfo);
    windowInteractor->AddObserver(vtkCommand::KeyPressEvent, observePressKey);
    observePressKey->Delete();
    
    // will try to render, that's why it is after all the observer declarations
    imageInfo.Update();
    
    double bounds[2];
    imageData[0]->GetScalarRange(bounds);
    std::cout << "[vtk] Data Range: " << bounds[0] << " to " << bounds[1] << std::endl;
    std::cout << "[vtk] Data type: " << imageData[0]->GetScalarTypeAsString() << std::endl;
    
    // initial window/level
    const int colorWindow = imageViewer->GetColorWindow();
    const int colorLevel = imageViewer->GetColorLevel();
    const std::string wlText = "W: " + toString(colorWindow) + ", L: " + toString(colorLevel);
    wlTextActor->SetInput(wlText.c_str());
    
    // launch
    windowInteractor->Initialize();
    imageViewer->Render();
    windowInteractor->Start();

    return EXIT_SUCCESS;
}

