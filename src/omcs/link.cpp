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

//#define DEBUG

#include "link.h"
#include "link_list.h"
#include "concept.h"
#include "find_cfg.h"
#include "debug.h"

bool CSemanticLink::FindPathTo(const SemanticID       a_dstID,
						   CFindPathConfig &a_config)
{
	DPRINT("visiting link.\n");

	a_config.VisitLink(this);

	if (m_dest)
	{
		if (m_dest->GetID() == a_dstID)
		{
			DPRINT("adding path to results.\n");

			a_config.AddPathToResults();

			a_config.LeaveLink();

			return true;
		}
		else if (true == a_config.AllowVisit())
		{
			DPRINT("queueing path.\n");
			a_config.QueuePath();
		}
	}
	else DPRINT("invalid link\n");


	a_config.LeaveLink();

	return false;
}


CSemanticLink::CSemanticLink()
{
	m_source = NULL;
	m_dest   = NULL;

	m_sourceID = 0;
	m_destID   = 0;

	m_linkID = SEMANTIC_ID_NONE;

	m_type	   = RELATION_NONE;
}


void CSemanticLink::SetSource(CSemanticConcept *a_concept)
{
	if (a_concept)
	{
		m_source = a_concept;

		m_sourceID = m_source->GetID();
	}
}

void CSemanticLink::SetDest(CSemanticConcept *a_concept)
{
	if (a_concept)
	{
		m_dest = a_concept;

		m_destID = m_dest->GetID();
	}
}

bool CSemanticLink::Load(FILE        *a_fp,
		         CConceptMap &a_conceptMap)
{
	if (!a_fp)
	{
		return false;
	}

	fread(&m_sourceID, sizeof(SemanticID), 1, a_fp);

	fread(&m_destID, sizeof(SemanticID), 1, a_fp);

	fread(&m_linkID, sizeof(SemanticID), 1, a_fp);

	fread(&m_type, sizeof(RelationType), 1, a_fp);

	CSemanticConcept *l_concept = NULL;

	l_concept = a_conceptMap.Lookup(m_sourceID);

	if (!l_concept)
	{
		printf("error linking source concept [%d]\n", (int)m_sourceID);

		return false;
	}

	m_source = l_concept;

	l_concept = a_conceptMap.Lookup(m_destID);

	if (!l_concept)
	{
		printf("error linking dest concept [%d]\n", (int)m_destID);

		return false;
	}

	m_dest = l_concept;

	m_source->AddLink(this, m_type);

	m_dest->AddBackwardLink(this, m_type);

	return true;
}


