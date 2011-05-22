// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// own header-file
#include "blReaders.h"

// BaseLib-includes
// ...none at the moment...

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <cassert>
#include <iostream>


using namespace base::meshio;


// if the compiler doesn't support the "export" keyword, then it is removed from the file
// and a set of pre-defined instances of the template defined (see also end of this file)
#ifndef USE_EXPORT_KEYWORD
  #define export /*nothing*/
#endif


// public function: documentation in corresponding .h file
position2D::position2D(int numMajorElements, int numMinorElements) {
	m_numMajorElements = numMajorElements;
	m_numMinorElements = numMinorElements;
	m_numElements = numMajorElements * numMinorElements;
	reset();
}

// public function: documentation in corresponding .h file
position2D::position2D() {
	m_numMajorElements = 0;
	m_numMinorElements = 0;
	m_numElements = 0;
	reset();
}

// public function: documentation in corresponding .h file
void position2D::reset() {
	m_majorPosition = 0;
	m_minorPosition = 0;
	m_position = 0;
}

// public function: documentation in corresponding .h file
void position2D::advance() {
	m_position++;
	m_minorPosition++;
	if (m_minorPosition >= m_numMinorElements) {
		m_majorPosition++;
		m_minorPosition = 0;
	}
}

// public function: documentation in corresponding .h file
void position2D::advance(int *out_position) {
	*out_position = m_position;
	m_position++;
	m_minorPosition++;
	if (m_minorPosition >= m_numMinorElements) {
		m_majorPosition++;
		m_minorPosition = 0;
	}
}

// public function: documentation in corresponding .h file
void position2D::advance(int *out_majorPosition, int *out_minorPosition) {
	*out_minorPosition = m_minorPosition;
	*out_majorPosition = m_majorPosition;
	m_position++;
	m_minorPosition++;
	if (m_minorPosition >= m_numMinorElements) {
		m_majorPosition++;
		m_minorPosition = 0;
	}
}

// public function: documentation in corresponding .h file
int position2D::getMajorPosition() {
	return m_majorPosition;
}

// public function: documentation in corresponding .h file
int position2D::getMinorPosition() {
	return m_minorPosition;
}

// public function: documentation in corresponding .h file
int position2D::getPosition() {
	return m_position;
}

// public function: documentation in corresponding .h file
int position2D::getNumMajorElements() {
	return m_numMajorElements;
}

// public function: documentation in corresponding .h file
int position2D::getNumMinorElements() {
	return m_numMinorElements;
}

// public function: documentation in corresponding .h file
int position2D::getNumElements() {
	return m_numElements;
}

// public function: documentation in corresponding .h file
bool position2D::hasNext() {
	return m_majorPosition < m_numMajorElements;
}

// public function: documentation in corresponding .h file
export template<typename D>
iDataReaderStaticRAPosition<D>::iDataReaderStaticRAPosition(int numElements, int *externalPosition) {
	m_numElements = numElements;
	if (externalPosition == NULL) {
		m_position = new int;
		m_deletePosition = true;
		reset();
	} else {
		m_position = externalPosition;
		m_deletePosition = false;
	}
}

// public function: documentation in corresponding .h file
export template<typename D>
iDataReaderStaticRAPosition<D>::~iDataReaderStaticRAPosition() {
    if (m_deletePosition) {
        delete m_position;
    }
}

// public function: documentation in corresponding .h file
export template<typename D>
int iDataReaderStaticRAPosition<D>::getNumSpaceElements() const {
	return m_numElements;
}

// public function: documentation in corresponding .h file
export template<typename D>
void iDataReaderStaticRAPosition<D>::reset() {
	*m_position = 0;
}

// public function: documentation in corresponding .h file
export template<typename D>
bool iDataReaderStaticRAPosition<D>::hasNext() const {
	return *m_position < m_numElements;
}

