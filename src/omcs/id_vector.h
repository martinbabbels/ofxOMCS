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

#ifndef _ID_VECTOR

#define _ID_VECTOR

#include <vector>

#include "semantic_id.h"


//!STL vector containing numeric semantic IDs
typedef std::vector<SemanticID> 	  CSemanticIDVector;

//!Iterator for semantic ID STL vector
typedef CSemanticIDVector::iterator 	  CSemanticIDVector_i;

//!Const iterator for semantic ID STL vector
typedef CSemanticIDVector::const_iterator CSemanticIDVector_ci;


#endif

