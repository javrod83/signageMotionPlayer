#include "Section.h"

#define USE_TRANSPARENCY true

//-----------------------------------------------------------------------------------------------------
void Section::init(int w,int h,int s,int vw,int vh){
	
	debug = true;

	if(debug) cout << "[[Section::init]] begin" << endl;
	//set dimensions
	screens = s;
	width = w;
	height = h;
	viewportWidth = vw;
	viewportHeight = vh ;

	//transition
	fadeOverlayAlpha = 255 ;
	fadeStep  = 10;
	fadeInFlag = false;
	fadeOutFlag = false;
	fadeOveylayOn = false;

	//get ready before play
	loading = false;
	
	initScreens();
	initMotions();
	 
	drawingCtrlView = false;
	ctrlScreen=0;
	logosSetup();
	contentManager.setSectionListener(this);

	//pause feature 
	pausedFrame=0;
	pausedToggle=false;


	if(debug) cout << "[[Section::init]] end" << endl;
}

//------------------------------------------------------------------------------------------------------
void Section::toggleCtrlView()
{
	drawingCtrlView = ! drawingCtrlView ;
}

//------------------------------------------------------------------------------------------------------
void Section::setDebug(bool _debug)
{
	debug = _debug;	
}

//-----------------------------------------------------------------------------------------------------
void Section::initScreens(){

	if(debug) cout << " [[Section::initScreens]] begin"<< endl ; 

	for (int i=0;i<screens;i++)
		{
			Screen * aScreen = new Screen(i,width,height,i*viewportWidth,0,viewportWidth,viewportHeight);
			aScreen->setFBO();
			screensVector.push_back(aScreen);
			if(debug) cout << "                          screen:"<< ofToString(i) << " initialized"<<endl ; 
		}
	
	menu = new ofxBerkeliumMenu();
	menu->initialize("http://127.0.0.1:3500/menu.html");
	menu->setPos(0,0,viewportWidth,viewportHeight);
	menu->setCustomListener(this);
	menu->setAlpha(255.0f);
	screensVector[2]->setMenu(menu);
	

	main = new ofxBerkeliumHandler();
	main->initialize("http://127.0.0.1:3500/section/1/main");
	main->setPos(0,0,viewportWidth,viewportHeight);
	main->setAlpha(255.0f);
	screensVector[1]->setMain(main);

	corner = new ofxBerkeliumMenu();
	corner->initialize("http://127.0.0.1:3500/section/1/corner");
	corner->setPos(0,0,viewportWidth,viewportHeight);
	corner->setAlpha(255.0f);
	screensVector[0]->setCorner(corner);

	
	if(debug) cout << " [[Section::initScreens]] end "<< endl ; 
}

//-----------------------------------------------------------------------------------------------------
void Section::initMotions(){
	if(debug) cout << "[[Section::initMotions]] begin"<< endl ; 
	for (int i=0;i<screens;i++)
		{
			ofGstVideoPlayer * aMotion = new ofGstVideoPlayer();
			aMotion->setPixelFormat(OF_PIXELS_BGRA);
			motionsVector.push_back(aMotion);
			cout << "                         screen:"<< ofToString(i) << " initialized"<<endl ; 
		}
	if(debug) cout << "[[Section::initMotions]] end"<< endl ; 
}

//-----------------------------------------------------------------------------------------------------
void Section::loadContent(ofxJSONElement _json,int section){
		cout << "[[Section::loadContent]]xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		loading=true;
		nextSection = section;
		contentManager.releasewebkits();
		contentManager.countSection(section);
		for (int i=0;i<screens;i++)
		{
			//if(debug) 
			cout << "          [ XXXXXX ]--------------------------------------------" <<endl;
			cout << "           screen: " + ofToString(i) << " LOAD begin " <<endl;
			cout << "           alpha: " + ofToString(_json["motion"]["screens"][i]["alpha"].asInt()) <<endl;
			



			string motionName  = _json["motion"]["name"].asString() +"_"+ ofToString( _json["motion"]["screens"][i]["number"].asInt());
			if (_json["motion"]["screens"][i]["alpha"].asInt() == 1 )	
				{
					motionName += ".mov";
					motionsVector[i]->setPixelFormat(OF_PIXELS_BGRA);

					cout << "LOAD MOTION NAME: [[[["+motionName << "]]]] " <<endl;
					motionsVector[i]->loadMovie(motionName);
					screensVector[i]->setAlphaMotion(motionsVector[i]);
				}
			else
				{
					motionName += ".mp4";
					motionsVector[i]->setPixelFormat(OF_PIXELS_RGB);
				    cout << "LOAD MOTION NAME: [[[["+motionName << "]]]] " <<endl;
					motionsVector[i]->loadMovie(motionName);
					screensVector[i]->setH264Motion(motionsVector[i]);
				}
			//screensVector[i]->stopMotion();
			screensVector[i]->terminateFull();
			screensVector[i]->setPoints(contentManager.getSpotsForMotion(_json,section,i));
			cout << "			screen: " + ofToString(i) +" LOAD done!" << endl;
			cout << "          [ XXXXXX ]----------------------------------------------" <<endl;
		}

		main->loadUrl("http://127.0.0.1:3500/section/"+ofToString(section)+"/main");
		corner->loadUrl("http://127.0.0.1:3500/section/"+ofToString(section)+"/corner");


		//a cada spot setearle la sesion

		map <string , ofxBerkeliumSpot *> content = contentManager.getWebkits();

		for(auto iterator = content.begin(); iterator != content.end(); iterator++)
		{
		
			iterator->second->setSectionListener(this);
		}
		

}

