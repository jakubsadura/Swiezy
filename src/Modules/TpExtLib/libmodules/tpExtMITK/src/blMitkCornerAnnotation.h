/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMitkCornerAnnotation_h
#define _blMitkCornerAnnotation_h

#include "TpExtLibMITKWin32Header.h"
#include <mitkBaseData.h>

#include "vtkSmartPointer.h"

class vtkRenderWindow;
class vtkCornerAnnotation;
class vtkOrientationAnnotation;

/**
 * \brief Draw the text for each 2D view: corner and orientation annotations.
 */
class TPEXTLIBMITK_EXPORT blMitkCornerAnnotation : public itk::DataObject
{
public:

	mitkClassMacro( blMitkCornerAnnotation, itk::DataObject );

	itkNewMacro( Self );

	/**
	* Sets the render window, in which the annotations
	* will be shown. Make sure, you have called this function
	* before calling Enable()
	*/
	void SetRenderWindow( vtkRenderWindow* renderWindow );

	//!
	bool GetShowCornerAnnotations() const;
	void SetShowCornerAnnotations(bool val);
	bool GetShowOrientationAnnotations() const;
	void SetShowOrientationAnnotations(bool val);

	//!
	std::string GetNorthAnnotation() const;
	void SetNorthAnnotation(const char* val);
	std::string GetSouthAnnotation() const;
	void SetSouthAnnotation( const char* val);
	std::string GetEastAnnotation() const;
	void SetEastAnnotation( const char* val);
	std::string GetOuestAnnotation() const;
	void SetWestAnnotation( const char* val);

	//!
	std::string GetUpLeftAnnotation() const;
	void SetUpLeftAnnotation( const char* val);
	std::string GetUpRightAnnotation() const;
	void SetUpRightAnnotation( const char* val);
	std::string GetDownLeftAnnotation() const;
	void SetDownLeftAnnotation( const char* val);
	std::string GetDownRightAnnotation() const;
	void SetDownRightAnnotation( const char* val);

protected:

	/**
	* Constructor
	*/
	blMitkCornerAnnotation();

	/**
	* Destructor
	*/
	~blMitkCornerAnnotation();

	//!
	void CreateCornerAnnotations();

	//!
	void UpdateAnnotations();

protected:
	//! Render window
	vtkRenderWindow* m_RenderWindow;

	//! Corner annotations
	std::string m_upLeftAnnotation;
	std::string m_upRightAnnotation;
	std::string m_downLeftAnnotation;
	std::string m_downRightAnnotation;

	//! Orientation annotations
	std::string m_NorthAnnotation;
	std::string m_SouthAnnotation;
	std::string m_EastAnnotation;
	std::string m_OuestAnnotation;

	//!
	vtkSmartPointer<vtkCornerAnnotation> m_CornerAnnotation;

	//!
	vtkSmartPointer<vtkOrientationAnnotation> m_OrientationAnnotation;

	//!
	bool m_bShowCornerAnnotations;

	//!
	bool m_bShowOrientationAnnotations;
};


#endif // _blMitkCornerAnnotation_h
