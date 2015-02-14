/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

#include "Controller.hpp"

#include <iostream>

#define GLFW_KEY_B 66
#define GLFW_KEY_C 67
#define GLFW_KEY_I 73
#define GLFW_KEY_Q 81
#define GLFW_KEY_S 83
#define GLFW_KEY_P 80

void GLFWCALL MousePosCallback(int x, int y){
	try{
			Controller* PolyFun = Controller::getInstance();
		// draw point when hovered over
		if(PolyFun->getSelectedPoint(x,y) != NULL){
			PolyFun->getSelectedPoint(x,y)->setHover(true);
			PolyFun->updateWindow();
		} else if(PolyFun->getSelectedPoint(x,y) == NULL && PolyFun->getOnHoveredPoint() != NULL){
			PolyFun->getOnHoveredPoint()->setHover(false);
			PolyFun->updateWindow();
		}
		// drag point while the mouse is over the canvas
		if(PolyFun->getSelectedPoint() != NULL && PolyFun->getCanvasSelection(x,y)){
			PolyFun->setPointPosition(x,y);
			PolyFun->updateWindow();
		} else if(PolyFun->getSelectedPoint() != NULL && !(PolyFun->getCanvasSelection(x,y))){
			PolyFun->releaseDraggedAnchorPoint(); // release the point when the mouse is outside the canvas
		}
		if(PolyFun->getSelectedSlider() != NULL){
			// set the values of the sliders if dragged
			Slider* s = PolyFun->getSelectedSlider();
			PolyFun->setSliderPosition(x,y);
			if(s->getName() == "widthSlider")
				PolyFun->setPolygonWidth(s->getValue());
			else if(s->getName() == "bezierT")
				PolyFun->setBezierT(s->getValue());
			else if(s->getName() == "quadraticSplineT")
				PolyFun->setQuadraticSplineT(s->getValue());
			else if(s->getName() == "cubicSplineT")
				PolyFun->setCubicSplineT(s->getValue());
			else if(s->getName() == "cubicInterpolationT")
				PolyFun->setCubicInterpolationT(s->getValue());
			else if(s->getName() == "bezierDepth")
				PolyFun->setBezierDepth(s->getValue());
			else if(s->getName() == "quadraticSplineDepth")
				PolyFun->setQuadraticSplineDepth(s->getValue());
			else if(s->getName() == "cubicSplineDepth")
				PolyFun->setCubicSplineDepth(s->getValue());
			else if(s->getName() == "cubicInterpolationDepth")
				PolyFun->setCubicInterpolationDepth(s->getValue());
			else if(s->getName() == "smoothPolygonT")
				PolyFun->setSmoothPolygonT(s->getValue());
			else if(s->getName() == "colourShade")
				PolyFun->setColourShade(s->getValue());
			PolyFun->updateWindow();
		}
		// gets the colour from the "colours" button
		if(PolyFun->getButton(x,y) != NULL && PolyFun->getButton(x,y)->getName() == "colours" && PolyFun->getButton("colours")->activated()){
			unsigned char pixel[3];
			glReadPixels(x,PolyFun->getWindowHeight() - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel); // read colour from pixel
			PolyFun->setCurrentColour(pixel[0]/255.0, pixel[1]/255.0, pixel[2]/255.0); // change colour
			PolyFun->updateWindow();
		}
	} catch(...){
		throw eFileNotFoundException();
	}
}

