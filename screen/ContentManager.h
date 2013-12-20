#pragma once

#include "ofMain.h"
#include "Spot.h"
#include "ofxBerkeliumHandler.h"
#include "SpotsDataSet.h"
#include "ofxBerkeliumCustomListener.h"

class ContentManager : public ofxBerkeliumSpotListener {

	private:
		map <string ,ofxBerkeliumSpot * > contentUsed;
		vector <ofxBerkeliumSpot * > contentFree;
		

		ofxBerkeliumSpotListener * section;
		
		map<int,map<int,SpotsDataSet *>> spotsMap;
	
		//load implementation
		int barrierReady;
		int barrierDelayed;
		int barrierLoading;
		int loadingCount;
		vector <ofxBerkeliumSpot * > delayedWebkits;


		map<string,string> mockmap;


		//debug 
		int currentShowing;
		string currentUrl;


		int loadsCounter;

		map<int,int> sectionsCounter;
		
	public:
			
		ContentManager(){
			map<int,SpotsDataSet *> screens00;
			map<int,SpotsDataSet *> screens01;
			map<int,SpotsDataSet *> screens02;

			spotsMap[0] = screens00;
			spotsMap[1] = screens01;
			spotsMap[2] = screens02;
			barrierReady = 0 ;
			barrierDelayed = 0 ;
			barrierLoading = 0 ;
			loadingCount   = 3 ;

			loadsCounter =0 ;

			/*
			checkerFBO.allocate(320,240,GL_RGBA);
			checkerFBO.begin();
			ofClear(0,0,0,255);
			checkerFBO.end();

			checkerImage.allocate(320,240,OF_IMAGE_COLOR);
			*/

		};

		void addToMockMap(string cmd);
	    void delFromMockMap(string cmd);

		void printMockMap();
		void clearMockMap();
	
		ofxBerkeliumSpot * getWebkit(string url);
		ofxBerkeliumSpot * getDelayedWebkit(string url);
		ofxBerkeliumSpot * getMockedWebkit(string url);

		void setSectionListener(ofxBerkeliumSpotListener * _section)
			{section = _section;}

		void loadMocks();

		void releasewebkits();
		void loadSpots(ofxJSONElement json,int motion,int screen);
		vector <Spot * > getSpotsForMotion(ofxJSONElement json ,int motion,int screen);
		void loadSpotsWebkits(ofxJSONElement json,int section,int screen);

		map <string,ofxBerkeliumSpot * > ContentManager::getWebkits();

		//load barrier
		void loading();
		void ready();
		void check();
		void delayed(ofxBerkeliumSpot * webkit);
		bool loaded();
		void checkDelayed();
		bool thereIsRoomInDelayBuffer();
		bool thereAreDelayedWebkits();

		void drawDebug(int x,int y,int w);
		void nextDebug();
		void prevDebug();
		void reloadDebug();
		void checkReadys();
		bool isNotOk(ofxBerkeliumSpot * aSpot);

		void loadDelayed();
		void countSection(int section);
		void countSectionInit();
		void countSectionDraw(int x,int y);
};