#include "ofxBerkeliumHandler.h"

#define USE_TRANSPARENCY true
#define WIDTH 1920
#define HEIGHT 1080

//--------------------------------------------------------------
void ofxBerkeliumHandler::mousePressed(int x, int y, int button){
		
	if(loaded) browser->mouseClick(x, y, button, true);
	cout << "berkelium MousePressed " << endl;
	}

//--------------------------------------------------------------
void ofxBerkeliumHandler::mouseReleased(int x, int y, int button){

	if(loaded) browser->mouseClick(x, y, button, false);
	cout << "berkelium mouseReleased " << endl;
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::mouseDragged(int x, int y){
	
	if(loaded) browser->mouseMoved(x, y);
	cout << "berkelium mouseDragged " << endl;

}

//--------------------------------------------------------------
void ofxBerkeliumHandler::mouseMoved(int x, int y){
	
	if(loaded)browser->mouseMoved(x, y);
	//cout << "berkelium mouseMoved " << endl;

}

//--------------------------------------------------------------
void  ofxBerkeliumHandler::setPos(int _xPos,int _yPos,int _width,int _height){
	
	xPos = _xPos;
	yPos = _yPos;
	width = _width;
	height = _height;
	
	//cout << " BERKELIUM SETPOS[[ END ]]" << endl;
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::setBackFrom(SpotPoint * point){

	if (point != NULL)
		{
				//cout << " ofxBerkeliumHandler::setFrom 00 " << endl;
				//cout << "x: "+ofToString(point->x)+" y: "+ofToString(point->y)+" w: "+ofToString(point->scale*1.3)+" h: "+ofToString(point->scale) << endl;

				//xPos = point->x;
				//yPos = point->y - (point->scale)/2;
				//width = point->scale*1.3;
				//height = point->scale;

				//cout << " ofxBerkeliumHandler::setFrom 01 " << endl;
		}
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::setFrontFrom(SpotPoint * point){
	
	if (point != NULL)
		{
				//cout << " ofxBerkeliumHandler::setFrom 00 " << endl;
				//cout << "x: "+ofToString(point->x)+" y: "+ofToString(point->y)+" w: "+ofToString(point->scale*1.3)+" h: "+ofToString(point->scale) << endl;
				//xPos = point->x - (point->scale*1.3 )/2;
				//yPos = point->y ;
				//width = point->scale*1.3;
				//height = point->scale;
				//cout << " ofxBerkeliumHandler::setFrom 01 " << endl;
		}

}

//--------------------------------------------------------------
void ofxBerkeliumHandler:: initialize(string url){
	
	initializeBrowser();
	loadUrl(url);
	loaded=false;


	myAlpha =255;
}


//--------------------------------------------------------------
void ofxBerkeliumHandler:: initializeBrowser(){
	
    browser = new ofxBerkelium(WIDTH, HEIGHT, USE_TRANSPARENCY);
	browser->setListener(this);
	setPos(0,0,WIDTH,HEIGHT);
			
}


//--------------------------------------------------------------
 void ofxBerkeliumHandler:: loadUrl(string newUrl){
	browser->getWindow()->navigateTo(newUrl.data(),newUrl.length());
	loaded=false;

}

//--------------------------------------------------------------
void ofxBerkeliumHandler:: draw(){

	//cout << " ofxBerkeliumHandler draw begin"<< endl;
	
	ofPushStyle();
	ofSetColor(255,255,255,myAlpha);
	browser->draw(xPos,yPos,width,height);
	ofPopStyle();
	//cout << " ofxBerkeliumHandler draw end"<< endl;
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onAddressBarChanged(string newURL){
	//cout << "*** onAddressBarChanged " << newURL << endl;
	//log.push_back("*** onAddressBarChanged "+newURL);
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onStartLoading(string newURL){
	//cout << "*** onStartLoading " << newURL << endl;
	//log.push_back("*** onStartLoading " +newURL);
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onLoad(){
	cout << "*** onLoad " << endl;
	//log.push_back("*** onLoad ");
	  
          
  //cout << ofToString(cont) << endl;
	//if (cont == 0)
	//	{
			//fboBerkelium1.begin();
			//browser->draw(0,0);
			//fboBerkelium1.end();
			//textures.push_back (  fboBerkelium1.getTextureReference() );  
	//	}
	//else 
	//	{
	//		fboBerkelium2.begin();
	//		browser->draw(0,0);
	//		fboBerkelium2.end();
			//textures.push_back ( fboBerkelium2.getTextureReference()  );
			//fboBerkelium2.getTextureReference()

	//	}
//	cont++;


	
	//  textures.push_back ( tex );  
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onCrashedWorker(){
	//cout << "*** onCrashedWorker " << endl;
	//log.push_back("*** onCrashedWorker ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onCrashedPlugin(wstring pluginName){
	//wcout << "*** onCrashedPlugin " << pluginName << endl;
	//log.push_back("*** onCrashedPlugin ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onProvisionalLoadError(string url, int errorCode, bool isMainFrame){
	//cout << "*** onProvisionalLoadError " << url << ": " << errorCode << (isMainFrame? " (main frame)":"") << endl;
	//log.push_back("*** onProvisionalLoadError " );
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onConsoleMessage(wstring message, wstring sourceId, int line_no){
	wcout << "*** onConsoleMessage " << message << " from " << sourceId << " line " << line_no << endl;
	
	string str( message.begin(), message.end() );
	if ( str == "ready")
	{cout << "CONSOLE" << endl;
		loaded=true;
	}
	//log.push_back("*** onConsoleMessage " + wstring2string(message));
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value){
	wcout << "*** onScriptAlert " << message << endl;
	//log.push_back("*** onScriptAlert ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction){
	cout << "*** onNavigationRequested " << newURL << " by " << referrer << (isNewWindow?"  (new window)" : " (same window)") << endl;
	//log.push_back("*** onNavigationRequested " + newURL);
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onLoadingStateChanged(bool isLoading){
	//cout << "*** onLoadingStateChanged " << (isLoading?"started":"stopped") << std::endl;
	//log.push_back("*** onLoadingStateChanged ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onTitleChanged(wstring title){
	//wcout << "*** onTitleChanged " << title << endl;
	//ofSetWindowTitle(wstring2string(title));
	//log.push_back("*** onTitleChanged ");
}


//--------------------------------------------------------------
void ofxBerkeliumHandler::onTooltipChanged(wstring text){
	//wcout << "*** onTooltipChanged " << text << endl;
	//log.push_back("*** onTooltipChanged ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onCrashed(){
	cout << "*** onCrashed " << endl;
	//log.push_back("*** onCrashed ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onUnresponsive(){
	//cout << "*** onUnresponsive " << endl;
//	log.push_back("*** onUnresponsive ");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onResponsive(){
	//cout << "*** onResponsive " << endl;
	//log.push_back("*** onResponsive ");
}








//--------------------------------------------------------------
void ofxBerkeliumHandler::onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags){
	cout << "*** onShowContextMenu at " << mouseX << "," << mouseY;
	cout << "; type: ";
	switch (mediaType) {
		case Berkelium::ContextMenuEventArgs::MediaTypeImage:
			cout << "image";
			break;
		case Berkelium::ContextMenuEventArgs::MediaTypeVideo:
			cout << "video";
			break;
		case Berkelium::ContextMenuEventArgs::MediaTypeAudio:
			cout << "audio";
			break;
		default:
			cout << "other";
	}
	if (isEditable || editFlags) {
		cout << " (";
		if (isEditable)
			cout << "editable; ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanUndo)
			cout << "Undo, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanRedo)
			cout << "Redo, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanCut)
			cout << "Cut, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanCopy)
			cout << "Copy, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanPaste)
			cout << "Paste, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanDelete)
			cout << "Delete, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanSelectAll)
			cout << "Select All";
		cout << ")";
	}
	cout << endl;
	if (linkUrl.length())
		cout << "        Link URL: " << linkUrl << endl;
	if (srcUrl.length())
		cout << "        Source URL: " << srcUrl << endl;
	if (pageUrl.length())
		cout << "        Page URL: " << pageUrl << endl;
	if (frameUrl.length())
		cout << "        Frame URL: " << frameUrl << endl;
	if (selectedText.length())
		wcout << "        Selected Text: " << selectedText << endl;
	
	//log.push_back("*** onShowContextMenu");
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs){
	
	cout << "*** onJavascriptCallback at URL " << url << ", " << (replyMsg?"synchronous":"async") << endl;
	
	wcout << "    Function name: " << funcName << endl;
	for (size_t i = 0; i < numArgs; i++) {
		Berkelium::WideString jsonStr = toJSON(args[i]);
		wcout << L"    Argument " << i << ": ";
		if (args[i].type() == Berkelium::Script::Variant::JSSTRING) {
			wcout << L"(string) " << args[i].toString() << endl;
		} else {
			wcout << jsonStr << endl;
		}
		Berkelium::Script::toJSON_free(jsonStr);
	}
	if (replyMsg) {
		browser->window()->synchronousScriptReturn(replyMsg, numArgs ? args[0] : Berkelium::Script::Variant());
	}
	
//	log.push_back("*** onJavascriptCallback at URL " + url);
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onRunFileChooser(int mode, wstring title, string defaultFile){
	wcout << "*** onRunFileChooser type " << mode << ", title " << title << ":" << endl;
	cout << defaultFile << endl;
	
	browser->window()->filesSelected(NULL);
}

//--------------------------------------------------------------
void ofxBerkeliumHandler::onExternalHost(wstring message, string origin, string target){
	cout << "*** onExternalHost at URL from " << origin << " to " << target << ":" <<endl;
	wcout << message << endl;
}




void ofxBerkeliumHandler::setAlpha(float newAlpha)
		{
			myAlpha = (int)newAlpha;
		}

void ofxBerkeliumHandler::tick()
		{

			std::string str = "Hello";
			std::wstring str2(str.length(), L' '); // Make room for characters

			// Copy string to wstring.
			std::copy(str.begin(), str.end(), str2.begin());

			//wstring script(L"getData();"); 
		  
			browser->getWindow()->executeJavascript(Berkelium::WideString::point_to(L"resetCounter();")); 


		}

