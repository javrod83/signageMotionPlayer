#pragma once

#include "ofMain.h"
#include "Screen.h"
#include "ofxJSONElement.h"
#include "ofGstVideoPlayer.h"
#include "ofxBerkeliumHandler.h"
#include "ofxBerkeliumCustomListener.h"

#include "ContentManager.h"

class Section : public  ofxBerkeliumMenuListener , public ofxBerkeliumSpotListener  {
	
private:
	
	vector<Screen *> screensVector;
	vector<ofGstVideoPlayer *> motionsVector;
	
	ofxBerkeliumMenu * menu;
	ofxBerkeliumHandler * corner;
	ofxBerkeliumHandler * main;
	
	ofxJSONElement json;

	ContentManager contentManager;

	//transition
	int		fadeOverlayAlpha;
	int		fadeStep;
	int		nextSection;
	bool	fadeInFlag;
	bool	fadeOutFlag;
	bool	fadeOveylayOn;

	//get ready before play
	bool loading;
	
	//Resolution of the app
	int width ;
	int height ;

	//we will warpyou (viewport)
	int viewportWidth ;
	int viewportHeight ;

	int screens;
	int maxPosts;
	bool drawingCtrlView;
	bool debug;

	//logoTimerAndAlpha
	float logosTimerCount;
	float logosStartTime;
	bool  logosTimerRunning;
	bool  logosFading;
	float logosAlphaStep;
	float logosCurrentAlpha;



	//pause feature
	int pausedFrame;
	bool pausedToggle;

	//ctrl view
	int ctrlScreen;

public:

	void setCtrlScreen(int scr);

	void logosUpdate();
	void logosStartFadeOut();
	void logosSetup();
	void toggleCtrlView();

	void setDebug(bool _debug);

	//inicializar la seccion incluyundo app (width,height) screens viewport (width,height) 
	void init(int w,int h,int s,int vw,int vh);
	void initMotions();
	void initScreens();
	void changeTo(int section);
	void loadSection(int section);


	void loadContent(ofxJSONElement _json,int section);
	void load(ofxJSONElement _json);
	void loadAlphaMotions(ofxJSONElement json);
	void loadH264Motions(ofxJSONElement json);
	void release();
	void draw();
	void drawScreen(int scr,float x,float y ,float w,float h);
	void update();
	
	void fadeOut();
	void fadeUpdate();
	void fadeIn();
	bool fadeIsFading();
	void fadeDraw();
	
	void play();
	void pause();
	bool isPaused();
	void stop();
	void fastFoward();
	void rewind();


	void debugDraw();
	void debugUp();
	void debugDown();
	void debugReload();
	void loadCurrentDelayed();
	void checkContent();
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void menuCommand(string cmd);
	void inScreenShowFull(int screen,int spot);
	void inScreenShowRandomFull();

	bool isLoading();


	void mockListPrint()
		{
			contentManager.printMockMap();	
		}
	void mockListClear()
		{
			contentManager.clearMockMap();
		}
};
