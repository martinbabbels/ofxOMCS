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

#ifndef _CONCEPT_MAP

#define _CONCEPT_MAP

#include <map>
#include <string>

#include "semantic_id.h"


class CSemanticConcept;
class CTextIDMap;

//!Map of semantic concepts
class CConceptMap
{
	public:
		//!Loads the concept map from a previously opened file
		bool Load(FILE       *a_fp,
			  CTextIDMap &a_idMap);

		//!Saves the concept map to a previously opened file
		bool Save(FILE *a_fp);

		//!Lookup a semantic concept in the map using its numeric ID
		CSemanticConcept *Lookup(const SemanticID a_id)
		{
			CSemanticConceptMap_i i;

			i = m_conceptMap.find(a_id);

			if (m_conceptMap.end() != i)
			{
				return (*i).second;
			}

			return NULL;
		}

		//!Store a semantic concept in the map
		void Store(CSemanticConcept *a_concept);

		//!Get the number of items in the map
		u_int GetSize() const
		{
			return m_conceptMap.size();
		}

		//!Constructor
		CConceptMap() {}

		//!Destructor
		~CConceptMap();

	private:
		//!STL map of concepts using their numeric ID as the key
		typedef std::map<SemanticID, CSemanticConcept *>
					   CSemanticConceptMap;
		//!Iterator for concept STL map
		typedef CSemanticConceptMap::iterator
					   CSemanticConceptMap_i;
		//!Const iterator for concept STL map
		typedef CSemanticConceptMap::const_iterator
					   CSemanticConceptMap_ci;

		//!STL map of semantic concepts
		CSemanticConceptMap m_conceptMap;
};

#endif

