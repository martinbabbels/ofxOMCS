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

#include "omcsnet.h"
#include "pred_fileio.h"
#include "pred_decoder.h"
#include "find_cfg.h"
#include "findc_cfg.h"
#include "finda_cfg.h"
#include "debug.h"



bool COMCSNet::LoadPredicateFile(const std::string &a_path)
{
	CPredicateFileIO l_io;

	if (true == l_io.OpenInputFile(a_path))
	{
		std::string l_predicateStr;
		u_int	    l_count	   = 0;

		do
		{
			l_predicateStr = l_io.GetPredicate(NULL);

			if (l_predicateStr.size() > 0)
			{
				CPredicateDecoder l_decoder;

				l_decoder.DecodePredicateText(l_predicateStr,
							      m_textIDMap,
							      m_idFactory,
							      m_conceptMap,
							      m_linkMap);

				l_count++;

				DPRINT("Loaded %d Predicates.\n", l_count);

				DPRINT("%d Concepts.\n",m_conceptMap.GetSize());
				DPRINT("%d Links.\n", m_linkMap.GetSize());
			}
		} while (l_predicateStr.size() > 0);

		l_io.CloseInputFile();
	}

	DPRINT("[%d] Concepts Loaded.\n", m_conceptMap.GetSize());
	DPRINT("%d Links.\n", m_linkMap.GetSize());

	return true;
}



bool COMCSNet::FindAnalogous(CFindAnalogousConfig &a_config)
{
	CSemanticConcept *l_concept = NULL;

	l_concept = LookupConcept(a_config.GetNodeName());

	if (!l_concept)
	{
		return false;
	}

	printf("gathering candidate nodes.\n");

	for (u_int i = 0;i < MAX_RELATIONS;++i)
	{
		DPRINT("processing %d\n", i);

		CLinkMap *l_linkMap = l_concept->GetLinks(i);

		p_AnalogousProcessLinkMap(l_linkMap, a_config);

		if (false == a_config.AllowVisit())
		{
			break;
		}
	}

	CAnalogousWeightNodeMap_i l_iter;

	l_iter = a_config.GetWeightedIterator();

	CAnalogousWeightNode *l_weightedNode = NULL;

	printf("processing weighted nodes.\n");

	do
	{
		l_weightedNode = a_config.GetWeightedNode(l_iter);

		if (l_weightedNode)
		{
			p_AnalogousProcessWeightedNode(l_weightedNode);
		}
	} while (l_weightedNode);

	typedef std::multimap<u_int, CAnalogousWeightNode *>   CWeightSortMap;
	typedef CWeightSortMap::reverse_iterator            CWeightSortMap_ri;

	CWeightSortMap    l_weightSortTable;
	CWeightSortMap_ri l_weightSortIter;

	printf("sorting results.\n");

	// copy results to sort map
	l_iter = a_config.GetWeightedIterator();

	do
	{
		l_weightedNode = a_config.GetWeightedNode(l_iter);

		if (l_weightedNode)
		{
			l_weightSortTable.insert(
				std::make_pair(l_weightedNode->GetWeightValue(),
			 	            l_weightedNode));
		}
	} while (l_weightedNode);

	printf("copying results to output.\n");

	// take top X results
	for (l_weightSortIter = l_weightSortTable.rbegin();
	     l_weightSortIter != l_weightSortTable.rend();++l_weightSortIter)
	{
		DPRINT("adding to result.\n");

		float l_result = (*l_weightSortIter).first / 200.0;

		if (l_result > 0.90)
		{
			l_result = 0.90;
		}

		if (false == a_config.AddNodeToResults(
			 	  (*l_weightSortIter).second, l_result))
		{
			break;
		}
	}

	printf("done.\n");

	if (a_config.GetNumResults() < 1)
	{
		return false;
	}

	printf("success!\n");

	return true;
}


