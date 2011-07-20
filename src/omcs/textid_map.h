/*  This file is part of OMCSNetCPP

    OMCSNetCPP is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    OMCSNetCPP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with OMCSNetCPP; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*! \file */

#ifndef _TEXTID_MAP

#define _TEXTID_MAP

#include <map>
#include <string>

#include "semantic_id.h"


//!Associative map of semantic IDs 
class CTextIDMap
{
	public:
		//!Lookup a semantic ID using a textual key
		SemanticID Lookup(const std::string &a_string)
		{
			CStringIDMap_i i;

			i = m_idMap.find(a_string);

			if (m_idMap.end() != i)
			{
				return (*i).second;
			}

			return SEMANTIC_ID_NONE;
		}

		//!Store a semantic ID in the map using a textual key
		const std::string *Store(const std::string  &a_string,
		    	                 const SemanticID    a_id)
		{
			CStringIDMap_i i;

			i = m_idMap.find(a_string);

			if (m_idMap.end() == i)
			{
				i = m_idMap.insert(m_idMap.begin(),
						std::make_pair(a_string, a_id));

				if (m_idMap.end() != i)
				{
					return &((*i).first);
				}
			}
			else
			{
				printf("%s found.. returning null.\n", a_string.c_str());
			}

			return NULL;
		}

		//!Constructor
		CTextIDMap() {}

		//!Destructor
		~CTextIDMap() {}

	private:
		//!STL map of semantic IDs using a STL string as the key
		typedef std::map<std::string, SemanticID> CStringIDMap;
		//!Iterator for semantic ID STL map
		typedef CStringIDMap::iterator		   CStringIDMap_i;
		//!Const iterator for semantic ID STL map
		typedef CStringIDMap::const_iterator	   CStringIDMap_ci;

		//!STL map of semantic IDs
		CStringIDMap m_idMap;
};

#endif

