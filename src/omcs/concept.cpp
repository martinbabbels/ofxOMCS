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

#include "concept.h"
#include "find_cfg.h"
#include "debug.h"

bool CSemanticConcept::FindPathTo(const SemanticID a_dstID, CFindPathConfig &a_config) {
	a_config.VisitNode();

	DPRINT("visiting %s %d %p\n", m_textualName.c_str(),
		GetID(), this);

	if (m_forwardLinks) {
		for (u_int i = 0;i < MAX_RELATIONS;++i) {
			if (m_forwardLinks[i]) {
				m_forwardLinks[i]->
					FindPathTo(a_dstID, a_config);
			}
		}
	}

	a_config.LeaveNode();

	return false;
}


