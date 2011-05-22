/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blMatrix_txx
#define __blMatrix_txx

#include "blMatrix.h"

template <class TElement>
void baselib::WriteToStream( const vnl_matrix<TElement>& m, std::ostream& os )
{
	unsigned _rows = m.rows();
	unsigned _cols = m.cols();

	os.write( (const char*)&_rows, sizeof( _rows ) );
	os.write( (const char*)&_cols, sizeof( _cols ) );

	const TElement* s = m.data_block();
	os.write( (const char*)s, _rows * _cols * sizeof( TElement ) );
};

template <class TElement>
void baselib::ReadFromStream( vnl_matrix<TElement>& m, std::istream& is )
{
	unsigned _rows, _cols;

	is.read( (char*)&_rows, sizeof( _rows ));
	is.read( (char*)&_cols, sizeof( _cols ));
	
	m.set_size(_rows, _cols);

	TElement* s = m.data_block();
	is.read( (char*)s, _rows * _cols * sizeof(TElement) );
};

#endif

