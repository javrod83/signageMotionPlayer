#include "ContentManager.h"	

//--------------------------------------------------------------------------------------------------------------
ofxBerkeliumSpot * ContentManager::getWebkit(string url)
{
	currentShowing = 0;
	cout << "ContactManager::get begin" << endl;
	//si no encuentra la key no existe
	if(contentUsed.count(url) == 0)
		{
			cout << "                     nueva url" << endl;
			cout << "					  free:"<<ofToString(contentFree.size()) << endl;
			cout << "					  cached:"<<ofToString(contentUsed.size()) << endl;
			//si no hay ningun berkelium libre
			if (contentFree.size() == 0)
				{
					cout << "                      no hay ningun berkelium libre" << endl;
					ofxBerkeliumSpot * newBerk = new ofxBerkeliumSpot();
					newBerk->setLoaderListener(this);
					newBerk->initialize(url);
					newBerk->setPos(0,0,0,0);
					contentUsed[url]=newBerk;
					cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<< endl;
					cout << "					  free:"<<ofToString(contentFree.size()) << endl;
					cout << "					  cached:"<<ofToString(contentUsed.size()) << endl;
					ofSleepMillis(100);
					return newBerk;
				}
			else //si hay algun berk disponible
				{
					cout << "                      berk disponible" << endl;
					// hago un pop del vector 
					ofxBerkeliumSpot * recycledBerk = contentFree[contentFree.size()-1];
					//lo elimino del vector
					contentFree.pop_back();
					contentUsed[url]=recycledBerk;
					recycledBerk->loadUrl(url);

					// lo retorno
					cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<< endl;
					cout << "					  free:"<<ofToString(contentFree.size()) << endl;
					cout << "					  cached:"<<ofToString(contentUsed.size()) << endl;
					ofSleepMillis(100);
					return recycledBerk;
				}

			
		}
	else// ya se enecuentra el contenido buscado
		{
			cout << "                     url ya existe en cache" << endl;
			//retorno el berkelium en uso
			return contentUsed[url];
		}
	cout << "ContactManager::get end" << endl;
}

//--------------------------------------------------------------------------------------------------------------
ofxBerkeliumSpot * ContentManager::getDelayedWebkit(string url)
{
	currentShowing = 0;
	cout << "ContactManager::getDelayedWebkit" << endl;
	cout << "              check: "<< url << endl;


	//si no encuentra la key no existe
	if(contentUsed.count(url) == 0)
		{
			//si no hay ningun berkelium libre
			if (contentFree.size() == 0)
				{
					cout << "              new berk" << endl;
					ofxBerkeliumSpot * newBerk = new ofxBerkeliumSpot();
					newBerk->setLoaderListener(this);
					newBerk->initializeDelayed(url);
					delayed(newBerk);
					newBerk->setPos(0,0,0,0);
					contentUsed[url]=newBerk;
					cout << "              free:"<<ofToString(contentFree.size())  << "cached:"<<ofToString(contentUsed.size()) << endl << endl;
					ofSleepMillis(100);
					return newBerk;
				}
			else //si hay algun berk disponible
				{
					cout << "              recycled berk " << endl;
					// hago un pop del vector 
					ofxBerkeliumSpot * recycledBerk = contentFree[contentFree.size()-1];
					//lo elimino del vector
					contentFree.pop_back();
					contentUsed[url]=recycledBerk;
					
					
					
					recycledBerk->setDelayedUrl(url);
					delayed(recycledBerk);
					//if (thereAreDelayedWebkits())
					//	checkDelayed();
					cout << "              free:"<<ofToString(contentFree.size())  << "cached:"<<ofToString(contentUsed.size()) << endl << endl;
					ofSleepMillis(100);
					return recycledBerk;
				}

			
		}
	else// ya se enecuentra el contenido buscado
		{
			cout << "              cached berk (nothing todo )" << endl << endl;
			//retorno el berkelium en uso
			return contentUsed[url];
		}
	cout << "ContactManager::getDelayedWebkit" << endl;
}

