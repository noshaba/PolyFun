/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

#include "GraphicObjects.hpp"

#include <iostream>
#include <iomanip> //setprecision
#include <sstream> // string stream
#include <math.h> // ceil

///////////////////
// GraphicObject //
///////////////////

/**
* 'GraphicObject' is an object rendered to the stage. 'GraphicObject' is an abstract class.
* - Provided functions: Getter and Setter to change the objects attributes
* Default selection state: false
*
* @param xpos - X-coordinate of the upper left corner of the object
* @param ypos - Y-coordinate of the upper left corner of the object
* @param width - Width of the object
* @param height - Height of the object
*/
GraphicObject::GraphicObject(double xpos, double ypos, double width, double height){
	traits.xpos = xpos;
	traits.ypos = ypos;
	traits.width = width;
	traits.height = height;
	selection = false; // by default unselected
}
/**
* Destructor of 'GraphicObject'. Called when a 'GraphicObject' has to free memory or do other clean up when the object is destroyed.
*/
GraphicObject::~GraphicObject(void){}
/**
* 'setPostition' changes the position of the 'GraphicObject' according to the X- and Y-coordinates of its upper left corner.
*
* @param xpos - new X-coordinate of upper left corner
* @param ypos - new Y-coordinate of upper left corner
*/
void GraphicObject::setPosition(double xpos, double ypos){
	traits.xpos = xpos;
	traits.ypos = ypos;
}
/**
* 'setWidth' changes the width of the 'GaphicObject'.
*
* @param width - new width of the object
*/
void GraphicObject::setWidth(double width){
	traits.width = width;
}
/**
* 'setHeight' changes the height of the 'GraphicObject'.
*
* @param height - new height of the 'GraphicObject'
*/
void GraphicObject::setHeight(double height){
	traits.height = height;
}
/**
* 'setSize' changes the width and the height of the 'GraphicObject'.
*
* @param width - new width
* @param height - new height
*/
void GraphicObject::setSize(double width, double height){
	traits.width = width;
	traits.height = height;
}
/**
* 'setTraits' sets the size and the position of the 'GraphicObject'.
*
* @param xpos - new X-coordinate of upper left corner
* @param ypos - new Y-coordinate of upper left corner
* @param width - new width
* @param height - new height
*/
void GraphicObject::setTraits(double xpos, double ypos, double width, double height){
	traits.xpos = xpos;
	traits.ypos = ypos;
	traits.width = width;
	traits.height = height;
}
/**
* 'setSelection' changes the current state of the 'GraphicObject's' selection
*
* @param selected - true when selected, false when not
*/
void GraphicObject::setSelection(bool selected){
	selection = selected;
}
/**
* 'getTraits' returns the 'ObjTraits' of the 'GraphicObject', containing information about the position and the size.
*
* @return traits - an 'ObjTraits' struct with the position and the size of the object
*/
ObjTraits GraphicObject::getTraits(void){
	return traits;
}
/**
* 'selected' returns the current selection state of the 'GraphicObject'
*/
bool GraphicObject::selected(void){
	return selection;
}

//////////////////
// RenderObject //
//////////////////

/**
* 'RenderObject' is a derived class from 'GraphicObject'. It provides an image path as an extension to draw objects with textures.
* Extensions of 'RenderObject': Image path
* Additional provided functions: Functions to load and display a texture
*
* @param xpos - X-coordinate of the upper left corner of the object
* @param ypos - Y-coordinate of the upper left corner of the object
* @param width - Width of the object
* @param height - Height of the object
* @param path - Image path of the texture of the object
*/
RenderObject::RenderObject(double xpos, double ypos, double width, double height, std::string path) : GraphicObject(xpos, ypos, width, height){
	imgPath = path;
	this->loadTexture(path);
}
/**
* Destructor of 'RenderObject'. Called when a 'RenderObject' has to free memory or do other clean up when the object is destroyed.
*/
RenderObject::~RenderObject(void){}
std::string RenderObject::getImgPath(void){
	return imgPath;
}
/**
* 'loadTexture' a texture from an image and gives it a texture ID which is then being returned.
*
* @param imgPath - Path of the texture
* @return texture - Texture ID
*/
void RenderObject::loadTexture(std::string imgPath){
	glGenTextures(1,&texture); //allocate the memory for texture
	glBindTexture(GL_TEXTURE_2D,texture); //Binding the texture
	if(glfwLoadTexture2D(imgPath.c_str(), GLFW_BUILD_MIPMAPS_BIT)){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	}
}
/**
* 'display' renders the object's texture to the stage.
*/
void RenderObject::display(void){
	// default display
	glBindTexture(GL_TEXTURE_2D,texture); //Binding the texture
	glPushMatrix();
	glEnable(GL_TEXTURE_2D); //Enable texture
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates
	glTexCoord2f(0.0f,1.0f); // top left corner
	glVertex2f(traits.xpos,traits.ypos);
	glTexCoord2f(1.0f,1.0f); // top right corner
	glVertex2f(traits.xpos + traits.width,traits.ypos);
	glTexCoord2f(1.0f,0.0f); // bottom right corner
	glVertex2f(traits.xpos + traits.width, traits.ypos + traits.height);
	glTexCoord2f(0.0f,0.0f); // bottom left corner
	glVertex2f(traits.xpos, traits.ypos + traits.height);
	glEnd(); //End quadrilateral coordinates
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &textID);
}

////////////
// Canvas //
////////////

