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

#include "concept_map.h"
#include "concept.h"


void CConceptMap::Store(CSemanticConcept *a_concept) {
	CSemanticConceptMap_i i;

	i = m_conceptMap.find(a_concept->GetID());

	if (m_conceptMap.end() == i) {
		m_conceptMap[a_concept->GetID()] = a_concept;
	}
}

bool CConceptMap::Load(FILE *a_fp, CTextIDMap &a_idMap) {
	if (!a_fp) {
		return false;
	}

	u_int l_size = 0;

	fread(&l_size, sizeof(u_int), 1, a_fp);

	printf("Loading %d concepts..\n", l_size);

	CSemanticConcept *l_concept = NULL;

	for (u_int i = 0;i < l_size;++i) {
		l_concept = new CSemanticConcept("", NULL, NULL);

		if (l_concept) {
			if (false == l_concept->Load(a_fp, a_idMap)) {
				printf("error loading concept %d of %d\n",
					i, l_size);

				return false;
			}
		}

		Store(l_concept);
	}

	return true;
}

bool CConceptMap::Save(FILE *a_fp) {
	if (!a_fp) {
		return false;
	}

	u_int l_size = m_conceptMap.size();

	fwrite(&l_size, sizeof(u_int), 1, a_fp);

	CSemanticConceptMap_i i;

	for (i = m_conceptMap.begin();i != m_conceptMap.end();++i) {
		if (false == (*i).second->Save(a_fp)) {
			return false;
		}
	}

	return true;
}


CConceptMap::~CConceptMap() {
	CSemanticConceptMap_i i;

	for (i = m_conceptMap.begin();i != m_conceptMap.end(); ++i) {
		delete (*i).second;
	}
}
