#include "Spot.h"

#define SCALE_FACTOR 12


void Spot:: loadFrontPoints( ofxJSONElement json,int _number){
	
	  //  cleanPoints();
		number = _number;

		frameCurrent=0;
		frameFirstFront=0;
		frameLastFront=0;
		
		int points = json.size();

		
		
		
		if(points>= 2) 
			{
				
				int pointBottomIndex=0;
				frameFirstFront= json[pointBottomIndex]["frame"].asInt();
				frameCurrent=frameFirstFront;

				SpotPoint * pointBottom = new SpotPoint(
					json[pointBottomIndex]["x"].asInt(),
					json[pointBottomIndex]["y"].asInt(),
					json[pointBottomIndex]["scale"].asDouble()*SCALE_FACTOR,
					json[pointBottomIndex]["alpha"].asInt()
				);

				//
				 frontFollower = new SpotPoint(
					json[pointBottomIndex]["x"].asInt(),
					json[pointBottomIndex]["y"].asInt(),
					json[pointBottomIndex]["scale"].asDouble()*SCALE_FACTOR,
					json[pointBottomIndex]["alpha"].asInt()
				);

				SpotPoint * pointTop;
				addFrontPoint(frameCurrent,pointBottom);
			
				frameCurrent++;

				for(int pointTopIndex=pointBottomIndex+1; pointTopIndex<points; pointTopIndex++)
					{
						pointTop = new SpotPoint(
									json[pointTopIndex]["x"].asInt(),
									json[pointTopIndex]["y"].asInt(),
									json[pointTopIndex]["scale"].asDouble()*SCALE_FACTOR,
									json[pointTopIndex]["alpha"].asInt()
						);
									
						while(frameCurrent<=json[pointTopIndex]["frame"].asInt())
							{

								addFrontPoint(frameCurrent,pointInterPolateFrom(
									pointBottom,
									pointTop,
									json[pointBottomIndex]["frame"].asInt(),
									frameCurrent,
									json[pointTopIndex]["frame"].asInt()
								));
				
								

								//cout << "added frame: "+ofToString(frameCurrent)+"  points: "+ofToString(pointsMap.size())<< endl;
								frameCurrent++;
							}

						addFrontPoint(frameCurrent,pointTop);

						pointBottom = pointTop;
						pointBottomIndex = pointTopIndex;
					}
				frameLastFront=frameCurrent;


				smoothOutFronts(frameFirstFront,frameLastFront);
				cout <<"         " << ofToString(frameCurrent)<< " frames "<< endl;
			}
		else
			{
				cout << "Spot: dont have any point"<< endl;
			}
		cout << "[frontPointsMap ONCE LOADED ]: "+ofToString(frontPointsMap.size())<< endl;
		cout << "[FRAMES]: "+ofToString(frameLastFront - frameFirstFront )<< endl;
		
		verdana.loadFont("verdana.ttf", 40, true, true);
		verdana.setLineHeight(34.0f);
		verdana.setLetterSpacing(1.035);
}