ofxBerkeliumSpot * ContentManager::getMockedWebkit(string url)
{
	currentShowing = 0;
	cout << "ContactManager::getMockedWebkit" << endl;
	cout << "              : "<< url << endl;


	//si no encuentra la key no existe
	if(contentUsed.count(url) == 0)
		{
			//si no hay ningun berkelium libre
			if (contentFree.size() == 0)
				{
					cout << "              new berk" << endl;
					ofxBerkeliumSpot * newBerk = new ofxBerkeliumSpot();
					newBerk->setLoaderListener(this);
					newBerk->initializeMocker(url);
				
					newBerk->setPos(0,0,0,0);
					contentUsed[url]=newBerk;
					cout << "              free:"<<ofToString(contentFree.size())  << "cached:"<<ofToString(contentUsed.size()) << endl << endl;
					ofSleepMillis(100);
					return newBerk;
				}
			else //si hay algun berk disponible
				{
					cout << "              recycled berk " << endl;
					// hago un pop del vector 
					ofxBerkeliumSpot * recycledBerk = contentFree[contentFree.size()-1];
					//lo elimino del vector
					contentFree.pop_back();
					contentUsed[url]=recycledBerk;
					
					
					
					recycledBerk->setDelayedUrl(url);
					//recycledBerk->callLoader();
					delayed(recycledBerk);
					//if (thereAreDelayedWebkits())
					//	checkDelayed();
					cout << "              free:"<<ofToString(contentFree.size())  << "cached:"<<ofToString(contentUsed.size()) << endl << endl;
					ofSleepMillis(100);
					return recycledBerk;
				}

			
		}
	else// ya se enecuentra el contenido buscado
		{
			cout << "              cached berk (nothing todo )" << endl << endl;
			//retorno el berkelium en uso
			return contentUsed[url];
		}
	cout << "ContactManager::getDelayedWebkit" << endl;
}

//--------------------------------------------------------------------------------------------------------------
map <string,ofxBerkeliumSpot * > ContentManager::getWebkits(){
	return contentUsed;
}

//--------------------------------------------------------------------------------------------------------------
void  ContentManager::loadSpotsWebkits(ofxJSONElement json,int section,int screen){

	cout << "[[ContactManager::loadSpotsWebkits]]:  begin"  << endl ; 
	int cantSposts = spotsMap[screen][section]->spots.size();
	int currentContent = 0;
	for (int i=0;i< cantSposts;i++)
		{
				 
			currentContent = json["motion"]["screens"][screen]["spots"][i]["number"].asInt();
			cout << "[[ CONTENT ]]: "<< ofToString(currentContent) << endl ; 
			
			spotsMap[screen][section]->spots[i]->setFront( getMockedWebkit(json["posts"][currentContent]["front"].asString()));
			spotsMap[screen][section]->spots[i]->setBack ( getMockedWebkit(json["posts"][currentContent]["back" ].asString()));
			spotsMap[screen][section]->spots[i]->setFull ( getMockedWebkit(json["posts"][currentContent]["full" ].asString()));
			

		}
	cout << "[[ContactManager::loadSpotsWebkits]]: end" << endl ; 
}

//--------------------------------------------------------------------------------------------------------------
void ContentManager:: releasewebkits(){
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "ContactManager::releasewebkits begin" << endl;	
	cout << "                              BEFORE" << endl;
	cout << "                                Used :" << ofToString(contentUsed.size()) << endl;
	cout << "                                Free :" << ofToString(contentFree.size()) << endl;
	
	//elimino el berk del mapa erase
	for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{

			contentFree.push_back(itr->second);
			contentUsed.erase(itr);
		}
	//lo agrego al vector de los libres 
	barrierReady = 0 ;
	barrierDelayed = 0 ;
	barrierLoading = 0 ;
	loadingCount = 4;
	loadsCounter++;


	cout << "                               CLEAN" << endl;
	cout << "                                Used :" << ofToString(contentUsed.size()) << endl;
	cout << "                                Free :" << ofToString(contentFree.size()) << endl;
	cout << "ContactManager::releasewebkits end" << endl;
	cout << "                              BEFORE" << endl;
	
}

//--------------------------------------------------------------------------------------------------------------
void ContentManager::loadSpots(ofxJSONElement json,int motion,int screen){
	cout << "[ContactManager::loadSpots]begin: " << endl;

	if (spotsMap[screen][motion] == NULL)
		spotsMap[screen][motion] = new SpotsDataSet(json);

	cout << "[ContactManager::loadSpots]end " << endl;
}

//--------------------------------------------------------------------------------------------------------------
vector <Spot * > ContentManager::getSpotsForMotion(ofxJSONElement json ,int motion,int screen ){

	cout << "[ContactManager::getSpotsForMotion]begin " << endl;
	cout << "                                   motion: "<< motion << endl;
	cout << "                                   screen: "<< screen << endl;
		
	if (spotsMap[screen].find(motion) == spotsMap [screen].end())
		{
			cout << "		[motion: ["+ofToString(motion)+"]["+ofToString(screen)+"]]]caching spots" << endl;
			loadSpots( json["motion"]["screens"][screen], motion,screen);
		}
	else
		cout << "			data allready cached =) " << endl;
	loadSpotsWebkits(json,motion,screen);
	cout << "[ContactManager::getSpotsForMotion]end  " << endl;
	//asignarwebkits a spots
	return spotsMap[screen][motion]->spots;
}

