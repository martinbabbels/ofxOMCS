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

#ifndef _RELATION_TYPE

#define _RELATION_TYPE

//!Semantic relation between two concepts
typedef int RelationType;

//!Reserved relation type that indicates no relation has been set
static const RelationType RELATION_NONE = (-1);

//!Enumerated list of semantic relation types
typedef enum
{
	RELATION_CAPABLE_OF,
	RELATION_CAPABLE_OF_STATE,
	RELATION_DESIRES_EVENT,
	RELATION_DESIRES_NOT_EVENT,
	RELATION_DO,
	RELATION_EFFECT_OF,
	RELATION_EFFECT_OF_IS_STATE,
	RELATION_EVENT_FOR_GOAL_EVENT,
	RELATION_EVENT_FOR_GOAL_STATE,
	RELATION_EVENT_REQUIRES_OBJECT,
	RELATION_FIRST_SUB_EVENT_OF,
	RELATION_IS_A,
	RELATION_LAST_SUB_EVENT_OF,
	RELATION_LOCATION_OF,
	RELATION_MADE_OF,
	RELATION_OFTEN_NEAR,
	RELATION_PART_OF,
	RELATION_POST_EVENT_OF,
	RELATION_PROPERTY_OF,
	RELATION_SUB_EVENT_OF,
	RELATION_USED_FOR,
	MAX_RELATIONS,
} RelationTypeValue;

//!Structure used for Type<->textual name mapping
typedef struct RelationMapping
{
	char              m_text[128];

	RelationTypeValue m_type;
};

//!Array of textual name <-> relation type mappings
static const RelationMapping g_RelationTypeNames[MAX_RELATIONS] =
{
	{"CapableOf",		RELATION_CAPABLE_OF},
	{"CapableOfState",	RELATION_CAPABLE_OF_STATE},
	{"DesiresEvent",	RELATION_DESIRES_EVENT},
	{"DesiresNotEvent",	RELATION_DESIRES_NOT_EVENT},
	{"Do",			RELATION_DO},
	{"EffectOf",		RELATION_EFFECT_OF},
	{"EffectOfIsState",	RELATION_EFFECT_OF_IS_STATE},
	{"EventForGoalEvent",	RELATION_EVENT_FOR_GOAL_EVENT},
	{"EventForGoalState",	RELATION_EVENT_FOR_GOAL_STATE},
	{"EventRequiresObject",	RELATION_EVENT_REQUIRES_OBJECT},
	{"FirstSubeventOf",	RELATION_FIRST_SUB_EVENT_OF},
	{"IsA",			RELATION_IS_A},
	{"LastSubeventOf",	RELATION_LAST_SUB_EVENT_OF},
	{"LocationOf",		RELATION_LOCATION_OF},
	{"MadeOf",		RELATION_MADE_OF},
	{"OftenNear",		RELATION_OFTEN_NEAR},
	{"PartOf",		RELATION_PART_OF},
	{"PostEventOf",		RELATION_POST_EVENT_OF},
	{"PropertyOf",		RELATION_PROPERTY_OF},
	{"SubeventOf",		RELATION_SUB_EVENT_OF},
	{"UsedFor",		RELATION_USED_FOR}
};


//!Provides Text->RelationType and RelationType->Text conversion for semantic relation types
class CRelationTypeLookup
{
	public:
		//!Lookup a textual representation of a semantic relation type
		std::string Lookup(const RelationType a_type)
		{
			if (a_type < MAX_RELATIONS)
			{
				return g_RelationTypeNames[a_type].m_text;
			}

			return "";
		}

		//!Lookup a numeric representation of a textual semantic relation
		RelationType Lookup(const std::string &a_name)
		{
			for (u_int i = 0;i < MAX_RELATIONS;++i)
			{
				if (g_RelationTypeNames[i].m_text ==
				    a_name)
				{
					return g_RelationTypeNames[i].m_type;
				}
			}
		
			return RELATION_NONE;
		}

	private:
};

#endif