void Spot:: loadBackPoints( ofxJSONElement json,int _number){
	  //  cleanPoints();
		number = _number;
		frameCurrent=0;
		frameFirstBack=0;
		frameLastBack=0;
		
		int points = json.size();

		
		
		if(points>= 2) 
			{
				
				int pointBottomIndex=0;
				frameFirstBack= json[pointBottomIndex]["frame"].asInt();
				frameCurrent=frameFirstBack;

				SpotPoint * pointBottom = new SpotPoint(
					json[pointBottomIndex]["x"].asInt(),
					json[pointBottomIndex]["y"].asInt(),
					json[pointBottomIndex]["scale"].asDouble()*SCALE_FACTOR,
					json[pointBottomIndex]["alpha"].asInt()
				);

				 backFollower = new SpotPoint(
					json[pointBottomIndex]["x"].asInt(),
					json[pointBottomIndex]["y"].asInt(),
					json[pointBottomIndex]["scale"].asDouble()*SCALE_FACTOR,
					json[pointBottomIndex]["alpha"].asInt()
				);

				SpotPoint * pointTop;
				addBackPoint(frameCurrent,pointBottom);
				//pointsMap[frameCurrent]=pointBottom;
				frameCurrent++;

				for(int pointTopIndex=pointBottomIndex+1; pointTopIndex<points; pointTopIndex++)
					{
						pointTop = new SpotPoint(
									json[pointTopIndex]["x"].asInt(),
									json[pointTopIndex]["y"].asInt(),
									json[pointTopIndex]["scale"].asDouble()*SCALE_FACTOR,
									json[pointTopIndex]["alpha"].asInt()
						);
									
						while(frameCurrent<=json[pointTopIndex]["frame"].asInt())
							{

								addBackPoint(frameCurrent,pointInterPolateFrom(
									pointBottom,
									pointTop,
									json[pointBottomIndex]["frame"].asInt(),
									frameCurrent,
									json[pointTopIndex]["frame"].asInt()
								));
				
								

								//cout << "added frame: "+ofToString(frameCurrent)+"  points: "+ofToString(pointsMap.size())<< endl;
								frameCurrent++;
							}

						addBackPoint(frameCurrent,pointTop);

						pointBottom = pointTop;
						pointBottomIndex = pointTopIndex;
					}
				frameLastBack=frameCurrent;
				smoothOutBacks(frameFirstBack,frameLastBack);
				cout <<"         " << ofToString(frameCurrent)<< " frames "<< endl;
			}
		else
			{
				cout << "Spot: any point"<< endl;
			}
		cout << "[MAP SIZE ONCE LOADED ]: "+ofToString(backPointsMap.size())<< endl;
		cout << "[FRAMES]: "+ofToString(frameLastBack - frameFirstBack )<< endl;

}

void Spot:: addFrontPoint(int frame,SpotPoint * aPoint)
{
	backPointsMap[frame]= aPoint;
}
//-------------------------------------------------------------------------------------------------------------
void Spot::smoothOutFronts(int first,int last)
	{

		if (frontPointsMap.find(first) != frontPointsMap.end())
			frontFollower->setFromPoint(frontPointsMap[first]);
	
		for (int currentFrame = first; currentFrame <= last;currentFrame ++ )
			{
				if (frontPointsMap.find(currentFrame) != frontPointsMap.end())
					{
						float aX=0,aY=0,aS=0,count=0,resolution =3;
						for(int selectedFrame = currentFrame -(resolution-1) ; selectedFrame <= currentFrame + resolution ; selectedFrame ++ )
							{
								if (frontPointsMap.find(selectedFrame) != frontPointsMap.end())
										{
											aX += frontPointsMap[selectedFrame]->x;
											aY += frontPointsMap[selectedFrame]->y;
											aS += frontPointsMap[selectedFrame]->scale;
											count++;
										}
							}
						
						
						frontPointsMap[currentFrame]->x = aX/count;
						frontPointsMap[currentFrame]->y = aY/count;
						frontPointsMap[currentFrame]->scale = aS/count;
						
						


					}
			}
	}

//-------------------------------------------------------------------------------------------------------------
void Spot:: smoothOutBacks(int first,int last)
	{
	
		for (int currentFrame = first; currentFrame <= last;currentFrame ++ )
			{
				if (backPointsMap.find(currentFrame) != backPointsMap.end())
					{
						float aX=0,aY=0,count=0,resolution =2;
						for(int selectedFrame = currentFrame -(resolution-1) ; selectedFrame <= currentFrame + resolution ; selectedFrame ++ )
							{
								if (backPointsMap.find(selectedFrame) != backPointsMap.end())
										{
											aX += backPointsMap[selectedFrame]->x;
											aY += backPointsMap[selectedFrame]->y;
											count++;
										}
							}
						backPointsMap[currentFrame]->x = aX/count;
						backPointsMap[currentFrame]->y = aY/count;
					}
			}
	}


		
