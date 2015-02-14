/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

/**
* The 'Controller' delegates all the tasks that the user enters to the specific models and tells the 'View' when to update the window for the user.
*/

#ifndef CONTROLLER_HPP
	#define CONTROLLER_HPP
	
	#include "Model.hpp"
	#include "View.hpp"
	
	class Controller{
		private:
			View* window; //Instance of the 'View'
			ButtonModel* buttonM; //Instance of the 'ButtonModel'
			PolygonModel* polygonM; //Instance of the 'PolygonModel'
			SliderModel* sliderM; //Instance of the 'SliderModel'
			Canvas* canvas; //Instance of the 'Canvas'
			CurrentColour* currentColour; //Instance of the 'CurrentColour'
			static Controller* instance; //Instance of the 'Controller'
			Controller(void); //Constructor
		public:
			struct{
				double r, g, b;
			}colour; //Currently selected colour
			Polygon::Hardness hardness; //Currently selected hardness of the polygon lines
			Polygon::DrawingMode drawingMode; //Currently selected drawing mode
			double polygonWidth; //Currently set line width
			double grayShade; //Currently selected value to darken or brighten the colours
			double bezierT, quadraticSplineT, cubicSplineT, cubicInterpolationT, smoothPolygonT; //Currently selected 't's
			double bezierDepth, quadraticSplineDepth, cubicSplineDepth, cubicInterpolationDepth; //Currently selected numbers of recursions
			static Controller* getInstance(void); //Returns the pointer to the controller
			static void resetInstance(void); //Deletes the pointer of the controller
			~Controller(void); //Destructor
			int getWindowHeight(); //Returns the height of the window
			int getWindowWidth(); //Returns the width of the window
			Button* getButton(std::string name); //Returns a button reference by its given name
			Button* getButton(double x, double y); //Returns a button reference when clicked on it
			Slider* getSlider(std::string name); //Returns a slider reference by its give name
			Slider* getSlider(double x, double y); //Returns a slider reference clicked or hovered over it
			Slider* getSelectedSlider(void); //Returns a slider reference when its selection state is set to 'true'
			Polygon* getSelectedPolygon(void); //Returns a polygon reference when its selection state is set to 'true'
			Polygon* getPolygon(double x, double y); //Returns a polygon reference when clicked or hovered over it
			Point* getSelectedPoint(double x, double y); //Returns a point reference when clicked or hovered over the point
			Point* getSelectedPoint(void); //Returns a point reference when its selection state is set to 'true'
			Point* getOnHoveredPoint(void); //Returns a point reference when its on hover state is set to 'true'
			bool getCanvasSelection(double x, double y); //Checks if the canvas has been hovered over or clicked on
			void updateWindow(void); //Tells the 'View' to update the window
			void setCurrentColour(double r, double g, double b); //Changes the currently selected colour
			void setColourShade(double grayScale); //Changes the currently selected grey shade to darken or brighten the colours
			void setPolygonWidth(double width); //Changes the currently set line width
			void setBezierT(double t); //Changes the currently set 't' of the Bezier algorithm
			void setQuadraticSplineT(double t); //Changes the currently set 't' of the quadratic spline algorithm
			void setCubicSplineT(double t); //Changes the currently set 't' of the cubic spline algorithm
			void setCubicInterpolationT(double t); //Changes the currently set 't' of the cubic interpolation algorithm
			void setSmoothPolygonT(double t); //Changes the currently set 't' of the smoothing algorithm
			void setBezierDepth(double depth); //Changes the number of recursions of the Bezier algorithm
			void setQuadraticSplineDepth(double depth); //Changes the number of recursions of the quadratic spline algorithm
			void setCubicSplineDepth(double depth); //Changes the number of recursions of the cubic spline algorithm
			void setCubicInterpolationDepth(double depth); //Changes the number of recursions of the cubic interpolation algorithm
			void setPointPosition(double x, double y); //Changes a points position
			void setSliderPosition(double x, double y); //Changes a sliders position
			void setHardness(Polytope2D::Hardness h); //Changes the currently selected hardness of the line
			void setDrawingMode(Polytope2D::DrawingMode dm); //Changes the currently selected drawing mode
			void newPolygon(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width); //Creates a new polygon
			void deselectAllPoints(void); //Deselects all points
			void deselectPolygon(void); //Deselects a selected polygon
			void deselectAllPolygons(void); //Deselects all polygons
			void turnOffAllButtons(void); //Turns all the buttons off
			void dragAnchorPoint(double x, double y); //Drags an anchor point
			void dragSlider(double x, double y); //Drags a slider
			void releaseDraggedAnchorPoint(void); //Releases a dragged anchor point
			void releaseSlider(void); //Releases a dragged slider
			void addAnchorPoint(double x, double y); //Adds an anchor to the selected polygon
			void selectPolygon(double x, double y); //Selects a polygon when clicked on it
			void bezierSubdivision(unsigned int depth, double t); //Calculates the Bezier curve
			void quadraticSplineSubdivision(unsigned int depth, double t); //Calculates the quadratic spline
			void cubicSplineSubdivision(unsigned int depth, double t); //Calculates the cubic spline
			void cubicInterpolation(unsigned int depth, double t); //Calculates the cubic interpoaltion
			void deleteSelectedPolygon(void); //Deletes a selected polygon
			void deletePoint(double x, double y); //Deletes a point when clicked on one
	};
#endif