/**
* Media Formats and Algorithms WS 2013/14 with Prof. Dr. habil. Hering-Bertram - University of Applied Sciences, Bremen, Germany
* Development work
* Author: Noshaba Cheema, (c) 2014
* Change Log: - v1.0 05.02.2014
*/

#include "Model.hpp"

#include <fstream>
#include <string>

/////////////////
// ButtonModel //
/////////////////

ButtonModel* ButtonModel::instance = 0;

ButtonModel::ButtonModel(void){
	database = "ButtonDatabase.txt";
	try{
		loadButtons();
	} catch (...) {
		throw eFileNotFoundException();
	}
}

ButtonModel* ButtonModel::getInstance(void){
	try{
		if(instance == NULL) instance = new ButtonModel();
		return instance;
	} catch(...){
		throw eFileNotFoundException();
	}
}

void ButtonModel::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}

ButtonModel::~ButtonModel(void){}

void ButtonModel::loadButtons(void){
	std::ifstream file(database.c_str());
	std::string name, xpos, ypos, width, height, imgPath;
	if(file.is_open() && file.good()){
		while(!file.eof()){
			// load buttons from file and save them as button objects
			getline(file,name);
			getline(file,xpos);
			getline(file,ypos);
			getline(file,width);
			getline(file,height);
			getline(file,imgPath);
			
			Button b(name,atof(xpos.c_str()),atof(ypos.c_str()),atof(width.c_str()),atof(height.c_str()),imgPath);
			buttonList.push_back(b);
		}
		file.close();
	} else {
		throw eFileNotFoundException();
	}
}

Button* ButtonModel::getButton(double x, double y){
	ObjTraits traits;
	for(unsigned int i = 0; i < buttonList.size(); i++){
		traits = buttonList[i].getTraits();
		// return button when clicked on
		if((x >= traits.xpos) && (x <= traits.xpos + traits.width) &&
		   (y >= traits.ypos) && (y <= traits.ypos + traits.height)) return &buttonList[i];
	}
	return NULL;
}

Button* ButtonModel::getButton(std::string name){
	// return button by name
	for(unsigned int i = 0; i < buttonList.size(); i++) if(buttonList[i].getName() == name) return &buttonList[i];
	return NULL;
}

std::vector<Button> ButtonModel::getButtonList(void){
	return buttonList;
}

void ButtonModel::turnOffAllButtons(void){
	for(unsigned int i = 0; i < buttonList.size(); i++) buttonList[i].setActivation(false);
}

void ButtonModel::displayButtons(void){
	for(unsigned int i = 0; i < buttonList.size(); i++) buttonList[i].display();
}

/////////////////
// SliderModel //
/////////////////

SliderModel* SliderModel::instance = 0;

SliderModel::SliderModel(void){
	database = "SliderDatabase.txt";
	try{
		loadSliders();
	} catch(...){
		throw eFileNotFoundException();
	}
}

SliderModel* SliderModel::getInstance(void){
	try{
		if(instance == NULL) instance = new SliderModel();
		return instance;
	} catch(...){
		throw eFileNotFoundException();
	}
}

void SliderModel::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}

SliderModel::~SliderModel(void){}

void SliderModel::loadSliders(void){
	std::ifstream file(database.c_str());
	std::string name, xpos, ypos, width, height, imgPath, minValue, maxValue, direction,
				bXpos, bYpos, bWidth, bHeight, bImgPath;
	if(file.is_open() && file.good()){
		while(!file.eof()){
			// load sliders from file and save them as button objects
			getline(file,name);
			getline(file,xpos);
			getline(file,ypos);
			getline(file,width);
			getline(file,height);
			getline(file,imgPath);
			getline(file,minValue);
			getline(file,maxValue);
			getline(file,direction);
			getline(file,bXpos);
			getline(file,bYpos);
			getline(file,bWidth);
			getline(file,bHeight);
			getline(file,bImgPath);
			
			Slider s(name, atof(xpos.c_str()), atof(ypos.c_str()), atof(width.c_str()), atof(height.c_str()), imgPath,
					 atof(minValue.c_str()), atof(maxValue.c_str()), direction, atof(bXpos.c_str()), atof(bYpos.c_str()),
					 atof(bWidth.c_str()), atof(bHeight.c_str()), bImgPath);
			sliderList.push_back(s);
		}
		file.close();
	} else {
		throw eFileNotFoundException();
	}
}