/**
* 'Canvas' is a derived class from 'GraphicObject'. The 'Canvas' is the area where a user can draw polygons.
* Extensions of 'Canvas': Width and height of the window
* Additional provided functions: Function to display the canvas
*
* Default size of canvas: 600 x 375 pixel
* Default position of canvas: 127 | 144 (ortho coordinates)
* Default window size: 960 x 540 pixel
*/
Canvas* Canvas::instance = 0; // init instance
Canvas::Canvas(void) : GraphicObject(127,144,600,375){
	windowWidth = 960;
	windowHeight = 540;
}
/**
* 'getInstance' returns the 'Canvas' object.
*
* @return instance - pointer of the 'Canvas' object
*/
Canvas* Canvas::getInstance(void){
	if(instance == NULL) instance = new Canvas();
	return instance;
}
/**
* 'resetInstance' deletes the 'Canvas' object and sets the pointer to NULL, so that 'getInstance' can create a new object when it is called.
*/
void Canvas::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}
/**
* Destructor of 'Canvas'. Called when the 'Canvas' has to free memory or do other clean up when the object is destroyed.
*/
Canvas::~Canvas(void){}
/**
* 'selected' checks if the mouse was hovered over / clicked on the canvas.
*
* @param x - X-coordinate of the position of the mouse
* @param y - Y-coordinate of the position of the mouse
* @return bool - true when hovered over / clicked on canvas, false when not
*/
bool Canvas::selected(double x, double y){
	if((x >= traits.xpos) && (x <= traits.xpos + traits.width) &&
	   (y >= traits.ypos) && (y <= traits.ypos + traits.height))
	   return true;
	return false;
}
/**
* 'getWindowWidth' returns the width of the window.
*
* @return windowWidth - width of window
*/
int Canvas::getWindowWidth(void){
	return windowWidth;
}
/**
* 'getWindowHeight' returns the height of the window.
*
* @return windowHeight - height of window
*/
int Canvas::getWindowHeight(void){
	return windowHeight;
}
/**
* 'displayCanvas' renders the area around the canvas, so that polygons will always be in the canvas.
*/
void Canvas::displayCanvas(void){
	// render frame outside the paper, so that the cubic interpolation will not be drawn outside the paper
	glColor3f(0.2f, 0.2f, 0.2f);
	// upper side
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(windowWidth, 0);
	glVertex2f(windowWidth, traits.ypos);
	glVertex2f(0, traits.ypos);
	glEnd();
	// left side
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(traits.xpos, 0);
	glVertex2f(traits.xpos, windowHeight);
	glVertex2f(0, windowHeight);
	glEnd();
	// right side
	glBegin(GL_POLYGON);
	glVertex2f(traits.xpos + traits.width, 0);
	glVertex2f(windowWidth, 0);
	glVertex2f(windowWidth, windowHeight);
	glVertex2f(traits.xpos + traits.width, windowHeight);
	glEnd();
	// lower side
	glBegin(GL_POLYGON);
	glVertex2f(0, traits.ypos + traits.height);
	glVertex2f(windowWidth, traits.ypos + traits.height);
	glVertex2f(windowWidth, windowHeight);
	glVertex2f(0, windowHeight);
	glEnd();
	
	// reset colours for textures
	glColor3f(1.0f, 1.0f, 1.0f);
}

///////////////////
// CurrentColour //
///////////////////