void Spot:: addBackPoint(int frame,SpotPoint * aPoint)
{

	frontPointsMap[frame]= aPoint;
}


//--------------------------------------------------------------------------------------------------------------
SpotPoint* Spot::getBackPointForFrame(int frame){
	if (frame >=frameFirstBack && frame <= frameLastBack )
		return frontPointsMap[frame];
	else 
		return NULL;
}


SpotPoint* Spot::getFrontPointForFrame(int frame){
	if (frame >=frameFirstFront && frame <= frameLastFront )
		return backPointsMap[frame];
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------------------------------
void Spot::cleanPoints(){
		while(!frontPointsMap.empty())
		{
			delete frontPointsMap.begin()->second;
			frontPointsMap.erase( frontPointsMap.begin()->first);
	    }
	
		while(!backPointsMap.empty())
		{
			delete backPointsMap.begin()->second;
			backPointsMap.erase( backPointsMap.begin()->first);
	    }
}

//--------------------------------------------------------------------------------------------------------------
bool  Spot::appearsBack(int frame)
	{
		//cout <<" Spot::appears " + ofToString(frameFirst)+" <= "+ ofToString(frame)+" <= "+ ofToString(frameLast)+" "  << endl;
		return(backPointsMap.find(frame) !=backPointsMap.end() );
	}

bool  Spot::appearsFront(int frame)
	{
		return(frontPointsMap.find(frame) !=frontPointsMap.end() );
		//cout <<" Spot::appears " + ofToString(frameFirst)+" <= "+ ofToString(frame)+" <= "+ ofToString(frameLast)+" "  << endl;
		//return (frame >=frameFirst && frame <= frameLast ) ;
	}

//--------------------------------------------------------------------------------------------------------------
void Spot::drawPointAt(int frame,int id){

	if (frame >=frameFirstBack && frame <= frameLastBack )
		{
			SpotPoint* p = backPointsMap[frame];
			ofPushStyle();
			ofSetColor((int)ofRandom(254));
			ofNoFill();
			ofSetLineWidth(2);
			ofRect(p->x,p->y,p->scale*1.334,p->scale);
			backWebkit->setPos(p->x,p->y,p->scale*1.334,p->scale);
			ofSetColor(254);
			backWebkit->draw();
			ofDrawBitmapString(ofToString(id)+" x: "+ofToString(p->x)+" y: "+ofToString(p->y)+" s: "+ofToString(p->scale), p->x, p->y - 15);									
			ofPopStyle();
		}
	//cout << "drawPointAt [[ END ! ]]" << endl;
}

//--------------------------------------------------------------------------------------------------------------
SpotPoint*  Spot::pointInterPolateFrom(SpotPoint* pointPrev,SpotPoint* pointCurr,int framePrev,int _frameCurrent,int frameTop){
	
	return new SpotPoint(
		ofMap(_frameCurrent,framePrev,frameTop,pointPrev->x,pointCurr->x,false),
		ofMap(_frameCurrent,framePrev,frameTop,pointPrev->y,pointCurr->y,false),
		ofMap(_frameCurrent,framePrev,frameTop,pointPrev->scale,pointCurr->scale,false),
		ofMap(_frameCurrent,framePrev,frameTop,pointPrev->alpha,pointCurr->alpha,false)
	);				
}

//--------------------------------------------------------------------------------------------------------------
bool Spot::empty(){

	return backPointsMap.empty();
}

//--------------------------------------------------------------------------------------------------------------
void Spot::drawPoints(){
    for(auto  inner_iter=backPointsMap.begin(); inner_iter!=backPointsMap.end(); ++inner_iter)
		{
		  
			
		ofNoFill();
		ofSetLineWidth(4);

		if (inner_iter->first /100 == 0 )
		ofRect(inner_iter->second->x,inner_iter->second->y,inner_iter->second->scale,inner_iter->second->scale);
	
    }
}


//--------------------------------------------------------------------------------------------------------------
void Spot::setCurrentFrame(int theCurrentFrame){
	frameCurrent = theCurrentFrame;
	
}

//--------------------------------------------------------------------------------------------------------------
void Spot::setBack(ofxBerkeliumSpot * newBack){
	backWebkit = newBack;
	backAlpha = 0;
}

//--------------------------------------------------------------------------------------------------------------
void Spot::setFront(ofxBerkeliumSpot * newFront){
	frontWebkit = newFront;
	frontAlpha = 0;
}

//--------------------------------------------------------------------------------------------------------------
void Spot::setFull(ofxBerkeliumSpot * newFull){
	fullWebkit = newFull;
}
//--------------------------------------------------------------------------------------------------------------
void Spot::drawBack(){

	//cout << "DRAW BACK: [[ BEGIN ]]" << endl;
	
	if (backPointsMap.find(frameCurrent) != backPointsMap.end())
	{	
		ofPushStyle();
		ofPushMatrix();
		SpotPoint* p = backPointsMap[frameCurrent];
		
		//ofNoFill();
		//ofSetLineWidth(2);
		//backFollower->updateFromPoint(p);
		backWebkit->setPos(p->x,p->y,p->scale*1.777778,p->scale);
		//backWebkit->setPos(backFollower->x,backFollower->y,backFollower->scale*1.777778,backFollower->scale);
		ofSetColor(255,255,255, ofMap(p->alpha,0,100,0,255));
		backWebkit->draw();
		//ofSetColor(255,0,0,ofMap(p->alpha,0,100,0,128));
		//ofRect(p->x,p->y,p->scale*1.777778,p->scale);
		//ofDrawBitmapString(ofToString(number)+" x: "+ofToString(p->x)+" y: "+ofToString(p->y)+" s: "+ofToString(p->scale), p->x-30, p->y - 15);
		
		ofPopStyle();
		ofPopMatrix();
	}
	
	//cout << "DRAW BACK: [[ END! ]]" << endl;
	
}

//--------------------------------------------------------------------------------------------------------------
void Spot::drawFront(){

	if (frontPointsMap.find(frameCurrent) != frontPointsMap.end())
		{
			SpotPoint* p = frontPointsMap[frameCurrent];
			ofPushStyle();
			ofPushMatrix();
			
			//frontWebkit->setPos(p->x -p->scale*1.777778 ,p->y - p->scale*.5,p->scale*1.777778,p->scale);
			ofSetColor(255,255,255,ofMap(p->alpha,0,100,0,255));
			//frontWebkit->draw();
			frontWebkit->drawCheck(p->x -p->scale*1.777778 ,p->y - p->scale*.5,p->scale*1.777778,p->scale);
			//ofSetColor(0,255,0,ofMap(p->alpha,0,100,0,128));
			//ofRect(p->x -p->scale*1.777778 ,p->y - p->scale*.5,p->scale*1.777778,p->scale);
			//ofSetColor(255,123,255,255);
			//ofDrawBitmapString("id: "+ofToString(number),p->x -p->scale*1.777778 ,p->y - p->scale*.51);
			//verdana.drawString("id: "+ofToString(number),p->x -p->scale*1.777778 ,p->y - p->scale*.51);	
			//ofSetColor(0,0,0,255);
			//verdana.drawString("ff: "+ofToString(frameFirstBack) + " fl: "+ofToString(frameLastBack),p->x ,(p->y - p->scale*.10));
			//if (backPointsMap.find(frameCurrent) != backPointsMap.end())

				//{
					//SpotPoint* pb=  backPointsMap[frameCurrent];
					//verdana.drawString("cf: "+ofToString(frameCurrent) + " cy: "+ofToString(pb->y),p->x ,(p->y - p->scale*.10)+40);
			//	}
						
			
			//SpotPoint* pb = backPointsMap[frameCurrent];
			//if(pb != NULL)
			//	{
				//	ofSetColor(255,0,0);
				//	ofNoFill();
				//	ofRect(pb->x,pb->y,pb->scale*1.777778,pb->scale);
				//	verdana.drawString(ofToString(number)+" w: "+ofToString(pb->scale*1.777778)+" h: "+ofToString(pb->scale),pb->x, pb->y - 20);
					//backFollower->updateFromPoint(pb);
				//	ofSetColor(0,255,0);
				//	ofNoFill();
				//	ofRect(backFollower->x,backFollower->y,backFollower->scale*1.777778,backFollower->scale);
				//	ofDrawBitmapString(ofToString(number)+" w: "+ofToString(backFollower->scale*1.777778)+" h: "+ofToString(backFollower->scale),backFollower->x, backFollower->y - 20);
				//}

			ofPopMatrix();						
			ofPopStyle();
		}
	}




//--------------------------------------------------------------------------------------------------------------
void Spot::printInfo(){
	cout << "XXXXX[printInfo mapsize]: "+ofToString(frontPointsMap.size())<< endl;
	cout << "XXXXX			 [FRAMES]: "+ofToString(frameLastBack - frameFirstBack )<< endl;
	cout << "XXXXX			 [CURRFR]: "+ofToString(frameCurrent )<< endl;
}

//--------------------------------------------------------------------------------------------------------------
string Spot:: getInfoString(){
	//cout << "getInfoString			 [BEGIN]: "+ofToString(frameCurrent )<< endl;
	string out = " ms: "+ofToString(frontPointsMap.size());
		   out += " ff: "+ofToString(frameFirstBack);
		   out += " cf: "+ofToString(frameCurrent);
		   out += " lf: "+ofToString(frameLastBack);

		   if(appearsFront(frameCurrent))out += "front:[ x: "+ofToString(frontPointsMap[frameCurrent]->x);
		   if(appearsFront(frameCurrent))out += " y: "+ofToString(frontPointsMap[frameCurrent]->y);
		   if(appearsFront(frameCurrent))out += " scale: "+ofToString(frontPointsMap[frameCurrent]->scale);
		   if(appearsFront(frameCurrent))out += " alpha: "+ofToString(frontPointsMap[frameCurrent]->alpha);


		   if(appearsBack(frameCurrent))out += "] BACK: [ x: "+ofToString(backPointsMap[frameCurrent]->x);
		   if(appearsBack(frameCurrent))out += " y: "+ofToString(backPointsMap[frameCurrent]->y);
		   if(appearsBack(frameCurrent))out += " scale: "+ofToString(backPointsMap[frameCurrent]->scale);
		   if(appearsBack(frameCurrent))out += " alpha: "+ofToString(backPointsMap[frameCurrent]->alpha)+ "]";


	//cout << "getInfoString			 [END]: "+ofToString(frameCurrent )<< endl;
	
	return out;
	
}

//--------------------------------------------------------------------------------------------------------------
void Spot::allMightyMethod(int frameCur){

	cout << "XXXXX[allMightyMethod mapsize]: "+ofToString(backPointsMap.size())<< endl;
	cout << "XXXXX			 [FRAMES]: "+ofToString(frameLastBack - frameFirstBack )<< endl;
	cout << "XXXXX			 [CURRFR]: "+ofToString(frameCurrent )<< endl;
	cout << "XXXXX			 [frameCur]: "+ofToString(frameCur )<< endl;

}

//--------------------------------------------------------------------------------------------------------------
bool Spot::mouseIsOver(int _x,int _y){
	//cout << "[ Spot::mouseIsOver] begin"<<endl;
	if(!appearsFront(frameCurrent))
		 return false;
	int x = frontPointsMap[frameCurrent]->x;
	int y = frontPointsMap[frameCurrent]->y;
	int s = frontPointsMap[frameCurrent]->scale;

	//cout << "is over ? :"+ofToString(((_x >=  frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334)) && (_x<=  frontPointsMap[frameCurrent]->x) ) && ( (_y >= frontPointsMap[frameCurrent]->y  - frontPointsMap[frameCurrent]->scale*.5) && ( _y <= (frontPointsMap[frameCurrent]->y - frontPointsMap[frameCurrent]->scale*.5)+frontPointsMap[frameCurrent]->scale)) )<<endl;
	//cout << "[ Spot::mouseIsOver] end"<<endl;
	return ( ((_x >=  frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334)) && (_x<=  frontPointsMap[frameCurrent]->x) ) && ( (_y >= frontPointsMap[frameCurrent]->y  - frontPointsMap[frameCurrent]->scale*.5) && ( _y <= (frontPointsMap[frameCurrent]->y - frontPointsMap[frameCurrent]->scale*.5)+frontPointsMap[frameCurrent]->scale)) );

}

