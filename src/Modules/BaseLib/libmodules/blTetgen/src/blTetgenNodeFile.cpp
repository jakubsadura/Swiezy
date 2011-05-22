// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// own header-file
#include "blTetgenNodeFile.h"

// BaseLib-includes
#include "blMeshTypes.h"
#include "blReaders.h"
#include "blCommonHelpers.h"

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>


// imports from other modules
using base::meshio::staticArrayReader;
using base::meshio::stringify;
using base::meshio::dVector3D;
using base::meshio::iDataReaderStaticRA;
using base::meshio::iDataReaderStaticI;


using namespace base::tetgen;


// public function: documentation in corresponding .h file
readNodeFile::readNodeFile(std::istream &input) {
	// read number of points
	int numPoints = -1;
	input >> numPoints;

	// guard: check for invalid input
	if (!input.good() || numPoints < 0) {
		throw readNodeNumberOfPointsIncorrectException();
	}

	// initialize duplicate-check-array
	bool *defined = new bool[numPoints];
	for (int i = 0; i < numPoints; i++) {
		defined[i] = false;
	}

	// drop 3 integer values
	int drop;
	input >> drop >> drop >> drop;

	// create array and fill it
	m_points = new dVector3D[numPoints];
	for (int i = 0; i < numPoints; i++) {
		// read index
		int index;
		input >> index;
		// guard: check for invalid input
		if (!input.good() || index < 1 || index > numPoints) {
			input.clear(); // repair stream so that it can tell us its position
			throw readNodePointIndexIncorrectException(input.tellg(), numPoints);
		}

		// guard: check, if element already defined
		if (defined[index - 1]) {
			throw readNodePointAlreadyDefinedException(input.tellg(), index);
		}

		// read point
		input >> m_points[index - 1];
		defined[index - 1] = true;
		// guard: check for invalid input
		if (!input.good()) {
			input.clear(); // repair stream so that it can tell us its position
			throw readNodePointIncorrectException(input.tellg(), i);
		}
	}

	m_reader = new staticArrayReader<dVector3D>(m_points, numPoints);

	delete[] defined;
}

// public function: documentation in corresponding .h file
readNodeFile::~readNodeFile() {
	delete m_points;
}

// public function: documentation in corresponding .h file
iDataReaderStaticRA<dVector3D> * readNodeFile::getPointReader() {
	return m_reader;
}


// public function: documentation in corresponding .h file
readNodeFileException::readNodeFileException(int position) {
	m_position = position;
	m_what = "Error reading .node file at position " + stringify(position);
}

// public function: documentation in corresponding .h file
int readNodeFileException::getPosition() {
	return m_position;
}

// public function: documentation in corresponding .h file
const char* readNodeFileException::what() const throw() {
	return m_what.c_str();
}

// public function: documentation in corresponding .h file
readNodeNumberOfPointsIncorrectException::readNodeNumberOfPointsIncorrectException()
: readNodeFileException(0)
{
	m_what = "A non-negative integer is expected as number of points at the beginning of the file.";
}

// public function: documentation in corresponding .h file
readNodePointIncorrectException::readNodePointIncorrectException(int position, int pointIndex)
: readNodeFileException(position)
{
	m_pointIndex = pointIndex;
	m_what = "Three floating-point numbers are expected for every point; error at point with index "
		+ stringify(m_pointIndex) + " at position " + stringify(position) + " in .node file.";
}

// public function: documentation in corresponding .h file
int readNodePointIncorrectException::getPointIndex() {
	return m_pointIndex;
}

// public function: documentation in corresponding .h file
readNodePointIndexIncorrectException::readNodePointIndexIncorrectException(int position, int maxIndex)
: readNodeFileException(position)
{
	m_what = "Wrong point index! Index must be an integer number between 1 and "
		+ stringify(maxIndex) + " at position " + stringify(position) + " in .node file.";
}

// public function: documentation in corresponding .h file
readNodePointAlreadyDefinedException::readNodePointAlreadyDefinedException(int position, int pointIndex)
: readNodeFileException(position) {
	m_pointIndex = pointIndex;
	m_what = "The point with number " + stringify(pointIndex) + " is defined a second time at position "
		+ stringify(position) + " in .node file.";
}

// public function: documentation in corresponding .h file
int readNodePointAlreadyDefinedException::getPointIndex() {
	return m_pointIndex;
}


// public function: documentation in corresponding .h file
void writeNodeFile::write(iDataReaderStaticI<dVector3D> *nodes, std::string filename) {
    // open file
    std::ofstream outFile(filename.c_str());

    // write header
    outFile << nodes->getNumSpaceElements() << " 0 0 0\n";

    // write every node
    nodes->reset();
    while (nodes->hasNext()) {
        int id;
        dVector3D node = nodes->readNextData(&id);
        outFile << (id + 1) << "   " << node << "\n";
    }

    // file automatically closed in ofstream-destructor at return from this method
}

