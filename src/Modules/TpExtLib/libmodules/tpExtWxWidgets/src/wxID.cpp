/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxID.h"
#include <map>
#include <wx/defs.h>

int wxID(const std::string& stringId)
{
	typedef std::map<std::string, int> StringIdToNumberIdMap;
	static StringIdToNumberIdMap stringIdToNumberId;

	int result(-1);
	StringIdToNumberIdMap::iterator it = stringIdToNumberId.find(stringId);
	if( it == stringIdToNumberId.end() )
	{
		result = wxID_HIGHEST + 1 + stringIdToNumberId.size();
		stringIdToNumberId[stringId] = result;
	}
	else
	{
		result = it->second;
	}
	return result;
}
