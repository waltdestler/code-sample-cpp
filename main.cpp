#include "ofMain.h"
#include "App.h"

/**
 * Main application entry point.
 * Sets up Open Frameworks and then runs the app.
 */
int main(int argc, char *argv[])
{
	ofSetupOpenGL(1024, 768, OF_FULLSCREEN);
	ofRunApp(new App());
}
