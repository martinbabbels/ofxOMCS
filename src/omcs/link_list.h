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

#ifndef _LINK_LIST

#define _LINK_LIST

#include <list>

//#include "link.h"
#include "id_map.h"

class CSemanticLink;

//!STL list of semantic links
typedef std::list<CSemanticLink *> 	CLinkList;

//!Iterator for semantic link STL list
typedef CLinkList::iterator		CLinkList_i;
//!Const iterator for semantic link STL list
typedef CLinkList::const_iterator	CLinkList_ci;
//!Reverse iterator for semantic link STL list
typedef CLinkList::reverse_iterator	CLinkList_ri;

//!List of semantic links
class CSemanticLinkList
{
	public:
		//!Clones the list, creating a new instance
		CSemanticLinkList *Clone()
		{
			CSemanticLinkList *l_list = new CSemanticLinkList;

			CLinkList_i i;

			for (i = m_links.begin();i != m_links.end();++i)
			{
				l_list->Store((*i));
			}

			return l_list;
		}

		//!Lookup a specific semantic link within the list
		bool Lookup(CSemanticLink *a_link)
		{
			CLinkList_i i;

			for (i = m_links.begin();i != m_links.end();++i)
			{
				if ((*i) == a_link)
				{
					return true;
				}
			}

			return false;
		}

		//!Get an iterator pointing to the beginning of list
		CLinkList_i GetIterator()
		{
			return m_links.begin();
		}

		//!Gets a reverse iterator pointing to the end of the list
		CLinkList_ri GetReverseIterator()
		{
			return m_links.rbegin();
		}

		//!Gets the next available semantic link from the list using a reverse iterator
		CSemanticLink *GetNext(CLinkList_ri &a_iter)
		{
			if (a_iter == m_links.rend())
			{
				return NULL;
			}

			return (*a_iter++);
		}	

		//!Gets the next available semantic link from the list using a forward iterator
		CSemanticLink *GetNext(CLinkList_i &a_iter)
		{
			if (a_iter == m_links.end())
			{
				return NULL;
			}

			return (*a_iter++);
		}	

		//!Appends a semantic link to the end of the list
		void Store(CSemanticLink *a_link)
		{
			m_links.push_back(a_link);
		}

		//!Inserts a semantic link at the beginning of the list
		void StoreFront(CSemanticLink *a_link)
		{
			m_links.push_front(a_link);
		}

		//!Removes a semantic link from the end of the list
		void Remove()
		{
			m_links.pop_back();
		}

		//!Removes a semantic link from the beginning of the list
		void RemoveFront()
		{
			m_links.pop_front();
		}

		//!Empties the list
		void Clear()
		{
			m_links.clear();
		}

		//!Gets the number of links in the list
		u_int GetSize() const
		{
			return m_links.size();
		}

		//!Constructor
		CSemanticLinkList() {}

		//!Destructor
		~CSemanticLinkList() {}

	private:
		CLinkList m_links;
};


#endif

