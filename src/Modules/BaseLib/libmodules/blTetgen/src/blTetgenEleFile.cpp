// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// own header-file
#include "blTetgenEleFile.h"

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
#include <locale>


// imports from other modules
using base::meshio::staticArrayReader;
using base::meshio::stringify;
using base::meshio::tetrahedron;
using base::meshio::iDataReaderStaticRA;
using base::meshio::iDataReaderStaticI;


using namespace base::tetgen;


// public function: documentation in corresponding .h file
readEleFile::readEleFile(std::istream &input) {
	// read number of elements
	int numElements = -1;
	input >> numElements;

	// guard: check for invalid input
	if (!input.good() || numElements < 0) {
		throw readEleNumberOfElementsIncorrectException();
	}

	// drop 2 integer values
	int drop;
	input >> drop >> drop;

	// initialize duplicate-check-array
	bool *defined = new bool[numElements];
	for (int i = 0; i < numElements; i++) {
		defined[i] = false;
	}

	// create array and fill it
	m_elements = new tetrahedron[numElements];
	for (int i = 0; i < numElements; i++) {
		// read index
		int index;
		input >> index;
		// guard: check for invalid index
		if (!input.good() || index < 1 || index > numElements) {
			input.clear(); // repair stream so that it can tell us its position
			throw readEleElementIndexIncorrectException(input.tellg(), numElements);
		}

		// guard: check, if element already defined
		if (defined[index - 1]) {
			throw readEleElementAlreadyDefinedException(input.tellg(), index);
		}

		// read tetrahedron
		input >> m_elements[index - 1];
		defined[index - 1] = true;
		// guard: check for invalid points
		if (!input.good() || m_elements[index - 1].m_cornerA < 1 || m_elements[index - 1].m_cornerB < 1
			|| m_elements[index - 1].m_cornerC < 1 || m_elements[index - 1].m_cornerD < 1) {
			input.clear(); // repair stream so that it can tell us its position
			throw readElePointIndicesIncorrectException(input.tellg(), index);
		}

		// adapt indices (with tetgen they start at 1)
		m_elements[index - 1].m_cornerA--;
		m_elements[index - 1].m_cornerB--;
		m_elements[index - 1].m_cornerC--;
		m_elements[index - 1].m_cornerD--;
	}

	m_elementsReader = new staticArrayReader<tetrahedron>(m_elements, numElements);

	delete[] defined;
}

// public function: documentation in corresponding .h file
readEleFile::~readEleFile() {
	delete[] m_elements;
	delete m_elementsReader;
}

// public function: documentation in corresponding .h file
iDataReaderStaticRA<tetrahedron> * readEleFile::getElementsReader() {
	return m_elementsReader;
}



// public function: documentation in corresponding .h file
readEleFileException::readEleFileException(int position) {
	m_position = position;
	m_what = "Error reading .ele file at position " + stringify(position);
}

// public function: documentation in corresponding .h file
int readEleFileException::getPosition() {
	return m_position;
}

// public function: documentation in corresponding .h file
const char* readEleFileException::what() const throw() {
	return m_what.c_str();
}

// public function: documentation in corresponding .h file
readEleNumberOfElementsIncorrectException::readEleNumberOfElementsIncorrectException()
: readEleFileException(0)
{
	m_what = "A non-negative integer is expected as number of elements at the beginning of the file.";
}

// public function: documentation in corresponding .h file
readElePointIndicesIncorrectException::readElePointIndicesIncorrectException(int position, int elementIndex)
: readEleFileException(position)
{
	m_elementIndex = elementIndex;
	m_what = "Four correct point indices (>= 1) are expected for every element; error at element-line with index "
		+ stringify(elementIndex) + " at position " + stringify(position)
		+ " in .ele file: Couldn't read the four indices.";
}

// public function: documentation in corresponding .h file
int readElePointIndicesIncorrectException::getElementIndex() {
	return m_elementIndex;
}

// public function: documentation in corresponding .h file
readEleElementIndexIncorrectException::readEleElementIndexIncorrectException(int position, int maxTetraIndex)
: readEleFileException(position)
{
	m_what = "The element index must be a number between 1 and " + stringify(maxTetraIndex)
		+ ". This condition is not met at position " + stringify(position);
}

// public function: documentation in corresponding .h file
readEleElementAlreadyDefinedException::readEleElementAlreadyDefinedException(int position, int elementIndex)
: readEleFileException(position) {
	m_elementIndex = elementIndex;
	m_what = "The element with number " + stringify(elementIndex) + " is defined a second time at position "
		+ stringify(position) + " in .ele file.";
}

// public function: documentation in corresponding .h file
int readEleElementAlreadyDefinedException::getElementIndex() {
	return m_elementIndex;
}


// public function: documentation in corresponding .h file
void writeEleFile::write(iDataReaderStaticI<tetrahedron> *elements, std::string filename) {
    // open file
    std::ofstream outFile(filename.c_str());

    // write header
    outFile << elements->getNumSpaceElements() << " 0 0\n";

    // write every tetra
    elements->reset();
    while (elements->hasNext()) {
        int id;
        tetrahedron tetra = elements->readNextData(&id);
        outFile << (id + 1) << "   " << (tetra.m_cornerA + 1) << " " << (tetra.m_cornerB + 1) << " "
            << (tetra.m_cornerC + 1) << " " << (tetra.m_cornerD + 1) << "\n";
    }

    // file automatically closed in ofstream-destructor at return from this method
}