// this function is being called when a mouse button is pressed
void GLFWCALL MouseButtonCallback(int button, int mouseEvent){
	int x,y;
	glfwGetMousePos(&x, &y);
	
	try{
		Controller* PolyFun = Controller::getInstance();
		
		// if mouse was clicked on canvas
		if(PolyFun->getCanvasSelection(x,y)){
			if(mouseEvent == GLFW_PRESS){
				switch(button){
					// if the pen tool is activated and the left button is pressed
					case GLFW_MOUSE_BUTTON_LEFT:
						if(PolyFun->getButton("penTool") != NULL && PolyFun->getButton("penTool")->activated() && PolyFun->getSelectedPolygon() != NULL){
							PolyFun->addAnchorPoint(x,y);
							PolyFun->updateWindow();
						};
						if(PolyFun->getButton("eraser") != NULL && PolyFun->getButton("eraser")->activated()) PolyFun->deletePoint(x,y); // delete point if clicked on one while the eraser is active
						break;
					case GLFW_MOUSE_BUTTON_RIGHT:
						if(PolyFun->getButton("penTool") != NULL) PolyFun->getButton("penTool")->setActivation(false); // turn the drawing button off
						if(PolyFun->getButton("eraser") != NULL) PolyFun->getButton("eraser")->setActivation(false); // turn the eraser off
						PolyFun->dragAnchorPoint(x,y);
						break;
					default: ;
						break;
				}
				PolyFun->selectPolygon(x,y); // selects the polygon with the coordinated x and y
				if(PolyFun->getPolygon(x,y) == NULL){
					PolyFun->deselectAllPolygons(); // deselect all polygons if clicked on the paper and not on a polygon
					if(PolyFun->getButton("penTool") != NULL) PolyFun->getButton("penTool")->setActivation(false); // turn the drawing button off
				}
				PolyFun->updateWindow();
			} else if(mouseEvent == GLFW_RELEASE){
				switch(button){
					case GLFW_MOUSE_BUTTON_RIGHT: PolyFun->releaseDraggedAnchorPoint();
						break;
					case GLFW_MOUSE_BUTTON_LEFT: PolyFun->releaseSlider();
						break;
					default: ;
						break;
				}
			}
		// if clicked outside the canvas
		} else {
			Button* b = PolyFun->getButton(x,y);
			Slider* s = PolyFun->getSlider(x,y);
			if(mouseEvent == GLFW_PRESS){
				switch(button){
					case GLFW_MOUSE_BUTTON_LEFT:
						if(b != NULL){
							if(b->getName() == "penTool"){
								if(b->activated()){
									// deactivate the activated button by clicking on it again
									std::cout << b->getName() << " has been deactivated" << std::endl;
									b->setActivation(false);
									PolyFun->deselectAllPolygons(); // reset selection of all polygons
								} else {
									// activate the pen tool and deactivate the eraser
									if(PolyFun->getButton("eraser") != NULL) PolyFun->getButton("eraser")->setActivation(false);
									b->setActivation(true);
									std::cout << b->getName() << " has been activated" << std::endl;
									if(PolyFun->getSelectedPolygon() == NULL) PolyFun->newPolygon(PolyFun->colour.r, PolyFun->colour.g, PolyFun->colour.b, PolyFun->hardness, PolyFun->drawingMode, PolyFun->polygonWidth);
								};
							} else if(b->getName() == "eraser"){
								if(b->activated()){
									PolyFun->deselectAllPolygons(); // reset selection of all polygons
									// deactivate the activated button by clicking on it again
									b->setActivation(false);
									std::cout << b->getName() << " has been deactivated" << std::endl;
								} else {
									// activate the eraser and deactivate the pen tool
									if(PolyFun->getButton("penTool") != NULL) PolyFun->getButton("penTool")->setActivation(false);
									b->setActivation(true);
									std::cout << b->getName() << " has been activated" << std::endl;
								};
							} else if(b->getName() == "lineStrip"){
								std::cout << b->getName() << " has been activated" << std::endl;
								// deactivate other mode buttons
								if(PolyFun->getButton("lineLoop") != NULL) PolyFun->getButton("lineLoop")->setActivation(false);
								if(PolyFun->getButton("filledPolygon") != NULL) PolyFun->getButton("filledPolygon")->setActivation(false);
								b->setActivation(true);
								PolyFun->setDrawingMode(Polytope2D::LINE);
							} else if(b->getName() == "lineLoop"){
								std::cout << b->getName() << " has been activated" << std::endl;
								// deactivate other mode buttons
								if(PolyFun->getButton("lineStrip") != NULL) PolyFun->getButton("lineStrip")->setActivation(false);
								if(PolyFun->getButton("filledPolygon") != NULL) PolyFun->getButton("filledPolygon")->setActivation(false);
								b->setActivation(true);
								PolyFun->setDrawingMode(Polytope2D::LINELOOP);
							} else if(b->getName() == "filledPolygon"){
								std::cout << b->getName() << " has been activated" << std::endl;
								// deactivate other mode buttons
								if(PolyFun->getButton("lineStrip") != NULL) PolyFun->getButton("lineStrip")->setActivation(false);
								if(PolyFun->getButton("lineLoop") != NULL) PolyFun->getButton("lineLoop")->setActivation(false);
								b->setActivation(true);
								PolyFun->setDrawingMode(Polytope2D::POLYGON);
							} else if(b->getName() == "hardness"){
								std::cout << b->getName() << " has been activated" << std::endl;
								// deactivate other mode buttons
								if(PolyFun->getButton("smoothness") != NULL) PolyFun->getButton("smoothness")->setActivation(false);
								b->setActivation(true);
								PolyFun->setHardness(Polytope2D::HARD);
							} else if(b->getName() == "smoothness"){
								std::cout << b->getName() << " has been activated" << std::endl;
								// deactivate other mode buttons
								if(PolyFun->getButton("hardness") != NULL) PolyFun->getButton("hardness")->setActivation(false);
								b->setActivation(true);
								PolyFun->setHardness(Polytope2D::SMOOTH);
							} else if(b->getName() == "colours" && !(b->activated())){
								b->setActivation(true);
								std::cout << b->getName() << " has been activated" << std::endl;
								glfwSetMousePosCallback(MousePosCallback);
							};
							PolyFun->updateWindow();
						} else if(s != NULL){
							PolyFun->dragSlider(x,y);
						};
						break;
					default: ;
						break;
				}
			} else if(mouseEvent == GLFW_RELEASE){
				switch(button){
					case GLFW_MOUSE_BUTTON_LEFT: PolyFun->releaseSlider();
						break;
					default: ;
						break;
				}
				if(PolyFun->getButton("colours") != NULL && PolyFun->getButton("colours")->activated()){
					PolyFun->getButton("colours")->setActivation(false);
					std::cout << "colours has been deactivated" << std::endl;
				}
			}
		}
	} catch(...){
		throw eFileNotFoundException();
	}
}

