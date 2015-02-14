/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

/**
* The 'View' initializes and updates the window after a new event. It tells the specific models to render their objects all over again.
*/

#ifndef VIEW_HPP
	#define VIEW_HPP
	
	#include "Model.hpp"
	#include <iostream>
	
	class View{
		private:
			View(void); //Constructor
			static View* instance; //Instance of the 'View'
			int windowWidth, windowHeight; //height and width of the window
			ButtonModel* buttonM; //Pointer to the button model
			SliderModel* sliderM; //Pointer to the slider model
			PolygonModel* polygonM; //Pointer to the polygon model
			Canvas* canvas; //Pointer to the canvas
			CurrentColour* currentColour; //Pointer to the currently selected colour
		public:
			static View* getInstance(void); //Returns the pointer of the view
			static void resetInstance(void); //Deletes the pointer of the view
			~View(void); //Destructor
			void initWindow(); //Initializes OpenGL, GLFW and the window
			int getWindowWidth(void); //Returns the window width
			int getWindowHeight(void); //Returns the window height
			void displayBackground(void); //Displays the background
			void displayButtons(void); //Displays the buttons
			void displaySliders(void); //Displays the sliders
			void displayPaper(void); //Displays the canvas
			void displayBase(void); //Displays all of the above to update the window
			void displayCurrentColour(void); //Displays the currently selected colour
			void updateWindow(void); //Renders the window all over again
	};
	
#endif