#pragma once

#include "ofMain.h"
#include "SpotPoint.h"
#include "ofxJSONElement.h"
#include "ofxBerkeliumHandler.h"
#include "ofxBerkeliumCustomListener.h"


class Spot{

	private:
		int frameCurrent;
		int frameFirstFront;
		int frameLastFront;
		int frameFirstBack;
		int frameLastBack;
		int number;

		//map <int,SpotPoint*> pointsMap;
		map <int,SpotPoint*> frontPointsMap;
		map <int,SpotPoint*> backPointsMap;

		SpotPoint * frontFollower, *backFollower;

		SpotPoint* pointInterPolateFrom(SpotPoint* pointPrev,SpotPoint* pointCurr,int framePrev,int _frameCurrent,int frameTop);
		//SpotPoint* currentPoint;


		ofxBerkeliumSpot* frontWebkit,* backWebkit,* fullWebkit;

		//last know screen
		//Screen * lastScreen;
	
		int frontAlpha;
		int backAlpha;


		ofTrueTypeFont	verdana;

	public:
		 Spot(){};
		//TODO SpotPoint* getPointForFrame(int frame);
		 
		//void loadPoints( ofxJSONElement json,int _number);
		void loadFrontPoints( ofxJSONElement json,int _number);
		void loadBackPoints( ofxJSONElement json,int _number);

		//void loadMockPoints( ofxJSONElement json,int motion,int spot,string screen);
		void cleanPoints();
		void drawPointAt(int frame,int id);
		SpotPoint* getBackPointForFrame(int frame);
		SpotPoint* getFrontPointForFrame(int frame);
		bool appearsFront(int frame);
		bool appearsBack(int frame);

		bool empty();
		
		void setCurrentFrame(int theCurrentFrame);
		void setBack(ofxBerkeliumSpot * newBack);
		void setFront(ofxBerkeliumSpot * newFront);
		void setFull(ofxBerkeliumSpot * newFull);

		void drawBack();
		void drawFull();
		void drawFront();
		void drawPoints();

		bool mouseIsOver(int _x,int _y);
		void mouseMoved(int _x, int _y);
		void mouseDragged(int _x, int _y, int button);
		void mousePressed(int _x, int _y, int button,int screen,int spot);
		void mouseReleased(int _x, int _y, int button);

		string getInfoString();

		void allMightyMethod(int frameCur);
		//void addPoint(int frame,SpotPoint * aPoint);
		void addBackPoint(int frame,SpotPoint * aPoint);
		void addFrontPoint(int frame,SpotPoint * aPoint);

		void smoothOutFronts(int first,int last);
		void smoothOutBacks(int first,int last);

		void printInfo();

		ofxBerkeliumSpot * getFull();
};