/**
* 'CurrentColour' is a derived class from 'GraphicObject'. The 'CurrentColour' shows the user which colour he/she has just selected.
* Extensions of 'CurrentColour': Currently selected colour
* Additional provided functions: Getter and Setter and a function to render the current colour to the stage
*
* Default size of the current colour display: 68 x 68 pixel
* Default position of the current colour display: 792 | 280 (ortho coordinates)
*/
CurrentColour* CurrentColour::instance = 0;
CurrentColour::CurrentColour(void) : GraphicObject(792,280,68,68){}
/**
* 'getInstance' returns the 'CurrentColour' object
*
* @return instance - pointer of the 'CurrentColour' object
*/
CurrentColour* CurrentColour::getInstance(void){
	if(instance == NULL) instance = new CurrentColour();
	return instance;
}
/**
* 'resetInstance' deletes the 'Canvas' object and sets the pointer to NULL, so that 'getInstance' can create a new object when it is called
*/
void CurrentColour::resetInstance(void){
	delete instance;
	instance = NULL;
}
/**
* Destructor of 'CurrentColour'. Called when the 'CurrentColour' has to free memory or do other clean up when the object is destroyed.
*/
CurrentColour::~CurrentColour(void){}
/**
* 'setColour' changes the currently selected colour
*
* @param r - Red value
* @param g - Green value
* @param b - Blue value
*/
void CurrentColour::setColour(double r, double g, double b){
	colour.r = r;
	colour.g = g;
	colour.b = b;
}
/**
* 'displayCurrentColour' renders the currently selected colour to the stage.
*/
void CurrentColour::displayCurrentColour(void){
	// frame
	glColor3f(25/255.0, 24/255.0, 24/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(traits.xpos, traits.ypos);
	glVertex2f(traits.xpos + traits.width, traits.ypos);
	glVertex2f(traits.xpos + traits.width, traits.ypos + traits.height);
	glVertex2f(traits.xpos, traits.ypos + traits.height );
	glEnd();
	
	// selected colour
	glColor3f(colour.r, colour.g, colour.b);
	glBegin(GL_POLYGON);
	// +- 8 to see the frame
	glVertex2f(traits.xpos + 8, traits.ypos + 8);
	glVertex2f(traits.xpos + traits.width - 8, traits.ypos + 8);
	glVertex2f(traits.xpos + traits.width - 8, traits.ypos + traits.height - 8);
	glVertex2f(traits.xpos + 8, traits.ypos + traits.height - 8);
	glEnd();
	
	// reset colour for textures
	glColor3f(1.0f, 1.0f, 1.0f);
}

///////////
// Point //
///////////

/**
* 'Point' is a derived class from 'GraphicObject'. 'Point' objects are used as anchor points for polytopes.
* Extensions of 'Point': State if hovered over
* Additional provided functions: Getter and Setter for hover and a function to render an on hovered point to the stage
*
* Default height of a point: 1 pixel
* Default hover state: false
*
* @param xpos - X-coordinate of the upper left corner of the object
* @param ypos - Y-coordinate of the upper left corner of the object
* @param width - Width of the object
*/
Point::Point(double xpos, double ypos, double width) : GraphicObject(xpos, ypos, width, 1){
	hover = false; // for when a point shall just be displayed by hovering over it
}
/**
* Destructor of 'Point'. Called when the 'Point' has to free memory or do other clean up when the object is destroyed.
*/
Point::~Point(void){}
/**
* 'setHover' changes the on hovered state of the point.
* 
* @param onHover - true when hovered over a point, false when not
*/
void Point::setHover(bool onHover){
	hover = onHover;
}
/**
* 'onHover' returns the current on hover state of the point.
*
* @return bool - true when hovered over, false when not
*/
bool Point::onHover(void){
	return hover;
}
/**
* 'draw' renders a square on the point when its being hovered over.
*/
void Point::draw(void){
	// Point is drawn when hovered over
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	// black frame
	glColor3f(0,0,0); // drawing colour
	glPointSize(traits.width + 4);
	glBegin(GL_POINTS);
	glVertex2f(traits.xpos, traits.ypos); // set vertex coordinates
	glEnd();
	// white fill
	glColor3f(1,1,1); // drawing colour
	glPointSize(traits.width + 2);
	glBegin(GL_POINTS);
	glVertex2f(traits.xpos, traits.ypos); // set vertex coordinates
	glEnd();
	
	// reset colours for textures
	glColor3f(1.0f, 1.0f, 1.0f);
}

////////////////
// Polytope2D //
////////////////

/**
* 'Polytope2D' is an abstract class derived from 'GraphicObject'. 'Polytope2D' can either be a 'Polygon' or a 'Subdivision'.
* Extensions of 'Polytope2D': Current colour of a polygon and its subdivisions, anchor points, hardness and the drawing mode of the polytopes and a display state
* Additional provided functions: Getter and Setter for its attributes and a toggle to toggle the display of a polygon, a function to clear an anchor point and one to deselect all anchor points
*
* Default height of a polytope: 1 pixel - since the height is not needed
* Default X-coordinate of a polytope: 0 - since there is no real upper left corner
* Default Y-coordinate of a polytope: 0 - since there is no real upper left corner
* Default display state: 'false'
*
* @param r - Red value
* @param g - Green value
* @param b - Blue value
* @param h - Hardness of the lines of the polytope (enum HARD | SOFT)
* @param dm - Drawing mode of the lines of the polytope (enum LINE | LINELOOP | POLYGON)
* @param width - Width of the line of the polytope
*/

Polytope2D::Polytope2D(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width) : GraphicObject(0, 0, width, 1){
	colour.r = r;
	colour.g = g;
	colour.b = b;
	hardness = h;
	drawingMode = dm;
	// init map
	drawingModeMap[Polytope2D::LINE] = 0x0003; // OpenGL value of LINE_STRIP
	drawingModeMap[Polytope2D::LINELOOP] = 0x0002; //OpenGL value of a LINE_LOOP
	drawingModeMap[Polytope2D::POLYGON] = 0x0009; //OpenGL value of a POLYGON
	display = false;
}
/**
* Destructor of 'Polytope2D'. Called when the 'Polytope2D' has to free memory or do other clean up when the object is destroyed.
*/
Polytope2D::~Polytope2D(void){}
/**
* 'getAnchorPoints' returns a vector of the anchor points of the polygon or the subdivision.
*
* @return anchorPoints - anchor points of polygon or subdivision
*/
std::vector<Point> Polytope2D::getAnchorPoints(void){
	return anchorPoints;
}
/**
* 'getSelectedPoint' returns a Point reference when the mouse has been hovered over or clicked on the point.
*
* @param x - X-Coordinate of the mouse position
* @param y - Y-Coordinate of the mouse position
* @return &anchorPoints[i] - Address of the hovered on / clicked anchor point
*/
Point* Polytope2D::getSelectedPoint(double x, double y){
	for(unsigned int i = 0; i < anchorPoints.size(); i++)
		if((x >= anchorPoints[i].getTraits().xpos - anchorPoints[i].getTraits().width - 5) && 
		   (x <= anchorPoints[i].getTraits().xpos + anchorPoints[i].getTraits().width + 5) &&
		   (y >= anchorPoints[i].getTraits().ypos - anchorPoints[i].getTraits().height - 5) &&
		   (y <= anchorPoints[i].getTraits().ypos + anchorPoints[i].getTraits().height + 5)){
				// return point if hovered over it
				return &anchorPoints[i];
			}
	
	// return NULL if there is no anchor point
	return NULL;
}
/**
* 'getSelectedPoint' returns a Point reference when the selection state of the point is 'true'.
*
* @return &anchorPoints[i] - Address of the selected anchor point
*/
Point* Polytope2D::getSelectedPoint(void){
	for(unsigned int i = 0; i < anchorPoints.size(); i++) if(anchorPoints[i].selected()) return &anchorPoints[i];
	return NULL;
}
/**
* 'getOnHoveredPoint' returns a Point reference when the current hover state of the point is set to 'true'.
*
* @return &anchorPoints[i] - Address of the on hovered anchor point
*/
Point* Polytope2D::getOnHoveredPoint(void){
	for(unsigned int i = 0; i < anchorPoints.size(); i++) if(anchorPoints[i].onHover()) return &anchorPoints[i];
	return NULL;
}
/**
* 'displayed' returns the display state of the polygon or subdivision.
*
* @return display - display state, 'true' when it shall be displayed on the canvas, 'false' when not
*/
bool Polytope2D::displayed(void){
	return display;
}
/**
* 'toggleDisplay' toggles the current display state of a polygon or subdivision.
*/
void Polytope2D::toggleDisplay(void){
	display ? (display = false) : (display = true);
}
/**
* 'setColour' changes the current colour of the polytope
*
* @param r - Red value
* @param g - Green value
* @param b - Blue value
*/
void Polytope2D::setColour(double r, double g, double b){
	colour.r = r;
	colour.g = g;
	colour.b = b;
}
/**
* 'setAnchorPoints' overwrites the current anchor points with new ones.
*
* @param anchors - new anchor points
*/
void Polytope2D::setAnchorPoints(std::vector<Point> anchors){
	anchorPoints = anchors;
}
/**
* 'setHardness' changes the hardness of the lines of the polygon or subdivision.
*
* @param h - Hardness of the line (enum SMOOTH | HARD)
*/
void Polytope2D::setHardness(Polytope2D::Hardness h){
	hardness = h;
}
/**
* 'setDrawingMode' changes the drawing mode of the lines of the polygon or subdivision.
*
* @param dm - Drawing mode of the line (enum LINE | LINELOOP | POLYGON)
*/
void Polytope2D::setDrawingMode(Polytope2D::DrawingMode dm){
	drawingMode = dm;
}
/**
* 'deselectAllPoints' changes sets the selection state of all the polytope's anchor points to 'false'.
*/
void Polytope2D::deselectAllPoints(void){
	for(unsigned int i = 0; i < anchorPoints.size(); i++) anchorPoints[i].setSelection(false);
}
/**
* 'addAnchorPoint' adds an anchor point to the polytope's anchor points
*
* @param x - X-Coordinate of the point (read from mouse position)
* @param y - Y-Coordinate of the point (read from mouse position)
*/
void Polytope2D::addAnchorPoint(double x, double y){
	Point point(x,y, traits.width); // width is the width of the polytope
	anchorPoints.push_back(point);	// add a new vertex to polytope
	std::cout << "n=" << anchorPoints.size() << " x=" << x << " y=" << y << std::endl;
}
/**
* 'addAnchorPoint' adds an anchor point to the polytope's anchor points
*
* @param p - a point object
*/
void Polytope2D::addAnchorPoint(Point p){
	anchorPoints.push_back(p);
}
/**
* 'clearAnchorPoints' deletes the anchor points of the polytope
*/
void Polytope2D::clearAnchorPoints(void){
	anchorPoints.clear();
}

////////////
// Button //
////////////

/**
* 'Button' is class derived from 'RenderObject'. It turns a feature of the application off or on.
* Extensions of 'Button': Name of the button, activation state and for the 'colours' button a grey shade to darken or brighten its texture
* Additional provided functions: Getter and Setter for its attributes
*
* Default activation state: 'false'
* Default grey shade: 1.0f (for white)
*
* @param _name - Name of the button
* @param xpos - X-coordinate of the upper left corner of the button
* @param ypos - Y-coordinate of the upper left corner of the button
* @param width - Width of the button
* @param height - Height of the button
* @param path - Image path of the texture of the button
*/

Button::Button(std::string _name, double xpos, double ypos, double width, double height, std::string path) : RenderObject(xpos, ypos, width, height, path){
	name = _name;
	activation = false;
	grayShade = 1;
}
/**
* Destructor of 'Button'. Called when the 'Button' has to free memory or do other clean up when the object is destroyed.
*/
Button::~Button(void){}
/**
* 'getName' returns the name of the button.
*
* @return name - button name
*/
std::string Button::getName(void){
	return name;
}
/**
* 'setActivation' changes the state of the activation of the button.
*
* @param activ - activation state, 'true' when the button is turned on, 'false' when it is turned off
*/
void Button::setActivation(bool activ){
	activation = activ;
}
/**
* 'setShade' darkens or brightens the texture of the 'colours' button.
*
* @param grayScale - grey shade (from 0 to 1)
*/
void Button::setShade(double grayScale){
	grayShade = grayScale;
}
/**
* 'activated' returned the current activation state of the button.
*
* @return activation - 'true' when activated, else 'false'.
*/
bool Button::activated(void){
	return activation;
}
/**
* 'display' overrides the 'display' method from 'RenderObject'.
* It renders the button according to their current activation states and to their colours.
*/
void Button::display(void){
	glBindTexture(GL_TEXTURE_2D,texture); //Binding the texture
	glPushMatrix();
	glEnable(GL_TEXTURE_2D); //Enable texture
	glBegin(GL_POLYGON); //Begin quadrilateral coordinates 
	if(name != "colours" && activation){
		// change the texture when a button is activated, except for the colours button
		// top left corner
		glTexCoord2f(0.5f,1.0f); glVertex2f(traits.xpos,traits.ypos);
		// top right corner
		glTexCoord2f(1.0f,1.0f); glVertex2f(traits.xpos + traits.width,traits.ypos);
		// bottom right corner
		glTexCoord2f(1.0f,0.0f); glVertex2f(traits.xpos + traits.width, traits.ypos + traits.height);
		// bottom left corner
		glTexCoord2f(0.5f,0.0f); glVertex2f(traits.xpos, traits.ypos + traits.height);
	} else if(name != "colours" && !activation){
		// top left corner
		glTexCoord2f(0.0f,1.0f); glVertex2f(traits.xpos,traits.ypos);
		// top right corner
		glTexCoord2f(0.5f,1.0f); glVertex2f(traits.xpos + traits.width,traits.ypos);
		// bottom right corner
		glTexCoord2f(0.5f,0.0f); glVertex2f(traits.xpos + traits.width, traits.ypos + traits.height);
		// bottom left corner
		glTexCoord2f(0.0f,0.0f); glVertex2f(traits.xpos, traits.ypos + traits.height);
	} else {
		if(name == "colours") glColor3f(grayShade, grayShade, grayShade);	// set grey shade for the colours
		glTexCoord2f(0.0f,1.0f); // top left corner
		glVertex2f(traits.xpos,traits.ypos);
		glTexCoord2f(1.0f,1.0f); // top right corner
		glVertex2f(traits.xpos + traits.width,traits.ypos);
		glTexCoord2f(1.0f,0.0f); // bottom right corner
		glVertex2f(traits.xpos + traits.width, traits.ypos + traits.height);
		glTexCoord2f(0.0f,0.0f); // bottom left corner
		glVertex2f(traits.xpos, traits.ypos + traits.height);
	}
	glEnd(); //End quadrilateral coordinates
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &textID);
	// reset colour
	glColor3f(1.0f, 1.0f, 1.0f);
}

