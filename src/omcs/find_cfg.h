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

#ifndef _FIND_CFG

#define _FIND_CFG

#include "concept_map.h"
#include "link_list.h"
#include "debug.h"

class CSemanticConcept;


//!Configuration for a 'Find Path From A to B' search
class CFindPathConfig
{
	public:
		//!Gets whether an additional semantic concept may be visited during searching
		bool AllowVisit()
		{
			if (m_curNodeVisits >= m_maxNodeVisits)
			{
				return false;
			}

			if (m_resultsList.size() >= m_maxNodeResults)
			{
				return false;
			}

			return true;
		}

		//!Notifies the configuration class that a concept was visited during searching
		void VisitNode()
		{
			m_curNodeVisits++;
		}

		//!Stores a semantic link in the current working path
		void VisitLink(CSemanticLink *a_link)
		{
			m_currentPath->Store(a_link);
		}

		//!Looks for a specific semantic link in the current working path
		bool CheckCurrentPath(CSemanticLink *a_link)
		{
			return m_currentPath->Lookup(a_link);
		}

		//!Sets the current working path, removing any previous path items
		void SetCurrentPath(CSemanticLinkList *a_list)
		{
			if (a_list)
			{
				delete m_currentPath;

				m_currentPath = a_list->Clone();
			}
		}

		//!Notifies the configuration class that semantic concept processing has stopped for the most recently visited concept
		void LeaveNode()
		{
		}

		//!Notifies the configuration class that semantic link processing has stopped for the most recently visited link
		void LeaveLink()
		{
			m_currentPath->Remove();
		}

		//!Adds the current working path to the list of search results
		void AddPathToResults()
		{
			CSemanticLinkList *a_list = m_currentPath->Clone();

			DPRINT("adding result path of size %d\n",
				a_list->GetSize());

			m_resultsList.push_back(a_list);
		}

		//!Gets the number of search results
		u_int GetNumResults() const
		{
			return m_resultsList.size();
		}

		//!Gets a specific path search result
		CSemanticLinkList *GetResult(const u_int a_index)
		{
			u_int l_count = 0;

			CResultsList_ci i;

			for (i = m_resultsList.begin();i != m_resultsList.end();
			     ++i)
			{
				if (a_index == l_count)
				{
					return (*i);
				}

				l_count++;
			}

			return NULL;
		}

		//!Gets the current number of concepts visited during searching
		u_int GetNumNodeVisits() const
		{
			return m_curNodeVisits;
		}

		//!Gets the first available semantic path from the processing queue
		CSemanticLinkList *GetQueuePath()
		{
			CResultsList_i i;

			i = m_pathQueue.begin();

			if (m_pathQueue.end() != i)
			{
				CSemanticLinkList *l_list = (*i);

				m_pathQueue.pop_front();

				return l_list;
			}

			return NULL;
		}

		//!Queues the current working path into the processing queue
		void QueuePath()
		{
			CSemanticLinkList *l_list = m_currentPath->Clone();

			m_pathQueue.push_back(l_list);
		}

		//!Constructor
		CFindPathConfig()
		{
			m_maxNodeVisits  = 50000;
			m_maxNodeResults = 200;

			m_curNodeVisits = 0;
			m_curNodeDepth  = 0;

			m_currentPath = new CSemanticLinkList;
		}

		//!Destructor
		~CFindPathConfig() {}

	private:
		//!Maximum number of concepts that may be visited during searching
		u_int m_maxNodeVisits;
		//!Maximum number of search results to get
		u_int m_maxNodeResults;

		//!Current number of concepts visted during searching
		u_int m_curNodeVisits;
		//!Current semantic link depth from source concept
		u_int m_curNodeDepth;

		//!Current working path (semantic links from source concept)
		CSemanticLinkList *m_currentPath;

		//!STL list of semantic link paths
		typedef std::list<CSemanticLinkList *>  CResultsList;

		//!Iterator for semantic link path STL list
		typedef CResultsList::iterator		CResultsList_i;
		//!Const iterator for semantic link path STL list
		typedef CResultsList::const_iterator	CResultsList_ci;

		//!List of search results
		CResultsList m_resultsList;

		//!Processing queue of semantic paths
		CResultsList m_pathQueue;
};

#endif

