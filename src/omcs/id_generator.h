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

#ifndef _ID_GENERATOR

#define _ID_GENERATOR

#include "semantic_id.h"

//TODO: Turn this into a singleton
//TODO: Consider separating ID ranges for concepts/links

//!Generates unique numeric IDs used to represent concepts and semantic links
class CSemanticIDGenerator
{
	public:
		//!Gets a new unique ID
		SemanticID Get()
		{
			return m_currentID++;
		}

		//!Constructor
		CSemanticIDGenerator()
		{
			m_currentID = SEMANTIC_ID_START;
		}

		//!Destructor
		~CSemanticIDGenerator() {}

	private:
		//!Current ID
		SemanticID m_currentID;
};


#endif