void GLFWCALL KeyCallback(int button, int action){
	try{
		Controller* PolyFun = Controller::getInstance();
		if(action == GLFW_RELEASE){
			switch(button){
				case GLFW_KEY_B:
					if(PolyFun->getSelectedPolygon() != NULL){
						if(PolyFun->getSelectedPolygon()->getAnchorPoints().size() > 1)
							(PolyFun->bezierSubdivision(PolyFun->bezierDepth, PolyFun->bezierT));
						else
							(std::cout << "Polygon has to have at least 2 anchor points for Bezier Subdivision!" << std::endl);
					};
					break;
				case GLFW_KEY_C:
					if(PolyFun->getSelectedPolygon() != NULL){
						if(PolyFun->getSelectedPolygon()->getAnchorPoints().size() > 2)
							(PolyFun->cubicSplineSubdivision(PolyFun->cubicSplineDepth, PolyFun->cubicSplineT));
						else
							(std::cout << "Polygon has to have at least 3 anchor points for Cubic Spline Subdivision!" << std::endl);
					};
					break;
				case GLFW_KEY_I:
					if(PolyFun->getSelectedPolygon() != NULL){
						if(PolyFun->getSelectedPolygon()->getAnchorPoints().size() > 2)
							(PolyFun->cubicInterpolation(PolyFun->cubicInterpolationDepth, PolyFun->cubicInterpolationT));
						else
							(std::cout << "Polygon has to have at least 3 anchor points for Cubic Interpolation!" << std::endl);
					};
					break;
				case GLFW_KEY_Q:
					if(PolyFun->getSelectedPolygon() != NULL){
						if(PolyFun->getSelectedPolygon()->getAnchorPoints().size() > 2)
							(PolyFun->quadraticSplineSubdivision(PolyFun->quadraticSplineDepth, PolyFun->quadraticSplineT));
						else
							(std::cout << "Polygon has to have at least 3 anchor points for Quadratic Spline Subdivision!" << std::endl);
					};
					break;
				case GLFW_KEY_S: if(PolyFun->getSelectedPolygon() != NULL) PolyFun->getSelectedPolygon()->smooth(PolyFun->smoothPolygonT);
					break;
				case GLFW_KEY_P: if(PolyFun->getSelectedPolygon() != NULL) PolyFun->getSelectedPolygon()->toggleDisplay(); // toggle if to be shown
					break;
				case GLFW_KEY_DEL: if(PolyFun->getSelectedPolygon() != NULL) PolyFun->deleteSelectedPolygon();
					break;
				default: ;
					break;
			}
			PolyFun->updateWindow();
		}
	} catch(...){
		throw eFileNotFoundException();
	}
}

Controller* Controller::instance = 0; // init static instance

