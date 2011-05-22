/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"

#include "wxMitkHistogramHelper.h"
#include "mitkImageAccessByItk.h"
#include "mitkImageTimeSelector.h"

using namespace mitk;
InstantiateAccessFunction_1(
	wxMitkHistogramHelper::InternalGenerateGradientHistogram, 
	wxMitkHistogramHelper::InternalHistogramType::ConstPointer&);



void CopyHistogram(mitk::HistogramGenerator::HistogramType& target, 
				   const wxMitkHistogramHelper::InternalHistogramType& source)
{
  // Initialize the target.
  mitk::HistogramGenerator::HistogramType::MeasurementVectorType minTarget, maxTarget;
  //minTarget.SetSize( 2 );
  //maxTarget.SetSize( 2 );

  mitk::HistogramGenerator::HistogramType::SizeType size;
  size.m_Size[ 0 ] = source.GetSize().GetElement( 0 );

  for (unsigned int i = 0; i < minTarget.Size(); i++)
    {
    minTarget[i] = source.GetBinMin(i, 0);
    }
  
  for (unsigned int i = 0; i < maxTarget.Size(); i++)
    {
    maxTarget[i] = source.GetBinMax(i, size[i] - 1);
    }

  target.Initialize(size, minTarget, maxTarget);

  // Copy the values.
  wxMitkHistogramHelper::InternalHistogramType::ConstIterator sourceIt = source.Begin();
  wxMitkHistogramHelper::InternalHistogramType::ConstIterator sourceEnd = source.End();
  mitk::HistogramGenerator::HistogramType::Iterator targetIt = target.Begin();
  mitk::HistogramGenerator::HistogramType::Iterator targetEnd = target.End();

  while (sourceIt != sourceEnd && targetIt != targetEnd)
    {
    wxMitkHistogramHelper::InternalHistogramType::AbsoluteFrequencyType 
      freq = sourceIt.GetFrequency();
      
    if (freq > 0)
      {
      targetIt.SetFrequency(freq);
      }
      
    ++sourceIt;
    ++targetIt;
    }
}

mitk::HistogramGenerator::HistogramType::ConstPointer
wxMitkHistogramHelper::GenerateGradientHistogram(mitk::Image::Pointer image)
{
	wxMitkHistogramHelper::InternalHistogramType::ConstPointer histogram;
	const_cast<mitk::Image*>(image.GetPointer())->SetRequestedRegionToLargestPossibleRegion(); //@todo without this, Image::GetScalarMin does not work for dim==3 (including sliceselector!)
	const_cast<mitk::Image*>(image.GetPointer())->Update();
	mitk::ImageTimeSelector::Pointer timeSelector=mitk::ImageTimeSelector::New();
	timeSelector->SetInput( image );
	timeSelector->SetTimeNr( 0 );
	timeSelector->UpdateLargestPossibleRegion();
	AccessByItk_1( timeSelector->GetOutput() , InternalGenerateGradientHistogram, histogram);

	//InternalHistogramType::SizeType size = histogram->GetSize( );

	mitk::HistogramGenerator::HistogramType::Pointer histogramOut;
	histogramOut = mitk::HistogramGenerator::HistogramType::New( );
	CopyHistogram( *histogramOut, *histogram );

    mitk::HistogramGenerator::HistogramType::ConstPointer result(histogramOut);
    return result;
}


mitk::HistogramGenerator::HistogramType::ConstPointer mitk::wxMitkHistogramHelper::ComputeHistogram( 
	const mitk::Image * image )
{
	wxMitkHistogramHelper::InternalHistogramType::ConstPointer histogram;

	const_cast<mitk::Image*>(image)->SetRequestedRegionToLargestPossibleRegion(); //@todo without this, Image::GetScalarMin does not work for dim==3 (including sliceselector!)
	const_cast<mitk::Image*>(image)->Update();
	mitk::ImageTimeSelector::Pointer timeSelector=mitk::ImageTimeSelector::New();
	timeSelector->SetInput(image);
	timeSelector->SetTimeNr( 0 );
	timeSelector->UpdateLargestPossibleRegion();
	AccessByItk_1( timeSelector->GetOutput() , InternalComputeHistogram, histogram);

	mitk::HistogramGenerator::HistogramType::Pointer histogramOut;
	histogramOut = mitk::HistogramGenerator::HistogramType::New( );
	CopyHistogram( *histogramOut, *histogram );

    mitk::HistogramGenerator::HistogramType::ConstPointer result(histogramOut);
    return result;
}