//-----------------------------------------------------------------------------------------------------
void Section::play()
	{
		for(int i =0;i< screens;i++)
			{
				screensVector[i]->playMotion();
				motionsVector[i]->setFrame(30*3);
			}
	}

//-----------------------------------------------------------------------------------------------------
bool Section::isPaused()
	{
		return pausedToggle;
	}

//-----------------------------------------------------------------------------------------------------
void Section::pause()
	{
			if (pausedToggle)
				{
					for(int i =0;i< screens;i++)
							{motionsVector[i]->setFrame(pausedFrame);}
					pausedToggle=!pausedToggle;
				}

			else
				{
					pausedFrame =  screensVector[0]->getCurrentFrame();
					pausedToggle=!pausedToggle;
				}

	}

//-----------------------------------------------------------------------------------------------------
void Section::fadeDraw()
	{
		if (fadeIsFading()|| fadeOveylayOn)
			{
				for(int i =0;i< screens;i++)
					{
						screensVector[i]->overlaySetAlpha(fadeOverlayAlpha);
					}

				ofPushStyle();
				ofSetColor(255,0,0);
				//ofDrawBitmapString("  fade Alpha: "+ofToString(fadeOverlayAlpha), 100, 75);
				ofPopStyle();
			}
	}

//-----------------------------------------------------------------------------------------------------
void Section::update(){
	if(debug) cout << "[[Section::update]]   begin" << endl ; 

	if (!loading)
		{
			if(debug) cout << "                    "+ofToString(screens)+"   screens to update" << endl ; 
			for(int i=0;i<screens;i++)
				{
					if(debug) cout << " screen: " << ofToString(i) << " update"  ; 
					
					if(!pausedToggle)
						{
							screensVector[i]->update();
							screensVector[i]->bakeFBO();
						}
					if(debug) cout <<" done" << endl ; 
				}

			logosUpdate();
		}
	else
		{
			contentManager.checkDelayed();
			//cout << "loading " << endl;
			if (contentManager.loaded())
				{
					loading= false;
					play();
					fadeOut();
					logosStartFadeOut();
					//contentManager.checkReadys();
					//contentManager.loadMocks();
					
					cout << "start logos fadeout" << endl;
				}
			
				
		}
		
	fadeUpdate();
	if(debug) cout << "[[Section::update]]   end" << endl ; 


	if (motionsVector[0]->getIsMovieDone())
		{
			cout << "MOVIE DONE" ;
			play();
		}
}

//-----------------------------------------------------------------------------------------------------
void Section::fastFoward()
	{
	if (!loading)
		{
			for(int i=0;i<screens;i++)
				{
					screensVector[i]->fastFoward();
				}
		}
	}

//-----------------------------------------------------------------------------------------------------
void Section::rewind()
	{
		if (!loading)
			{
				for(int i=0;i<screens;i++)
					{
						screensVector[i]->rewind();
					}
			}
	}

//-----------------------------------------------------------------------------------------------------
void Section::draw(){
	if(debug) cout << "[[Section::draw]]   begin" << endl ; 
	ofEnableAlphaBlending();
	
	ofBackground(255,255,255);
	for(int i=0;i<screens;i++)
		{
				
			if(debug) cout << "                  pre draw" << endl ; 
			screensVector[i]->drawAt(i*viewportWidth,0,viewportWidth,viewportHeight);
			if(debug) cout << "                  pos draw" << endl ; 
		}
	
	fadeDraw();
	ofDisableAlphaBlending();
	if(debug) cout << "[[Section::draw]]   end" << endl ; 
}

