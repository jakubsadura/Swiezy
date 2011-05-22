/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CGNSFileReaderWidget_H
#define _CGNSFileReaderWidget_H

#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sstream>

#include "blTagMap.h"

#include "CGNSFileReaderWidgetUI.h"
#include "coreBaseWindow.h"

namespace Core{
	namespace Widgets {

		/**
		\author Albert Sanchez
		\date 10 06 2010
		*/

		#define wxID_CGNSFileReaderWidget wxID("wxID_CGNSFileReaderWidget")


		class CGNSFileReaderWidget : 
			public CGNSFileReaderWidgetUI,
			public Core::BaseWindow 
		{
			// OPERATIONS
		public:
			coreDefineBaseWindowReaderFactory( CGNSFileReaderWidget, Core::VolumeMeshTypeId, ".cgns" )

			//!
			CGNSFileReaderWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

			//!
			~CGNSFileReaderWidget( );

			//!
			void CheckBoxClicked(wxCommandEvent& event);

			//!
			void CompressionOrNot(wxCommandEvent& event);

			//!
			Core::BaseProcessor::Pointer GetProcessor( );

			/** Set and Get properties of the window to serialize them
			or visualize in the GUI
			*/
			void SetProperties( blTagMap::Pointer tagMap );
			void GetProperties( blTagMap::Pointer tagMap );


		private:
			DECLARE_EVENT_TABLE()

			// ATTRIBUTES
		private:

			std::vector<wxCheckBox*> checkBoxVector;
			std::vector<std::string> namesVector;
			std::vector<int> originalValues;
			blTagMap::Pointer m_tagMap;

		};

	} //namespace Widgets
} //namespace Core

#endif //_CGNSFileReaderWidget_H
