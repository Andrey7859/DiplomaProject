#pragma once

#include <irrlicht.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
#include <string.h>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <map>
#include <string_view>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


using namespace std;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define BUTTON_SIZE 32
#define OFFSET ((Height / 10) - (BUTTON_SIZE * 2)) / 2

#define BUFSIZE 255

IrrlichtDevice *device;
IGUITreeView* SceneTree;

// Определяем значения, которые мы будем использовать для идентификации отдельных элементов управления GUI.
enum 
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,

	GUI_ID_DIALOG_ROOT_WINDOW,
	GUI_ID_DIALOG_ROOT_2_WINDOW,
	GUI_ID_DIALOG_ROOT_3_WINDOW,

	//Свойства (афинные преобразования)
	GUI_ID_X_POS,
	GUI_ID_Y_POS,
	GUI_ID_Z_POS,

	GUI_ID_X_ROT,
	GUI_ID_Y_ROT,
	GUI_ID_Z_ROT,

	GUI_ID_X_SCALE,
	GUI_ID_Y_SCALE,
	GUI_ID_Z_SCALE,

	GUI_ID_BUTTON_SET_SCALE,
	GUI_ID_BUTTON_SCALE_MUL10,
	GUI_ID_BUTTON_SCALE_DIV10,

	// Buttons (Кнопки)
	GUI_ID_ADD_BUTTON,
	GUI_ID_SAVE_BUTTON,
	GUI_ID_DELETE_BUTTON,

	GUI_ID_SELECT_BUTTON,
	GUI_ID_BRUSH_BUTTON,
	GUI_ID_MOVE_BUTTON,
	
	GUI_ID_PERSPECTIVE_BUTTON,
	GUI_ID_TOP_BUTTON,
	GUI_ID_FRONT_BUTTON,
	GUI_ID_LEFT_BUTTON,
	GUI_ID_SPLIT_BUTTON,

	GUI_ID_SIMPLE_BUTTON,
	GUI_ID_BBOX_BUTTON,
	GUI_ID_WIREFRAME_BUTTON,

	// Topbar (Верхнее меню)
	GUI_ID_OPEN_MODEL,
	GUI_ID_SAVE_MODEL,
	GUI_ID_DELETE_MODEL,
	GUI_ID_QUIT,

	GUI_ID_SELECT,
	GUI_ID_BRUSH,
	GUI_ID_MOVE,
	
	GUI_ID_VIEW,
	GUI_ID_CAMERA,

	GUI_ID_SOLID,
	GUI_ID_WAREFRANE,
	GUI_ID_REFLECATION,

	GUI_ID_PERSPECTIVE,
	GUI_ID_TOP,
	GUI_ID_LEFT,
	GUI_ID_FRONT,
	GUI_ID_BACK,
	GUI_ID_BOTTOM,
	GUI_ID_RIGHT,

	GUI_ID_ABOUT
	
};

int currentViewState = GUI_ID_PERSPECTIVE_BUTTON;
