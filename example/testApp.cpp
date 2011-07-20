#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	
	// Load the predicates
	omcsnet.loadPredicates(ofToDataPath("predicates.dat"));
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	// Analogous concepts
	if(key=='a') {
		
		string concept = "apollo";
		list<AnalogousResult> r;
		list<AnalogousResult>::iterator result_iterator;
		
		list<AnalogousConcept> c;
		list<AnalogousConcept>::iterator concepts_iterator;
		
		omcsnet.findAnalogous(concept, r);
		printf("*************************************************\n");
		printf("Analogous concepts for %s\n", concept.c_str());
		printf("*************************************************\n");
		result_iterator = r.begin();
		while(result_iterator != r.end()) {
			
			printf("%s (%s%%) because both:\n", 
				   (*result_iterator).concept.c_str(), 
				   ofToString((int)(*result_iterator).score).c_str()
			);
			
			concepts_iterator = (*result_iterator).concepts.begin();
			while(concepts_iterator != (*result_iterator).concepts.end()) {
				printf("\t%s %s\n", 
					   (*concepts_iterator).relation.c_str(), 
					   (*concepts_iterator).concept.c_str()
				);
				++concepts_iterator;
			}
			
			printf("\n\n");
			++result_iterator;
		}
	}
	
	// Path cocept A to B
	if(key=='p') {
		string concept_a = "freak";
		string concept_b = "day";
		
		printf("*************************************************\n");
		printf("Path from %s to %s\n", concept_a.c_str(), concept_b.c_str());
		printf("*************************************************\n");
		list<PathResults> r;
		list<PathResults>::iterator result_iterator;
		list<PathResult>::iterator path_iterator;
		
		omcsnet.findPath(concept_a, concept_b, r);
		
		result_iterator = r.begin();
		while (result_iterator != r.end()) {
			printf("\n");
			path_iterator = (*result_iterator).paths.begin();
			while(path_iterator != (*result_iterator).paths.end()) {
				printf("%s is %s %s\n",
					   (*path_iterator).conceptA.c_str(), 
					   (*path_iterator).relation.c_str(), 
					   (*path_iterator).conceptB.c_str()
				);
				++path_iterator;
			}
			++result_iterator;
		}
	}
	
	// Context concepts
	if(key=='c') {
		string concept = "framework";
		
		printf("*************************************************\n");
		printf("Context for %s\n", concept.c_str());
		printf("*************************************************\n");
		list<ContextResult> r;
		list<ContextResult>::iterator result_iterator;
		
		omcsnet.findContext(concept, r);
		
		result_iterator = r.begin();
		while(result_iterator != r.end()) {
			printf("%s (%s%%)\n", 
				   (*result_iterator).concept.c_str(), 
				   ofToString((*result_iterator).score).c_str()
			);
			++result_iterator;
		}
	}
	
	// Look up a concept
	if(key=='l') {
		string concept = "computer";
		
		printf("*************************************************\n");
		printf("Lookup concept %s\n", concept.c_str());
		printf("*************************************************\n");
		LookUpResult r;
		
		list<LookUpResultLinks>::iterator result_iterator;
		list<LookUpResultLink>::iterator concepts_iterator;
		
		omcsnet.lookUp(concept, r);
		
		printf("Forward:\n");
		result_iterator = r.forward.begin();
		while(result_iterator != r.forward.end()) {
			printf("[%s]\n", (*result_iterator).relation.c_str());
			
			concepts_iterator = (*result_iterator).concepts.begin();
			while(concepts_iterator != (*result_iterator).concepts.end()) {
				printf("\t%s\n", 
					   (*concepts_iterator).concept.c_str()
					   );
				++concepts_iterator;
			}
			++result_iterator;
		}
		
		printf("Backward:\n");
		result_iterator = r.backward.begin();
		while(result_iterator != r.backward.end()) {
			printf("[%s]\n", (*result_iterator).relation.c_str());
			
			concepts_iterator = (*result_iterator).concepts.begin();
			while(concepts_iterator != (*result_iterator).concepts.end()) {
				printf("\t%s\n", 
					   (*concepts_iterator).concept.c_str()
					   );
				++concepts_iterator;
			}
			++result_iterator;
		}
	}
	 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}