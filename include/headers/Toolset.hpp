#pragma once
#include "headers/View.hpp"

IGUIEditBox* editboxPosX;
IGUIEditBox* editboxPosY;
IGUIEditBox* editboxPosZ;
IGUIEditBox* editboxRotX;
IGUIEditBox* editboxRotY;
IGUIEditBox* editboxRotZ;
IGUIEditBox* editboxScaleX;
IGUIEditBox* editboxScaleY;
IGUIEditBox* editboxScaleZ;

// Функция созданя Toolset -> Properties
void createToolset(){
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    
	if (e)
        e->remove();

    // Созадния окна Toolset   				  от края     отступ с верху  		 ширина    			длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, EXPLORER_WINDOW_POS_Y, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H), false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);

    // Location (Расположение)
    env->addStaticText(L"Location:", rect<s32>( 35, 25, 80, 45), false, false, wnd);
	env->addStaticText(L"X:", core::rect<s32>(10,52,18,70), false, false, wnd);
	editboxPosX = env->addEditBox(L"1.0", rect<s32>(25 , 50, 85, 70), true, wnd, GUI_ID_X_POS);
	env->addStaticText(L"Y:", core::rect<s32>(10,82,18,100), false, false, wnd);
    editboxPosY = env->addEditBox(L"1.0", rect<s32>(25, 80, 85, 100), true, wnd, GUI_ID_Y_POS);
	env->addStaticText(L"Z:", core::rect<s32>(10,112,18,130), false, false, wnd);
    editboxPosZ = env->addEditBox(L"1.0", rect<s32>(25, 110, 85, 130), true, wnd, GUI_ID_Z_POS);

	scrollbarPosX = env->addScrollBar(true,core::rect<s32>(100 , 50, 280, 70), wnd, GUI_ID_X_SCROLL_POS);
	scrollbarPosX->setMin(-100);
	scrollbarPosX->setMax(100);
	scrollbarPosX->setPos(0);

	scrollbarPosY = env->addScrollBar(true,core::rect<s32>(100 , 80, 280, 100), wnd, GUI_ID_Y_SCROLL_POS);
	scrollbarPosY->setMin(-100);
	scrollbarPosY->setMax(100);
	scrollbarPosY->setPos(0);

	scrollbarPosZ = env->addScrollBar(true,core::rect<s32>(100 , 110, 280, 130), wnd, GUI_ID_Z_SCROLL_POS);
	scrollbarPosZ->setMin(-100);
	scrollbarPosZ->setMax(100);
	scrollbarPosZ->setPos(0);

	// Rotation (Расположение :: Варащение)
    env->addStaticText(L"Rotation:", rect<s32>( 35, 135, 80, 155), false, false, wnd);
    env->addStaticText(L"X:", core::rect<s32>(10,162,18,180), false, false, wnd);
	editboxRotX = env->addEditBox(L"1.0", rect<s32>(25 , 160, 85, 180), true, wnd, GUI_ID_X_ROT);
	env->addStaticText(L"Y:", core::rect<s32>(10,192,18,210), false, false, wnd);
    editboxRotY = env->addEditBox(L"1.0", rect<s32>(25 , 190, 85, 210), true, wnd, GUI_ID_Y_ROT);
	env->addStaticText(L"Z:", core::rect<s32>(10,222,18,240), false, false, wnd);
    editboxRotZ = env->addEditBox(L"1.0", rect<s32>(25 , 220, 85, 240), true, wnd, GUI_ID_Z_ROT);

	scrollbarRotX = env->addScrollBar(true,core::rect<s32>(100 , 160, 280, 180), wnd, GUI_ID_X_SCROLL_ROT);
	scrollbarRotX->setMin(-360);
	scrollbarRotX->setMax(360);
	scrollbarRotX->setPos(0);

	scrollbarRotY = env->addScrollBar(true,core::rect<s32>(100 , 190, 280, 210), wnd, GUI_ID_Y_SCROLL_ROT);
	scrollbarRotY->setMin(-360);
	scrollbarRotY->setMax(360);
	scrollbarRotY->setPos(0);

	scrollbarRotZ = env->addScrollBar(true,core::rect<s32>(100 , 220, 280, 240), wnd, GUI_ID_Z_SCROLL_ROT);
	scrollbarRotZ->setMin(-360);
	scrollbarRotZ->setMax(360);
	scrollbarRotZ->setPos(0);
	// Location Scale (Расположение :: Маштаб)
    
	env->addStaticText(L"Scale:", rect<s32>(35, 245, 80, 265), false, false, wnd);
    env->addStaticText(L"X:", core::rect<s32>(10,272,18,290), false, false, wnd);
	editboxScaleX = env->addEditBox(L"1.0", rect<s32>(25 , 270, 85, 290), true, wnd, GUI_ID_X_SCALE);
	env->addStaticText(L"Y:", core::rect<s32>(10,302,18,320), false, false, wnd);
    editboxScaleY = env->addEditBox(L"1.0", rect<s32>(25 , 300, 85, 320), true, wnd, GUI_ID_Y_SCALE);
	env->addStaticText(L"Z:", core::rect<s32>(10,332,18,350), false, false, wnd);
    editboxScaleZ = env->addEditBox(L"1.0", rect<s32>(25 , 330, 85, 350), true, wnd, GUI_ID_Z_SCALE);

    scrollbarScaleX = env->addScrollBar(true,core::rect<s32>(100 , 270, 280, 290), wnd, GUI_ID_X_SCROLL_SCALE);
	scrollbarScaleX->setMin(-10);
	scrollbarScaleX->setMax(10);
	scrollbarScaleX->setPos(0);
	scrollbarScaleX->setLargeStep(1);
	scrollbarScaleX->setSmallStep(1);

	scrollbarScaleY = env->addScrollBar(true,core::rect<s32>(100 , 300, 280, 320), wnd, GUI_ID_Y_SCROLL_SCALE);
	scrollbarScaleY->setMin(-10);
	scrollbarScaleY->setMax(10);
	scrollbarScaleY->setPos(0);
	scrollbarScaleY->setLargeStep(1);
	scrollbarScaleY->setSmallStep(1);

	scrollbarScaleZ = env->addScrollBar(true,core::rect<s32>(100 , 330, 280, 350), wnd, GUI_ID_Z_SCROLL_SCALE);
	scrollbarScaleZ->setMin(-10);
	scrollbarScaleZ->setMax(10);
	scrollbarScaleZ->setPos(0);
	scrollbarScaleZ->setLargeStep(1); // диапазон шага стрелки
	scrollbarScaleZ->setSmallStep(1);

	CurrentObject.updatePosInfo();
}