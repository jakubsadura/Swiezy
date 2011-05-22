/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynLib.h"
#include <stdexcept>
#include <sstream>
#include "dynLibImplWin32.h"
#include "dynLibImplLinux.h"

dynLib::dynLib( )
{
#ifdef _WIN32
	m_Impl = new dynLibImplWin32( );
#else
	m_Impl = new dynLibImplLinux( );
#endif
}

dynLib::~dynLib()
{
	delete m_Impl;
	m_Impl = NULL;
}

std::string dynLib::GetLibraryPath() const
{
	return m_Impl->GetLibraryPath();
}

void dynLib::SetLibraryPath( std::string val )
{
	m_Impl->SetLibraryPath( val );
}

dynLib::SymbolPointer dynLib::FindSymbol(
	const std::string &name )
{
	return m_Impl->FindSymbol( name );
}


bool dynLib::CheckMissingDependencies( 
	const char* filename, 
	const char* path,
	std::string &dllArrayNames )
{
	m_Impl->GetTreeNode( filename, path );

	std::vector<std::string> dllArrayName;
	m_Impl->CheckMissingDlls( dllArrayName );

	std::ostringstream stream;
	for ( unsigned i = 0 ; i < dllArrayName.size() ; i++ )
	{
		stream << dllArrayName[ i ];
		if ( i != dllArrayName.size() - 1 )
			stream << ", ";
	}
	stream << std::endl;
	dllArrayNames = stream.str();

	return true;

}
