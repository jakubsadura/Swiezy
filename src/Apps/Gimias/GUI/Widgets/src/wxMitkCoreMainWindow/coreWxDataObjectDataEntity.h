/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWxDataObjectDataEntity_H
#define coreWxDataObjectDataEntity_H

#include "gmWidgetsWin32Header.h"
#include "coreDataEntity.h"

#include <wx/clipbrd.h>
#include <wx/dataobj.h>

namespace Core
{

/**
 * \brief A wxDataObject represents data that can be copied to or from the 
 * clipboard, or dragged and dropped.
 *
 * Internal DataEntity is used increasing reference count
 *
 * \ingroup gmWidgets
 * \author Xavi Planes
 * \date 26 Nov 2008
 */
class GMWIDGETS_EXPORT wxDataObjectDataEntity : public wxDataObject
{
public:
	//!
	wxDataObjectDataEntity( );

	//!
	static const wxChar *GetFormatId( );

	//!
	wxDataFormat GetPreferredFormat(Direction WXUNUSED(dir)) const;

	//!
	size_t GetFormatCount(Direction dir) const;

	//! Convert from buf to Core::DataEntity* and set m_DataEntity to this
	bool SetData(const wxDataFormat& format, size_t len, const void *buf);

	//!
	Core::DataEntity::Pointer GetDataEntity( );

	//! m_DataEntity = data
	void SetDataEntity( Core::DataEntity::Pointer data );

	//!
	void GetAllFormats(wxDataFormat *formats, Direction dir) const;

	//!
    size_t GetDataSize(const wxDataFormat& format) const;

	//! set pBuf to the pointer m_DataEntity
    bool GetDataHere(const wxDataFormat& format, void *pBuf) const;

private:
	//! Format of this data
	wxDataFormat        m_format;

	//! Internal data entity
	Core::DataEntity::Pointer m_DataEntity;
};

} // Core namespace

#endif