//-----------------------------------------------------------------------------------------------------
void Section::drawScreen(int scr,float x,float y ,float w,float h){

	ofPushStyle();
	ofEnableAlphaBlending();
	ofBackground(153,0,255);
	screensVector[scr]->drawAt(x,y,w,h);
	ofSetColor(0);
	ofRect(x,y,250,20);
	ofDisableAlphaBlending();
	ofSetColor(255);
	ofDrawBitmapString(" scr: "+ofToString(scr) + " fps: "+ofToString(ofGetFrameRate()), x+20, y+18);
		
	
	
	ofPopStyle();
}

//-----------------------------------------------------------------------------------------------------
void Section::mouseMoved(int x, int y ){
	//cout << " Section::mouseMoved x: " << ofToString(ofMap(x-(1280*2),0,1280,0,1920)) << endl;
	//cout << "                       y: " << ofToString(ofMap(y,0,1024,0,1080)) << endl;

	for(int i=0;i<screens;i++)
		{
			if (screensVector[i]->mouseIsOver(x,y))
			  screensVector[i]->mouseMoved(x,y);	
		}

	//menu->mouseMoved(ofMap(x-(1280*3),0,1280,0,1920),ofMap(y,0,1024,0,1080));
}

//-----------------------------------------------------------------------------------------------------
void Section::mouseDragged(int x, int y, int button){
	//cout << " Section::mouseDragged x: " << ofToString(ofMap(x-(1280*2),0,1280,0,1920)) << endl;
	//cout << "                       y: " << ofToString(ofMap(y,0,1024,0,1080)) << endl;


	for(int i=0;i<screens;i++)
		{
			if (screensVector[i]->mouseIsOver(x,y))
			  screensVector[i]->mouseDragged(x,y,button);	
		}

	//menu->mouseDragged(ofMap(x-(1280*3),0,1280,0,1920),ofMap(y,0,1024,0,1080));
}

//-----------------------------------------------------------------------------------------------------
void Section::mousePressed(int x, int y, int button){
	//cout << " Section::mousePressed x: " << ofToString(ofMap(x-(1280*2),0,1280,0,1920)) << endl;
	//cout << "                       y: " << ofToString(ofMap(y,0,1024,0,1080)) << endl;
	

	menu->tick();
	for(int i=0;i<screens;i++)
		{
			if (screensVector[i]->mouseIsOver(x,y))
			  screensVector[i]->mousePressed(x,y,button);	
		}
	//menu->mousePressed(ofMap(x-(1280*3),0,1280,0,1920),ofMap(y,0,1024,0,1080),button);
}

//-----------------------------------------------------------------------------------------------------
void Section::mouseReleased(int x, int y, int button){
	//cout << " Section::mouseReleased x: " << ofToString(ofMap(x-(1280*2),0,1280,0,1920)) << endl;
	//cout << "                        y: " << ofToString(ofMap(y,0,1024,0,1080)) << endl;
	
	for(int i=0;i<screens;i++)
		{
			if (screensVector[i]->mouseIsOver(x,y))
			  screensVector[i]->mouseReleased(x,y,button);	
		}

	//menu->mouseReleased(ofMap(x-(1280*3),0,1280,0,1920),ofMap(y,0,1024,0,1080),button);
}

//-----------------------------------------------------------------
void Section::menuCommand(string cmd){
	cout << " Section::menuCommand [" +cmd+"]" << endl;
}

//-----------------------------------------------------------------
void Section::changeTo(int section)
		{
			cout << "[Section::changeTo] begin" + ofToString(section)<< endl;
			nextSection = section;
			fadeIn();
			//get json
			//fade in overlay
			//loadContent
			//fade out overlay
			cout << "[Section::changeTo] end" << endl;
		}

//-----------------------------------------------------------------------------------------------------
void Section::fadeOut(){

	fadeOutFlag=true;
	fadeOverlayAlpha = 255;
	for(int i =0;i< screens;i++)
		{
			screensVector[i]->overlaySetDraw(true);
		}
}

//-----------------------------------------------------------------------------------------------------
void Section::fadeIn(){

	fadeInFlag=true;
	fadeOverlayAlpha = 0;
	for(int i =0;i< screens;i++)
		{
			screensVector[i]->overlaySetDraw(true);
		}
}

