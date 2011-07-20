/*
 *  ofxOMCS.h
 *  
 *
 *  Created by Martin Bartels on 5/23/10.
 *  Copyright 2010 Apollomedia. All rights reserved.
 *
 */

// Include OF
#include "ofMain.h"

// Include OMCS & helpers
#include <sys/types.h>
#include "omcsnet.h"
#include "concept.h"
#include "find_cfg.h"
#include "findc_cfg.h"
#include "finda_cfg.h"

#ifndef _OMCS
#define _OMCS

typedef struct {
	string concept;
	string relation;
} AnalogousConcept;

typedef struct {
	string concept;
	int score;
	list<AnalogousConcept> concepts;
} AnalogousResult;

typedef struct {
	string conceptA;
	string conceptB;
	string relation;
} PathResult;

typedef struct {
	list<PathResult> paths;
} PathResults;

typedef struct {
	string concept;
	int score;
} ContextResult;

typedef struct {
	string concept;
	string relation;
} LookUpResultLink;

typedef struct {
	string relation;
	list<LookUpResultLink> concepts;
} LookUpResultLinks;

typedef struct {
	list<LookUpResultLinks> forward;
	list<LookUpResultLinks> backward;
} LookUpResult;

class ofxOMCS {

public:
	
	void savePredicates(string pathToPredicates);
	void loadPredicates(string pathToPredicates);
	void loadPredicatesText(string pathToPredicates);
	
	void findAnalogous(string &concept, list<AnalogousResult> &results);
	void findPath(string &concept_a, string &concept_b, list<PathResults> &results);
	void findContext(string &concept, list<ContextResult> &results);
	void lookUp(string &concept, LookUpResult &result);
	
	COMCSNet getOMCS();

private:
	
	list<LookUpResultLink> lookUpLinks(CLinkMap *linkMap, const RelationType relation, const bool backwardFlag);
	bool existInList(string n, vector <string> h);
	string upToLow(string s);
	
	COMCSNet OMCSNet;
};
#endif