//--------------------------------------------------------------------------------------------------------------
void Spot::mouseMoved(int _x, int _y ){
	_x -= frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334);
	_y -= frontPointsMap[frameCurrent]->y -frontPointsMap[frameCurrent]->scale*.5 ;
	frontWebkit->mouseMoved(
		ofMap(_x,0,frontPointsMap[frameCurrent]->scale*1.334,0,1920),
		ofMap(_y,0,frontPointsMap[frameCurrent]->scale,0,1080)
		);

}

//--------------------------------------------------------------------------------------------------------------
void Spot::mouseDragged(int _x, int _y, int button){

	_x -= frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334);
	_y -= frontPointsMap[frameCurrent]->y -frontPointsMap[frameCurrent]->scale*.5 ;
	
	frontWebkit->mouseDragged(
		ofMap(_x,0,frontPointsMap[frameCurrent]->scale*1.334,0,1920),
		ofMap(_y,0,frontPointsMap[frameCurrent]->scale,0,1080)
	);
}

//--------------------------------------------------------------------------------------------------------------
void Spot::mousePressed(int _x, int _y, int button,int screen,int spot){
	
	cout << "[Spot::mousePressed] begin "<<endl;
	
	cout << "in[x:"+ofToString(_x)+" y:"+ofToString(_y)+"] ";
	_x -= frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334);
	_y -= frontPointsMap[frameCurrent]->y -frontPointsMap[frameCurrent]->scale*.5 ;
	
	
	cout << "in[x:"+ofToString(_x)+" y:"+ofToString(_y)+"] ";
	
	
	cout << "wk[x:"+ofToString(ofMap(_x,0,frontPointsMap[frameCurrent]->scale*1.334,0,1920))+" y:"+ofToString(ofMap(_y,0,frontPointsMap[frameCurrent]->scale,0,1080))+"] "<< endl;

	frontWebkit->mousePressed(
		ofMap(_x,0,frontPointsMap[frameCurrent]->scale*1.334,0,1920),
		ofMap(_y,0,frontPointsMap[frameCurrent]->scale,0,1080),
		button,
		screen,
		spot
	);

	cout << "[Spot::mousePressed] end "<<endl;

}

//--------------------------------------------------------------------------------------------------------------
void Spot::mouseReleased(int _x, int _y, int button){
	_x -= frontPointsMap[frameCurrent]->x-(frontPointsMap[frameCurrent]->scale*1.334);
	_y -= frontPointsMap[frameCurrent]->y -frontPointsMap[frameCurrent]->scale*.5 ;
	
	frontWebkit->mouseReleased(
		ofMap(_x,0,frontPointsMap[frameCurrent]->scale*1.334,0,1920),
		ofMap(_y,0,frontPointsMap[frameCurrent]->scale,0,1080),
		button
	);
}






//--------------------------------------------------------------------------------------------------------------
ofxBerkeliumSpot * Spot::getFull(){
	return fullWebkit;
}