//-----------------------------------------------------------------------------------------------------
void Section::fadeUpdate(){
		if(fadeInFlag)
			{
				fadeOverlayAlpha+=fadeStep;

				if (fadeOverlayAlpha> 255)
					{
						fadeInFlag = false;
						fadeOveylayOn= true;
						fadeOverlayAlpha = 255;						
						try {
									loadSection(nextSection);
						} catch( std::exception& e) {
							std::cout << " IN BERKELIUM UPDATE EVETN" << e.what() << "'\n";
						}
					}
			}
		else if (fadeOutFlag)
			{
				fadeOverlayAlpha-=fadeStep;
						
				if (fadeOverlayAlpha< 0)
					{
						fadeOutFlag = false;
						fadeOveylayOn= false;
						for(int i =0;i< screens;i++)
							{
								screensVector[i]->overlaySetDraw(false);
							}
						fadeOverlayAlpha = 0;
					}
			}
}

//-----------------------------------------------------------------------------------------------------
bool Section::fadeIsFading()
	{
		return ( fadeInFlag || fadeOutFlag ) ;
	}

//-----------------------------------------------------------------------------------------------------
void Section::loadSection(int section)
	{
		json.open("http://127.0.0.1:3500/section/"+ofToString(section)+"/json");

		loadContent(json,section); 
	}

//-----------------------------------------------------------------------------------------------------
void Section::inScreenShowFull(int screen,int spot)
{
	cout << "[Section:inScreenShowFull]" << endl;
	screensVector[screen]->setFullfromSpot(spot);
}

//-----------------------------------------------------------------------------------------------------
void Section:: inScreenShowRandomFull()
	{
		//screensVector[1]->setFullfromSpot(screensVector[1]->getRandomSpotIndex());
	}

//-----------------------------------------------------------------------------------------------------
void Section::logosUpdate()
{
	 if (logosTimerRunning)
		{
			if (logosStartTime + logosTimerCount  > ofGetElapsedTimeMillis())
				{
					logosTimerRunning = false;
					logosFading = true;
					logosCurrentAlpha = 255;
				}
		}
		
	if (logosFading)
		{
			logosCurrentAlpha -= logosAlphaStep ;
			if (logosCurrentAlpha < 0 )
				{
					logosCurrentAlpha=0.0f;
					logosFading=false;
				}
			

			// set Alpha for corner & main
			main->setAlpha(logosCurrentAlpha);
			corner->setAlpha(logosCurrentAlpha);

		}
		// 

	//cout << "[Section:inScreenShowFull]" << endl;
	//screensVector[screen]->setFullfromSpot(spot);
}

//-----------------------------------------------------------------------------------------------------
void Section::setCtrlScreen(int scr)
	{
		ctrlScreen = scr ; 
	}

//-----------------------------------------------------------------------------------------------------
void Section::logosStartFadeOut()
	{
		logosTimerRunning =true;
		logosStartTime = ofGetElapsedTimeMillis();
		logosCurrentAlpha = 255.0f;
	}

//-----------------------------------------------------------------------------------------------------
void Section::logosSetup()
	{
		logosTimerCount = 5000.0f;
		logosStartTime = .0f;
		logosTimerRunning  =false;
		logosFading =false ;
		logosAlphaStep = 255.0f / 180.0f;
		logosCurrentAlpha = 255;
	}

//-----------------------------------------------------------------------------------------------------
bool Section::isLoading()
	{
		return loading ||fadeInFlag || fadeOutFlag;
	}

//-----------------------------------------------------------------------------------------------------
void Section::debugUp()
	{
		contentManager.nextDebug();
	}

//-----------------------------------------------------------------------------------------------------
void Section::debugDown()
	{
		contentManager.prevDebug();
	}

//-------------------------------------------------------------------------------------------------------
void Section::debugReload()
	{
		contentManager.reloadDebug();
		cout << "section debugReload" << endl ;  
	}

//-------------------------------------------------------------------------------------------------------
void Section::debugDraw()
{
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(0,300);
	contentManager.drawDebug(0,0,1280);
	ofSetColor(255,0,115);
	ofTranslate(0,60);
	ofDrawBitmapString("section: "+ofToString(nextSection),1280*.66,((1280*.33)/1.77777778) + 25);
	ofDrawBitmapString("Frame: "+ofToString(screensVector[0]->getCurrentFrame()) +((isPaused())?" paused":" playing"),1280*.66,((1280*.33)/1.77777778) + (25*2));
	ofPopStyle();
	ofPopMatrix();
}

//-------------------------------------------------------------------------------------------------------
void Section::loadCurrentDelayed()
{
	contentManager.loadMocks();
	//contentManager.loadDelayed();
}
	
	
