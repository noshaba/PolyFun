/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

/**
* 'GraphicObjects' are the objects that are rendered to the stage. Each 'GraphicObject' contains 'ObjTraits' - a struct with the basic
* information about 'GraphicObjects', such as the position and size of the object.
* All other classes are derived from 'GraphicObjects'.
* 'Button' and 'Slider' are additionally derived from 'RenderObject'. 'RenderObject' has the extension of an image path for textures.
* 'Polygon' and 'Subdivision' are additionally derived from 'Polytope2D'.
* Further information about the functions of the classes is in the .cpp file.
*/

#ifndef GRAPHICOBJECTS_HPP
	#define GRAPHICOBJECTS_HPP
	
	#include <vector>
	#include <stdlib.h>
	#include <string>
	#include <map>
	#include <GL/gl.h>		// OpenGL
	#include <GL/glfw.h>    // GL-Framework
	
	#include "Exceptions.hpp"
	
	/**
	* 'ObjTraits' contains the basic information about an object rendered to the stage. 
	*/
	
	struct ObjTraits{
		double xpos, ypos, width, height;
	};
	
	////////////////
	// Base Class //
	////////////////
	
	/**
	* 'GraphicObject' is an object rendered to the stage. 'GraphicObject' is an abstract class.
	* - Provided functions: Getter and Setter to change the objects attributes
	*
	* @param xpos - X-coordinate of the upper left corner of the object
	* @param ypos - Y-coordinate of the upper left corner of the object
	* @param width - width of the object
	* @param height - height of the object
	*/
	
	class GraphicObject{
		protected:
			ObjTraits traits; // Position and size information
			bool selection; // Selection state
		public:
			GraphicObject(double xpos, double ypos, double width, double height); // Constructor
			virtual ~GraphicObject(void); // Destructor
			virtual ObjTraits getTraits(void); // Returns the traits (X/Y-position, height and width) of the object
			virtual bool selected(void); // Returns is the object is selected or not
			virtual void setPosition(double xpos, double ypos); // Sets the position of the object, according to the coordinates of the upper left corner
			virtual void setSelection(bool selected); // Changes the selection state of the object
			virtual void setWidth(double width); // Changes the width of the object
			virtual void setHeight(double height); // Changes the height of the object
			virtual void setSize(double width, double height); // Changes the width and height of the object
			virtual void setTraits(double xpos, double ypos, double width, double height); // Changes the position and size of the object
	};
	
	/////////////////////
	// Derived Classes //
	/////////////////////
	
	/**
	* 'RenderObject' is a derived class from 'GraphicObject'. It provides an image path as an extension to draw objects with textures.
	* Extensions of 'RenderObject': Image path
	* Additional provided functions: Functions to load and display a texture
	*
	* @param xpos - X-coordinate of the upper left corner of the object
	* @param ypos - Y-coordinate of the upper left corner of the object
	* @param width - width of the object
	* @param height - height of the object
	* @param path - image path of the texture of the object
	*/
	
	class RenderObject : public GraphicObject{
		protected:
			std::string imgPath; // Image path of the texture
			GLuint texture;
		public:
			RenderObject(double xpos, double ypos, double width, double height, std::string path); // Constructor
			~RenderObject(void); // Destructor
			std::string getImgPath(void); // Returns the image path of the texture
			void loadTexture(std::string imgPath); // Loads the texture and adds a texture ID to it which is being returned
			void display(void); // default render option to display the texture
	};
	
	/**
	* 'Canvas' is a derived class from 'GraphicObject'. The 'Canvas' is the area where a user can draw polygons.
	* Extensions of 'Canvas': Width and height of the window
	* Additional provided functions: Function to display the canvas
	*
	* Default size of canvas: 600 x 375 pixel
	* Default position of canvas: 127 | 144 (ortho coordinates)
	* Default window size: 960 x 540 pixel
	*/
	
	class Canvas : public GraphicObject{
		private:
			int windowWidth, windowHeight; // Size of window
			static Canvas* instance; // Instance of canvas
			Canvas(void); // Constructor
		public:
			static Canvas* getInstance(void); // Returns the 'Canvas' object
			static void resetInstance(void); // Deletes the 'Canvas' object
			~Canvas(void); // Destructor
			bool selected(double x, double y); // Checks if the mouse was hovered over / clicked on the canvas
			int getWindowWidth(void); // Returns the window width
			int getWindowHeight(void); // Returns the window height
			void displayCanvas(void); // Renders the canvas to the stage
	};
	
	/**
	* 'CurrentColour' is a derived class from 'GraphicObject'. The 'CurrentColour' shows the user which colour he/she has just selected.
	* Extensions of 'CurrentColour': Currently selected colour
	* Additional provided functions: Getter and Setter and a function to render the current colour to the stage
	*
	* Default size of the current colour display: 68 x 68 pixel
	* Default position of the current colour display: 792 | 280 (ortho coordinates)
	*/
	
	class CurrentColour : public GraphicObject{
		private:
			static CurrentColour* instance; // Instance of the currently selected colour
			CurrentColour(void); // Constructor
			struct RGB{
				double r,g,b;
			} colour; // currently selected colour
		public:
			static CurrentColour* getInstance(void); // Returns the 'CurrentColour' object
			static void resetInstance(void); // Deletes the 'CurrentColour' object
			~CurrentColour(void); // Destructor
			void setColour(double r, double g, double b); // Changes the currently selected colour
			void displayCurrentColour(void); // renders the 'CurrentColour' display to the stage
	};
	
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
	
	class Point : public GraphicObject{
		private:
			bool hover; // Current on hover state
		public:
			Point(double xpos, double ypos, double width); // Constructor
			~Point(void); // Destructor
			void draw(void); // Renders a square on the point when it is being hovered over
			void setHover(bool onHover); // Changes the current on hover state of the point
			bool onHover(void); // Returns if the point is being hovered over or not
	};
	
	/**
	* 'Polytope2D' is an abstract class derived from 'GraphicObject'. 'Polytope2D' can either be a 'Polygon' or a 'Subdivision'.
	* Extensions of 'Polytope2D': Current colour of a polygon and its subdivisions, anchor points, hardness and the drawing mode of the polytopes and a display state
	* Additional provided functions: Getter and Setter for its attributes and a toggle to toggle the display of a polygon, a function to clear an anchor point and one to deselect all anchor points
	*
	* Default height of a polytope: 1 pixel - since the height is not needed
	* Default X-coordinate of a polytope: 0 - since there is no real upper left corner
	* Default Y-coordinate of a polytope: 0 - since there is no real upper left corner
	*
	* @param r - Red value
	* @param g - Green value
	* @param b - Blue value
	* @param h - Hardness of the lines of the polytope (enum HARD | SOFT)
	* @param dm - Drawing mode of the lines of the polytope (enum LINE | LINELOOP | POLYGON)
	* @param width - Width of the line of the polytope
	*/
	
	class Polytope2D : public GraphicObject{
		protected:
			struct RGB{
				double r,g,b;
			} colour; // Current colour of the polytope
			std::vector<Point> anchorPoints; // Anchor points of the polytope
			bool display; // Display state of the polytope, if it shall be displayed on the canvas or not
		public:
			enum Hardness{HARD, SMOOTH} hardness; // Hardness of the lines of the polytope
			enum DrawingMode{LINE, LINELOOP,POLYGON} drawingMode; // Drawing mode of the lines of the polytope
			std::map<Polytope2D::DrawingMode, double> drawingModeMap; // Map with the OpenGL values for the drawing modes
			Polytope2D(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width); // Constructor
			virtual ~Polytope2D(void); // Destructor
			virtual std::vector<Point> getAnchorPoints(void); // Returns the anchor points of the polytope
			virtual Point* getSelectedPoint(double x, double y); // Returns a reference to an anchor point if hovered over / clicked on one
			virtual Point* getSelectedPoint(void); // Returns a selected anchor point
			virtual Point* getOnHoveredPoint(void); // Returns an anchor point when its on hover flag is set to 'true'
			virtual bool displayed(void); // Returns if the polytope shall be displayed or not, 'true' when it shall be, 'false' when not
			virtual void toggleDisplay(void); // Toggles the display mode of the polytope
			virtual void setColour(double r, double g, double b); // Changes the colour of the polytope
			virtual void setHardness(Polytope2D::Hardness h); // Changes the hardness of the lines of the polytope
			virtual void setDrawingMode(Polytope2D::DrawingMode dm); // Changes the drawing mode of the lines of the polytope
			virtual void deselectAllPoints(void); // Sets the selection state of all anchor points to 'false'
			virtual void addAnchorPoint(double x, double y); // Adds an anchor point to the anchor points
			virtual void addAnchorPoint(Point p); // Adds an anchor point to the anchor points
			virtual void clearAnchorPoints(void); // Deletes the anchor points
			virtual void setAnchorPoints(std::vector<Point> anchors); // Overwrites the anchor points
	};
	
	///////////////////////////////
	// Derived From RenderObject //
	///////////////////////////////
	
	/**
	* 'Button' is a class derived from 'RenderObject'. It turns a feature of the application off or on.
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
	
	class Button : public RenderObject{
		private:
			std::string name; // Name of the button
			bool activation; // Activation state of the button
			double grayShade; // Grey shade of the 'colours' button to darken/brighten its colours
		public:
			Button(std::string _name, double xpos, double ypos, double width, double height, std::string path); //Constructor
			~Button(void); //Destructor
			std::string getName(void); //Returns the name of a button
			void setActivation(bool activ); //Changes the activation state of a button
			bool activated(void); //Returns the current activation state of a button
			void display(void);	// Overrides the 'display' method from 'RenderObject', renders the buttons according to their current state
			void setShade(double grayScale); //Changes the grey shade of the 'colours' button
	};
	
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
	
	class Slider : public RenderObject{
		private:
			std::string name; //Name of slider
			double minValue; //Minimum value of slider
			double maxValue; //Maximum value of slider
			RenderObject sliderBar; //Slider bar
			double minXPos; //Minimum X-Coordinate of the slider's position
			double minYPos; //Minimum Y-Coordinate of the slider's position
			double maxXPos; //Maximum X-Coordinate of the slider's position
			double maxYPos; //Maximum Y-Coordinate of the slider's position
			double value; //Current value of the slider
			std::string displayedValue; //Displayed value of the slider (*was originally intended to render dynamically the current value according to the string on the stage but due to time issues this was not implemented anymore*)
			enum Direction{VERTICAL, HORIZONTAL} direction; //Direction of the slider (enum VERTICAL | HORIZONTAL)
		public:
			Slider(std::string _name, double xpos, double ypos, double width, double height, std::string path, double min, double max, std::string dir,
				   double bXpos, double bYpos, double bWidth, double bHeight, std::string bPath); //Constructor
			~Slider(void); //Destructor
			std::string getName(void); //Returns the name of the slider
			double getValue(void); //Returns the current value of the slider
			void displaySlider(void); //Renders the slider and its title and bar to the stage
			void setPosition(double x, double y); //Overrides the 'setPosition' method from 'GraphicObject' to calculate a specific value and to make sure that the slider is not moved outside of its position range
			void setValue(double v); //Changes the position of the slider according to its value
	};
	
	/////////////////////////////
	// Derived From Polytope2D //
	/////////////////////////////
	
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
	
	class Subdivision : public Polytope2D{
		private:
			unsigned int depth; //Number of recursions
			double t; //Value between 0 and 1 to change the loading of the coefficients of the subdivisions
		public:
			Subdivision(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width); //Constructor
			~Subdivision(void); //Destructor
			unsigned int getDepth(void); //Returns the number of recursions
			double getT(void); //Returns the value that changed the loading of the coefficients
			void setDepth(unsigned int _depth); //Changes the number of recursions
			void setT(double _t); //Changes the value to change the coefficients
	};
	
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
	
	class Polygon : public Polytope2D{
		private:
			Subdivision bezierCurve; //Bezier subdivision
			Subdivision quadraticSpline; //Quadratic spline subdivision
			Subdivision cubicSpline; //Cubis spline subdivision
			Subdivision cubicInterpolation; //Cubis interpolation subdivision
		public:
			Polygon(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width); //Constructor
			~Polygon(void); //Destructor
			Subdivision* getBezierCurve(void); //Returns the Bezier curve of the polygon
			Subdivision* getQuadraticSpline(void); //Returns the quadratic spline of the polygon
			Subdivision* getCubicSpline(void); //Return the cubic spline of the polygon
			Subdivision* getCubicInterpolation(void); //Returns the cubic interpolation of the polygon
			void deletePoint(double x, double y); //Deletes an anchor point if clicked on it while the 'eraser' button is turned on
			void showBezierCurve(unsigned int depth, double t); //Toggles the display state of the Bezier curve and sets the depth and 't' for further calculation
			void showQuadraticSpline(unsigned int depth, double t); //Toggles the display state of the quadratic spline and sets the depth and 't' for further calculation
			void showCubicSpline(unsigned int depth, double t); //Toggles the display state of the cubic spline and sets the depth and 't' for further calculation
			void showCubicInterpolation(unsigned int depth, double t); //Toggles the display state of the cubic interpolation and sets the depth and 't' for further calculation
			void bezierSubdivision(std::vector<Point> anchors, unsigned int depth, double t); //Calculates the Bezier curve
			void quadraticSplineSubdivision(std::vector<Point> anchors, unsigned int depth, double t); //Calculates the quadratic spline
			void cubicSplineSubdivision(std::vector<Point> anchors, unsigned int depth, double t); //Calculates the cubic spline
			void cubicInterpolationSubdivision(std::vector<Point> anchors, unsigned int depth, double t); //Calculates the cubic interpolation
			void smooth(double t); //Pulls the anchor points of the polygon together to make it appear smoother
			void draw(void); //Draws the polygon and its subdivisions
	};
#endif