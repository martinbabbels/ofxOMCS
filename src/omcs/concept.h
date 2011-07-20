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

#ifndef _CONCEPT

#define _CONCEPT

#include <string>

#include "semantic_id.h"
#include "link.h"
#include "link_map.h"

class CSemanticLinkList;
class CFindPathConfig;

//!Semantic Concept
class CSemanticConcept {
public:
	//!Loads the concept from an open file
	bool Load(FILE *a_fp, CTextIDMap &a_idMap) {
		if (!a_fp) {
			return false;
		}
		
		u_int l_size;
		
		fread(&l_size, sizeof(u_int), 1, a_fp);
		
		char *l_textualName = new char[l_size];
		
		fread(l_textualName, l_size, 1, a_fp);
		
		std::string l_textualStr;
		
		l_textualStr.assign(l_textualName, l_size);
		
		//delete l_textualName;
		
		fread(&m_conceptID, sizeof(SemanticID), 1, a_fp);
		
		m_textualName =a_idMap.Store(l_textualStr, m_conceptID);
		
		if (!l_textualName) {
			printf("invalid name\n");
			return false;
		}
		
		// forward and backward links filled in after link_map
		// and concept_map loading (within omcsnet main)
		
		return true;
	}
	
	//!Saves the concept to an open file
	bool Save(FILE *a_fp) {
		if (!a_fp) {
			return false;
		}
		
		u_int l_size = m_textualName->size();
		
		fwrite(&l_size, sizeof(u_int), 1, a_fp);
		fwrite(m_textualName->c_str(), l_size, 1, a_fp);
		fwrite(&m_conceptID, sizeof(SemanticID), 1, a_fp);
		
		// no need to save links as link_map saves them
		// globally
		
		return true;
	}
	
	//!Gets the concept name
	std::string GetName() const {
		if (m_textualName) {
			return *m_textualName;
		}
		
		return "";
	}		
	
	//!Finds all paths to a particular destination concept
	bool FindPathTo(const SemanticID a_dstID, CFindPathConfig &a_config);
	
	//!Gets the concept ID
	SemanticID GetID() const {
		return m_conceptID;
	}
	
	//!Adds a forward link to another concept
	void AddLink(CSemanticLink *a_link, const RelationType   a_relation) {
		if (!m_forwardLinks) {
			p_InitForwardLinks();
		}
		
		if (!m_forwardLinks[a_relation]) {
			m_forwardLinks[a_relation] = new CLinkMap;
		}
		
		m_forwardLinks[a_relation]->Store(a_link);
	}
	
	//!Gets a link map for a particular type of forward relation
	CLinkMap *GetLinks(const RelationType a_relation) {
		if (m_forwardLinks) {
			return m_forwardLinks[a_relation];
		}
		
		return NULL;
	}
	
	//!Adds a backward link to another concept
	void AddBackwardLink(CSemanticLink *a_link, const RelationType   a_relation) {
		if (!m_backwardLinks) {
			p_InitBackwardLinks();
		}
		
		if (!m_backwardLinks[a_relation]) {
			m_backwardLinks[a_relation] =
			new CLinkMap;
		}
		
		m_backwardLinks[a_relation]->Store(a_link);
	}
	
	//!Gets a backward link map for a particular relation type
	CLinkMap *GetBackwardLinks(const RelationType a_relation) {
		if (m_backwardLinks) {
			return m_backwardLinks[a_relation];
		}
		
		return NULL;
	}
	
	//!Constructor
	CSemanticConcept(const std::string &a_name, CSemanticIDGenerator *a_idGen, CTextIDMap *a_idMap) {
		m_textualName = NULL;
		
		if (a_idGen) {
			m_conceptID = a_idGen->Get();
		}
		
		if (a_idMap) {
			m_textualName = a_idMap->Store(a_name, m_conceptID);
		}
		
		m_forwardLinks  = NULL;
		m_backwardLinks = NULL;
	}
	
	//!Destructor
	~CSemanticConcept() {
		if (m_forwardLinks) {
			for (u_int i = 0;i < MAX_RELATIONS;++i) {
				if (m_forwardLinks[i]) {
					//delete m_forwardLinks[i];
				}
			}
			
			//delete m_forwardLinks;
		}
		
		if (m_backwardLinks) {
			for (u_int i = 0;i < MAX_RELATIONS;++i) {
				if (m_backwardLinks[i])
				{
					//delete m_backwardLinks[i];
				}
			}
			
			//delete m_backwardLinks;
		}
		
		// Note: links themselves are deleted by the main
		// link map, rather than here.
	}
	
private:
	//!Initializes the forward links data structure
	void p_InitForwardLinks() {
		m_forwardLinks = new CLinkMap *[MAX_RELATIONS];
		
		for (u_int i = 0;i < MAX_RELATIONS;++i) {
			m_forwardLinks[i] = NULL;
		}
	}
	
	//!Initializes the backward links data structure
	void p_InitBackwardLinks() {
		m_backwardLinks = new CLinkMap *[MAX_RELATIONS];
		
		for (u_int i = 0;i < MAX_RELATIONS;++i) {
			m_backwardLinks[i] = NULL;
		}
	}
	
	//!Textual name for this concept
	const std::string *m_textualName;
	
	//!Numeric ID for this concept
	SemanticID  m_conceptID;
	
	//!Forward links
	CLinkMap   **m_forwardLinks;
	
	//!Backward links
	CLinkMap   **m_backwardLinks;
};


#endif

