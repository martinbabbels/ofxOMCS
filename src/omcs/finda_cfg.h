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

#ifndef _FINDA_CFG

#define _FINDA_CFG

#include <string>

#include "semantic_id.h"
#include "link_map.h"
#include "debug.h"


//!Weighted concept node used during FindAnalogous searches
class CAnalogousWeightNode
{
	public:
		//!Sets the concept that the node represents
		void SetConcept(CSemanticConcept *a_concept)
		{
			m_concept = a_concept;
		}

		//!Gets the concept associated with the node
		CSemanticConcept *GetConcept()
		{
			return m_concept;
		}

		//!Adds a new link to be used in the weighting process
		void AddLink(CSemanticLink *a_link)
		{
			m_links.Store(a_link);
		}

		//!Gets an iterator pointing to the beginning of the link list
		CSemanticLinkMap_i GetLinkIterator()
		{
			return m_links.GetIterator();
		}

		//!Gets a link from the link list using an iterator
		CSemanticLink *GetLink(CSemanticLinkMap_i &a_iter)
		{
			return m_links.GetNext(a_iter);
		}

		//!Gets the current node weight value
		u_int GetWeightValue() const
		{
			return m_weight;
		}

		//!Adds to the current node weight value
		void AddWeightValue(const u_int a_value)
		{
			m_weight += a_value;
		}

		//!Constructor
		CAnalogousWeightNode()
		{
			m_concept = NULL;

			m_weight = 0;
		}

		//!Destructor
		~CAnalogousWeightNode()
		{
			// NOTE: concept and link list not deallocated because
			// they are deallocated centrally by the main concept
			// and link maps
		}

	private:
		//!Semantic concept represented by this node
		CSemanticConcept *m_concept;

		//!List of links to be used during the weighting process
		CLinkMap          m_links;

		//!Current weight value for this node
		u_int		  m_weight;
};


//!STL map of analogous weight nodes using their numeric ID as the key
typedef std::map<SemanticID, CAnalogousWeightNode *>
				CAnalogousWeightNodeMap;
//!Iterator for the analogous weight node STL map
typedef CAnalogousWeightNodeMap::iterator
				CAnalogousWeightNodeMap_i;
//!Reverse iterator for the analogous weight node STL map
typedef CAnalogousWeightNodeMap::reverse_iterator
				CAnalogousWeightNodeMap_ri;
//!Const iterator for the analogous weight node STL map
typedef CAnalogousWeightNodeMap::const_iterator
				CAnalogousWeightNodeMap_ci;

//!Pair representing a floating point weight value and analogous weight node
typedef std::pair<CAnalogousWeightNode *, float>
				CAnalogousWeightPair;

//!STL list of analogous weight node pairs (used during sorting of results)
typedef std::list<CAnalogousWeightPair> CAnalogousWeightNodeList;
//!Const iterator for analogous weight node pair STL list
typedef CAnalogousWeightNodeList::iterator CAnalogousWeightNodeList_i;


//!Configuration for a 'Find Analogous Concepts' search
class CFindAnalogousConfig
{
	public:
		//!Gets whether an additional semantic concept can be visited during processing
		bool AllowVisit()
		{
			if (m_curNodeVisits >= m_maxNodeVisits)
			{
				return false;
			}

			return true;
		}

		//!Notifies the configuration class that a semantic concept was visited during processing
		void VisitNode()
		{
			m_curNodeVisits++;
		}

		//!Gets the source concept name to be used during the search
		std::string GetNodeName() const
		{
			return m_nodeName;
		}

		//!Sets the source concept name to be used during the search
		void SetNodeName(const std::string &a_name)
		{
			m_nodeName = a_name;
		}

		//!Stores a weighted link to be used during search processing
		void StoreWeightedLink(CSemanticLink *a_link)
		{
			CAnalogousWeightNodeMap_i i;

			i = m_weightedNodeMap.find(a_link->GetSourceID());

			if (i != m_weightedNodeMap.end())
			{
				DPRINT("adding link to existing node.\n");

				(*i).second->AddLink(a_link);	
			}
			else
			{
				DPRINT("adding new node.\n");

				CAnalogousWeightNode *l_node;

				l_node = new CAnalogousWeightNode;

				l_node->SetConcept(a_link->GetSource());

				l_node->AddLink(a_link);

				m_weightedNodeMap[a_link->GetSourceID()] =
									l_node;
			}
		}

		//!Gets an iterator pointing to the beginning of the weighted links map
		CAnalogousWeightNodeMap_i GetWeightedIterator()
		{
			DPRINT("get weight iterator size %d\n",
				m_weightedNodeMap.size());

			return m_weightedNodeMap.begin();
		}

		//!Gets a weighted node using an iterator
		CAnalogousWeightNode *GetWeightedNode(
					    CAnalogousWeightNodeMap_i &a_iter)
		{
			if (m_weightedNodeMap.end() != a_iter)
			{
				CAnalogousWeightNode *l_node = NULL;

				l_node = (*a_iter).second;

				++a_iter;

				return l_node;
			}

			return NULL;
		}

		//!Adds a weighted node to the results list, along with it's associated score value
		bool AddNodeToResults(CAnalogousWeightNode *a_node,
				      const float           a_score)
		{
			CAnalogousWeightPair l_pair;

			if (m_resultsList.size() >= m_maxNodeResults)
			{
				return false;
			}

			l_pair.first  = a_node;
			l_pair.second = a_score;

			m_resultsList.push_back(l_pair);

			return true;
		}

		//!Gets the number of search results found
		u_int GetNumResults() const
		{
			return m_resultsList.size();
		}

		//!Gets a specific search result using an index
		CAnalogousWeightNode *GetResult(const u_int  a_index,
					          float *a_score)
		{
			u_int l_count = 0;

			CAnalogousWeightNodeList_i i;

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

		//!Gets the number of concepts visited during search processing
		u_int GetNumNodeVisits() const
		{
			return m_curNodeVisits;
		}

		//!Constructor
		CFindAnalogousConfig()
		{
			m_maxNodeVisits  = 1000000;
			m_maxNodeResults = 200;

			m_curNodeVisits = 0;
		}

		//!Destructor
		virtual ~CFindAnalogousConfig() {}

	private:
		//!Name of the concept for which analogous concepts are being searched
		std::string m_nodeName;

		//!Map containing all weighted nodes collected during search processing
		CAnalogousWeightNodeMap m_weightedNodeMap;

		//!Maximum number of concepts that may be visited during processing
		u_int m_maxNodeVisits;

		//!Maximum number of results that may be returned during processing
		u_int m_maxNodeResults;

		//!Current number of concepts visited during search processing
		u_int m_curNodeVisits;

		//!List of analogous search results
		CAnalogousWeightNodeList m_resultsList;
};


#endif

