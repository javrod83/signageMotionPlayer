#pragma once

#include "ofMain.h"
#include "ofGstVideoPlayer.h"

#include "ofxJSONElement.h"

#include "ofxBerkelium.h"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"
#include "ofxBerkeliumHandler.h"
#include "ofxBerkeliumCustomListener.h"
#include "Spot.h"

class Screen : public ofxBerkeliumSpotListener {

	private:
		int screenNumber;
		int width;
		int height;
		int viewPortWidth;
		int viewPortHeight;
		float xOffset;
		float yOffset;
		int motionCurrentFrame;

		//SPOTS 
		vector<Spot*> spotsVector;

		//MOTION 
		ofGstVideoPlayer * motion;
		bool motionAlpha;
		ofTexture motionTexture;

		//FBO
		 ofFbo       fbo;
		 ofShader    shader;

		//CONTENT
		vector<ofxBerkeliumSpot *> contentFront;
		vector<ofxBerkeliumSpot *> contentBack;
		
		//MENU
		ofxBerkeliumHandler * menu;
		bool useMenu;

		//LOGO
		ofxBerkeliumHandler * main;
		bool useMain; 

		//BIG LOGO
		ofxBerkeliumHandler * corner;
		bool useCorner;
		
		//Full Webkit
		ofxBerkeliumSpot * full;
		bool useFull;
		bool fullFading;
		bool fullFadeOut;
		float fullAlpha;
		float fullStep;

		//Overlay
		bool overlaySetDrawing;
		int overLayAlpha;

	public:

		void overlaySetDraw(bool draw);
		void overlaySetAlpha(int alpha);
		void overlayDraw();

		Screen(int number,int _width,int _height,float xOff,float yOff,int vpw,int vph);
		//TO-DO destructor
		void setSpotsFromJson(ofxJSONElement json,map <int,ofxBerkeliumHandler*> * fronts,map <int,ofxBerkeliumHandler*> * backs);
		void setAlphaMotion(ofGstVideoPlayer * _motion);
		void setH264Motion(ofGstVideoPlayer * _motion);
		void playMotion();
		void stopMotion();
		void setFBO();
		void addContent(ofxBerkeliumSpot * front,ofxBerkeliumSpot * back);
		void bakeFBO();
		void saveFrame();
		void update();
		void drawSpots();
		void drawCurrentSpots();
		void draw();
		void drawWarped(int x,int y,int w,int h);
		void drawAt(int x,int y ,int w,int h);
		void drawDebug();
		void drawInfo();
		bool usingMenu();
		void setMenu(ofxBerkeliumHandler * _menu);
		void drawMenu();
		
		void fastFoward();
		void rewind();

		void setFullfromSpot(int spot);
		bool usingFull();
		void setFull(ofxBerkeliumSpot * _full);
		void drawFull();
		void showFull();
		void hideFull();
		void fadeFullUpdate();
		void terminateFull();
		
		bool usingMain();
		void setMain(ofxBerkeliumHandler * _main);
		void drawMain();

		bool usingCorner();
		void setCorner(ofxBerkeliumHandler * _corner);
		void drawCorner();

		void setPoints(vector<Spot *> _spots);
		int getCurrentFrame();
		//mouse interface
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		bool mouseIsOver(int x,int y);

		//the fucking mock
		ofxJSONElement mockJson;
		void mockJsonSet(ofxJSONElement mj,string scr);
		string selectedScreen;
		void mockJsonUpdate(ofxBerkeliumHandler *  content,bool front);
		float x1,x2,dx,sx,cx;
		float y1,y2,dy,sy,cy;
		int f1,f2,df,dcf;
		float s1,s2,ds,ss,cs;
		int motionID;
		int spots;



};

class ScreenListener: public Screen {

		//spots
		//motion
		//alpha

	public:

		void unMetodo();
		
};



class ScreenBrodcaster: public Screen {

	private:
		//spots
		//motion
		//alpha

		vector<ScreenListener *> listener; 

	public:

		void addListener(ScreenListener * anotherListener);

};