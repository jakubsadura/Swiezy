/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxCaptureWindow.h"
#include <wx/wx.h>
#include <wx/string.h>


void wxCaptureWindow( const wxWindow *inputWindow, const std::string filename )
{
	// Get coordinates of window
	wxRect captureRect = wxRect( inputWindow->GetSize( ) );
	captureRect.x = inputWindow->GetScreenPosition( ).x;
	captureRect.y = inputWindow->GetScreenPosition( ).y;

	// Create bitmap
	wxBitmap bitmap(captureRect.width, captureRect.height);
	wxScreenDC dc;
	wxMemoryDC memDC;
	memDC.SelectObject(bitmap);
	memDC.Blit(0, 0, captureRect.width, captureRect.height, &dc, captureRect.x, captureRect.y);
	memDC.SelectObject( wxNullBitmap );

	// Save file
	wxImage image = bitmap.ConvertToImage( );
	image.SaveFile( wxString(filename.c_str(), wxConvUTF8) );
}

void wxCaptureWindowThumbnail( const wxWindow *inputWindow, const std::string filename, int width, int height, bool proportional )
{
	// Get coordinates of window
	wxRect captureRect = wxRect( inputWindow->GetSize( ) );
	captureRect.x = inputWindow->GetScreenPosition( ).x;
	captureRect.y = inputWindow->GetScreenPosition( ).y;

	// Create bitmap
	wxBitmap bitmap(captureRect.width, captureRect.height);
	wxScreenDC dc;
	wxMemoryDC memDC;
	memDC.SelectObject(bitmap);
	memDC.Blit(0, 0, captureRect.width, captureRect.height, &dc, captureRect.x, captureRect.y);
	memDC.SelectObject( wxNullBitmap );

	// Save file
	wxImage image = bitmap.ConvertToImage();

	int newWidth;
	int newHeight;
	
	if (captureRect.width > captureRect.height)
	{
		newWidth = width;

		if (proportional)
		{
			double proportion = (double)width / (double)captureRect.width;
			double newHeightDouble = proportion * (double)captureRect.height;
			newHeight = (int)( newHeightDouble );
		}
		else
		{
			newHeight = height;
		}
	}
	else
	{
		newHeight = height;

		if (proportional)
		{
			double proportion = (double)height / (double)captureRect.height;
			double newWidthDouble = proportion * (double)captureRect.width;
			newWidth = (int)( newWidthDouble );
		}
		else
		{
			newWidth = width;
		}
	}

	image.Rescale(newWidth,newHeight,wxIMAGE_QUALITY_HIGH);
	image.SaveFile( wxString(filename.c_str(), wxConvUTF8) );
}


void wxCaptureWindowsAndMerge( std::vector<wxWindow *> inputWindows, 
							  const std::string filename, 
							  bool rescale,
							  int width, 
							  int height, 
							  bool proportional )
{
	if (inputWindows.size() == 0) return;

	int totalWidth=0;
	int totalHeight=0;

	for (size_t i=0; i<inputWindows.size(); i++)
	{
		// Get coordinates of window
		wxRect captureRectTemp = wxRect( inputWindows[i]->GetSize( ) );
		captureRectTemp.x = inputWindows[i]->GetScreenPosition( ).x;
		captureRectTemp.y = inputWindows[i]->GetScreenPosition( ).y;

		totalWidth = std::max(totalWidth,captureRectTemp.width);
		totalHeight += captureRectTemp.height;
	}


	// Get coordinates of window
	wxRect captureRect = wxRect( inputWindows[0]->GetSize( ) );
	captureRect.x = inputWindows[0]->GetScreenPosition( ).x;
	captureRect.y = inputWindows[0]->GetScreenPosition( ).y;

	wxBitmap bitmap(totalWidth, totalHeight);
	wxScreenDC dc;
	wxMemoryDC memDC;

	int starty=0;

	memDC.SelectObject(bitmap);
	memDC.Blit(0, starty, captureRect.width, captureRect.height, &dc, captureRect.x, captureRect.y);

	starty += captureRect.height;

	if ( 0 < inputWindows.size()-1 )
	{
		memDC.SelectObject( wxNullBitmap );
	}

	for (size_t i=1; i<inputWindows.size(); i++)
	{
		// Get coordinates of window
		captureRect = wxRect( inputWindows[i]->GetSize( ) );
		captureRect.x = inputWindows[i]->GetScreenPosition( ).x;
		captureRect.y = inputWindows[i]->GetScreenPosition( ).y;

		memDC.SelectObject(bitmap);
		memDC.Blit(0, starty, captureRect.width, captureRect.height, &dc, captureRect.x, captureRect.y);

		starty += captureRect.height;

		if ( 0 < inputWindows.size()-1 )
		{
			memDC.SelectObject( wxNullBitmap );
		}
	}

	// Save file
	wxImage image = bitmap.ConvertToImage( );

	if (rescale)
	{
		int newWidth;
		int newHeight;

		if (totalWidth > totalHeight)
		{
			newWidth = width;

			if (proportional)
			{
				double proportion = (double)width / (double)totalWidth;
				double newHeightDouble = proportion * (double)totalHeight;
				newHeight = (int)( newHeightDouble );
			}
			else
			{
				newHeight = height;
			}
		}
		else
		{
			newHeight = height;

			if (proportional)
			{
				double proportion = (double)height / (double)totalHeight;
				double newWidthDouble = proportion * (double)totalWidth;
				newWidth = (int)( newWidthDouble );
			}
			else
			{
				newWidth = width;
			}
		}
		image.Rescale(newWidth,newHeight,wxIMAGE_QUALITY_HIGH);
	}
	
	image.SaveFile( wxString(filename.c_str(), wxConvUTF8) );
}