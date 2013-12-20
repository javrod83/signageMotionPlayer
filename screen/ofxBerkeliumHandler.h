#pragma once

#include "ofMain.h"
#include "ofxBerkelium.h"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"
#include "SpotPoint.h"

 class ofxBerkeliumHandler :  ofxBerkeliumListener {

	public:
		int xPos,yPos,width,height;
		void initialize(string url);
		void initializeBrowser();

		void draw();
		virtual void loadUrl(string newUrl);
		void setPos(int _xPos,int _yPos,int _width,int _height);
		void setBackFrom(SpotPoint * point);
		void setFrontFrom(SpotPoint * point);
		ofxBerkelium* browser;
		int myAlpha;

		string url;
		void tick();

		void setAlpha(float newAlpha);

		bool loaded;
		//input
		virtual void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y);
		void mouseMoved(int x, int y);

		//Berkelium Listerner
		void onAddressBarChanged(string newURL);
		void onStartLoading(string newURL);
		void onLoad();
		void onCrashedWorker();
		void onCrashedPlugin(wstring pluginName);
		void onProvisionalLoadError(string url, int errorCode, bool isMainFrame);
		void onConsoleMessage(wstring message, wstring sourceId, int line_no);
		void onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value);
		void onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction);
		void onLoadingStateChanged(bool isLoading);
		void onTitleChanged(wstring title);
		void onTooltipChanged(wstring text);
		void onCrashed();
		void onUnresponsive();
		void onResponsive();
		void onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags);
		void onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs);
		void onRunFileChooser(int mode, wstring title, string defaultFile);
		void onExternalHost(wstring message, string origin, string target);
};