Controller::Controller(void){	
	// initialize attributes
	colour.r = 135/255.0;
	colour.g = 129/255.0;
	colour.b = 190/255.0;
	grayShade = 1;
	polygonWidth = 1;
	bezierT = 0.5;
	quadraticSplineT = cubicSplineT = cubicInterpolationT = 0.25;
	bezierDepth = quadraticSplineDepth = cubicSplineDepth = cubicInterpolationDepth = 3;
	smoothPolygonT = 1/3.0;
	hardness = Polygon::SMOOTH;
	try{
		buttonM = ButtonModel::getInstance();
		sliderM = SliderModel::getInstance();
		polygonM = PolygonModel::getInstance();
		canvas = Canvas::getInstance();
		currentColour = CurrentColour::getInstance();
		currentColour->setColour(colour.r, colour.g, colour.b);
		if(getButton("lineStrip") != NULL) getButton("lineStrip")->setActivation(true);
		if(getButton("smoothness") != NULL) getButton("smoothness")->setActivation(true);
		if(getSlider("widthSlider") != NULL) getSlider("widthSlider")->setValue(polygonWidth);
		if(getSlider("bezierT") != NULL) getSlider("bezierT")->setValue(bezierT);
		if(getSlider("quadraticSplineT") != NULL) getSlider("quadraticSplineT")->setValue(quadraticSplineT);
		if(getSlider("cubicSplineT") != NULL) getSlider("cubicSplineT")->setValue(cubicSplineT);
		if(getSlider("cubicInterpolationT") != NULL) getSlider("cubicInterpolationT")->setValue(cubicInterpolationT);
		if(getSlider("bezierDepth") != NULL) getSlider("bezierDepth")->setValue(bezierDepth);
		if(getSlider("quadraticSplineDepth") != NULL) getSlider("quadraticSplineDepth")->setValue(quadraticSplineDepth);
		if(getSlider("cubicSplineDepth") != NULL) getSlider("cubicSplineDepth")->setValue(cubicSplineDepth);
		if(getSlider("cubicInterpolationDepth") != NULL) getSlider("cubicInterpolationDepth")->setValue(cubicInterpolationDepth);
		if(getSlider("smoothPolygonT") != NULL) getSlider("smoothPolygonT")->setValue(smoothPolygonT);
		if(getSlider("colourShade") != NULL) getSlider("colourShade")->setValue(grayShade);
		window = View::getInstance();
	} catch(...){
		throw eFileNotFoundException();
	}
}

Controller* Controller::getInstance(void){
	try{
		if(instance == NULL) instance = new Controller();
		return instance;
	} catch(...){
		throw eFileNotFoundException();
	}
}

void Controller::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}

Controller::~Controller(void){
	View::resetInstance();
}

int Controller::getWindowHeight(void){
	try{
		return window->getWindowHeight();
	} catch(...){
		throw eFileNotFoundException();
	}
}

int Controller::getWindowWidth(void){
	try{
		return window->getWindowWidth();
	} catch(...){
		throw eFileNotFoundException();
	}
}

Button* Controller::getButton(std::string name){
	try{
		return buttonM->getButton(name);
	} catch(...){
		throw eFileNotFoundException(); 
	}
}

Button* Controller::getButton(double x, double y){
	try{
		return buttonM->getButton(x,y);
	} catch(...){
		throw eFileNotFoundException();
	}
}

Slider* Controller::getSlider(std::string name){
	try{
		return sliderM->getSlider(name);
	} catch(...){
		throw eFileNotFoundException();
	}
}

Slider* Controller::getSlider(double x, double y){
	try{
		return sliderM->getSlider(x,y);
	} catch(...){
		throw eFileNotFoundException();
	}
}

Slider* Controller::getSelectedSlider(void){
	try{
		return sliderM->getSelectedSlider();
	} catch(...){
		throw eFileNotFoundException();
	}
}

Polygon* Controller::getSelectedPolygon(void){
	return polygonM->getSelectedPolygon();
}

Polygon* Controller::getPolygon(double x, double y){
	return polygonM->getPolygon(x,y);
}

Point* Controller::getSelectedPoint(double x, double y){
	return polygonM->getSelectedPoint(x,y);
}

Point* Controller::getSelectedPoint(void){
	return polygonM->getSelectedPoint();
}

Point* Controller::getOnHoveredPoint(void){
	return polygonM->getOnHoveredPoint();
}

bool Controller::getCanvasSelection(double x, double y){
	return canvas->selected(x,y);
}

void Controller::updateWindow(void){
	try{
		window->updateWindow();
	} catch(...){
		throw eFileNotFoundException();
	}
}

void Controller::setCurrentColour(double r, double g, double b){
	colour.r = r;
	colour.g = g;
	colour.b = b;
	CurrentColour::getInstance()->setColour(r, g, b);
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->setColour(r, g, b);
}

void Controller::setColourShade(double grayScale){
	try{
		if(getButton("colours") != NULL) getButton("colours")->setShade(grayScale);
	} catch(...){
		throw eFileNotFoundException();
	}
}

void Controller::setPolygonWidth(double width){
	polygonWidth = width;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->setWidth(width);
}

void Controller::setBezierT(double t){
	bezierT = t;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getBezierCurve()->setT(t);
}

void Controller::setQuadraticSplineT(double t){
	quadraticSplineT = t;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getQuadraticSpline()->setT(t);
}

