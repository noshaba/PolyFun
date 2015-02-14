/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/
#include "View.hpp"

View* View::instance = 0;

View* View::getInstance(void){
	try{
		if(instance == NULL) instance = new View();
		return instance;
	} catch(...){
		throw eFileNotFoundException();
	}
}

View::View(void){
	try{
		buttonM = ButtonModel::getInstance();
		sliderM = SliderModel::getInstance();
	} catch(...){
		throw eFileNotFoundException();
	}
	polygonM = PolygonModel::getInstance();
	canvas = Canvas::getInstance();
	currentColour = CurrentColour::getInstance();
	windowWidth = canvas->getWindowWidth();
	windowHeight = canvas->getWindowHeight();
	
	// initialize window with OpenGL and GLFW
	initWindow();
	
}

void View::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}

View::~View(void){
	ButtonModel::resetInstance();
	PolygonModel::resetInstance();
	SliderModel::resetInstance();
	Canvas::resetInstance();
	CurrentColour::resetInstance();
	glfwTerminate();
}

void View::initWindow(){

	glfwInit();  // init GLFW and open an OpenGL Window
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE); // disable resize
	glfwOpenWindow(windowWidth, windowHeight, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
	
	glfwSetWindowTitle("PolyFun");
	
	// init viewport to canvassize
    glViewport( 0, 0, windowWidth, windowHeight);
    
	// projection matrix with clipping planes in three directions (same as window size and +- 1 in z)
	glMatrixMode( GL_PROJECTION);
    glLoadIdentity();
	glOrtho( 0, windowWidth, windowHeight, 0, -1, 1);

	// modelview transform (identity)
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	
	// enable blending for soft lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// display buttons and canvas
	displayBase();
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glfwSwapBuffers();
}

void View::displayBase(void){
	try{
		// render buttons and paper
		displayPaper();
		displayBackground();
		displayButtons();
		displaySliders();
		displayCurrentColour();
		
		// reset colours
		glColor3f(1.0f, 1.0f, 1.0f);
	} catch(...){
		throw eFileNotFoundException();
	}
}

void View::displayBackground(void){
	glColor3f(17/255.0, 17/255.0, 17/255.0);
	//Left side
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates
	glVertex2f(0,0);
	glVertex2f(102,0);
	glVertex2f(102, 540);
	glVertex2f(0,540);
	glEnd(); //End quadrilateral coordinates
	//Right side
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates
	glVertex2f(756,0);
	glVertex2f(960,0);
	glVertex2f(960, 540);
	glVertex2f(756,540);
	glEnd(); //End quadrilateral coordinates
	// reset colours
	glColor3f(1.0f, 1.0f, 1.0f);
	//Line Width title
	GLuint textID = RenderObject::loadTexture("Textures/widthTitle.tga");
	glPushMatrix();
	glEnable(GL_TEXTURE_2D); //Enable texture
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates
	glTexCoord2f(0.0f,1.0f); // top left corner
	glVertex2f(865,360);
	glTexCoord2f(1.0f,1.0f); // top right corner
	glVertex2f(938,360);
	glTexCoord2f(1.0f,0.0f); // bottom right corner
	glVertex2f(938, 379);
	glTexCoord2f(0.0f,0.0f); // bottom left corner
	glVertex2f(865,379);
	glEnd(); //End quadrilateral coordinates
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &textID);
	// reset colours
	glColor3f(1.0f, 1.0f, 1.0f);
	//Header
	textID = RenderObject::loadTexture("Textures/header.tga");
	glPushMatrix();
	glEnable(GL_TEXTURE_2D); //Enable texture
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates
	glTexCoord2f(0.0f,1.0f); // top left corner
	glVertex2f(0,0);
	glTexCoord2f(1.0f,1.0f); // top right corner
	glVertex2f(960,0);
	glTexCoord2f(1.0f,0.0f); // bottom right corner
	glVertex2f(960, 105);
	glTexCoord2f(0.0f,0.0f); // bottom left corner
	glVertex2f(0,105);
	glEnd(); //End quadrilateral coordinates
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &textID);
	// reset colours
	glColor3f(1.0f, 1.0f, 1.0f);
}

void View::updateWindow(void){
	// clear canvas and set background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	polygonM->drawPolygons();
	try{
		displayBase();
	} catch(...){
		throw eFileNotFoundException();
	}
	glfwDisable(GLFW_AUTO_POLL_EVENTS); // disables glfwPollEvents which is called implicitly by glfwSwapBuffers, in order to swap the buffers faster while updating the window
	glfwSwapBuffers();
}

int View::getWindowWidth(void){
	return windowWidth;
}

int View::getWindowHeight(void){
	return windowHeight;
}

void View::displayButtons(void){
	try{
		buttonM->displayButtons();
	}catch(...){
		throw eFileNotFoundException();
	}
}

void View::displaySliders(void){
	try{
		sliderM->displaySliders();
	}catch(...){
		throw eFileNotFoundException();
	}
}

void View::displayPaper(void){
	canvas->displayCanvas();
}

void View::displayCurrentColour(void){
	currentColour->displayCurrentColour();
}