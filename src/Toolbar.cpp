#include "headers/Toolbar.hpp"

// Создания меню
void createMenu(){
	gui::IGUIContextMenu* menu = env->addMenu();
	menu->addItem(L"File", -1, true, true);		// 0	
	menu->addItem(L"Edit", -1, true, true);		// 1
	menu->addItem(L"View", -1, true, true);		// 2
	menu->addItem(L"Help", -1, true, true);		// 3

	// Вкладкай файл (file)
	gui::IGUIContextMenu* submenu;
	submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open Model File", GUI_ID_OPEN_MODEL);
	submenu->addItem(L"Save", GUI_ID_SAVE_MODEL);
	submenu->addItem(L"Delete", GUI_ID_DELETE_MODEL);
	submenu->addSeparator();
	submenu->addItem(L"Quit", GUI_ID_QUIT);

	// Вкладка редактровать (Edit)
	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Brush", GUI_ID_BRUSH);

	// Вкладка вид (View)
	submenu = menu->getSubMenu(2);
	submenu->addItem(L"Camera", GUI_ID_VIEW, true, true);
	submenu->addItem(L"View", GUI_ID_CAMERA, true, true);
		//Подвкладка вид 1(View)
		submenu = menu->getSubMenu(2)->getSubMenu(0);
		submenu->addItem(L"Perspective", GUI_ID_PERSPECTIVE);
		submenu->addItem(L"Top", GUI_ID_TOP);
		submenu->addItem(L"Left", GUI_ID_LEFT);
		submenu->addItem(L"Front", GUI_ID_FRONT);
		submenu->addItem(L"Split", GUI_ID_SPLIT);
		//Подвкладка вид 2(View)
		submenu = menu->getSubMenu(2)->getSubMenu(1);
		submenu->addItem(L"Bounding box", GUI_ID_BBOX);
		submenu->addItem(L"Wareframe", GUI_ID_WAREFRANE);
		
	// Вкладка о прииложении (About)
	submenu = menu->getSubMenu(3);
	submenu->addItem(L"About", GUI_ID_ABOUT);
}

//Функция создание поля кнопок
void createButtonsField(){
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_2_WINDOW, true);

	if (e)
        e->remove();

    // Создание верхней панели для кнопок
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, OFFSET, Width, BUTTON_SIZE + (OFFSET * 2)), false, L"Q", 0, GUI_ID_DIALOG_ROOT_2_WINDOW);
	wnd->setDrawTitlebar(false);

	// Создание кнопок 1 Блок (Добавить, сохранить, удалить)
	IGUIButton* addButton = env->addButton(rect<s32>(OFFSET, OFFSET / 2 , BUTTON_SIZE + OFFSET, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_ADD_BUTTON, L" ",L"Add object");
	addButton->setImage(driver->getTexture("../media/icon/add.jpg"));
	
	IGUIButton* saveButton = env->addButton(rect<s32>((OFFSET * 2) + BUTTON_SIZE, OFFSET / 2, (BUTTON_SIZE + OFFSET) * 2, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SAVE_BUTTON, L" ", L"Save project");
	saveButton->setImage(driver->getTexture("../media/icon/save.jpg"));

	IGUIButton* deleteButton = env->addButton(rect<s32>((OFFSET * 3) + (BUTTON_SIZE * 2), OFFSET / 2, (BUTTON_SIZE + OFFSET) * 3, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_DELETE_BUTTON, L" ", L"Delete project");
	deleteButton->setImage(driver->getTexture("../media/icon/delete.jpg"));

	// Создание кнопок 2 Блок (Выбор)
	IGUIButton* selectButton = env->addButton(rect<s32>((OFFSET * 5) + (BUTTON_SIZE * 3), OFFSET / 2, (BUTTON_SIZE * 4) + (OFFSET * 5), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SELECT_BUTTON, L" ", L"Select object");
	selectButton->setImage(driver->getTexture("../media/icon/select.jpg"));

	//Создание кнопок 3 Блок (Перспектива, топ, фронт, лефт, сплит)
	IGUIButton* perspectiveButton = env->addButton(rect<s32>((OFFSET * 7) + (BUTTON_SIZE * 4), OFFSET / 2, (BUTTON_SIZE * 5) + (OFFSET * 7), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_PERSPECTIVE_BUTTON, L" ", L"Perspective view");
	perspectiveButton->setImage(driver->getTexture("../media/icon/perspective.jpg"));

	IGUIButton* topButton = env->addButton(rect<s32>((OFFSET * 8) + (BUTTON_SIZE * 5), OFFSET / 2, (BUTTON_SIZE * 6) + (OFFSET * 8), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_TOP_BUTTON, L" ", L"Top view");
	topButton->setImage(driver->getTexture("../media/icon/top.jpg"));

	IGUIButton* frontButton = env->addButton(rect<s32>((OFFSET * 9) + (BUTTON_SIZE * 6), OFFSET / 2, (BUTTON_SIZE * 7) + (OFFSET * 9), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_FRONT_BUTTON, L" ", L"Front view");
	frontButton->setImage(driver->getTexture("../media/icon/front.jpg"));
	
	IGUIButton* leftButton = env->addButton(rect<s32>((OFFSET * 10) + (BUTTON_SIZE * 7), OFFSET / 2, (BUTTON_SIZE * 8) + (OFFSET * 10), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_LEFT_BUTTON, L" ", L"Left view");
	leftButton->setImage(driver->getTexture("../media/icon/left.jpg"));

	IGUIButton* splitButton = env->addButton(rect<s32>((OFFSET * 11) + (BUTTON_SIZE * 8), OFFSET / 2, (BUTTON_SIZE * 9) + (OFFSET * 11), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SPLIT_BUTTON, L" ", L"Split view");
	splitButton->setImage(driver->getTexture("../media/icon/split.png"));

	//Создание кнопок 4 Блок (bbox, wireFrame)
	IGUIButton* bboxButton = env->addButton(rect<s32>((OFFSET * 13) + (BUTTON_SIZE * 9), OFFSET / 2, (BUTTON_SIZE * 10) + (OFFSET * 13), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_BBOX_BUTTON, L" ", L"Bounding Box");
	bboxButton->setImage(driver->getTexture("../media/icon/bbox.png"));

	IGUIButton* wireFrameButton = env->addButton(rect<s32>((OFFSET * 14) + (BUTTON_SIZE * 10), OFFSET / 2, (BUTTON_SIZE * 11) + (OFFSET * 14), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_WIREFRAME_BUTTON, L" ", L"Wire Frame");
	wireFrameButton->setImage(driver->getTexture("../media/icon/wireFrame.jpg"));
}