void Controller::setCubicSplineT(double t){
	cubicSplineT = t;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getCubicSpline()->setT(t);
}

void Controller::setCubicInterpolationT(double t){
	cubicInterpolationT = t;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getCubicInterpolation()->setT(t);
}

void Controller::setSmoothPolygonT(double t){
	smoothPolygonT = t;
}

void Controller::setBezierDepth(double depth){
	bezierDepth = depth;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getBezierCurve()->setDepth(depth);
}

void Controller::setQuadraticSplineDepth(double depth){
	quadraticSplineDepth = depth;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getQuadraticSpline()->setDepth(depth);
}

void Controller::setCubicSplineDepth(double depth){
	cubicSplineDepth = depth;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getCubicSpline()->setDepth(depth);
}

void Controller::setCubicInterpolationDepth(double depth){
	cubicInterpolationDepth = depth;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->getCubicInterpolation()->setDepth(depth);
}

void Controller::setHardness(Polytope2D::Hardness h){
	hardness = h;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->setHardness(h);
}

void Controller::setDrawingMode(Polytope2D::DrawingMode dm){
	drawingMode = dm;
	if(getSelectedPolygon() != NULL) getSelectedPolygon()->setDrawingMode(dm);
}

void Controller::setPointPosition(double x, double y){
	getSelectedPoint()->setPosition(x,y);
}

void Controller::setSliderPosition(double x, double y){
	getSelectedSlider()->setPosition(x,y);
}

void Controller::deselectAllPoints(void){
	polygonM->deselectAllPoints();
}

void Controller::deselectAllPolygons(void){
	polygonM->deselectAllPolygons();
}

void Controller::deselectPolygon(void){
	getSelectedPolygon()->setSelection(false);
}

void Controller::turnOffAllButtons(void){
	try{
		buttonM->turnOffAllButtons();
	} catch(...){
		throw eFileNotFoundException();
	}
}

void Controller::newPolygon(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width){
	polygonM->newPolygon(r, g, b, h, dm, width);
}

void Controller::addAnchorPoint(double x, double y){
	getSelectedPolygon()->addAnchorPoint(x,y);
}

void Controller::dragAnchorPoint(double x, double y){
	if(getPolygon(x,y) != NULL){
		deselectAllPoints();
		getSelectedPoint(x,y)->setSelection(true);
		glfwSetMousePosCallback(MousePosCallback);
	}
}

void Controller::dragSlider(double x, double y){
	try{
		getSlider(x,y)->setSelection(true);
		glfwSetMousePosCallback(MousePosCallback);
	} catch(...){
		throw eFileNotFoundException();
	}
}

void Controller::selectPolygon(double x, double y){
	polygonM->selectPolygon(x,y);
}

void Controller::releaseDraggedAnchorPoint(void){
	if(getSelectedPoint() != NULL){
		getSelectedPoint()->setSelection(false);
	}
}

void Controller::releaseSlider(void){
	if(getSelectedSlider() != NULL){
		getSelectedSlider()->setSelection(false);
	}
}

void Controller::bezierSubdivision(unsigned int depth, double t){
	getSelectedPolygon()->showBezierCurve(depth, t);
}

void Controller::quadraticSplineSubdivision(unsigned int depth, double t){
	getSelectedPolygon()->showQuadraticSpline(depth, t);
}

void Controller::cubicSplineSubdivision(unsigned int depth, double t){
	getSelectedPolygon()->showCubicSpline(depth, t);
}

void Controller::cubicInterpolation(unsigned int depth, double t){
	getSelectedPolygon()->showCubicInterpolation(depth, t);
}

void Controller::deleteSelectedPolygon(void){
	polygonM->deleteSelectedPolygon();
}

void Controller::deletePoint(double x, double y){
	if(getPolygon(x,y) != NULL) getPolygon(x,y)->deletePoint(x,y);
}

int main(){
	try{
		Controller* PolyFun = Controller::getInstance();
		// register events
		glfwSetMouseButtonCallback(MouseButtonCallback);
		glfwSetKeyCallback(KeyCallback);
		glfwSetMousePosCallback(MousePosCallback);
	} catch (const eFileNotFoundException &e){
		std::cerr << "ERROR: Database not found!" << std::endl;
		std::cin.ignore();
	}
	// endless loop until window is closed
	while(true) {
		glfwWaitEvents(); // do nothing until user presses mouse button
		
		if(!glfwGetWindowParam(GLFW_OPENED)) {
			break;
		}
	}
	Controller::resetInstance(); //delete pointers and quit
	return 0;
}