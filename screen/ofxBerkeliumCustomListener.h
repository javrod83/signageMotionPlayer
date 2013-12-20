#pragma once
#include "ofMain.h"
#include "ofxBerkeliumHandler.h"


class ofxBerkeliumMenuListener {

	public:	
		virtual void menuCommand(string cmd)
			{
				cout << "[ofxBerkeliumCustomListerner]new command from menu \n cmd : ["+cmd+"]" << endl;
			}

		virtual void changeTo(int sectionNumber)
			{
				cout << "[ofxBerkeliumSectionListener:changeSection]["+ofToString(sectionNumber)+"]" << endl;
			}
};


class ofxBerkeliumSpotListener {
	public:	

		virtual void loading()
		{
			cout << "[ofxBerkeliumSectionListener:loading]" << endl;
		}

		virtual void ready()
		{
			cout << "[ofxBerkeliumSectionListener:ready]" << endl;
		}
		virtual void requested()
		{
			cout << "[ofxBerkeliumSectionListener:requested]" << endl;
		}
		virtual void  showFull(int spot)
		{
			cout << "[ofxBerkeliumSectionListener:showFull]" << endl;
		}

	virtual void inScreenShowFull(int screen,int section){
			cout << "[ofxBerkeliumSectionListener:inScreenShowFull]" << endl;
		}

	virtual void hideFull(){
			cout << "[ofxBerkeliumSectionListener:hideFull]" << endl;
		}


	virtual void addToMockMap(string cmd){}
	virtual void delFromMockMap(string cmd){}

	virtual	void printMockMap(){}
	virtual	void clearMockMap(){}
	virtual void check(){}
};




class ofxBerkeliumMenu :  public  ofxBerkeliumHandler {