////////////
// Slider //
////////////

/**
* 'Slider' is a class derived from 'RenderObject'. Sliders change the states of various attributes of other objects.
* Extensions of 'Slider': Name of the slider, minimum and maximum value of the slider, current value, minimum and maximum X/Y-position, displayed value, direction of the slider and a slider bar (RenderObject)
* Additional provided functions: Getter and Setter for its attributes
*
* Default current value: minimum value
*
* @param _name - Name of the slider
* @param xpos - X-coordinate of the upper left corner of the slider
* @param ypos - Y-coordinate of the upper left corner of the slider
* @param width - Width of the slider
* @param height - Height of the slider
* @param path - Image path of the texture of the slider
* @param min - Minimum value of the slider
* @param max - Maximum value of the slider
* @param d - Direction of the slider (horizontal | vertical)
* @param bXpos - X-coordinate of the upper left corner of the slider's bar
* @param bYpos - Y-coordinate of the upper left corner of the slider's bar
* @param bWidth - Width of the slider's bar
* @param bHeight - Height of the slider's bar
* @param bPath - Image path of the texture of the slider's bar
*/
Slider::Slider(std::string _name, double xpos, double ypos, double width, double height, std::string path, double min, double max, std::string d,
			   double bXpos, double bYpos, double bWidth, double bHeight, std::string bPath) :
			   RenderObject(xpos, ypos, width, height, path), sliderBar(bXpos, bYpos, bWidth, bHeight, bPath){
					name = _name;
					minValue = min;
					maxValue = max;
					minXPos = traits.xpos;
					minYPos = traits.ypos;
					maxXPos = sliderBar.getTraits().xpos + sliderBar.getTraits().width - traits.width;
					maxYPos = sliderBar.getTraits().ypos + sliderBar.getTraits().height - traits.height;
					value = minValue;
					(d == "vertical") ? (direction = Slider::VERTICAL) : (direction = Slider::HORIZONTAL);
			   }
