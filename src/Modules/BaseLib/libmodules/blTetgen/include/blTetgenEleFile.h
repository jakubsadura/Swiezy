// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#ifndef BLTetgenEleFile_H
#define BLTetgenEleFile_H


// BaseLib-includes
#include "blReaders.h"
#include "blMeshTypes.h"

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <string>
#include <exception>
#include <iostream>


namespace base {
namespace tetgen {


// imports from other library modules
using base::meshio::tetrahedron;
using base::meshio::iDataReaderStaticRA;
using base::meshio::iDataReaderStaticI;


//! Superclass for exceptions that can be thrown reading .ele files
class readEleFileException : public std::exception {
protected:
	//! position in the input stream at which the error occurred
	int m_position;

	//! Detailed human-readable description of the error (has to be written by the constructor of the subclass)
	std::string m_what;

public:
	//! Constructor storing the error position in the input stream
	readEleFileException(int position);

	//!
	virtual ~readEleFileException() throw() {};

	//! Position in the input stream at which the error occurred
	int getPosition();

	//! Detailed human-readable description of the error
	virtual const char* what() const throw();
};


//! Exception thrown, if the number of tetras at the beginning of the file could not be read
class readEleNumberOfElementsIncorrectException : public readEleFileException {
public:
	//!
	readEleNumberOfElementsIncorrectException();
};

//! Exception thrown, if the point indices of the corners of a tetrahedron could not be read correctly
class readElePointIndicesIncorrectException : public readEleFileException {
private:
	//! Index of the erronous tetrahedron
	int m_elementIndex;

public:
	//! Constructor storing the error position in the stream and the index of the erronous tetrahedron line
	readElePointIndicesIncorrectException(int position, int elementIndex);

	//! Index of the erronous tetrahedron
	int getElementIndex();
};

//! Exception thrown, if the index of a tetrahedron could not be read or is out of range
class readEleElementIndexIncorrectException : public readEleFileException {
public:
	//! Constructor storing the error position in the stream and the maximum allowed tetrahedron index
	readEleElementIndexIncorrectException(int position, int maxTetraIndex);
};

//! Exception thrown, if a tetrahedron is defined twice
class readEleElementAlreadyDefinedException : public readEleFileException {
private:
	//! Index of the doubly-defined tetrahedron
	int m_elementIndex;

public:
	//! Constructor storing the error position in the stream and the index of the doubly defined tetrahedron
	readEleElementAlreadyDefinedException(int position, int elementIndex);

	//! Index of the doubly-defined tetrahedron
	int getElementIndex();
};


//! Reading of .ele files (restricted to tetrahedra!)
class readEleFile {
private:
	//! Array of the read tetra point indices indices
	tetrahedron *m_elements;

	//! Reader for the tetra array
	iDataReaderStaticRA<tetrahedron> *m_elementsReader;

public:
	//! Constructor reading a .ele file from a given input stream
	readEleFile(std::istream &input);

	//!
	~readEleFile();

	/*
	Returns the reader for the tetrahedra point indices read from the .ele file.
	Returns always the same reader object; it exists as long as the readEleFile object exists.
	*/
	iDataReaderStaticRA<tetrahedron> *getElementsReader();
};


class writeEleFile {
public:
	void write(iDataReaderStaticI<tetrahedron> *elements, std::string filename);
};


} // tetgen
} // base


#endif // BLTetgenEleFile_H
