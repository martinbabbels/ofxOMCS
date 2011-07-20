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

#include "link_map.h"
#include "link_list.h"
#include "link.h"
#include "find_cfg.h"
#include "debug.h"


bool CLinkMap::FindPathTo(const SemanticID       a_dstID,
					      CFindPathConfig &a_config)
{
	CSemanticLinkMap_i i;

	DPRINT("visiting link map.\n");

	for (i = m_linkMap.begin();i != m_linkMap.end();++i)
	{
		a_config.VisitNode();

		(*i).second->FindPathTo(a_dstID, a_config);
	}

	return false;
}


void CLinkMap::Remove(const SemanticID a_id)
{
	CSemanticLinkMap_i i;

	i = m_linkMap.find(a_id);

	if (m_linkMap.end() != i)
	{
		m_linkMap.erase(i);
	}
}

void CLinkMap::Store(CSemanticLink *a_link)
{
	CSemanticLinkMap_i i;

	i = m_linkMap.find(a_link->GetID());

	if (m_linkMap.end() == i)
	{
		m_linkMap[a_link->GetID()] = a_link;
	}
}


bool CLinkMap::Load(FILE        *a_fp,
		    CConceptMap &a_conceptMap)
{
	if (!a_fp)
	{
		return false;
	}

	u_int l_size = 0;

	fread(&l_size, sizeof(u_int), 1, a_fp);

	printf("Loading %d links...\n", l_size);

	CSemanticLink *l_link = NULL;

	for (u_int i = 0;i < l_size;++i)
	{
		l_link = new CSemanticLink();

		if (l_link)
		{
			if (false == l_link->Load(a_fp, a_conceptMap))
			{
				printf("error loading link %d of %d\n",
					i, l_size);

				return false;
			}
		}

		Store(l_link);
	}

	return true;
}


CLinkMap::~CLinkMap()
{
	CSemanticLinkMap_i i;

	for (i = m_linkMap.begin();i != m_linkMap.end();
	     ++i)
	{
		delete (*i).second;
	}
}