/**
* Destructor of 'Slider'. Called when the 'Slider' has to free memory or do other clean up when the object is destroyed.
*/
Slider::~Slider(void){}
/**
* 'getName' returns the name of the slider.
*
* @return name - slider name
*/
std::string Slider::getName(void){
	return name;
}
/**
* 'getValue' returns the current value of the slider.
*
* @return value - current value of slider 
*/
double Slider::getValue(void){
	return value;
}
/**
* 'displaySlider' renders the slider and its bar and title to the stage.
*/
void Slider::displaySlider(void){
	sliderBar.display();
	display();
}
/**
* 'setValue' changes the position of the slider according to its value.
*
* @param v - new value of the slider
*/
void Slider::setValue(double v){
	(direction == Slider::VERTICAL) ? (traits.ypos = ((v - minValue)/(maxValue - minValue)) * maxYPos + (1 - (v - minValue)/(maxValue - minValue)) * minYPos) : 
									  (traits.xpos = ((v - minValue)/(maxValue - minValue)) * maxXPos + (1 - (v - minValue)/(maxValue - minValue)) * minXPos);
}
/**
* 'setPosition' overrides the 'setPosition' method from 'GraphicObeject'.
* It adds a specific value to the slider when the position is changed.
*
* @param x - X-Coordinate of the mouse position
* @param y - Y-Coordinate of the mouse position
*/
void Slider::setPosition(double x, double y){
	double buffer;
	if(direction == Slider::VERTICAL){
		// if the slider is vertical, only change the position in Y-direction
		if(y >= maxYPos){
			traits.ypos = maxYPos;
		} else if(y <= minYPos){
			traits.ypos = minYPos;
		} else {
			traits.ypos = y - traits.height/2.0;
		}
		buffer = ((traits.ypos - minYPos)/(maxYPos - minYPos) * (maxValue - minValue)) + minValue;
	} else {
		// if the slider is horizontal, only change the position in X-direction
		if(x >= maxXPos){
			traits.xpos = maxXPos;
		} else if(x <= minXPos){
			traits.xpos = minXPos;
		} else {
			traits.xpos = x - traits.width/2.0;
		} 
		buffer = ((traits.xpos - minXPos)/(maxXPos - minXPos) * (maxValue - minValue)) + minValue;
	}
	
	if(buffer <= maxValue && buffer >= minValue){
		value = buffer;
		std::stringstream ss;
		ss << std::setprecision(2) << value;
		displayedValue = ss.str();
		ss.clear();
		std::cout << displayedValue << std::endl; // 'displayedValue' was originally intended to render dynamically a texture on the stage according to the letters of the string but due to time issues this function was left out
	}
}

/////////////////
// Subdivision //
/////////////////

/**
* 'Subdivision' is a class derived from 'Polytope2D'. Subdivisions are subdivided polygons. These are subdivided by different algorithms.
* A subdivision can be a Bezier curve, a quadratic or cubic spline or a cubic interpolation.
* Extensions of 'Subdivision': Depth (number of recursions the) and 't' (value between 0 and 1 to change the loading of the coefficients of the subdivisions)
* Additional provided functions: Getter and Setter for its attributes
*
* @param r - Red value
* @param g - Green value
* @param b - Blue value
* @param h - Hardness of the lines of the subdivision (enum HARD | SOFT)
* @param dm - Drawing mode of the lines of the subdivision (enum LINE | LINELOOP | POLYGON)
* @param width - Width of the line of the subdivision
*/
Subdivision::Subdivision(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width) : Polytope2D(r, g, b, h, dm, width){}
/**
* Destructor of 'Subdivision'. Called when the 'Subdivision' has to free memory or do other clean up when the object is destroyed.
*/
Subdivision::~Subdivision(void){}
/**
* 'getDepth' returns the number of recursions that were made while applying the algorithm.
*
* @return depth - number of recursions
*/
unsigned int Subdivision::getDepth(void){
	return depth;
}
/**
* 'getT' returns the 't' (value between 0 and 1 to change the loading of the coefficients of the subdivisions) of a subdivision.
*
* @return t - value between 0 and 1 to change the loading of the coefficients of the subdivisions
*/
double Subdivision::getT(void){
	return t;
}
/**
* 'setDepth' sets how often the algorithm for the subdivision shall be applied.
*
* @param _depth - recursions
*/
void Subdivision::setDepth(unsigned int _depth){
	depth = _depth;
}
/**
* 'setT' changes value of the coefficients.
*
* @param _t - value between 0 and 1 to change the loading of the coefficients of the subdivisions
*/
void Subdivision::setT(double _t){
	t = _t;
}

/////////////
// Polygon //
/////////////

/**
* 'Polygon' is a class derived from 'Polytope2D'. Polygons are 2-dimensional polytopes.
* Extensions of 'Polygon': a Bezier curve, a quadratic spline, a cubic spline and a cubic interpolation
* Additional provided functions: Getter and Setter for its attributes, subdivision algorithms, smoothing function to smooth the polygon, a drawing function, a function to delete one of its anchor points
*
* Default display state: 'true'
*
* @param r - Red value
* @param g - Green value
* @param b - Blue value
* @param h - Hardness of the lines of the polygon (enum HARD | SOFT)
* @param dm - Drawing mode of the lines of the polygon (enum LINE | LINELOOP | POLYGON)
* @param width - Width of the line of the polygon
*/
Polygon::Polygon(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width) :
	Polytope2D(r, g, b, h, dm, width), bezierCurve(r, g, b, h, dm, width), quadraticSpline(r, g, b, h, dm, width), cubicSpline(r, g, b, h, dm, width), cubicInterpolation(r, g, b, h, dm, width){
		display = true; // display by default true
	}
