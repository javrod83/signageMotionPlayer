#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"


#define PRODUCTION true

//========================================================================
int main( ){

    ofAppGlutWindow window;

	if(PRODUCTION)
		ofSetupOpenGL(&window, 1920*3,1080,OF_WINDOW);			// <-------- setup the GL context
	else
		ofSetupOpenGL(&window, 1920,1080,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	bool showWindowBorder = !  PRODUCTION;
if (!showWindowBorder) {
	HWND m_hWnd = WindowFromDC(wglGetCurrentDC());
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~WS_DLGFRAME;
	style &= ~WS_CAPTION;
	style &= ~WS_BORDER;
	style &= WS_POPUP;
	LONG exstyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	exstyle &= ~WS_EX_DLGMODALFRAME;
	::SetWindowLong(m_hWnd, GWL_STYLE, style);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, exstyle);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
}

	
	
	ofRunApp( new testApp());

	

}
