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

#ifndef _LINK_MAP

#define _LINK_MAP

#include <map>
#include <string>

#include "link.h"
#include "link_list.h"

class CFindPathConfig;
class CTextIDMap;

//!STL map of semantic links using a numeric ID as a key
typedef std::map<SemanticID, CSemanticLink *>	CSemanticLinkMap;

//!Iterator for the semantic link STL map
typedef CSemanticLinkMap::iterator		CSemanticLinkMap_i;
//!Const iterator for the semantic link STL map
typedef CSemanticLinkMap::const_iterator   	CSemanticLinkMap_ci;

//!Associative map of semantic links
class CLinkMap
{
	public:
		//!Loads the contents of the map from a previously opened file
		bool Load(FILE        *a_fp,
			  CConceptMap &a_conceptMap);

		//!Saves the contents of the map to a previously opened file
		bool Save(FILE *a_fp)
		{
			if (!a_fp)
			{
				return false;
			}

			u_int l_size = m_linkMap.size();

			fwrite(&l_size, sizeof(u_int), 1, a_fp);

			CSemanticLinkMap_i i;

			for (i = m_linkMap.begin();i != m_linkMap.end();++i)
			{
				if (false == (*i).second->Save(a_fp))
				{
					return false;
				}
			}

			return true;
		}

		//!Utility function used by the 'Find Path from A to B' search
		bool FindPathTo(const SemanticID       a_dstID,
				      CFindPathConfig &a_config);

		//!Gets an iterator pointing to the first link within the map
		CSemanticLinkMap_i GetIterator()
		{
			return m_linkMap.begin();
		}

		//!Gets a link from the map using an iterator
		CSemanticLink *GetNext(CSemanticLinkMap_i &a_iter)
		{
			if (a_iter != m_linkMap.end())
			{
				CSemanticLink *l_link = (*a_iter).second;

				++a_iter;

				return l_link;
			}

			return NULL;
		}	

		//!Search for a specific semantic link in the map using a numeric ID
		CSemanticLink *Lookup(const SemanticID a_id)
		{
			CSemanticLinkMap_i i;

			i = m_linkMap.find(a_id);

			if (m_linkMap.end() != i)
			{
				return (*i).second;
			}

			return NULL;
		}

		//!Stores a semantic link in the map
		void Store(CSemanticLink *a_link);

		//!Removes a specific semantic link from the map using a numeric ID 
		void Remove(const SemanticID a_id);

		//!Gets the number of links in the map
		u_int GetSize() const
		{
			return m_linkMap.size();
		}

		//!Constructor
		CLinkMap() {}

		//!Destructor
		~CLinkMap();

	private:
		//!STL map of semantic links
		CSemanticLinkMap m_linkMap;
};

#endif