/**
* Destructor of 'Polygon'. Called when the 'Polygon' has to free memory or do other clean up when the object is destroyed.
*/
Polygon::~Polygon(void){}
/**
* 'deletePoint' deletes one of the polygons anchor points if clicked on it while the 'eraser' button is turned on.
*
* @param x - X-coordinate of the mouse position
* @param y - Y-coordinate of the mouse position
*/
void Polygon::deletePoint(double x, double y){
	for(unsigned int i = 0; i < anchorPoints.size(); i++){
		if((x >= anchorPoints[i].getTraits().xpos - anchorPoints[i].getTraits().width - 5) && 
		   (x <= anchorPoints[i].getTraits().xpos + anchorPoints[i].getTraits().width + 5) &&
		   (y >= anchorPoints[i].getTraits().ypos - anchorPoints[i].getTraits().height - 5) &&
		   (y <= anchorPoints[i].getTraits().ypos + anchorPoints[i].getTraits().height + 5)){
			// delete point clicked on it
			anchorPoints.erase(anchorPoints.begin() + i);
			break;
		}
	}
}
/**
* 'getBezierCurve' returns the Bezier subdivision of the polygon.
*
* @return &bezierCurve - reference to the address of the Bezier curve
*/
Subdivision* Polygon::getBezierCurve(void){
	return &bezierCurve;
}
/**
* 'getQuadraticSpline' returns the quadratic spline subdivision of the polygon.
*
* @return &quadraticSpline - reference to the address of the quadratic spline
*/
Subdivision* Polygon::getQuadraticSpline(void){
	return &quadraticSpline;
}
/**
* 'getCubicSpline' returns the cubic spline subdivision of the polygon.
*
* @return &cubicSpline - reference to the address of the cubic spline
*/
Subdivision* Polygon::getCubicSpline(void){
	return &cubicSpline;
}
/**
* 'getCubicInterpolation' returns the cubic interpolation subdivision of the polygon.
*
* @return &cubicInterpolation - reference to the address of the cubic interpolation
*/
Subdivision* Polygon::getCubicInterpolation(void){
	return &cubicInterpolation;
}
/**
* 'showBezierCurve' toggles the display of the Bezier curve and sets the depth and the 't' for further calculation.
*
* @param depth - number of recursions
* @param t - value to change the coefficients
*/
void Polygon::showBezierCurve(unsigned int depth, double t){
	bezierCurve.toggleDisplay();	// toggle view
	bezierCurve.setDepth(depth);
	bezierCurve.setT(t);
}
/**
* 'showQuadraticSpline' toggles the display of the quadratic spline and sets the depth and the 't' for further calculation.
*
* @param depth - number of recursions
* @param t - value to change the coefficients
*/
void Polygon::showQuadraticSpline(unsigned int depth, double t){
	quadraticSpline.toggleDisplay();	// toggle view
	quadraticSpline.setDepth(depth);
	quadraticSpline.setT(t);
}
/**
* 'showCubicSpline' toggles the display of the cubic spline and sets the depth and the 't' for further calculation.
*
* @param depth - number of recursions
* @param t - value to change the coefficients
*/
void Polygon::showCubicSpline(unsigned int depth, double t){
	cubicSpline.toggleDisplay();	// toggle view
	cubicSpline.setDepth(depth);
	cubicSpline.setT(t);
}
/**
* 'showCubicInterpolation' toggles the display of the cubic interpolation and sets the depth and the 't' for further calculation.
*
* @param depth - number of recursions
* @param t - value to change the coefficients
*/
void Polygon::showCubicInterpolation(unsigned int depth, double t){
	cubicInterpolation.toggleDisplay();	// toggle view
	cubicInterpolation.setDepth(depth);
	cubicInterpolation.setT(t);
}
/**
* 'bezierSubdivision' calculates the Bezier curve of the polygons anchor points dependent on the given depth and t.
*
* @param anchors - anchor points
* @param depth - number of recursions
* @param t - value to change the loading of the coefficients
*/
void Polygon::bezierSubdivision(std::vector<Point> anchors, unsigned int depth, double t){
	unsigned int divided = 0;
	std::vector<Point> subdividedPolygon;
	
	if(depth > 0){
		std::vector<Point> firstSubdivision; //left subdivision
		std::vector<Point> secondSubdivision; //right subdivision
		
		//push the first and the last anchor points
		firstSubdivision.push_back(anchors[0]);
		secondSubdivision.push_back(anchors[anchors.size() - 1]);
		
		std::vector<Point> tmpSubdivision1;
		tmpSubdivision1 = anchors;
		std::vector<Point> tmpSubdivision2;
		
		// Casteljau Algorithm
		while(tmpSubdivision1.size() > 1){
			for(unsigned int i = 0; i < tmpSubdivision1.size() - 1; i++){
				Point p((1 - t) * tmpSubdivision1[i].getTraits().xpos + t * tmpSubdivision1[i+1].getTraits().xpos,
						(1 - t) * tmpSubdivision1[i].getTraits().ypos + t * tmpSubdivision1[i+1].getTraits().ypos,
						tmpSubdivision1[i].getTraits().width);
				p.setSelection(false);
				tmpSubdivision2.push_back(p);
			}
			// first and last value of the algorithm are saved
			firstSubdivision.push_back(tmpSubdivision2[0]);
			secondSubdivision.push_back(tmpSubdivision2[tmpSubdivision2.size() - 1]);
			
			tmpSubdivision1.clear();
			tmpSubdivision1 = tmpSubdivision2;
			tmpSubdivision2.clear();
		}
		
		for(unsigned int i = 0; i < firstSubdivision.size(); i++) subdividedPolygon.push_back(firstSubdivision[i]);
		for(int i = secondSubdivision.size() - 1; i > -1 ; i--) subdividedPolygon.push_back(secondSubdivision[i]);
		
		bezierSubdivision(subdividedPolygon, depth - 1, t);
		divided++;
	}
	
	if(divided == depth) bezierCurve.setAnchorPoints(subdividedPolygon);
}
/**
* 'quadraticSplineSubdivision' calculates the quadratic spline of the polygons anchor points dependent on the given depth and t.
*
* @param anchors - anchor points
* @param depth - number of recursions
* @param t - value to change the loading of the coefficients
*/
void Polygon::quadraticSplineSubdivision(std::vector<Point> anchors, unsigned int depth, double t){
	std::vector<Point> subdividedPolygon;
	unsigned int divided = 0;
	
	if(depth > 0){
		// push first point
		subdividedPolygon.push_back(anchors[0]);
		
		// calculation for second point
		Point p((anchors[0].getTraits().xpos + anchors[1].getTraits().xpos)/2,
				(anchors[0].getTraits().ypos + anchors[1].getTraits().ypos)/2,
				 anchors[0].getTraits().width);
		subdividedPolygon.push_back(p);
		
		// calculation for 3rd to (n - 2)th point
		for(unsigned int i = 1; i < anchors.size() - 2; i++){
			Point p1((1 - t) * anchors[i].getTraits().xpos + t * anchors[i+1].getTraits().xpos,
					 (1 - t) * anchors[i].getTraits().ypos + t * anchors[i+1].getTraits().ypos,
					 anchors[i].getTraits().width);
			subdividedPolygon.push_back(p1);
			Point p2((1 - t) * anchors[i+1].getTraits().xpos + t * anchors[i].getTraits().xpos,
					 (1 - t) * anchors[i+1].getTraits().ypos + t * anchors[i].getTraits().ypos,
					 anchors[i+1].getTraits().width);
			subdividedPolygon.push_back(p2);
		}
		
		// calculation for (n - 1)th point
		Point p3((anchors[anchors.size()-2].getTraits().xpos + anchors[anchors.size()-1].getTraits().xpos)/2,
				 (anchors[anchors.size()-2].getTraits().ypos + anchors[anchors.size()-1].getTraits().ypos)/2,
				  anchors[anchors.size()-2].getTraits().width);
		subdividedPolygon.push_back(p3);
		
		// push last point
		subdividedPolygon.push_back(anchors[anchors.size()-1]);
		
		quadraticSplineSubdivision(subdividedPolygon, depth - 1, t);
		divided++;
	}
	
	if(divided == depth) quadraticSpline.setAnchorPoints(subdividedPolygon);
}
/**
* 'cubicSplineSubdivision' calculates the cubic spline of the polygons anchor points dependent on the given depth and t.
*
* @param anchors - anchor points
* @param depth - number of recursions
* @param t - value to change the loading of the coefficients
*/
void Polygon::cubicSplineSubdivision(std::vector<Point> anchors, unsigned int depth, double t){
	std::vector<Point> subdividedPolygon;
	unsigned int divided = 0;
	
	if(depth > 0){
		// push first anchor point
		subdividedPolygon.push_back(anchors[0]);
		
		// calculation for 2nd to (n-2)th anchor points
		for(unsigned int i = 0; i < anchors.size() - 2; i++){
			Point p((anchors[i].getTraits().xpos + anchors[i+1].getTraits().xpos)/2,
					(anchors[i].getTraits().ypos + anchors[i+1].getTraits().ypos)/2,
					 anchors[i].getTraits().width);
			subdividedPolygon.push_back(p);
			Point p1((t/2) * anchors[i].getTraits().xpos + (1 - t) * anchors[i+1].getTraits().xpos + (t/2) * anchors[i+2].getTraits().xpos,
					 (t/2) * anchors[i].getTraits().ypos + (1 - t) * anchors[i+1].getTraits().ypos + (t/2) * anchors[i+2].getTraits().ypos,
					 anchors[i].getTraits().width);
			subdividedPolygon.push_back(p1);
		}
		
		// calculation for (n-1)th anchor point
		Point p2((anchors[anchors.size() - 2].getTraits().xpos + anchors[anchors.size() - 1].getTraits().xpos)/2, 
				 (anchors[anchors.size() - 2].getTraits().ypos + anchors[anchors.size() - 1].getTraits().ypos)/2,
				  anchors[anchors.size() - 2].getTraits().width);
		subdividedPolygon.push_back(p2);
		
		// push last anchor point
		subdividedPolygon.push_back(anchors[anchors.size() - 1]);
		
		cubicSplineSubdivision(subdividedPolygon, depth - 1, t);
		divided++;
	}
	
	if(divided == depth) cubicSpline.setAnchorPoints(subdividedPolygon);
}
/**
* 'cubicInterpolationSubdivision' calculates the cubic interpolation of the polygons anchor points dependent on the given depth and t.
*
* @param anchors - anchor points
* @param depth - number of recursions
* @param t - value to change the loading of the coefficients
*/
void Polygon::cubicInterpolationSubdivision(std::vector<Point> anchors, unsigned int depth, double t){
	std::vector<Point> subdividedPolygon;
	unsigned int divided = 0;
	
	if(depth > 0){
		//push first point
		subdividedPolygon.push_back(anchors[0]);
		
		//calculation of the point between the first and the third
		Point p((3*(t/2.0)) * anchors[0].getTraits().xpos + (1 - t) * anchors[1].getTraits().xpos + (-1*(t/2.0)) * anchors[2].getTraits().xpos,
				(3*(t/2.0)) * anchors[0].getTraits().ypos + (1 - t) * anchors[1].getTraits().ypos + (-1*(t/2.0)) * anchors[2].getTraits().ypos,
				 anchors[1].getTraits().width);
		subdividedPolygon.push_back(p);
		
		//calculation of the 2nd to (n - 3)th anchor point
		for(unsigned int i = 1; i < anchors.size() - 2; i++){
			//always push back the current point and then calculate the point between three neighboured points
			subdividedPolygon.push_back(anchors[i]);
			
			Point p1((-1*(t/4.0)) * anchors[i-1].getTraits().xpos + (0.5+(t/4.0)) * anchors[i].getTraits().xpos + (0.5+(t/4.0)) * anchors[i+1].getTraits().xpos + (-1*(t/4.0)) * anchors[i+2].getTraits().xpos,
					 (-1*(t/4.0)) * anchors[i-1].getTraits().ypos + (0.5+(t/4.0)) * anchors[i].getTraits().ypos + (0.5+(t/4.0)) * anchors[i+1].getTraits().ypos + (-1*(t/4.0)) * anchors[i+2].getTraits().ypos,
					 anchors[i].getTraits().width);
			subdividedPolygon.push_back(p1);
		}
		
		subdividedPolygon.push_back(anchors[anchors.size() - 2]);
		
		Point p2((3*(t/2.0)) * anchors[anchors.size() - 1].getTraits().xpos + (1 - t) * anchors[anchors.size() - 2].getTraits().xpos + (-1*(t/2.0)) * anchors[anchors.size() - 3].getTraits().xpos,
				 (3*(t/2.0)) * anchors[anchors.size() - 1].getTraits().ypos + (1 - t) * anchors[anchors.size() - 2].getTraits().ypos + (-1*(t/2.0)) * anchors[anchors.size() - 3].getTraits().ypos,
				 anchors[anchors.size() - 2].getTraits().width);
		subdividedPolygon.push_back(p2);
		
		subdividedPolygon.push_back(anchors[anchors.size() - 1]);
		
		cubicInterpolationSubdivision(subdividedPolygon, depth - 1, t);
		divided++;
	}
	
	if(divided == depth) cubicInterpolation.setAnchorPoints(subdividedPolygon);
}
/**
* 'smooth' changes the position of the anchor points pulling a point between two anchor points closer to them, so the polygon will appear smoother.
*
* @param t - value to change the loading of the coefficients
*/
void Polygon::smooth(double t){
	for(unsigned int i = 1; i < anchorPoints.size() - 1; i++){
		anchorPoints[i].setPosition(anchorPoints[i-1].getTraits().xpos * ((1-t)/2.0) + anchorPoints[i].getTraits().xpos * t + anchorPoints[i+1].getTraits().xpos * ((1-t)/2.0),
									anchorPoints[i-1].getTraits().ypos * ((1-t)/2.0) + anchorPoints[i].getTraits().ypos * t + anchorPoints[i+1].getTraits().ypos * ((1-t)/2.0));
	}
}
/**
* 'draw' draws the polygon and its subdivisions to the stage when their 'display' state is on. The polytopes are drawn by their given features such as
* the soft- or hardness of the line, the colour, the line width or the drawing mode of the polytopes.
*/
void Polygon::draw(void){
	// calculate the subdivisions if checked
	std::vector<Point> bezierAnchors;
	std::vector<Point> quadraticSplineAnchors;
	std::vector<Point> cubicSplineAnchors;
	std::vector<Point> cubicInterpolationAnchors;
	if(bezierCurve.displayed()){
		bezierCurve.setAnchorPoints(anchorPoints);
		bezierSubdivision(bezierCurve.getAnchorPoints(), bezierCurve.getDepth(), bezierCurve.getT());
		bezierAnchors = bezierCurve.getAnchorPoints();
	}
	if(quadraticSpline.displayed()){
		quadraticSpline.setAnchorPoints(anchorPoints);
		quadraticSplineSubdivision(quadraticSpline.getAnchorPoints(), quadraticSpline.getDepth(), quadraticSpline.getT());
		quadraticSplineAnchors = quadraticSpline.getAnchorPoints();
	}
	if(cubicSpline.displayed()){
		cubicSpline.setAnchorPoints(anchorPoints);
		cubicSplineSubdivision(cubicSpline.getAnchorPoints(), cubicSpline.getDepth(), cubicSpline.getT());
		cubicSplineAnchors = cubicSpline.getAnchorPoints();
	}
	if(cubicInterpolation.displayed()){
		cubicInterpolation.setAnchorPoints(anchorPoints);
		cubicInterpolationSubdivision(cubicInterpolation.getAnchorPoints(), cubicInterpolation.getDepth(), cubicInterpolation.getT());
		cubicInterpolationAnchors = cubicInterpolation.getAnchorPoints();
	}
	glColor3f(colour.r, colour.g, colour.b); // drawing colour
	glPointSize(traits.width);
	glLineWidth(traits.width);
	if(hardness == Polytope2D::SMOOTH){
		// enable smoothness
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		
		/// to get a smooth transition between two lines draw additional points between them
		
		if(display){ // polygon
			glBegin(GL_POINTS);
			for(unsigned int i = 0; i < anchorPoints.size(); i++){
				glVertex2f(anchorPoints[i].getTraits().xpos, anchorPoints[i].getTraits().ypos); // set vertex coordinates
			}
			glEnd();  // end of GL_POINTS
		}
		
		if(bezierCurve.displayed()){ // bezier
			glBegin(GL_POINTS);
			for(unsigned int i = 0; i < bezierAnchors.size(); i++){
				glVertex2f(bezierAnchors[i].getTraits().xpos, bezierAnchors[i].getTraits().ypos); // set vertex coordinates
			}
			glEnd();  // end of GL_POINTS
		}
		
		if(quadraticSpline.displayed()){ // quadratic spline
			glBegin(GL_POINTS);
			for(unsigned int i = 0; i < quadraticSplineAnchors.size(); i++){
				glVertex2f(quadraticSplineAnchors[i].getTraits().xpos, quadraticSplineAnchors[i].getTraits().ypos); // set vertex coordinates
			}
			glEnd();  // end of GL_POINTS
		}
		
		if(cubicSpline.displayed()){ // cubic spline
			glBegin(GL_POINTS);
			for(unsigned int i = 0; i < cubicSplineAnchors.size(); i++){
				glVertex2f(cubicSplineAnchors[i].getTraits().xpos, cubicSplineAnchors[i].getTraits().ypos); // set vertex coordinates
			}
			glEnd();  // end of GL_POINTS
		}
		
		if(cubicInterpolation.displayed()){ // cubic interpolation
			glBegin(GL_POINTS);
			for(unsigned int i = 0; i < cubicInterpolationAnchors.size(); i++){
				glVertex2f(cubicInterpolationAnchors[i].getTraits().xpos, cubicInterpolationAnchors[i].getTraits().ypos); // set vertex coordinates
			}
			glEnd();  // end of GL_POINTS
		}
		
	} else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
	// draw polygon if display = true
	if(display){
		glBegin(drawingModeMap[drawingMode]);
		for(unsigned int i = 0; i < anchorPoints.size(); i++){
			glVertex2f(anchorPoints[i].getTraits().xpos, anchorPoints[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	// eventually the subdivisions
	if(bezierCurve.displayed()){ // bezier
		glBegin(drawingModeMap[drawingMode]);
		for(unsigned int i = 0; i < bezierAnchors.size(); i++){
			glVertex2f(bezierAnchors[i].getTraits().xpos, bezierAnchors[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	if(quadraticSpline.displayed()){ // quadratic spline
		glBegin(drawingModeMap[drawingMode]);
		for(unsigned int i = 0; i < quadraticSplineAnchors.size(); i++){
			glVertex2f(quadraticSplineAnchors[i].getTraits().xpos, quadraticSplineAnchors[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	if(cubicSpline.displayed()){ // cubic spline
		glBegin(drawingModeMap[drawingMode]);
		for(unsigned int i = 0; i < cubicSplineAnchors.size(); i++){
			glVertex2f(cubicSplineAnchors[i].getTraits().xpos, cubicSplineAnchors[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	if(cubicInterpolation.displayed()){ // cubic interpolation
		glBegin(drawingModeMap[drawingMode]);
		for(unsigned int i = 0; i < cubicInterpolationAnchors.size(); i++){
			glVertex2f(cubicInterpolationAnchors[i].getTraits().xpos, cubicInterpolationAnchors[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	// if the polygon is selected, draw additionally squares on the anchor points (no matter if display is true or not)
	if(selection){
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		// black frame
		glColor3f(0,0,0); // drawing colour
		glPointSize(traits.width + 4);
		glBegin(GL_POINTS);
		for(unsigned int i = 0; i < anchorPoints.size(); i++){
			glVertex2f(anchorPoints[i].getTraits().xpos, anchorPoints[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
		// white fill
		glColor3f(1,1,1); // drawing colour
		glPointSize(traits.width + 2);
		glBegin(GL_POINTS);
		for(unsigned int i = 0; i < anchorPoints.size(); i++){
			glVertex2f(anchorPoints[i].getTraits().xpos, anchorPoints[i].getTraits().ypos); // set vertex coordinates
		}
		glEnd();
	}
	
	// draw on hovered points
	if(getOnHoveredPoint() != NULL) getOnHoveredPoint()->draw();
	
	// reset colours for textures
	glColor3f(1.0f, 1.0f, 1.0f);
}