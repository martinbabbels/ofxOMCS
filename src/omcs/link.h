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

#ifndef _LINK

#define _LINK

#include "textid_map.h"
#include "id_generator.h"
#include "relation_type.h"

class CSemanticLinkList;
class CSemanticConcept;
class CFindPathConfig;
class CConceptMap;


//!Semantic link between two concepts
class CSemanticLink
{
	public:
		//!Loads the contents of the link from a previously opened file
		bool Load(FILE        *a_fp,
			  CConceptMap &a_conceptMap);

		//!Saves the contents of the link to a previously opened file
		bool Save(FILE *a_fp)
		{
			if (!a_fp)
			{
				return false;
			}

			fwrite(&m_sourceID, sizeof(SemanticID), 1, a_fp);

			fwrite(&m_destID, sizeof(SemanticID), 1, a_fp);

			fwrite(&m_linkID, sizeof(SemanticID), 1, a_fp);

			fwrite(&m_type, sizeof(RelationType), 1, a_fp);

			return true;
		}

		//!Utility function used by the 'Find Path From A to B' search
		bool FindPathTo(const SemanticID       a_dstID,
						    CFindPathConfig &a_config);

		//!Gets the semantic relation type
		RelationType GetType() const
		{
			return m_type;
		}

		//!Sets the semantic relation type
		void SetType(const RelationType a_type)
		{
			m_type = a_type;
		}

		//!Gets the source concept
		CSemanticConcept *GetSource()
		{
			return m_source;
		}

		//!Gets the destination concept
		CSemanticConcept *GetDest()
		{
			return m_dest;
		}

		//!Gets the ID of the source concept
		SemanticID GetSourceID() const
		{
			return m_sourceID;
		}

		//!Gets the ID of the destination concept
		SemanticID GetDestID() const
		{
			return m_destID;
		}

		//!Sets the source concept
		void SetSource(CSemanticConcept *a_concept);

		//!Sets the destination concept
		void SetDest(CSemanticConcept *a_concept);

		//!Sets the source concept ID
		void SetSourceID(const SemanticID a_id)
		{
			m_sourceID = a_id;

			m_source   = NULL;
		}

		//!Sets the destination concept ID
		void SetDestID(const SemanticID a_id)
		{
			m_destID = a_id;

			m_dest   = NULL;
		}

		//!Gets the ID for this semantic link
		SemanticID GetID() const
		{
			return m_linkID;
		}

		//!Sets the ID for this semantic link
		void SetID(const SemanticID a_id)
		{
			m_linkID = a_id;
		}

		//!Constructor
		CSemanticLink();

		//!Destructor
		~CSemanticLink()
		{
			// NOTE: source/dest concepts not deallocated as this
			// is performed centrally by the main concept map
		}

	private:
		//!ID of the source concept
		SemanticID m_sourceID;

		//!ID of the destination concept
		SemanticID m_destID;

		//!ID of this semantic link
		SemanticID m_linkID;

		//!Semantic relation type
		RelationType m_type;

		//!Source concept
		CSemanticConcept *m_source;

		//!Destination concept
		CSemanticConcept *m_dest;
};

#endif

