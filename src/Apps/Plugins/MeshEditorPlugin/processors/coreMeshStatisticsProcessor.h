#ifndef coreMESHSTATISTICSPROCESSOR_H
#define coreMESHSTATISTICSPROCESSOR_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "corePluginMacros.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "coreDataEntityHelper.txx"

#include "meMeshStatistics.h"

#include "blSignalCollective.h"
#include "blSignal.h"


namespace Core{

/**
\ingroup MeshEditorPlugin
*/
class Quality
{
public:
	Quality(me::MeshStatistics::QualityType _qualityType, bool _selected=false)
	{
		m_qualityType = _qualityType;
		m_selected = _selected;
		m_averageValue = 0;
		m_histogramData = NULL;
	}

public:
	me::MeshStatistics::QualityType m_qualityType;
	bool m_selected;
	double m_averageValue;
	vtkDataObject* m_histogramData;
};

/**
\ingroup MeshEditorPlugin
*/
struct StatisticsData
{
	unsigned int numberOfCells;
	double averageArea;
	double averageAspectRadio;
	double averageEdgeRadio;
	double averageRadiusRadio;
	double averageDistortion;
	double averageTetraMinAngle;
	double averageTetraAspectRadio;
	double averageTetraEdgeRadio;

};


/**
\ingroup MeshEditorPlugin
*/
struct EnabledQualities
{
	bool averageArea;
	bool averageAspectRadio;
	bool averageEdgeRatio;
	bool averageRadiusRatio;
	bool averageDistortion;
	bool averageTetraMinAngle;
	bool averageTetraAspectRadio;
	bool averageTetraEdgeRatio;

};

typedef boost::shared_ptr<StatisticsData> StatisticsDataPtr;
typedef vtkSmartPointer< vtkDataObject > VtkDataObjectPtr;
typedef std::pair<std::string, Quality*> QualityStringPair;
typedef std:: map <std::string, Quality*> QualityStringMap;


/**
This class retrieves desired mesh statistics.
\ingroup MeshEditorPlugin
\author Chiara Riccobene (from previous class of Jakub Lyko)
\date  13 jan 2010
*/
class PLUGIN_EXPORT MeshStatisticsProcessor : public Core::BaseProcessor
{
public:
	//!
	coreDeclareSmartPointerClassMacro(MeshStatisticsProcessor, Core::BaseProcessor);
	//!
	void Update();
	
	//! quality string map
	QualityStringMap qualityStringMap;

	unsigned int GetNumberofBins(void);
	void SetNumberofBins(unsigned int textnumberofbins );

private:
	//!
	MeshStatisticsProcessor ();

private:
	
	//! output statistics
 	StatisticsDataPtr statisticsData;
	//! output histogram data
	VtkDataObjectPtr histogramData;

	unsigned int numberofbins;

	//blSignalCollective::Pointer signal;
};

} // MeshStatistics

#endif //MESHSTATISTICSPROCESSOR_H
