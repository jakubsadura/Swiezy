/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreLightData_H
#define coreLightData_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include "boost/filesystem.hpp"

namespace Core{

/** 
\brief A LightData is a light representation of DataEntity. It is used in
Session class to store essential information about data entity which later
can be stored in xml session file

\author Jakub Lyko
\date 10 Dec 2009
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT LightData
{
public:	
	//!
	LightData();

	//!
	virtual ~LightData();
	
	//!
	std::string GetName() const { return m_name; }

	//!
	void SetName(std::string val) { m_name = val; }
	
	//!
	std::string GetFilepath() const { return m_filepath; }

	//!
	std::string GetRelativePath() const { return m_relativePath; }

	//!
	unsigned int GetNumOfTimeSteps() const { return std::atoi(m_numOfTimeSteps.c_str());}

	//!
	void SetFilepath(std::string val) { m_filepath = val; }

	//!
	void SetNumberOfTimeSteps(std::string val) { m_numOfTimeSteps = val; }

	//!
	std::vector<LightData*> GetChildren() {return m_childrenList;}

	//!
	void AddChild(LightData* child);

	//! give access to private members for serialization
	friend class boost::serialization::access;

private:
	template<class Archive> void serialize(Archive &ar, const unsigned int version);

private:
	//!
	std::string m_name;

	//!
	std::string m_filepath; 

	//!
	std::string m_relativePath;

	//!
	std::string m_numOfTimeSteps;

	//!
	std::vector<LightData*> m_childrenList;
};

template<class Archive> void LightData::serialize(Archive &ar, const unsigned int version)
{
	boost::filesystem::path pathFile(m_filepath);
	m_relativePath =  boost::filesystem::basename(pathFile) 
		+ boost::filesystem::extension(pathFile);

	using boost::serialization::make_nvp;
	ar & make_nvp("Name", m_name);
	ar & make_nvp("TimeSteps", m_numOfTimeSteps );
	ar & make_nvp("Filepath", m_relativePath);
	ar & make_nvp("Children", m_childrenList);
}

} // end namespace Core

#endif