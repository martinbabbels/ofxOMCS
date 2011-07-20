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

#ifndef _FINDC_CFG

#define _FINDC_CFG

#include "concept_map.h"
#include "link_list.h"
#include "debug.h"

class CSemanticConcept;

//!STL pair containing a semantic concept and it's associated score
typedef std::pair<CSemanticConcept *, float>
				CSemanticConceptPair;

//!STL list of semantic concept + score pairs
typedef std::list<CSemanticConceptPair>      CResultsList;
//!Iterator for semantic concept + score pair STL list
typedef CResultsList::iterator		     CResultsList_i;
//!Const iterator for semantic concept + score pair STL list
typedef CResultsList::const_iterator	     CResultsList_ci;

//!Configuration for a 'Find Context Concepts' search
class CFindContextConfig
{
	public:
		//!Gets whether an additional semantic concept can be visited during searching
		bool AllowVisit()
		{
			if (m_curNodeVisits >= m_maxNodeVisits)
			{
				return false;
			}

			return true;
		}

		//!Notifies the configuration class that a semantic concept was visited during searching
		void VisitNode()
		{
			m_curNodeVisits++;
		}

		//!Gets the score reduction factor used for discounting the weight of concepts during searching
		float GetDiscountFactor() const
		{
			return 0.5;
		}

		//!Adds a concept to the search results list
		bool AddNodeToResults(CSemanticConcept *a_concept,
				      const float       a_score)
		{
			CSemanticConceptPair l_pair;

			if (m_resultsList.size() >= m_maxNodeResults)
			{
				return false;
			}

			l_pair.first  = a_concept;
			l_pair.second = a_score;

			m_resultsList.push_back(l_pair);

			return true;
		}

		//!Gets the number of search results 
		u_int GetNumResults() const
		{
			return m_resultsList.size();
		}

		//!Gets a specific search result
		CSemanticConcept *GetResult(const u_int  a_index,
					          float *a_score)
		{
			u_int l_count = 0;

			CResultsList_ci i;

			for (i = m_resultsList.begin();i != m_resultsList.end();
			     ++i)
			{
				if (a_index == l_count)
				{
					if (a_score)
					{
						*a_score = (*i).second;
					}

					return (*i).first;
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

		//!Gets an iterator pointing to the beginning of the scored concepts queue
		CResultsList_i GetScoreIterator()
		{
			DPRINT("get score iterator size %d\n",
				m_scoreQueue.size());

			return m_scoreQueue.begin();
		}

		//!Gets a scored concept from the processing queue using an iterator
		CSemanticConcept *GetScoreQueue(CResultsList_i &a_iter,
						float          *a_score)
		{
			if (m_scoreQueue.end() != a_iter)
			{
				CSemanticConcept *l_concept;

				l_concept = (*a_iter).first;

				if (a_score)
				{
					*a_score = (*a_iter).second;
				}

				a_iter++;

				return l_concept;
			}

			return NULL;
		}

		//!Gets the first available scored concept from the processing queue, removing it from the queue
		CSemanticConcept *GetScoreQueue(float *a_score)
		{
			CResultsList_i i;

			i = m_scoreQueue.begin();

			if (m_scoreQueue.end() != i)
			{
				CSemanticConcept *l_concept;

				l_concept = (*i).first;

				if (a_score)
				{
					*a_score = (*i).second;
				}

				m_scoreQueue.pop_front();

				return l_concept;
			}

			return NULL;
		}

		//!Queues a concept in the processing queue along with its associated score
		void QueueScore(      CSemanticConcept *a_concept,
				const float             a_score)
		{
			CSemanticConceptPair l_pair;

			l_pair.first  = a_concept;
			l_pair.second = a_score;

			m_scoreQueue.push_back(l_pair);
		}

		//!Constructor
		CFindContextConfig()
		{
			m_maxNodeVisits  = 1000;
			m_maxNodeResults = 200;

			m_curNodeVisits = 0;
		}

		//!Destructor
		virtual ~CFindContextConfig() {}

	private:
		//!Maximum number of concepts to be visited during searching
		u_int m_maxNodeVisits;
		//!Maximum number of search results to return
		u_int m_maxNodeResults;

		//!Current number of concepts visited during search processing
		u_int m_curNodeVisits;

		//!List of search results
		CResultsList m_resultsList;

		//!Processing queue of scored semantic concepts
		CResultsList m_scoreQueue;
};

#endif

