#include "Screen.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------------------------------------------------------
Screen::Screen(int number,int _width,int _height,float xOff,float yOff,int vpw,int vph){

	screenNumber=number;

	xOffset = xOff;
	yOffset = yOff;

	width = _width;
	height = _height;

	viewPortWidth  = vpw;
	viewPortHeight = vph;
		
	useMenu		= false;
	useMain		= false;
	useCorner	= false;
	useFull		= false;
	
	overlaySetDrawing = false;
	overLayAlpha = 255;

	fullStep = 255/60;
	fullAlpha = 0;

	menu    = NULL;
	main    = NULL;
	corner	= NULL;
	full	= NULL;
	motionAlpha = false;
	
}

//--------------------------------------------------------------------------------------------------------------
void Screen::overlaySetDraw(bool draw){
	overlaySetDrawing = draw;
}

//--------------------------------------------------------------------------------------------------------------
void Screen::overlaySetAlpha(int alpha)
{
	overLayAlpha = alpha ;
}

//--------------------------------------------------------------------------------------------------------------
void Screen::overlayDraw()
	{
		if(overlaySetDrawing)
		{
			ofPushStyle();
				ofSetColor(255,255,255,overLayAlpha);
				ofRect(0,0,width,height);
			ofPopStyle();
		}
	}

void Screen::setPoints(vector<Spot *> _spots)
	{

	spotsVector.clear();
	for (int i= 0 ;i<_spots.size();i++)
		{
			spotsVector.push_back(_spots[i]);
			//_spots[i]->setSCr
		}
		

	//spotsVector=_spots;
}
//--------------------------------------------------------------------------------------------------------------

void Screen::drawInfo(){
	cout << "[SCREEN PRINT INFO]: " << endl;
	cout << "[SPOTS]: "+ofToString(spotsVector.size()) << endl;
	for (int i = 0 ; i< spotsVector.size();i++)
		{
			spotsVector[i]->allMightyMethod(motionCurrentFrame);
		}
}