// public function: documentation in corresponding .h file
export template<typename D>
D iDataReaderStaticRAPosition<D>::readNextData(int *out_id) {
	*out_id = *m_position;
	return this->readData((*m_position)++);
}

// public function: documentation in corresponding .h file
export template<typename D>
iDataReaderDynamicRAPosition<D>::iDataReaderDynamicRAPosition(position2D *externalPosition) {
	m_position = externalPosition;
	m_deletePosition = false;
}

// public function: documentation in corresponding .h file
export template<typename D>
iDataReaderDynamicRAPosition<D>::~iDataReaderDynamicRAPosition() {
    if (m_deletePosition) {
        delete m_position;
    }
}

// public function: documentation in corresponding .h file
export template<typename D>
iDataReaderDynamicRAPosition<D>::iDataReaderDynamicRAPosition(int numTimeElements, int numSpaceElements) {
	m_position = new position2D(numTimeElements, numSpaceElements);
	m_deletePosition = true;
}

// public function: documentation in corresponding .h file
export template<typename D>
int iDataReaderDynamicRAPosition<D>::getNumSpaceElements() const {
	return m_position->getNumMinorElements();
}

// public function: documentation in corresponding .h file
export template<typename D>
int iDataReaderDynamicRAPosition<D>::getNumTimeSteps() const {
	return m_position->getNumMajorElements();
}

// public function: documentation in corresponding .h file
export template<typename D>
void iDataReaderDynamicRAPosition<D>::reset() {
	m_position->reset();
}

// public function: documentation in corresponding .h file
export template<typename D>
bool iDataReaderDynamicRAPosition<D>::hasNext() const {
	return m_position->hasNext();
}

// public function: documentation in corresponding .h file
export template<typename D>
D iDataReaderDynamicRAPosition<D>::readNextData(int *out_time, int *out_id) {
	m_position->advance(out_time, out_id);
	return this->readData(*out_time, *out_id);
}

// public function: documentation in corresponding .h file
export template<typename D>
dynamicArrayReader<D>::dynamicArrayReader(const D *data, int numTimeSteps, int numSpaceElements)
: iDataReaderDynamicRAPosition<D>(numTimeSteps, numSpaceElements) {
	m_data = data;
	m_numSpaceElements = numSpaceElements;
}

// public function: documentation in corresponding .h file
export template<typename D>
dynamicArrayReader<D>::dynamicArrayReader(const D *data, position2D *externalPosition)
: iDataReaderDynamicRAPosition<D>(externalPosition) {
	m_data = data;
	m_numSpaceElements = externalPosition->getNumMinorElements();
}

// public function: documentation in corresponding .h file
export template<typename D>
D dynamicArrayReader<D>::readNextData(int *out_time, int *out_id) {
	D result = m_data[this->m_position->getPosition()];
	this->m_position->advance(out_time, out_id);
	return result;
}

// public function: documentation in corresponding .h file
export template<typename D>
D dynamicArrayReader<D>::readData(int time, int id) const {
	return m_data[time * m_numSpaceElements + id];
}

// public function: documentation in corresponding .h file
export template<typename D>
staticArrayReader<D>::staticArrayReader(const D *data, int numElements, int *externalPosition)
: iDataReaderStaticRAPosition<D>(numElements, externalPosition) {
	m_data = data;
}

// public function: documentation in corresponding .h file
export template<typename D>
D staticArrayReader<D>::readNextData(int *out_id) {
	*out_id = *this->m_position;
	return m_data[(*this->m_position)++];
}

// public function: documentation in corresponding .h file
export template<typename D>
D staticArrayReader<D>::readData(int id) const {
	return m_data[id];
}

export template <typename D>
staticConstantReader<D>::staticConstantReader(D value, int num, int *externalPosition)
: iDataReaderStaticRAPosition<D>(num, externalPosition) {
    this->value = value;
}

export template <typename D>
D staticConstantReader<D>::readData(int id) const {
    return value;
}