//--------------------------------------------------------------------------------------------------------------
void ContentManager::loading()
	{
		barrierLoading++;
	}

//--------------------------------------------------------------------------------------------------------------
void ContentManager::ready()
	{
		barrierReady++;
		barrierLoading--;
	}

void ContentManager::check()
	{
		barrierLoading--;
		if(!loaded())
			barrierReady++;

	}
//--------------------------------------------------------------------------------------------------------------
void ContentManager::delayed(ofxBerkeliumSpot * webkit)
	{

		delayedWebkits.push_back(webkit);
		barrierDelayed++;
	}

//--------------------------------------------------------------------------------------------------------------
bool ContentManager::loaded()
	{

		return ( barrierReady == contentUsed.size() );
	}

//--------------------------------------------------------------------------------------------------------------
void ContentManager::checkDelayed(){
	//cout << "ContentManager::checkdelayed " << endl;
		if (thereIsRoomInDelayBuffer()  && thereAreDelayedWebkits() )
			{
				//cout << "            load a delayed  " << endl;
	
				delayedWebkits[0]->callLoader();
				delayedWebkits.erase(delayedWebkits.begin());
			}
		//else
			//cout << "            dont load nothing" << endl;
	
	}

//--------------------------------------------------------------------------------------------------------------
bool ContentManager::thereIsRoomInDelayBuffer()
	{
		
		return(barrierLoading < loadingCount);
	}

//--------------------------------------------------------------------------------------------------------------
bool ContentManager::thereAreDelayedWebkits()
	{
		//cout << "delayedWebkits.size = " <<ofToString(delayedWebkits.size() )<< endl;
		return(delayedWebkits.size() > 0 );
	}

//--------------------------------------------------------------------------------------------------------------
void ContentManager::drawDebug(int x,int y,int w){

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x,y);
	ofPushStyle();
	ofSetColor(0,0,0,1280);
	ofFill();
	ofRect(0,0,w,ofGetHeight()-y);
	
	ofSetColor(255,0,115);
	ofNoFill();
	ofSetLineWidth(2);
	ofRect(0,0,w,ofGetHeight()-y);
	ofDrawBitmapString("USED:"+ofToString(contentUsed.size())+" LOADING:"+ofToString(barrierLoading)+" READY: "+ofToString(barrierReady),10,35);
	
	ofDrawBitmapString("SELECTED URL :"+currentUrl,10,45);


	    int secs = ((int)ofGetElapsedTimef())%60;
		int mins = (int)((ofGetElapsedTimef()/60))%60 ;
		int hours = (int)(ofGetElapsedTimef()/3600)  ;
	countSectionDraw(w*.66,20);
	ofDrawBitmapString("CHANGE N: "+ofToString(loadsCounter)+" uptime:"+ofToString(hours)+":"+ofToString(mins)+":"+ofToString(secs),w*.66,55);

	//elimino el berk del mapa erase
	ofPopStyle();
	
	ofTranslate(0,70);
	
	int prevWidth = 120;
	int prevHeight = prevWidth/1.7777778;
	int colls = (w*.66)/(prevWidth+5);
	int space = 5 ;
	int i=0;
	for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
			int xPos =(i%colls)*(prevWidth+space);
			int yPos =(i/colls)*(prevHeight+space);
			//draw preview
			itr->second->drawDebug(xPos,yPos,prevWidth,prevHeight);
	
			if (i==currentShowing)
				{
					itr->second->drawDebug((w*.66),0,(w*.33),(w*.33)/1.777778);
					ofPushStyle();
					ofSetColor(0,255,255,255);
					ofNoFill();
					ofSetLineWidth(2);
					ofRect((w*.66),0,(w*.33),(w*.33)/1.777778);
					ofRect(xPos,yPos,prevWidth,prevHeight);
					ofPopStyle();

					currentUrl  = itr->second->getUrl();
					
					}
				i++;
		}

	ofPopMatrix();
	ofPopStyle();
}

//---------------------------------------------------------------------------
void ContentManager::nextDebug(){

	currentShowing = (currentShowing + 1) % contentUsed.size(); 

	int i=0;
	for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
		
			if (i==currentShowing)
				{
					currentUrl  = itr->second->getUrl();		
				}
			i++;
		}

	cout <<  "URL: "+ currentUrl<< endl;
}

//---------------------------------------------------------------------------
void ContentManager::prevDebug(){

	currentShowing = (currentShowing-1  < 0 )?contentUsed.size()-1:currentShowing - 1;
	int i=0;
	for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
		
			if (i==currentShowing)
				{
					currentUrl  = itr->second->getUrl();		
				}
			i++;
		}
	cout <<  "URL: "+ currentUrl << endl;

}

