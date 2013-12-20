#include "testApp.h"

#define STRINGIFY(A) #A

//Resolution of the app
#define WIDTH 1920
#define HEIGHT 1080

//warp to viewport

#define PRODUCTION true 

#define VIEWPORTWIDTH 1920
#define VIEWPORTHEIGHT 1080

#define VIEWPORTWIDTH_DEBUG 600
#define VIEWPORTHEIGHT_DEBUG 337

#define USE_TRANSPARENCY true

//--------------------------------------------------------------
void testApp::setup(){
	debug= false;
	if(debug)cout << " testApp::setup begin" << endl ; 
	ofSetVerticalSync(true);
	
	setupJson();
	
	Berkelium::init( Berkelium::FileString::empty() );

	//if (PRODUCTION)
		mySection.init(WIDTH,HEIGHT,3,VIEWPORTWIDTH,VIEWPORTHEIGHT);
		//mySection.init(WIDTH,HEIGHT,3,VIEWPORTWIDTH_DEBUG,VIEWPORTHEIGHT_DEBUG);


	mySection.setDebug(debug);
	
	
	mySection.loadSection(2);
	if(debug)cout << " testApp::setup end" << endl ; 
	
	if(PRODUCTION)ofHideCursor();

	updating=true;
}

void testApp::setupJson(){
	if(debug)cout << " testApp::setupJson begin" << endl ; 
   
	//JSON SETUP !
	string url = "http://127.0.0.1:3500/section/2/json";
    bool gotJson = json.open(url);
	

	while(!gotJson)
		{
			ofSleepMillis(500);
			gotJson = json.open(url);
			cout << " WAITNG !!!!!! !!!!!!!!!!" <<endl;
		}


	//JSON SETUP !
	string url2 = "http://127.0.0.1:3500/section/2/json";
    bool gotJson2 = json2.open(url2);
	
	cout << "[[testApp::setupJson]] posts: "<< ofToString(json["posts"].size())<< " begin" << endl ; 

	if(debug)cout << " testApp::setupJson end" << endl ; 
}
 

//--------------------------------------------------------------
void testApp::update(){
	
	if(debug)cout << "testApp::update begin" << endl;

	mySection.update();

	    try {
		    Berkelium::update();

    } catch( std::exception& e) {
        std::cout << " IN BERKELIUM UPDATE EVETN" << e.what() << "'\n";
    }
 


	
	if(debug)cout << "testApp::update end" << endl;
}


void testApp::draw(){

	mySection.draw();

	if (!PRODUCTION)
		mySection.debugDraw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
	case '1':
		mySection.changeTo(1);
		break;
	case '2':
		mySection.changeTo(2);
		break;
	case '3':
		mySection.changeTo(3);
		break;
	case '4':
		mySection.changeTo(4);
		break;
	case '5':
		mySection.changeTo(5);
		break;
	case '6':
		mySection.changeTo(6);
		break;
	case '7':
		mySection.changeTo(7);
		break;
	//case ' ':
		//mySection.toggleCtrlView();
		//break;
	//case 'p':
	//	updating = !updating;
		//break;
	//case 'p':
	//	updating = !updating;
		//break;
	case 'p':
		mySection.pause();
		break;
	case 't':
		mySection.loadCurrentDelayed();
		break;
	case 'y':
		mySection.mockListPrint();
		break;
	case 'u':
		mySection.mockListClear();
		break;
		
	case OF_KEY_DOWN:
		mySection.debugUp();
		break;
	case OF_KEY_UP:
		mySection.debugDown();
		break;
	case OF_KEY_LEFT:
		mySection.rewind ();
		break;
	case OF_KEY_RIGHT:
		mySection.fastFoward();
		break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	mySection.mouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	mySection.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	mySection.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

 	mySection.mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}

//--------------------------------------------------------------
void testApp::urlResponse(ofHttpResponse & response){}



