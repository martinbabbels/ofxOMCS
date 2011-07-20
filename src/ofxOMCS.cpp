#include "ofxOMCS.h"


// Public
void ofxOMCS::savePredicates(string pathToPredicates) {
	if (!OMCSNet.Save(pathToPredicates)) {
		printf("Error saving %s\n", pathToPredicates.c_str());
	}
}

void ofxOMCS::loadPredicates(string pathToPredicates) {
	if (!OMCSNet.Load(pathToPredicates)) {
		printf("Error loading %s\n", pathToPredicates.c_str());
	}
}

void ofxOMCS::loadPredicatesText(string pathToPredicates) {
	if (!OMCSNet.LoadPredicateFile(pathToPredicates)) {
		printf("Error loading %s\n", pathToPredicates.c_str());
	}
}

void ofxOMCS::findAnalogous(string &concept, list<AnalogousResult> &results) {
	
	CFindAnalogousConfig cfg;
	cfg.SetNodeName(concept);
	bool result = OMCSNet.FindAnalogous(cfg);
	
	if(result) {
		CAnalogousWeightNode *wn = NULL;
		float f_score = 0.0;
		
		for(u_int i=0; i<cfg.GetNumResults(); i++) {
			wn = cfg.GetResult(i, &f_score);
			
			if(wn) {
				
				CSemanticLinkMap_i linkIterator;
				linkIterator = wn->GetLinkIterator();
				CSemanticLink *link = NULL;
				
				CRelationTypeLookup lookup;
				AnalogousResult ar;
				
				ar.concept = wn->GetConcept()->GetName();
				
				int i_score = f_score *100 + 0.5;
				ar.score = i_score;
				
				do {
					link = wn->GetLink(linkIterator);
					if(link) {
						AnalogousConcept ac;
						ac.concept = link->GetDest()->GetName().c_str();
						ac.relation = lookup.Lookup(link->GetType());
						ar.concepts.push_back(ac);
					}
					
				} while(link);
				
				results.push_back(ar); 
			}
		}
	}
}

void ofxOMCS::findPath(string &concept_a, string &concept_b, list<PathResults> &results) {
	CFindPathConfig cfg;
	
	bool result = OMCSNet.FindPathFromA_To_B(concept_a, concept_b, cfg);
	
	if(result) {
		
		for(u_int i=0; i<cfg.GetNumResults(); i++) {
			CSemanticLinkList *linkList = cfg.GetResult(i);
			CLinkList_i linkIterator;
			CSemanticLink *link = NULL;
			linkIterator = linkList->GetIterator();
			list<PathResult> p;
			PathResults prs;
			
			do {
				link = linkList->GetNext(linkIterator);
				if(link) {
					CRelationTypeLookup lookup;
					PathResult pr;
					pr.conceptA = OMCSNet.LookupString(link->GetSourceID());
					pr.conceptB = OMCSNet.LookupString(link->GetDestID());
					pr.relation = lookup.Lookup(link->GetType());
					p.push_back(pr);
				}
				
			} while(link);
			prs.paths = p;
			results.push_back(prs);
		}
	}
}


void ofxOMCS::findContext(string &concept, list<ContextResult> &results) {
	CFindContextConfig cfg;
	
	bool result = OMCSNet.FindContext(concept, cfg);
	
	if(result) {
		CSemanticConcept *semanticConcept = NULL;
		float f_score = 0.0;
		
		for(u_int i=0; i<cfg.GetNumResults(); i++) {
			semanticConcept = cfg.GetResult(i, &f_score);
			if(semanticConcept) {
				ContextResult cr;
				cr.concept = semanticConcept->GetName();
				cr.score = (int)(f_score*100);
				results.push_back(cr);
			}
		}
	}
}

void ofxOMCS::lookUp(string &concept, LookUpResult &result) {
	CSemanticConcept *semanticConcept = OMCSNet.LookupConcept(concept);
	
	if(semanticConcept) {
		
		CLinkMap *linkMap = NULL;
		CRelationTypeLookup lookup;
		list<LookUpResultLink> lrl;
		LookUpResultLinks lrls;
		
		u_int i=0;
		
		for(i=0; i<MAX_RELATIONS; i++) {
			// Forward links
			linkMap = semanticConcept->GetLinks(i);
			if(linkMap!=NULL) {
				lrl = lookUpLinks(linkMap, (RelationType)i, false);
				lrls.concepts = lrl;
				lrls.relation = lookup.Lookup((RelationType)i);
				result.forward.push_back(lrls);
			}
			
			
		}
	
		for(i=0; i<MAX_RELATIONS; i++) {
			// Backward links
			linkMap = semanticConcept->GetBackwardLinks(i);
			if(linkMap!=NULL) {
				lrl = lookUpLinks(linkMap, (RelationType)i, true);
				lrls.concepts = lrl;
				lrls.relation = lookup.Lookup((RelationType)i);
				result.backward.push_back(lrls);
			}
		}
	}
}

COMCSNet ofxOMCS::getOMCS() {
	return OMCSNet;
}

// Private
list<LookUpResultLink> ofxOMCS::lookUpLinks(CLinkMap *linkMap, const RelationType relation, const bool backwardFlag) {
	
	list<LookUpResultLink> r;
	CRelationTypeLookup lookup;
	CSemanticLinkMap_i linkIterator = linkMap->GetIterator();
	CSemanticLink *link = NULL;
	
	do {
		
		link = linkMap->GetNext(linkIterator);
		
		if(link) {
			LookUpResultLink lrl;
			lrl.relation = lookup.Lookup(relation);
			if(backwardFlag) {
				lrl.concept = OMCSNet.LookupString(link->GetSourceID());
			}
			else {
				lrl.concept = OMCSNet.LookupString(link->GetDestID());
			}
			
			r.push_back(lrl);
		}
	} while (link);
	
	return r;
}

string ofxOMCS::upToLow(string s) {
    for (int i=0;i<strlen(s.c_str());i++) 
        if (s[i] >= 0x41 && s[i] <= 0x5A) 
            s[i] = s[i] + 0x20;
    return s;
}

bool ofxOMCS::existInList(string n, vector <string> h) {
	for(int i=0; i<h.size(); i++) {
		if(n==h[i]) return true;
	}
	
	return false;
}

