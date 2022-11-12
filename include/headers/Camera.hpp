#pragma once
#include "headers/Header.hpp"
#include "headers/Model.hpp"
#include "headers/View.hpp"

Model CurrentObject;

// Создание камер
void createCameras(){
	// camera[0] = smgr->addCameraSceneNode(0, *(CurrentObject.getCoord()) + vector3df(0, 30, -40), *(CurrentObject.getCoord()));
    // camera[0]->setFarValue(200000.f); Устанавливает значение дальней плоскости отсечения (по умолчанию: 2000.0f).
	//Основная
	camera[0] = smgr->addCameraSceneNodeMaya();
    camera[0]->setTarget(*(CurrentObject.getCoord()));
	// Top
	camera[1] = smgr->addCameraSceneNode(0, vector3df(0,50,0), vector3df(0,0,0));
	//Front
	camera[2] = smgr->addCameraSceneNode(0, vector3df(50,0,0), vector3df(0,0,0));
	//Left
	camera[3] = smgr->addCameraSceneNode(0, vector3df(0,0,50), vector3df(0,0,0));
}

// Разделение экрана на 4 части
void splitscreen() {
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// !!! Левая верхняя часть перспектива 
	smgr->setActiveCamera(camera[0]);
	driver->setViewPort(rect<s32>( EXPLORER_WINDOW_WIDTH, EXPLORER_WINDOW_POS_Y, WINDOW_SPLIT_WIDTH, WINDOW_SPLIT_HEIGHT));
	camera[0]->setPosition(*(CurrentObject.getCoord()) + vector3df(0, 30, -40));
	camera[0]->setTarget(*(CurrentObject.getCoord()));
	smgr->drawAll();
	//Правая верхняя часть топ 
	smgr->setActiveCamera(camera[1]);
	driver->setViewPort(rect<s32>( WINDOW_SPLIT_WIDTH, EXPLORER_WINDOW_POS_Y, Width, WINDOW_SPLIT_HEIGHT));
	camera[1]->setTarget(*(CurrentObject.getCoord()));
	smgr->drawAll();
	//Нижняя левая часть фронт
	smgr->setActiveCamera(camera[2]);
	driver->setViewPort(rect<s32>( EXPLORER_WINDOW_WIDTH, WINDOW_SPLIT_HEIGHT, WINDOW_SPLIT_WIDTH, Height));
	camera[2]->setTarget(*(CurrentObject.getCoord()));
	smgr->drawAll();
	// Нижняя првая часть с лева 
	smgr->setActiveCamera(camera[3]);
	driver->setViewPort(rect<s32>( WINDOW_SPLIT_WIDTH, WINDOW_SPLIT_HEIGHT, Width, Height));
	camera[3]->setTarget(*(CurrentObject.getCoord()));
	smgr->drawAll();
}

// Выбор состояния камер
void cameraViewState(){
	switch (currentViewState){
		case GUI_ID_PERSPECTIVE_BUTTON:
			smgr->setActiveCamera(camera[0]);
			driver->setViewPort(rect<s32>( 0, 0, Width, Height));
			smgr->drawAll();
			break;
		case GUI_ID_TOP_BUTTON:
			smgr->setActiveCamera(camera[1]);
			driver->setViewPort(rect<s32>( 0, 0, Width, Height));
			smgr->drawAll();
			break;
		case GUI_ID_FRONT_BUTTON:
			smgr->setActiveCamera(camera[2]);
			driver->setViewPort(rect<s32>( 0, 0, Width, Height));
			smgr->drawAll();
			break;
		case GUI_ID_LEFT_BUTTON:
			smgr->setActiveCamera(camera[3]);
			driver->setViewPort(rect<s32>( 0, 0, Width, Height));
			smgr->drawAll();
			break;
		case GUI_ID_SPLIT_BUTTON:
			splitscreen();
			break;		
	}	
}
