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

#ifndef _PRED_DECODER
#define _PRED_DECODER
#include <string>

#include "concept.h"
#include "concept_map.h"
#include "textid_map.h"
#include "id_generator.h"
#include "relation_type.h"
#include "link_map.h"
#include "debug.h"
#include <vector>

// Poco regex
#include "Poco/RegularExpression.h"

using Poco::RegularExpression;

//!Raw predicate decoder
class CPredicateDecoder
{
	public:
		//!Decodes a raw predicate, adding the results to the semantic network
		void DecodePredicateText(const std::string &a_string, CTextIDMap &a_idMap, CSemanticIDGenerator &a_idGen, CConceptMap &a_conceptMap, CLinkMap &a_linkMap) {

			Poco::RegularExpression l_regex("\\((\\w+)\\s+\\\"(.+)\\\"\\s+\\\"(.+)\\\"");
			RegularExpression::MatchVec matches;
			
			if (l_regex.match(a_string,0, matches) == true) {
				std::string l_relationship;
				std::string l_source;
				std::string l_destination;

				l_relationship = a_string.substr(matches[1].offset, matches[1].length);
				l_source = a_string.substr(matches[2].offset, matches[2].length);
				l_destination =  a_string.substr(matches[3].offset, matches[3].length);
				
				RelationType l_relationType;

				l_relationType = m_relationLookup.Lookup(l_relationship);

				SemanticID l_sourceID;

				l_sourceID = a_idMap.Lookup(l_source);

				CSemanticConcept *l_srcConcept = NULL;

				if (SEMANTIC_ID_NONE == l_sourceID) {
					l_srcConcept = new CSemanticConcept(
								    l_source,
								    &a_idGen,
								    &a_idMap);

					a_conceptMap.Store(l_srcConcept);


					DPRINT("storing %s %d.\n",
						l_source.c_str(),
						l_relationType);


					l_sourceID = l_srcConcept->GetID();
				}
				else
				{
					l_srcConcept =
						a_conceptMap.Lookup(l_sourceID);
				}

				SemanticID l_destinationID;

				l_destinationID = a_idMap.Lookup(l_destination);

				CSemanticConcept *l_dstConcept = NULL;

				if (SEMANTIC_ID_NONE == l_destinationID)
				{
					l_dstConcept = new CSemanticConcept(
								 l_destination,
								 &a_idGen,
								 &a_idMap);

					a_conceptMap.Store(l_dstConcept);

					DPRINT("storing %s %d.\n",
						l_destination.c_str(),
						l_relationType);


					l_destinationID = l_dstConcept->GetID();
				}
				else
				{
					l_dstConcept =
					   a_conceptMap.Lookup(l_destinationID);
				}

				CSemanticLink *l_link = NULL;

				DPRINT("storing link\n");

				l_link = new CSemanticLink();
							
				l_link->SetSource(l_srcConcept);
				l_link->SetDest(l_dstConcept);

				l_link->SetType(l_relationType);

				l_link->SetID(a_idGen.Get());

				a_linkMap.Store(l_link);

				DPRINT("storing forward lnk\n");

				l_srcConcept->AddLink(l_link,
							l_relationType);

				DPRINT("storing bckward lnk\n");

				l_dstConcept->AddBackwardLink(
						l_link, l_relationType);
			}
		}

		//!Constructor
		CPredicateDecoder() {}

		//!Destructor
		~CPredicateDecoder() {}

	private:
		//!Lookup table for semantic relation <-> textual names
		CRelationTypeLookup m_relationLookup;
};

#endif