staticIDReader::staticIDReader(int offset, int num, int *externalPosition)
: iDataReaderStaticRAPosition<int>(num, externalPosition) {
    this->m_offset = offset;
}

int staticIDReader::readData(int id) const {
    return id + m_offset;
}


export template <typename D>
staticPermutedReader<D>::staticPermutedReader(iDataReaderStaticRA<int> *permutation, iDataReaderStaticRA<D> *values)
: iDataReaderStaticRAPosition<D>(permutation->getNumSpaceElements()) {
    this->m_permutation = permutation;
    this->m_values = values;
}

export template <typename D>
D staticPermutedReader<D>::readData(int id) const {
    return m_values->readData(m_permutation->readData(id));
}


export template <typename D>
dynamicPermutedReader<D>::dynamicPermutedReader(iDataReaderStaticRA<int> *permutation, iDataReaderDynamicRA<D> *values)
: iDataReaderDynamicRAPosition<D>(values->getNumTimeSteps(), permutation->getNumSpaceElements()) {
    this->m_permutation = permutation;
    this->m_values = values;
}

export template <typename D>
D dynamicPermutedReader<D>::readData(int time, int id) const {
    return m_values->readData(time, m_permutation->readData(id));
}






// f***ing "separate compilation model" and compilers that don't support template-exports bully us
// to restrict the range of template-instantiations to a finite set:

#ifndef USE_EXPORT_KEYWORD

#include "blMeshTypes.h"
//#include "csPkjeFile.h"
#include "boost/cstdint.hpp" // workaround because of missing "stdint.h" (C++ include) in some compilers

template class dynamicArrayReader<float>;
template class dynamicArrayReader<double>;
template class dynamicArrayReader<bool>;
template class dynamicArrayReader<char>;
template class dynamicArrayReader<short>;
template class dynamicArrayReader<int>;
template class dynamicArrayReader<boost::uint8_t>;
template class dynamicArrayReader<boost::uint16_t>;
template class dynamicArrayReader<boost::uint32_t>;
template class dynamicArrayReader<boost::uint64_t>;
template class dynamicArrayReader<boost::int8_t>;
//template class dynamicArrayReader<boost::int16_t>;
//template class dynamicArrayReader<boost::int32_t>;
template class dynamicArrayReader<boost::int64_t>;
template class dynamicArrayReader<void*>;
template class dynamicArrayReader<char*>;
template class dynamicArrayReader<std::string>;
template class dynamicArrayReader<dVector3D>;
template class dynamicArrayReader<tetrahedron>;
template class dynamicArrayReader<triangle>;
template class dynamicArrayReader<line>;
//template class dynamicArrayReader<CaSi::Carp::pkjecable*>;

template class iDataReaderDynamicRAPosition<float>;
template class iDataReaderDynamicRAPosition<double>;
template class iDataReaderDynamicRAPosition<bool>;
template class iDataReaderDynamicRAPosition<char>;
template class iDataReaderDynamicRAPosition<short>;
template class iDataReaderDynamicRAPosition<int>;
template class iDataReaderDynamicRAPosition<boost::uint8_t>;
template class iDataReaderDynamicRAPosition<boost::uint16_t>;
template class iDataReaderDynamicRAPosition<boost::uint32_t>;
template class iDataReaderDynamicRAPosition<boost::uint64_t>;
template class iDataReaderDynamicRAPosition<boost::int8_t>;
//template class iDataReaderDynamicRAPosition<boost::int16_t>;
//template class iDataReaderDynamicRAPosition<boost::int32_t>;
template class iDataReaderDynamicRAPosition<boost::int64_t>;
template class iDataReaderDynamicRAPosition<void*>;
template class iDataReaderDynamicRAPosition<char*>;
template class iDataReaderDynamicRAPosition<std::string>;
template class iDataReaderDynamicRAPosition<dVector3D>;
template class iDataReaderDynamicRAPosition<tetrahedron>;
template class iDataReaderDynamicRAPosition<triangle>;
template class iDataReaderDynamicRAPosition<line>;
//template class iDataReaderDynamicRAPosition<CaSi::Carp::pkjecable*>;


