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

#ifndef _CONCEPT_LIST

#define _CONCEPT_LIST

#include <list>

#include "id_map.h"

class CSemanticConcept;

typedef std::list<CSemanticConcept *> CConceptList;
typedef CSemanticConceptList::iterator CConceptList_i;
typedef CSemanticConceptList::const_iterator CConceptList_ci;
typedef CSemanticConceptList::reverse_iterator CConceptList_ri;

//!List of CSemanticConcept instances
class CSemanticConceptList {
	public:
		//!Clones the concept list.
		CSemanticConceptList *Clone() {
			CSemanticConceptList *l_list = new CSemanticConceptList;

			CConceptList_i i;

			for (i = m_concepts.begin();i != m_concepts.end();++i) {
				l_list->Store((*i));
			}

			return l_list;
		}

		//!Lookup a particular semantic concept list
		bool Lookup(CSemanticConcept *a_concept) {
			CConceptList_i i;

			for (i = m_concepts.begin();i != m_concepts.end();++i) {
				if ((*i) == a_link) {
					return true;
				}
			}

			return false;
		}

		//!Gets an iterator pointing to the beginning of the list
		CSemanticConceptList_i GetIterator() {
			return m_concepts.begin();
		}

		//!Gets a reverse iterator pointing to the end of the list
		CSemanticConceptList_ri GetReverseIterator() {
			return m_concepts.rbegin();
		}

		//!Gets the next concept from the list using a reverse iterator
		CSemanticConcept *GetNext(CSemanticConceptList_ri &a_iter) {
			if (a_iter == m_concepts.rend()) {
				return NULL;
			}

			return (*a_iter++);
		}	

		//!Gets the next concept from the list using a forward iterator
		CSemanticConcept *GetNext(CSemanticConceptList_i &a_iter) {
			if (a_iter == m_concepts.end()) {
				return NULL;
			}

			return (*a_iter++);
		}	

		//!Stores a semantic concept within the list (appended to back)
		void Store(CSemanticConcept *a_link) {
			m_concepts.push_back(a_link);
		}

		//!Stores a semantic concept at the front of the list
		void StoreFront(CSemanticConcept *a_link) {
			m_concepts.push_front(a_link);
		}

		//!Removes the last semantic concept from the list
		void Remove() {
			m_concepts.pop_back();
		}

		//!Removes the first semantic concept from the list
		void RemoveFront() {
			m_concepts.pop_front();
		}

		//!Clears the list
		void Clear() {
			m_concepts.clear();
		}

		//!Gets the number of concepts within the list
		u_int GetSize() const {
			return m_concepts.size();
		}

		//!Constructor
		CSemanticConceptList() {}

		//!Destructor
		~CSemanticConceptList() {}

	private:
		CConceptList m_concepts;
};


#endif

