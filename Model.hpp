/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

/**
* The models have all the objects created on the stage saved in a list. This list is used to get a specific object from it and to change one or more attributes
* of it or to change the attributes of all the objects in the list. The model also creates these objects. Functions like drawing objects are delegated to the
* specific objects.
*/

#ifndef MODEL_HPP
	#define MODEL_HPP
	
	#include "GraphicObjects.hpp"
	
	/////////////////
	// ButtonModel //
	/////////////////
	
	/**
	* The 'ButtonModel' manages all the buttons in the application.
	*/
	
	class ButtonModel{
		private:
			std::string database; //Database to read the button attributes from
			std::vector<Button> buttonList; //Vector with all the created buttons
			static ButtonModel* instance; //Instance of the model
			ButtonModel(void); //Constructor
		public:
			static ButtonModel* getInstance(void); //Returns the pointer of the model
			static void resetInstance(void); //Deletes the instance
			~ButtonModel(void); //Destructor
			void loadButtons(void); //Reads the button data from a file and creates all buttons
			Button* getButton(double x, double y); //Returns a button reference when clicked on
			Button* getButton(std::string name); //Returns a button reference by its given name
			std::vector<Button> getButtonList(void); //Returns the list with all the buttons
			void turnOffAllButtons(void); //Turns every button the button list off
			void displayButtons(void); //Displays all the buttons
	};
	
	/////////////////
	// SliderModel //
	/////////////////
	
	/**
	* The 'SliderModel' manages all the sliders in the application.
	*/
	
	class SliderModel{
		private:
			std::string database; //Database to read the slider attributes from
			std::vector<Slider> sliderList; //Vector with all the created sliders
			static SliderModel* instance; //Instance of the model
			SliderModel(void); //Constructor
		public:
			static SliderModel* getInstance(void); //Returns the pointer of the model
			static void resetInstance(void); //Deletes the instance
			~SliderModel(void); //Destructor
			void loadSliders(void); //Reads the slider data from a file and creates all sliders
			Slider* getSlider(double x, double y); //Returns a slider reference when clicked on
			Slider* getSlider(std::string name); //Returns a slider reference by its given name
			Slider* getSelectedSlider(void); //Returns a slider reference when its selection state is 'true'
			std::vector<Slider> getSliderList(void); //Returns the list with all the sliders
			void displaySliders(void); //Displays all the sliders
	};
	
	
	//////////////////
	// PolygonModel //
	//////////////////
	
	/**
	* The 'PolygonModel' manages all the polygons, its subdivisions and its points in the application.
	*/
	
	class PolygonModel{
		private:
			std::vector<Polygon> polygonList; //Vector with all the created polygons
			static PolygonModel* instance; //Instance of the model
			PolygonModel(void); //Constructor
		public:
			static PolygonModel* getInstance(void); //Returns the pointer of the model
			static void resetInstance(void); //Deletes the instance
			~PolygonModel(void); //Destructor
			Polygon* getSelectedPolygon(void); //Returns a polygon reference when its selection state is 'true'
			Polygon* getPolygon(double x, double y); //Returns a polygon reference when clicked or hovered on
			Point* getSelectedPoint(double x, double y); //Returns an anchor point reference of the polygon when clicked or hovered on
			Point* getSelectedPoint(void); //Returns a point reference when its selection state is 'true'
			Point* getOnHoveredPoint(void); //Returns a point reference when its on hover state is 'true'
			std::vector<Polygon> getPolygonList(void); //Returns the list with all the polygons
			void deselectAllPolygons(void); //Sets the selection state of all polygons to 'false'
			void newPolygon(double r, double g, double b, Polygon::Hardness h, Polygon::DrawingMode dm, double width); //Creates a new polygon
			void deselectAllPoints(void); //Sets the selection state of all the anchor points of all the polygons to 'false'
			void drawPolygons(void); //Draws all the polygons and their subdivisions
			void selectPolygon(double x, double y); //Sets the selection state of a polygon to 'true' when clicked on it
			void deleteSelectedPolygon(void); //Deletes the selected from the polygon list
	};
	
#endif