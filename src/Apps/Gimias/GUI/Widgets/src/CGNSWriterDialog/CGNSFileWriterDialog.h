/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CGNSFileWriterDialog_H
#define _CGNSFileWriterDialog_H

#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sstream>

#include "blTagMap.h"

#include "CGNSFileWriterDialogUI.h"

#include "coreBaseWindow.h"

namespace Core{
	namespace Widgets {

		/**
		\author Albert Sanchez
		\date 10 06 2010
		*/

		#define wxID_CGNSFileWriterDialog wxID("wxID_CGNSFileWriterDialog")


		class CGNSFileWriterDialog : 
			public CGNSFileWriterDialogUI,
			public Core::BaseWindow
		{
			// OPERATIONS
		public:

			//!
			CGNSFileWriterDialog( );

			//!
			~CGNSFileWriterDialog( );

			//!
			virtual Core::BaseProcessor::Pointer GetProcessor( );

			//!
			virtual void SetProperties( blTagMap::Pointer tagMap );
			virtual void GetProperties( blTagMap::Pointer tagMap );

		private:
			DECLARE_EVENT_TABLE()

			//!
			void CheckBoxClicked(wxCommandEvent& event);

			// ATTRIBUTES
		private:

			std::map<int,std::string> m_MapIdToString;
			std::map<int,std::pair<int,bool> > m_MapSelected;
			std::map<int,wxCheckBox*> m_MapIdToCheckBox;

		};

	} //namespace Widgets
} //namespace Core

#endif //_CGNSFileWriterDialog_H
