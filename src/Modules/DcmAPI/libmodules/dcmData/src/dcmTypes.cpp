/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmTypes.h"

std::string dcmAPI::CreateDateFromRawDcmTagDate(const std::string& rawDate)
{
	std::string strDate("");
	if(rawDate.size() == 8)
	{
		char buff[80];
		tm date;

		date.tm_mday = atoi(rawDate.substr(6,2).c_str()); //1-31
		date.tm_mon =  atoi(rawDate.substr(4,2).c_str()) - 1; //0-11
		date.tm_year = atoi(rawDate.substr(0,4).c_str()) - 1900; // since 1900
		strftime(buff, 80, "%d %m %Y", &date);
		strDate = buff;
	}
	return strDate;
}

std::ostream& operator<< (std::ostream& stream, const dcmAPI::TagId& tagId)
{
    char res[10];
    sprintf(res,"%04x|%04x", tagId.m_group, tagId.m_element);

    stream << "[" << res << "] \"" << tagId.m_description << "\"";
    return stream;
}