//--------------------------------------------------------------------------------------------------------------
void Screen::setAlphaMotion(ofGstVideoPlayer * _motion){
	motion = _motion;
	//motion->setThreadAppSink(true);
	motionAlpha = true;
	motion->setLoopState(OF_LOOP_NONE);
	motionTexture.allocate(motion->getWidth(),motion->getHeight(),GL_RGBA);
	cout <<"motion RGBA TextureAlocated w: "+ofToString(motion->getWidth())+" h: "+ofToString(motion->getHeight())<< endl;
	motionCurrentFrame = 0;
	//con fines de prueba pongo play al inicio
	playMotion();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::setH264Motion(ofGstVideoPlayer * _motion){
	motion = _motion;
	//motion->setThreadAppSink(true);
	motionAlpha=false;
	motion->setLoopState(OF_LOOP_NONE);
	motionTexture.allocate(motion->getWidth(),motion->getHeight(),GL_RGB);
	cout <<"motion RGB TextureAlocated w: "+ofToString(motion->getWidth())+" h: "+ofToString(motion->getHeight())<< endl;
	motionCurrentFrame = 0;
	//con fines de prueba pongo play al inicio
	playMotion();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::playMotion(){
	motionCurrentFrame = 0;
	motion->setPosition(motionCurrentFrame);
	motion->play();
}


void Screen::stopMotion()
	{
		motion->stop();
	}

//--------------------------------------------------------------------------------------------------------------
void Screen::setFBO(){
	fbo.allocate(width,height);
	fbo.begin();
	ofClear(255,255,255,255);
	fbo.end();

	string shaderProgram = STRINGIFY(uniform sampler2DRect tex0;
                        void main (void){
                            vec2 st = gl_TexCoord[0].st;
                            vec4 image = texture2DRect(tex0, st);    
                            gl_FragColor = vec4(image.g,image.b,image.a,image.r);\n\
                        });

	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
	shader.linkProgram(); 
}

//--------------------------------------------------------------------------------------------------------------
void Screen::addContent(ofxBerkeliumSpot * front,ofxBerkeliumSpot * back){
	contentFront.push_back(front);
	contentBack.push_back(back);
}

//--------------------------------------------------------------------------------------------------------------
void Screen::bakeFBO(){
	
	ofEnableAlphaBlending();
	fbo.begin();
    ofClear(255, 255, 255,255);
	
	
	for(int i=0;i<spotsVector.size();i++)
		{
			//spotsVector[i]->allMightyMethod(motionCurrentFrame);
			spotsVector[i]->setCurrentFrame(motionCurrentFrame);//for all backs draw
			spotsVector[i]->drawBack();//for all backs draw
		}

	
	if (motionAlpha)
		{
			shader.begin();
 			shader.setUniformTexture("tex0",motionTexture,0);
    
			glBegin(GL_QUADS);  
			glTexCoord2f(0, 0); glVertex3f(0, 0, 0);  
			glTexCoord2f(motion->getWidth(), 0); glVertex3f(width, 0, 0);  
			glTexCoord2f(motion->getWidth(), motion->getHeight()); glVertex3f(width, height, 0);  
			glTexCoord2f(0,motion->getHeight());  glVertex3f(0,height, 0);  
			glEnd();
			shader.end();
		}
	else
		motionTexture.draw(0,0,width,height);

	
	
	//cout << "BAKE FBO: [[ MOTION DONE! ]]" << endl;
	


	ofPushStyle();

	for(int i=0;i<spotsVector.size();i++)
		{
			//ofDrawBitmapString("spot: "+ofToString(i) + "poitns:" + spotsVector[i]->getInfoString(), 100,100 +  30*i); 
			//spotsVector[i]->drawPointAt(motionCurrentFrame,i);
			spotsVector[i]->drawFront();//for all backs draw
			//spotsVector[i]->drawPoints();
		}
	//cout << "BAKE FBO: [[ FRONTS DONE! ]]" << endl;
	
	
	ofPopStyle();
	if (usingMenu())
			drawMenu();

	if (usingCorner())
			drawCorner();

	if (usingMain())
			drawMain();

	if (usingFull())
			drawFull();

	if(overlaySetDrawing) overlayDraw();
    fbo.end();
	ofDisableAlphaBlending();


	//cout << "BAKE FBO: [[ END ]]" << endl;
	
}

//--------------------------------------------------------------------------------------------------------------
void Screen::update(){
	//cout << " Screen::update 00 " << endl;
	motion->update();
	if(motionAlpha)
		{
			motionTexture.loadData(motion->getPixels(),motion->getWidth(),motion->getHeight(),GL_RGBA);
		}
	else
		{
			motionTexture.loadData(motion->getPixels(),motion->getWidth(),motion->getHeight(),GL_RGB);
	
		}
	
	motionCurrentFrame = motion->getCurrentFrame();

	//cout << " Screen::update 03 " << endl;
}



//--------------------------------------------------------------------------------------------------------------
void Screen::fastFoward(){

	motion->setFrame((motion->getCurrentFrame()+30)%motion->getTotalNumFrames());

}



//--------------------------------------------------------------------------------------------------------------
int Screen::getCurrentFrame()
	{
		return motionCurrentFrame;
	}

//--------------------------------------------------------------------------------------------------------------
void Screen::rewind(){
	
	motion->setFrame((motion->getCurrentFrame()-30 > 0 )?motion->getCurrentFrame()-30 : motion->getTotalNumFrames()-30);
}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawSpots(){
	ofPushMatrix();
	//ofTranslate(xOffset,yOffset);
	 std::cout << "spot size: " + ofToString(spotsVector.size())<< std::endl;
	for(int i=0;i<spotsVector.size();i++)
		{
			 std::cout << "spot: " + ofToString(i)<< std::endl;
			 spotsVector[i]->drawPoints();
		}
	ofPopMatrix();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawCurrentSpots(){
	std::cout << "spot size: " + ofToString(spotsVector.size())<< std::endl;
	for(int i=0;i<spotsVector.size();i++)
		{
			//std::cout << "spot: " + ofToString(i)<< std::endl;
			spotsVector[i]->drawPointAt(motionCurrentFrame,i);
		}
}

//--------------------------------------------------------------------------------------------------------------
void Screen::draw(){
	ofEnableAlphaBlending();
	fbo.draw(xOffset,yOffset);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawWarped(int x,int y,int w,int h){
	ofEnableAlphaBlending();
	fbo.draw(x,y,w,h);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawDebug(){
	ofPushMatrix();
	ofTranslate(xOffset,yOffset);
	motionTexture.draw(width-160,0,160,120);
	contentBack[0]->setPos(width-160,360,160,120);
	contentFront[0]->setPos(width-160,480,160,120);
	contentBack[0]->draw();
	contentFront[0]->draw();
	drawAt(width-160,600,160,120);
	ofPushStyle();
	ofSetColor(255,0,0);
	ofDrawBitmapString("frame: "+ofToString(motionCurrentFrame), 100, 75);
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawAt(int x,int y ,int w,int h){
	ofEnableAlphaBlending();
	fbo.draw(x,y,w,h);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------------------------------------------------------
bool Screen::usingMenu()
	{
		return useMenu;
	}

//--------------------------------------------------------------------------------------------------------------
void Screen::setMenu(ofxBerkeliumHandler * _menu)
	{
		cout << "setMenu begin" << endl;
		menu = _menu;
		cout << "setMenu begin" << endl;
		useMenu = true;
		cout << "setMenu middle"  << endl;

		if (_menu == NULL)
			cout << "es null"  << endl;
		else
			cout << "no es null"  << endl;
		menu->setPos(0,0,1920,1080);
		cout << "setMenu end" << endl;
	}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawMenu()
	{
		//cout << "drawMenu begin" << endl;
		menu->draw();
		//cout << "drawMenu end" << endl;
	}



		
//--------------------------------------------------------------------------------------------------------------
bool Screen::usingFull()
	{return useFull;}

//--------------------------------------------------------------------------------------------------------------
void Screen::setFull(ofxBerkeliumSpot * _full)
	{
		useFull = true;
		full = _full;
		full->setPos(0,0,width,height);

	}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawFull()
	{
		//cout << "[[Screen::drawFull]]" << endl;
		fadeFullUpdate();
		ofPushStyle();

		//ofSetColor(255,255,255,fullAlpha);
		full->setAlpha(fullAlpha);
		full->draw();
		ofPopStyle();
				
	}


void Screen::terminateFull(){
		fullFading = false;
		fullFadeOut= false;
		useFull=false;
		fullAlpha = 0;
}

//-----------------------------------------------------------------------------------------------------
void Screen::fadeFullUpdate(){

		if(fullFading)
			{

				cout << "[Screen::fadeFullUpdate]" << endl;

			if (fullFadeOut)
				{

					fullAlpha-=fullStep;	
					if (fullAlpha< 0)
						{
							terminateFull();
						}

					cout << "fadeout"  +ofToString(fullAlpha) << endl;
				}
			else 
				{
					fullAlpha+=fullStep;
					if (fullAlpha> 255)
						{
							fullFading=false;
							fullAlpha = 255;
						}

					cout << "fadein: " +ofToString(fullAlpha)<< endl;

				}

			}
		 
		
} 
//--------------------------------------------------------------------------------------------------------------
void Screen::setFullfromSpot(int spot)
	{
		cout << "[Sect Screen::setFullfromSpot]" << endl;
	
		full = spotsVector[spot]->getFull();
		full->setPos(0,0,width,height);
		full->setScreenListener(this);
		useFull=true;
		fullFading =true;
	}




//--------------------------------------------------------------------------------------------------------------
void Screen::hideFull(){

	fullFading =true;
	fullFadeOut = true;
	fullAlpha = 255;
	

}



//--------------------------------------------------------------------------------------------------------------
void Screen::showFull(){
	fullFading = true;
	fullAlpha = 0 ;
	fullFadeOut = false;
}





//--------------------------------------------------------------------------------------------------------------
bool Screen::usingMain(){return useMain;}

//--------------------------------------------------------------------------------------------------------------
void Screen::setMain(ofxBerkeliumHandler * _main)
	{
		useMain = true;
		main = _main;
		main->setPos(0,0,width,height);

	}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawMain()
	{
		main->draw();
	}

//--------------------------------------------------------------------------------------------------------------
bool Screen:: usingCorner()
	{
		return useCorner;
	}

		//--------------------------------------------------------------------------------------------------------------
void Screen::setCorner(ofxBerkeliumHandler * _corner)
	{
		useCorner = true;
		corner = _corner;
		corner->setPos(0,0,width,height);

	}

//--------------------------------------------------------------------------------------------------------------
void Screen::drawCorner(){
		corner->draw();
	}






//--------------------------------------------------------------------------------------------------------------
void Screen::saveFrame(){}

//------------------FUCKING MOCK


void Screen::mockJsonSet(ofxJSONElement mj,string scr){

	mockJson = mj;
	selectedScreen = scr;
}

//--------------------------------------------------------------
void Screen::mockJsonUpdate(ofxBerkeliumHandler *  content,bool front){

	//drawBerks=false;
		 motionID = 1;
		// selectedScreen= "S1";
		 spots = 0;
		spots = mockJson[motionID]["spots"].size();
		//cout << "motion: " << jsonMock[motion]["name"].asString() << endl;
		//cout << "spots: " <<ofToString(spots) << endl;

		//variables temporales 
		int x = 0;
		int y = 0;
		double scale = 0.0;
		int frame = 0;
			string spotName = "";
		for(int spot=0; spot<spots; spot++){
			spotName = mockJson[motionID]["spots"][spot]["name"].asString();
			//cout << " SPOT NAME: "+spotName<< endl;
			if(spotName == "P1"||spotName == "P2"||spotName == "P3"||spotName == "P4"||spotName == "P5")
			{
				
				int points = 0;
				points = mockJson[motionID]["spots"][spot]["points"].size();

				int point=0;
				bool found =false;
				x1=0;
				x2=0;
				dx=0;
				sx=0;
				cx=0;
		
				y1=0;
				y2=0;
				dy=0;
				sy=0;
				cy=0;
		
				f1=0;
				f2=0;
				df=0;
				dcf=0;
	
				s1=0;
				s2=0;
				ds=0;
				ss=0;
				cs=0;


				while( point<points && !found)
				{
					string screen = "";
					screen = mockJson[motionID]["spots"][spot]["points"][point]["screen"].asString();
		
					if (selectedScreen == screen){		
						 frame = mockJson[motionID]["spots"][spot]["points"][point]["frame"].asInt();
						 found = (frame>= motionCurrentFrame);
					}

					if(!found)
					point++;		
				}
		
				///////////////////
				if(found)
					{

						scale = mockJson[motionID]["spots"][spot]["points"][point]["scale"].asDouble();

						if (point >= 0  )
							{
								
								x1 = mockJson[motionID]["spots"][spot]["points"][point-1]["x"].asDouble();
								x2 = mockJson[motionID]["spots"][spot]["points"][point]["x"].asDouble();
								dx = x2-x1;
		
								y1 = mockJson[motionID]["spots"][spot]["points"][point-1]["y"].asDouble();
								y2 = mockJson[motionID]["spots"][spot]["points"][point]["y"].asDouble();
								dy = y2 - y1;
							
								f1 = mockJson[motionID]["spots"][spot]["points"][point-1]["frame"].asInt();
								f2 = mockJson[motionID]["spots"][spot]["points"][point]["frame"].asInt();
								df = f2 - f1;
								
								s1= mockJson[motionID]["spots"][spot]["points"][point-1]["scale"].asDouble();
								s2= mockJson[motionID]["spots"][spot]["points"][point]["scale"].asDouble();
								ds=abs(s2-s1);
								
								ss=ds/df;
								sx = dx/df;
								sy = dy/df;
								dcf = motionCurrentFrame -f1 ;
								cx =  x1 + ( dcf*sx );
								cy =  y1 + ( dcf*sy );
								cs= s1 + (dcf*ss);

								//cx = ofMap(currentFrame,f1,f2,x1,x2,false);

								//cy = ofMap(currentFrame,f1,f2,y1,y2,false);
							

								//cs = ofMap(currentFrame,f1,f2, s1,s2,false);



								if (cs >1 )
									{
										cs *=11;
										
									//if(!front)
										//browser->draw(cx,cy,cs,cs);
										//bImage.draw(cx,cy,cs,cs);
									    //drawBerks=true;
										//berkelium.setPos(cx,cy,cs,cs*1.3);
										//berkelium2.setPos(cx-cs/2,cy,cs,cs);
										//berkelium.draw();
										//ofPushStyle();
										//ofSetColor((int)((255/spots)*spot),0,0,255);
										//ofNoFill();
										//ofSetLineWidth(4);
										//ofRect(cx,cy,cs,cs);
										//ofSetColor(255,255,255);
										

										//cout << "spots[ x:"+ofToString(cx)<< endl;
									

										if (front)
											{

												
												content->setPos(cx-cs,cy,cs,cs);
												content->draw();
											}
										else
											{
												content->setPos(cx,cy,cs,cs);
												content->draw();
												//cout <<"index: "+ofToString(spot%content.size()) << endl;
												//cout <<"posx: "+ofToString(cx) << endl;


												
											}
	

										//cout << "aca hay un spot " << endl;
										//berkelium.setPos(cx,cy,cs,cs);
										
										//ofSetColor((int)((255/spots)*spot),0,0,128);
										//ofFill();

										//ofRect(cx,cy,cs,cs);

										//ofDrawBitmapString(spotName+" scr: "+selectedScreen+ " x: "+ofToString(cx)+" y: "+ofToString(cy)+" s: "+ofToString(cs), cx, cy);
										//ofSetColor(0,0,255);
										//ofPopStyle();
										
									}

							}

					
				
					}

			}
			///////////////////

		}

}



//mouse interface
//--------------------------------------------------------------
void Screen::mouseMoved(int x, int y ){
	x-=xOffset;
	x= ofMap(x,0,viewPortWidth,0,width);
	y= ofMap(y,0,viewPortHeight,0,height);



	if (usingFull())
		{
			cout << "using full" << endl;
			full->mouseMoved(x,y);
		}
	else
		{
	
			for (int i =0 ;i<spotsVector.size();i++)
				{
					if(spotsVector[i]->mouseIsOver(x,y))
						spotsVector[i]->mouseMoved(x,y);
				}

			if(usingMenu()) menu->mouseMoved(x,y);
		}
	
}

//--------------------------------------------------------------
void Screen:: mouseDragged(int x, int y, int button){
	x-=xOffset;
	
	x= ofMap(x,0,viewPortWidth,0,width);
	y= ofMap(y,0,viewPortHeight,0,height);



	if (usingFull())
		{
			cout << "using full" << endl;
			full->mouseDragged(x,y);
		}
	else
		{
	
	for (int i =0 ;i<spotsVector.size();i++)
		{
			if(spotsVector[i]->mouseIsOver(x,y))
				spotsVector[i]->mouseDragged(x,y,button);
		}
	if(usingMenu()) menu->mouseDragged(x,y);
	}
}

//--------------------------------------------------------------
void Screen::mousePressed(int x, int y, int button){
	
	cout <<"[Screen::mousePressed] begin " << endl;
	cout <<"					in  [x: "+ofToString(x)+"   y: "+ofToString(y)+"] begin " << endl;
	
	x-=xOffset;
	
	x= ofMap(x,0,viewPortWidth,0,width);
	y= ofMap(y,0,viewPortHeight,0,height);
	cout <<"					mod [x: "+ofToString(x)+"   y: "+ofToString(y)+"] begin " << endl;
	
	if (usingFull())
		{
			cout << "using full" << endl;
			full->mousePressed(x,y,button,0,0);
		}
	else
		{
			for (int i =0 ;i<spotsVector.size();i++)
				{
					if(spotsVector[i]->mouseIsOver(x,y))
						spotsVector[i]->mousePressed(x,y,button,screenNumber,i);
				}
			if(usingMenu()) menu->mousePressed(x,y,button);
	
		}

	cout <<"[Screen::mousePressed] end " << endl;
	
}

//--------------------------------------------------------------
void Screen::mouseReleased(int x, int y, int button)
	{
		x-=xOffset;
	
		x= ofMap(x,0,viewPortWidth,0,width);
		y= ofMap(y,0,viewPortHeight,0,height);

		if (usingFull())
		{
			cout << "using full" << endl;
			full->mouseReleased(x,y,button);
		}
		else
		{
	
			for (int i =0 ;i<spotsVector.size();i++)
				{
					if(spotsVector[i]->mouseIsOver(x,y))
						spotsVector[i]->mouseReleased(x,y,button);
				}

			if(usingMenu()) menu->mouseReleased(x,y,button);
	
		}
	}

//--------------------------------------------------------------
bool Screen::mouseIsOver(int x,int y)
	{
		//if ( (x >= xOffset  && x <= (xOffset+viewPortWidth))&&( y >= yOffset && y<=( yOffset + viewPortHeight)))
		//cout << "screen: ["+ofToString(screenNumber)+"] mouse is over " << endl;


		return ( (x >= xOffset  && x <= (xOffset+viewPortWidth))&&( y >= yOffset && y<=( yOffset + viewPortHeight)));
	}