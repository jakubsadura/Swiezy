// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#ifndef BLTetgenNodeFile_H
#define BLTetgenNodeFile_H


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
using base::meshio::iDataReaderStaticI;
using base::meshio::iDataReaderStaticRA;
using base::meshio::dVector3D;


//! Superclass for exceptions that can be thrown reading .node files
class readNodeFileException : public std::exception {
protected:
	//! position in the input stream at which the error occurred
	int m_position;

	//! Detailed human-readable description of the error (has to be written by the constructor of the subclass)
	std::string m_what;

public:
	//! Constructor storing the error position in the input stream
	readNodeFileException(int position);

	//!
	virtual ~readNodeFileException() throw() {};

	//! Position in the input stream at which the error occurred
	int getPosition();

	//! Detailed human-readable description of the error
	virtual const char* what() const throw();
};


//! Exception thrown, if the number of nodes at the beginning of the file could not be read
class readNodeNumberOfPointsIncorrectException : public readNodeFileException {
public:
	//!
	readNodeNumberOfPointsIncorrectException();
};


//! Exception thrown, if the coordinates of a point could not be read
class readNodePointIncorrectException : public readNodeFileException {
private:
	// Index of the point, whose coordinates could not be read
	int m_pointIndex;

public:
	//! Constructor storing the error position in the input stream and the index of the erronous point
	readNodePointIncorrectException(int position, int pointIndex);

	// Index of the point, whose coordinates could not be read
	int getPointIndex();
};


//! Exception thrown, if the index of a point could not be read at all or is invalid (non-positive or too high)
class readNodePointIndexIncorrectException : public readNodeFileException {
public:
	//! Constructor storing the error position and the maximum allowed value for the read index
	readNodePointIndexIncorrectException(int position, int maxIndex);
};


//! Exception thrown, if the point with the same index is defined twice
class readNodePointAlreadyDefinedException : public readNodeFileException {
private:
	//! Index of the doubly-defined point
	int m_pointIndex;

public:
	//! Constructor storing the error position and the index of the doubly defined point
	readNodePointAlreadyDefinedException(int position, int pointIndex);

	//! Returns the index of the doubly-defined point
	int getPointIndex();
};


//! Reading of .node files
class readNodeFile {

private:
	//! Array of the read point coordinates
	dVector3D *m_points;

	//! Reader for the point coordinate array
	iDataReaderStaticRA<dVector3D> *m_reader;

public:
	//! Constructor reading a .node file from a given input stream
	readNodeFile(std::istream &input);

	//!
	~readNodeFile();

	/*
	Returns the reader for the read point coordinates from the .node file.
	Returns always the same reader object; it exists as long as the readNodeFile object exists.
	*/
	iDataReaderStaticRA<dVector3D> *getPointReader();
};


class writeNodeFile {
public:
	void write(iDataReaderStaticI<dVector3D> *nodes, std::string filename);
};


} // tetgen
} // base


#endif // BLTetgenNodeFile_H