Slider* SliderModel::getSlider(double x, double y){
	ObjTraits traits;
	for(unsigned int i = 0; i < sliderList.size(); i++){
		traits = sliderList[i].getTraits();
		// return slider when clicked on
		if((x >= traits.xpos) && (x <= traits.xpos + traits.width) &&
		   (y >= traits.ypos) && (y <= traits.ypos + traits.height)) return &sliderList[i];
	}
	return NULL;
}

Slider* SliderModel::getSlider(std::string name){
	// return slider by name
	for(unsigned int i = 0; i < sliderList.size(); i++) if(sliderList[i].getName() == name) return &sliderList[i];
	return NULL;
}

Slider* SliderModel::getSelectedSlider(void){
	for(unsigned int i = 0; i < sliderList.size(); i++) if(sliderList[i].selected()) return &sliderList[i];
	return NULL;
}

std::vector<Slider> SliderModel::getSliderList(void){
	return sliderList;
}

void SliderModel::displaySliders(void){
	for(unsigned int i = 0; i < sliderList.size(); i++) sliderList[i].displaySlider();
}

//////////////////
// PolygonModel //
//////////////////

PolygonModel* PolygonModel::instance = 0;

PolygonModel::PolygonModel(void){}

PolygonModel* PolygonModel::getInstance(void){
	if(instance == NULL) instance = new PolygonModel();
	return instance;
}

void PolygonModel::resetInstance(void){
	delete instance;
	instance = NULL;	// so getInstance will still work
}

PolygonModel::~PolygonModel(void){}

Polygon* PolygonModel::getSelectedPolygon(void){
	for(unsigned int i = 0; i < polygonList.size(); i++) if(polygonList[i].selected()) return &polygonList[i];
	return NULL;
}

Polygon* PolygonModel::getPolygon(double x, double y){
	for(unsigned int i = 0; i < polygonList.size();i++) if(polygonList[i].getSelectedPoint(x,y) != NULL)
		return &polygonList[i];
	return NULL;
}

Point* PolygonModel::getSelectedPoint(double x, double y){
	for(unsigned int i = 0; i < polygonList.size();i++) if(polygonList[i].getSelectedPoint(x,y) != NULL)
		return polygonList[i].getSelectedPoint(x,y);
	return NULL;
}

Point* PolygonModel::getSelectedPoint(void){
	for(unsigned int i = 0; i < polygonList.size();i++) if(polygonList[i].getSelectedPoint() != NULL)
		return polygonList[i].getSelectedPoint();
	return NULL;
}

Point* PolygonModel::getOnHoveredPoint(void){
	for(unsigned int i = 0; i < polygonList.size();i++) if(polygonList[i].getOnHoveredPoint() != NULL)
		return polygonList[i].getOnHoveredPoint();
	return NULL;
}

void PolygonModel::deselectAllPoints(void){
	for(unsigned int i = 0; i < polygonList.size(); i++) polygonList[i].deselectAllPoints();
}

void PolygonModel::deselectAllPolygons(void){
	for(unsigned int i = 0; i < polygonList.size(); i++) polygonList[i].setSelection(false);
}

void PolygonModel::selectPolygon(double x, double y){
	deselectAllPolygons();
	for(unsigned int i = 0; i < polygonList.size();i++)
		if(polygonList[i].getSelectedPoint(x,y) != NULL) polygonList[i].setSelection(true);
}

std::vector<Polygon> PolygonModel::getPolygonList(void){
	return polygonList;
}

void PolygonModel::newPolygon(double r, double g, double b, Polytope2D::Hardness h, Polytope2D::DrawingMode dm, double width){
	Polygon p(r, g, b, h, dm, width);
	p.setSelection(true); // mark the newly created as selected
	polygonList.push_back(p);
}

void PolygonModel::drawPolygons(void){
	for(unsigned int i = 0; i < polygonList.size(); i++) polygonList[i].draw();
}

void PolygonModel::deleteSelectedPolygon(void){
	for(unsigned int i = 0; i < polygonList.size(); i++){
		if(polygonList[i].selected()) {
			polygonList.erase(polygonList.begin() + i);
			break;
		}
	}
}