void COMCSNet::p_AnalogousProcessWeightedNode(CAnalogousWeightNode *a_node)
{
	if (!a_node)
	{
		return;
	}

	CSemanticLinkMap_i i;

	i = a_node->GetLinkIterator();

	CSemanticLink *l_link = NULL;

	typedef std::map<RelationType, RelationType> CRelationTypeMap;
	typedef CRelationTypeMap::iterator           CRelationTypeMap_i;

	CRelationTypeMap   l_relationMap;
	CRelationTypeMap_i l_relationIter;

	do
	{
		l_link = a_node->GetLink(i);

		if (l_link)
		{
			l_relationIter = l_relationMap.find(l_link->GetType());

			if ((l_relationIter      == l_relationMap.end()) &&
			    (RELATION_OFTEN_NEAR != l_link->GetType()))
			{
				l_relationMap[l_link->GetType()] =
							l_link->GetType();
			}

			if (RELATION_OFTEN_NEAR == l_link->GetType())
			{
				a_node->AddWeightValue(1);
			}
			else if (RELATION_LOCATION_OF == l_link->GetType())
			{
				a_node->AddWeightValue(5);
			}
			else if ((RELATION_USED_FOR    == l_link->GetType()) ||
			         (RELATION_PROPERTY_OF == l_link->GetType()))
			{
				a_node->AddWeightValue(12);
			}
			else
			{
				a_node->AddWeightValue(8);
			}
		}
	} while (l_link);

	a_node->AddWeightValue(25 * l_relationMap.size());
}