template class staticArrayReader<float>;
template class staticArrayReader<double>;
template class staticArrayReader<bool>;
template class staticArrayReader<char>;
template class staticArrayReader<short>;
template class staticArrayReader<int>;
template class staticArrayReader<boost::uint8_t>;
template class staticArrayReader<boost::uint16_t>;
template class staticArrayReader<boost::uint32_t>;
template class staticArrayReader<boost::uint64_t>;
template class staticArrayReader<boost::int8_t>;
//template class staticArrayReader<boost::int16_t>;
//template class staticArrayReader<boost::int32_t>;
template class staticArrayReader<boost::int64_t>;
template class staticArrayReader<void*>;
template class staticArrayReader<char*>;
template class staticArrayReader<std::string>;
template class staticArrayReader<dVector3D>;
template class staticArrayReader<tetrahedron>;
template class staticArrayReader<triangle>;
template class staticArrayReader<line>;
//template class staticArrayReader<CaSi::Carp::pkjecable*>;

template class staticConstantReader<float>;
template class staticConstantReader<double>;
template class staticConstantReader<bool>;
template class staticConstantReader<char>;
template class staticConstantReader<short>;
template class staticConstantReader<int>;
template class staticConstantReader<boost::uint8_t>;
template class staticConstantReader<boost::uint16_t>;
template class staticConstantReader<boost::uint32_t>;
template class staticConstantReader<boost::uint64_t>;
template class staticConstantReader<boost::int8_t>;
//template class staticConstantReader<boost::int16_t>;
//template class staticConstantReader<boost::int32_t>;
template class staticConstantReader<boost::int64_t>;
template class staticConstantReader<void*>;
template class staticConstantReader<char*>;
template class staticConstantReader<std::string>;
template class staticConstantReader<dVector3D>;
template class staticConstantReader<tetrahedron>;
template class staticConstantReader<triangle>;
template class staticConstantReader<line>;
//template class staticConstantReader<CaSi::Carp::pkjecable*>;

template class iDataReaderStaticRAPosition<float>;
template class iDataReaderStaticRAPosition<double>;
template class iDataReaderStaticRAPosition<bool>;
template class iDataReaderStaticRAPosition<char>;
template class iDataReaderStaticRAPosition<short>;
template class iDataReaderStaticRAPosition<int>;
template class iDataReaderStaticRAPosition<boost::uint8_t>;
template class iDataReaderStaticRAPosition<boost::uint16_t>;
template class iDataReaderStaticRAPosition<boost::uint32_t>;
template class iDataReaderStaticRAPosition<boost::uint64_t>;
template class iDataReaderStaticRAPosition<boost::int8_t>;
//template class iDataReaderStaticRAPosition<boost::int16_t>;
//template class iDataReaderStaticRAPosition<boost::int32_t>;
template class iDataReaderStaticRAPosition<boost::int64_t>;
template class iDataReaderStaticRAPosition<void*>;
template class iDataReaderStaticRAPosition<char*>;
template class iDataReaderStaticRAPosition<std::string>;
template class iDataReaderStaticRAPosition<dVector3D>;
template class iDataReaderStaticRAPosition<tetrahedron>;
template class iDataReaderStaticRAPosition<triangle>;
template class iDataReaderStaticRAPosition<line>;
//template class iDataReaderStaticRAPosition<CaSi::Carp::pkjecable*>;

template class dynamicPermutedReader<double>;

template class staticPermutedReader<double>;

// if your compiler does support the "export" keyword, then please define the USE_EXPORT_KEYWORD symbol

#endif // USE_EXPORT_KEYWORD