	private:
		ofxBerkeliumMenuListener * listener;
		bool listenerSeted;
	
	
	public:
		virtual void setCustomListener(ofxBerkeliumMenuListener * _customListener)
			{
				listener = _customListener;
			}
		virtual void onConsoleMessage(wstring message, wstring sourceId, int line_no)
			{
				wcout << "ofxBerkeliumMenu" << endl;
				wcout << "message : [" << message << "]" << endl;
				string str( message.begin(), message.end() );
				if ( str == "ready")
					
				{
						loaded=true;
			
				}
				
				
				string delimiter = "=";
				size_t pos = 0;
				std::string token;
				vector <string> command;
				pos = str.find(delimiter);
				token = str.substr(0, pos);
				cout << "CMD:" + token << endl;
				if (token == "section")
					{
						str.erase(0, pos + delimiter.length());
						pos = str.find(delimiter);
						token = str.substr(0, pos);
						cout << "CHAGNE TO SECTION:" + token << endl;
						listener->changeTo(ofToInt(token));
					}

			
				}



};



 class ofxBerkeliumSpot :  public  ofxBerkeliumHandler {

	private:
	
		ofxBerkeliumSpotListener * section;
		ofxBerkeliumSpotListener * screen;
		ofxBerkeliumSpotListener * loader;
		bool listenerSeted;
		int  clickedScreen;
		int  clickedSpot;
		int  id;
		int  requested;
		
		string delayedUrl;

		ofTrueTypeFont	verdana;
		ofImage		readySnapShot;
		unsigned char* 	readySnapShotPixels;

		ofImage		loadSnapShot;
		unsigned char* 	loadSnapShotPixels;
		ofFbo			fbo;
		int				reloadCount;
		string			areEqual;

	public:
		

		 

		void  initializeDelayed(string url){
	
			initializeBrowser();
			setDelayedUrl(url);
			loaded=false;
			myAlpha =255;

			fbo.allocate(320,240,GL_RGBA);
			
			readySnapShot.allocate(320,240,OF_IMAGE_COLOR_ALPHA);
			loadSnapShot.allocate(320,240,OF_IMAGE_COLOR_ALPHA);
			readySnapShotPixels = new unsigned char[320*240*4]; 
			loadSnapShotPixels = new unsigned char[320*240*4]; 

			verdana.loadFont("verdana.ttf", 10, true, true);
			reloadCount=0;
		}

		void drawDebug(int x,int y, int w,int h)
			{
				ofPushStyle();
				if (!loaded){	ofSetColor(255,0,0);}
				else	{	ofSetColor(0,255,0);}

				ofNoFill();
				ofPushMatrix();
				ofTranslate(x,y);
				ofSetLineWidth(2);
				ofRect(1,1,w-1,h-1);
				ofSetColor(255,255,255);
				browser->draw(0,0,w,h);


				//loadSnapShot.draw(0,0,w*.2,h*.2);
				//readySnapShot.draw(w*.8,0,w*.2,h*.2);
				//fbo.draw(w*.8,h*.2,w*.2,h*.2);
				//verdana.(h*.7);
				verdana.drawString("r: "+ofToString(reloadCount)+" "+areEqual,3 ,h-(h*.7));

				ofPopMatrix();
				ofPopStyle();
			}


		void drawCheck(int x,int y, int w,int h)
			{
				ofPushStyle();
				ofSetColor(255,255,255);
				browser->draw(x,y,w,h);
				ofPopMatrix();
				ofPopStyle();
			}

		void mousePressed(int x, int y, int button,int screen,int spot)
			{
		
				if(loaded) browser->mouseClick(x, y, button, true);
				cout << "berkelium MousePressed " << endl;
				clickedScreen = screen;
				clickedSpot = spot;
			}

		void setSectionListener(ofxBerkeliumSpotListener * _customListener)
			{
				section = _customListener;
			}

		void setScreenListener(ofxBerkeliumSpotListener * _customListener )
			{
				screen = _customListener;
			}

		void setLoaderListener(ofxBerkeliumSpotListener * _customListener)
			{
				loader = _customListener;
			}

		void onConsoleMessage(wstring message, wstring sourceId, int line_no)
			{
				wcout << "ofxBerkeliumSpot" << endl;
				wcout << "message : [" << message << "]" << endl;
				string str( message.begin(), message.end() );
				if ( str == "ready") 
					{	
						onReady();
					}
				else if(str =="minimize")
					{
						screen->hideFull();
					}
				else if (str == "full")
					{
						section->inScreenShowFull(clickedScreen,clickedSpot);
						
					}
				else if (str == "check")
					{
						onCheck();
					}
			}



		void onCheck()
			{
					loader->delFromMockMap(url);
					loader->check();
			}


		void snaptShotTakeOnReady()
			{
				cout << "snaptShotTakeOnReady"<<endl;
				ofPushStyle();
				ofPushMatrix();
				fbo.begin();
				ofClear(0,0,0,255);
				drawCheck(0,0,320,240);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);    
				glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, readySnapShotPixels);    
				readySnapShot.setFromPixels(readySnapShotPixels,320,240,OF_IMAGE_COLOR_ALPHA);
				fbo.end();
				
				ofPopMatrix();
				ofPopStyle();
			}

		void snaptShotTakeOnLoadUrl()
			{
				
				cout << "snaptShotTakeOnLoadUrl"<<endl;
				
				ofPushStyle();
				ofPushMatrix();
				fbo.begin();
				ofClear(0,0,0,255);
				drawCheck(0,0,320,240);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);    
				glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, loadSnapShotPixels);    
				loadSnapShot.setFromPixels(loadSnapShotPixels,320,240,OF_IMAGE_COLOR_ALPHA);
				
				fbo.end();
				ofPopMatrix();
				ofPopStyle();
			}
		
		bool snaptShotAreFine(){
			
			int counter = 0 ;
			for (int i = 0; i < 320*240*4 ; i ++ )
			{
				if (loadSnapShotPixels[i] == readySnapShotPixels[i] )
					counter++;
			}

			if (counter/(320*240*4) > .9)
				areEqual = ""+ofToString((counter/(320*240*4.0))*100.0)+"%[1]  ";
			else
				areEqual = ""+ofToString((counter/(320*240*4.0))*100.0)+"%[0]  ";
			
			return counter/(320*240*4) > .9;
		}
		

		void onLoadUrl()
			{
				//snaptShotTakeOnLoadUrl();
			}


		void onReady()
			{
					
						loaded=true;
						loader->ready();
					
						
					cout << "onReady --> IS READY!"<<endl;
	
					
				
			}

		void loadUrl(string newUrl)
			{
				url  = newUrl;
				browser->getWindow()->navigateTo(newUrl.data(),newUrl.length());
				loaded=false;
				requested = false;
				loader->loading();
				reloadCount=0;
				onLoadUrl();
			}

		void initializeMocker(string cmd)
			{	
				initializeBrowser();
				setDelayedUrl(cmd);
				myAlpha =255;

				fbo.allocate(320,240,GL_RGBA);
			
				readySnapShot.allocate(320,240,OF_IMAGE_COLOR_ALPHA);
				loadSnapShot.allocate(320,240,OF_IMAGE_COLOR_ALPHA);
				readySnapShotPixels = new unsigned char[320*240*4]; 
				loadSnapShotPixels = new unsigned char[320*240*4]; 

				verdana.loadFont("verdana.ttf", 10, true, true);
				

				cout <<  "initializeMocker begin" << endl ;
				url = cmd;
			
				string mock  =  "http://127.0.0.1:3500/mock.html";
				browser->getWindow()->navigateTo(mock.data(),mock.length());
				loaded=false;
				requested = false;
				loader->loading();
				reloadCount=0;
				onLoadUrl();
				cout <<  "initializeMocker end" << endl ;
				
			}

		void callLoader()
			{
					cout << " CALL LOADER " + delayedUrl << endl;
					//string cmd = "loadContent('"+delayedUrl+"');";
					string cmd = "loadContent('"+delayedUrl+"');";

					cout << " call loader CMD :"+ cmd << endl;
					//std::string str = " $('body').load('"+newUrl+"');";
			
				//	std::string str = "window.location = '"+newUrl+"';";|
					std::wstring str2(cmd.length(), L' '); // Make room for characters

					// Copy string to wstring.
					std::copy(cmd.begin(), cmd.end(), str2.begin());

					//wstring script(L"getData();"); 
					browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(str2)); 
					loader->loading();
					loader->addToMockMap(url);


			}

		void hry(string newUrl)
			{
					//browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(L"window.location = '"+newUrl+"';")); 
				url  = newUrl;
				//std::string str = " $('body').load('"+newUrl+"');";
			
				std::string str = "window.location = '"+newUrl+"';";
				std::wstring str2(str.length(), L' '); // Make room for characters

				// Copy string to wstring.
				std::copy(str.begin(), str.end(), str2.begin());

				//wstring script(L"getData();"); 
				browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(str2)); 
				loader->loading();

			}



		void setDelayedUrl(string newUrl)
			{
				delayedUrl = newUrl;
			}

		void loadDelayedUrl()
			{
				loadUrl(delayedUrl);
			}


		string getUrl()
			{
				return url;
			}

		void onLoad()
			{
				cout << url + " LOADED" << endl;
			}




//--------------------------------------------------------------
void onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction)
	{
		
	}



//void onProvisionalLoadError(string url, int errorCode, bool isMainFrame)
//{
//	loader->requested();
//		requested = true;
//		cout<< "[[ REQ ]]" + url<<endl;
//}

//void onStartLoading(string newURL)
void onCrashed()
	{
				cout<< "[[ ON CRASHED ]]"+url + url<<endl;

	}
void onTitleChanged(wstring title)
{
	loader->requested();
		requested = true;
		cout<< "[[ REQ ]]" + url<<endl;
}

};