bool COMCSNet::FindContext(const std::string &a_conceptList, CFindContextConfig &a_config) {
	
	std::vector<std::string> elements;
	// Skip delimiters at beginning.
	std::string::size_type lastPos = a_conceptList.find_first_not_of(",", 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = a_conceptList.find_first_of(",", lastPos);
	
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
    	elements.push_back(a_conceptList.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = a_conceptList.find_first_not_of(",", pos);
        // Find next "non-delimiter"
        pos = a_conceptList.find_first_of(",", lastPos);
    }

	CSemanticConcept *l_concept = NULL;
	
	int i = 0;
	// get initial list of origin score
	for (i = 0; i<elements.size();i++) {
		l_concept = LookupConcept(elements[i]);

		if (l_concept)
		{
			a_config.QueueScore(l_concept, 1.0);
		}
	}

	//DPRINT("%d source concepts.\n", l_conceptNames.size());

	float l_score = 0.0;

	CResultsList_i l_iter = a_config.GetScoreIterator();

	// build list of initial scores based on connecting nodes
	do
	{
		l_concept = a_config.GetScoreQueue(l_iter, &l_score);

		DPRINT("building old score %f\n", l_score);

		if (l_concept)
		{
			for (u_int i = 0;i < MAX_RELATIONS;++i)
			{
				CLinkMap *l_linkMap = l_concept->GetLinks(i);

				p_FindContextProcessLinkMap(
						l_linkMap, a_config, l_score);

				if (false == a_config.AllowVisit())
				{
					break;
				}
			}
		}
	} while ((l_concept) && (true == a_config.AllowVisit()));

	typedef std::map<SemanticID, float> CScoreMap;
	typedef CScoreMap::iterator         CScoreMap_i;

	CScoreMap   l_scoreLookupTable;
	CScoreMap_i l_scoreIter;

	l_iter = a_config.GetScoreIterator();

	float l_lookupTableScore = 0.0;
	float l_minScore	 = 0.0;
	float l_maxScore	 = 0.0;
	
	DPRINT("calculating score list.\n");

	// calculate score list
	do
	{
		l_concept = a_config.GetScoreQueue(l_iter, &l_score);

		if (l_concept)
		{
			l_scoreIter = l_scoreLookupTable.find(
							l_concept->GetID());

			if (l_scoreLookupTable.end() != l_scoreIter)
			{
				l_lookupTableScore = (*l_scoreIter).second;
			}
			else
			{
				l_lookupTableScore = 0.0;
			}

			if (l_lookupTableScore > l_score)
			{
				l_maxScore = l_lookupTableScore;

				l_minScore = l_score;
			}
			else
			{
				l_minScore = l_lookupTableScore;

				l_maxScore = l_score;
			}

			l_score = l_maxScore +
				  ((1.0 - l_maxScore) * l_minScore);

			l_scoreLookupTable[l_concept->GetID()] = l_score;
		}
	} while (l_concept);
	
	DPRINT("score lookup table size = %d\n", l_scoreLookupTable.size());

	DPRINT("delete origin nodes from score list.\n");

	// delete origin nodes from score list
	for (i = 0; i<elements.size();i++) { 
		l_concept = LookupConcept(elements[i]);

		if (l_concept) {
			l_scoreIter = l_scoreLookupTable.find(
							l_concept->GetID());

			if (l_scoreLookupTable.end() != l_scoreIter) {
				DPRINT("deleting item.\n");
				l_scoreLookupTable.erase(l_scoreIter);
			}
		}
	}

	typedef std::multimap<float, SemanticID>         CScoreSortMap;
	typedef CScoreSortMap::reverse_iterator     CScoreSortMap_ri;

	CScoreSortMap    l_scoreSortTable;
	CScoreSortMap_ri l_scoreSortIter;

	DPRINT("sorting results.\n");

	// copy results to sort map
	for (l_scoreIter = l_scoreLookupTable.begin();
	     l_scoreIter != l_scoreLookupTable.end();++l_scoreIter)
	{
		DPRINT("adding to sort table.\n");

		//l_scoreSortTable[(*l_scoreIter).second] = (*l_scoreIter).first;

		l_scoreSortTable.insert(std::make_pair((*l_scoreIter).second,
					          (*l_scoreIter).first));
	}

	DPRINT("copying results to output.\n");

	// take top X results
	for (l_scoreSortIter = l_scoreSortTable.rbegin();
	     l_scoreSortIter != l_scoreSortTable.rend();++l_scoreSortIter) {
		l_concept = m_conceptMap.Lookup((*l_scoreSortIter).second);

		if (l_concept) {
			DPRINT("adding to result.\n");

			if (false == a_config.AddNodeToResults(l_concept,
					  	  (*l_scoreSortIter).first)) {
				break;
			}
		}
	}

	DPRINT("done.\n");

	if (a_config.GetNumResults() < 1) {
		return false;
	}

	DPRINT("success!\n");

	return true;
}


bool COMCSNet::p_AnalogousProcessLinkMap(CLinkMap *a_linkMap, CFindAnalogousConfig &a_config) {
	DPRINT("processing linkmap.\n");

	if (!a_linkMap) {
		return false;
	}

	CSemanticLinkMap_i l_linkIter;

	l_linkIter = a_linkMap->GetIterator();

	CSemanticLink *l_link = NULL;

	do {
		l_link = a_linkMap->GetNext(l_linkIter);

		if ((l_link) && (a_config.AllowVisit()))
		{
			p_AnalogousProcessLink(l_link, a_config);
		}
	} while ((l_link) && (a_config.AllowVisit()));

	return true;
}


bool COMCSNet::p_AnalogousProcessBackwardLinkMap(CLinkMap         *a_linkMap,
					     CFindAnalogousConfig &a_config,
					     const RelationType    a_relation)
{
	DPRINT("processing backward linkmap.\n");

	if (!a_linkMap)
	{
		return false;
	}

	CSemanticLinkMap_i l_linkIter;

	l_linkIter = a_linkMap->GetIterator();

	CSemanticLink *l_link = NULL;

	do
	{
		l_link = a_linkMap->GetNext(l_linkIter);

		if ((l_link) && (a_config.AllowVisit()))
		{
			p_AnalogousProcessBackwardLink(l_link, a_config,
						       a_relation);
		}
	} while ((l_link) && (a_config.AllowVisit()));

	return true;
}


void COMCSNet::p_AnalogousProcessLink(CSemanticLink        *a_link,
				      CFindAnalogousConfig &a_config)
{
	CSemanticConcept *l_concept;
	RelationType      l_relation;

	l_relation = a_link->GetType();
	l_concept  = a_link->GetDest();

	if (l_concept)
	{
		a_config.VisitNode();

		for (u_int i = 0;i < MAX_RELATIONS;++i)
		{
			CLinkMap *l_linkMap = l_concept->GetBackwardLinks(i);
		
			p_AnalogousProcessBackwardLinkMap(l_linkMap, a_config,
							  l_relation);

			if (false == a_config.AllowVisit())
			{
				break;
			}
		}
	}
}


void COMCSNet::p_AnalogousProcessBackwardLink(CSemanticLink        *a_link,
				              CFindAnalogousConfig &a_config,
					      const RelationType    a_relation)
{
	CSemanticConcept *l_concept;

	DPRINT("processing backward link.\n");

	l_concept  = a_link->GetSource();

	if (l_concept)
	{
		if ((a_config.GetNodeName() != l_concept->GetName()) &&
		    (a_relation             == a_link->GetType()))
		{
			a_config.VisitNode();

			DPRINT("storing weighted link.\n");

			a_config.StoreWeightedLink(a_link);
		}
	}
}

bool COMCSNet::p_FindContextProcessLinkMap(CLinkMap             *a_linkMap,
					   CFindContextConfig &a_config,
					   const float           a_oldScore)
{
	DPRINT("OLD %f\n", a_oldScore);

	if (!a_linkMap)
	{
		return false;
	}

	CSemanticLinkMap_i l_linkIter;

	l_linkIter = a_linkMap->GetIterator();

	CSemanticLink *l_link = NULL;

	do
	{
		l_link = a_linkMap->GetNext(l_linkIter);

		if ((l_link) && (a_config.AllowVisit()))
		{
			CSemanticConcept *l_concept;

			l_concept = l_link->GetDest();

			if (l_concept)
			{
				a_config.VisitNode();

				float l_score = (a_oldScore *
					        a_config.GetDiscountFactor());

				a_config.QueueScore(l_concept, l_score);
			}
		}
	} while ((l_link) && (a_config.AllowVisit()));

	return true;
}


bool COMCSNet::FindPathFromA_To_B(const std::string     &a_src,
		                  const std::string     &a_dst,
				        CFindPathConfig &a_config)
{
	CSemanticConcept *l_srcConcept;
	CSemanticConcept *l_dstConcept;

	l_srcConcept = LookupConcept(a_src);
	l_dstConcept = LookupConcept(a_dst);

	if ((l_srcConcept) && (l_dstConcept))
	{
		SemanticID l_dstID = l_dstConcept->GetID();
	
		CSemanticConcept *l_concept = l_srcConcept;

		CSemanticLinkList *l_list;

		while ((l_concept) && (true == a_config.AllowVisit()))
		{
			DPRINT("findpathto for concept.\n");

			l_concept->FindPathTo(l_dstID, a_config);

			l_concept = NULL;

			l_list = a_config.GetQueuePath();

			if (l_list)
			{
				DPRINT("got queued path.\n");

				CLinkList_ri tail_i;

				tail_i = l_list->GetReverseIterator();

				CSemanticLink *l_link = l_list->GetNext(tail_i);

				if (l_link)
				{
					DPRINT("got tail link\n");

					l_concept = l_link->GetDest();

					if (false ==
					   a_config.CheckCurrentPath(l_link))
					{
						a_config.SetCurrentPath(l_list);
					}
				}	
			}		
		}

		if (a_config.GetNumResults() > 0)
		{
			DPRINT("%d nodes visited.\n", a_config.GetNumNodeVisits());

			DPRINT("%d result paths found.\n", a_config.GetNumResults());

			return true;
		}
	}

	return false;
}


std::string COMCSNet::LookupString(const SemanticID a_id)
{
	if (SEMANTIC_ID_NONE != a_id)
	{
		CSemanticConcept *l_concept;

		l_concept = m_conceptMap.Lookup(a_id);

		if (l_concept)
		{
			return l_concept->GetName();
		}
	}

	return "";
}


