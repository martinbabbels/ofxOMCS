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

#ifndef _OMCSNET

#define _OMCSNET

#include <string>
#include <vector>

#include "textid_map.h"
#include "id_generator.h"
#include "concept_map.h"
#include "link_map.h"
#include "link_list.h"

class CFindAnalogousConfig;
class CAnalogousWeightNode;
class CFindContextConfig;
class CFindPathConfig;


//!OpenMind CommonSense Main API
class COMCSNet
{
	public:
		//!Loads an optimized binary predicates file from disk
		bool Load(const std::string &a_path)
		{
			FILE *l_fp;

			l_fp = fopen(a_path.c_str(), "rb");

			if (l_fp)
			{
				if (false ==
				    m_conceptMap.Load(l_fp, m_textIDMap))
				{
					return false;
				}

				if (false == m_linkMap.Load(l_fp, m_conceptMap))
				{
					return false;
				}

				// TODO: set latest idfactory value

				fclose(l_fp);

				return true;
			}

			return false;
		}

		//!Saves an optimized binary predicates file to disk
		bool Save(const std::string &a_path)
		{
			FILE *l_fp;

			l_fp = fopen(a_path.c_str(), "wb");

			if (l_fp)
			{
				if (false == m_conceptMap.Save(l_fp))
				{
					return false;
				}

				if (false == m_linkMap.Save(l_fp))
				{
					return false;
				}

				fclose(l_fp);

				return true;
			}

			return false;
		}

		//!Loads a raw text predicates file
		bool LoadPredicateFile(const std::string &a_path);

		//!Lookup a string in the TextID Map using a semantic ID
		std::string LookupString(const SemanticID a_id);

		//!Find all concepts that are analogous to a concept
		bool FindAnalogous(CFindAnalogousConfig &a_config);

		//!Finds all concepts within the same context as one of more source concepts
		bool FindContext(const std::string          &a_conceptList,
			                 CFindContextConfig &a_config);

		//!Finds all semantic paths between two concepts
		bool FindPathFromA_To_B(const std::string     &a_src,
				        const std::string     &a_dst,
					      CFindPathConfig &a_config);

		//!Lookup a specific concept using its textual name
		CSemanticConcept *LookupConcept(const std::string &a_name)
		{
			SemanticID l_id;

			l_id = m_textIDMap.Lookup(a_name);

			if (SEMANTIC_ID_NONE != l_id)
			{
				return m_conceptMap.Lookup(l_id);
			}

			return NULL;
		}

		//!Lookup a specific concept using its numeric ID
		CSemanticConcept *LookupConcept(const SemanticID   a_id)
		{
			return m_conceptMap.Lookup(a_id);
		}

		//!Constructor
		COMCSNet() {}

		//!Destructor
		~COMCSNet() {}

	private:
		//!Process a link map during 'Find Analogous' searching
		bool p_AnalogousProcessLinkMap(CLinkMap             *a_linkMap,
					       CFindAnalogousConfig &a_config);

		//!Process a backwards link map during 'Find Analogous' searching
		bool p_AnalogousProcessBackwardLinkMap(CLinkMap   *a_linkMap,
					     CFindAnalogousConfig &a_config,
					     const RelationType    a_relation);

		//!Process a semantic link during 'Find Analogous' searching
		void p_AnalogousProcessLink(CSemanticLink        *a_link,
					    CFindAnalogousConfig &a_config);

		//!Process a backwards link during 'Find Analogous' searching
		void p_AnalogousProcessBackwardLink(CSemanticLink *a_link,
					     CFindAnalogousConfig &a_config,
					     const RelationType    a_relation);

		//!Process a weighted candidate node during 'Find Analogous' searching
		void p_AnalogousProcessWeightedNode(
						CAnalogousWeightNode *a_node);

		//!Process a link map during 'Find Context' searching
		bool p_FindContextProcessLinkMap(
					     CLinkMap             *a_linkMap,
					     CFindContextConfig &a_config,
					     const float           a_oldScore);

		//!Map of TextualName->ID associations for concepts
		CTextIDMap m_textIDMap;

		//!Generates unique IDs for concepts and semantic links
		CSemanticIDGenerator m_idFactory;

		//!Map of semantic concepts
		CConceptMap	     m_conceptMap;
	
		//!Map of semantic links
		CLinkMap	     m_linkMap;
};


#endif

