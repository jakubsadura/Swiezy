/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef MultiLevelROIProcessor_H
#define MultiLevelROIProcessor_H

#include "gmProcessorsWin32Header.h"
#include "itkImage.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreCommonDataTypes.h"
#include "corePluginMacros.h"
#include "coreDataEntityList.h"
#include "coreDataEntityHelper.h"
#include "coreRenderingTree.h"
#include "coreBaseProcessor.h"
#include "coreProcessorFactory.h"
#include "blSignal.h"

#include <map>
#include <vector>


/**
\brief this is the processor for multi level roi
\sa MultiLevelROIPanelWidget
\ingroup gmProcessors
\author Albert Sanchez
\date  June 2010
*/

class MultiLevelROIProcessor : public Core::BaseProcessor
{
public:
	//!
	coreProcessor(MultiLevelROIProcessor, Core::BaseProcessor);

	typedef itk::Image<short,3> ImageTypeSeg;

	typedef enum
	{
		INPUT_ROI,
		NUMBER_OF_INPUTS,
	} INPUT_TYPE;

	typedef enum
	{
		PROPAGATED_ROI,
		SIGNAL,
		NUMBER_OF_OUTPUTS,
	} OUTPUT_TYPE;


	//!
	MultiLevelROIProcessor( );

	//!
	~MultiLevelROIProcessor();

	//! Sets the range. Range must be less than (number of time steps / 2)
	void SetRange(int left, int right);

	//! Propagates the ROI over time
	void Propagate();

	//! Sets the selected level
	void SetSelectedLevel(int selectedLevel);

	//! Creates the signal
	void CreateSignal();

	//! Computes intensity measures
	std::vector<blSignal::Pointer> ComputeIntensityMeasures();

	//! Returns the intensity measures map
	std::map<std::string,double> GetIntensityMeasures( );

	//! Computes intensity measures of a single time step
	void CalculateIntensityMeasures( int timeStep );

	//! domain = 1 => use all volume. domain = 2 => use X current slice. = 3 => Y. = 4 => Z.
	void SetDomain(int domain);

	//!
	void SetPos(int pos);


private:

	//! Purposely not implemented
	MultiLevelROIProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );


private:
	//! Range left
	int m_left;
	//! Range right
	int m_right;

	//! # time steps
	int m_NumberOfTimeSteps;

	//! Image measures
	std::map<std::string,double> m_measuresMap;

	//! Selected level
	int m_selectedLevel;

	//!
	int m_Domain;

	//!
	int m_Pos;


};

#endif //MultiLevelROIProcessor_H