//---------------------------------------------------------------------------
void ContentManager::reloadDebug()
	{
		cout << "Content manager reloadDebug  cs:["+ofToString(currentShowing)+"] ts:["+ofToString(contentUsed.size())+"]"<< endl;
	
		int i=0;
		for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
			if (i==currentShowing)
				{
					itr->second->callLoader();
					addToMockMap(itr->second->getUrl());
					//itr->second->loadUrl(itr->second->getUrl());
					cout << "  [[ RELOAD ]]  !"<<endl<<" url: "+currentUrl<<endl;
				}
		i++;
		}
	}



//--------------------------------------------------------------------------------------------------------------
void ContentManager::loadMocks()
	{
		cout << "loadMocks"<< endl;

		for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
				{
					//itr->second->loadDelayedUrl();
					itr->second->callLoader();
					cout << "  [[ RELOAD ]]  !"<<endl<<" url: "+currentUrl<<endl;
				}

		}
	}

//---------------------------------------------------------------------------
void ContentManager::loadDelayed()
	{
		cout << "Content manager loadDelayed  cs:["+ofToString(currentShowing)+"] ts:["+ofToString(contentUsed.size())+"]"<< endl;
	
		int i=0;
		for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
		{
			if (i==currentShowing)
				{
					//itr->second->loadDelayedUrl();
					itr->second->callLoader();
					cout << "  [[ RELOAD ]]  !"<<endl<<" url: "+currentUrl<<endl;
				}
		i++;
		}
	}

//---------------------------------------------------------------------------
void ContentManager::checkReadys()
	{
		cout << "checkReadys" <<  endl;

		//para cada 
		for (auto itr = contentUsed.begin(); itr != contentUsed.end(); ++itr) 
				{			
					if ( itr->second->loaded &&  isNotOk(itr->second))
						{
							cout << "NOT OKJ NOT OK NOT OK  "<< endl;
							itr->second->loadUrl(itr->second->getUrl());
							
						}
					else
						cout << " OK  OK OK OK OK "<<endl;
				}

	}

bool ContentManager::isNotOk(ofxBerkeliumSpot * aSpot)
{
	/*
		checkerFBO.begin();
		ofClear(0,0,0,255);
		aSpot->drawCheck(0,0,320,240);
		checkerFBO.end();

		checkerFBO.readToPixels(checkerImage.getPixelsRef());

		unsigned char * pixels = checkerImage.getPixels();

		int total = checkerImage.width * checkerImage.height * 3;
		
		int sum = 0 ;
		
		for (int i = 0 ;i < total; i ++)
			{
				sum += pixels[i]; 
			}

		cout <<"idNoTOk: " +ofToString((sum/total <= 1 ))<< endl;
		//for (int i = 0 ; i < )


		return (sum/total <= 1 );
	*/
	return false;
} 





void ContentManager::printMockMap()
	{
		cout << " MOCKS DE MIERDA QUE NO QUEREMOS !!!!!!!!";

		for (auto itr = mockmap.begin(); itr != mockmap.end(); ++itr) 
		{
			cout << "DEMIERDA MOCK :"  +itr->second << endl;

			
		}


	}


void ContentManager::clearMockMap()
{
	cout << "clear mock map "<< endl;


	for (auto itr = mockmap.begin(); itr != mockmap.end(); ++itr) 
		{
			mockmap.erase(itr);
		}
}



void ContentManager::addToMockMap(string cmd)
	{

		cout <<"[+]addToMockMap url:"+cmd << endl;
		cout << "i: "+ofToString(mockmap.size()) << endl;


		mockmap[cmd] = cmd;
	}

void ContentManager::delFromMockMap(string cmd)
	{

		cout <<"[-]delFromMockMap url:"+cmd << endl;
		cout << "i: "+ofToString(mockmap.size()) << endl;


		mockmap.erase(cmd);

	}

//--------------------------------------------------------------------------------------------------------------------------------
void ContentManager::countSection(int section)
	{
		sectionsCounter[section]++;
	}

//--------------------------------------------------------------------------------------------------------------------------------
void ContentManager::countSectionInit()
	{
		for (int i =1 ;i<=7;i++)
		sectionsCounter[i]=0;
	}


//--------------------------------------------------------------------------------------------------------------------------------
void ContentManager::countSectionDraw(int x,int y)
	{

		ofPushMatrix();
		ofTranslate(x,y+15);
		ofDrawBitmapString("setions loads: ",0,0);
		ofTranslate(10,15);
		string str;
		for (int i =1 ;i<=7;i++)
			{
		
				str=str + "["+ofToString(sectionsCounter[i])+"] ";
			}

		ofDrawBitmapString(str,130,-15);
		ofPopMatrix();

	}

