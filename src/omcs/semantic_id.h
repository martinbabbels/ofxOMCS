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

#ifndef _SEMANTIC_ID

#define _SEMANTIC_ID

#include <sys/types.h>

//!Numeric ID used to index concepts and semantic links
typedef u_long SemanticID;

//!Reserved ID used to indicate that no real ID has been set
static const SemanticID SEMANTIC_ID_NONE  = 0;
//!Reserved ID indicating the beginning of the usable ID range
static const SemanticID SEMANTIC_ID_START = 1;


#endif

