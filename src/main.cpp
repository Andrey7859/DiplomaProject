#include <irrlicht.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define BUTTON_SIZE 32
#define OFFSET ((Height / 10) - (BUTTON_SIZE * 2)) / 2

int Width;
int Height;
/*
По этим подключениям остались вопросы

Чтобы иметь возможность использовать файл Irrlicht.DLL, нам нужно связать его с Irrlicht.lib. Мы могли бы установить эту опцию в настройках проекта,
но для простоты мы используем библиотеку pragma comment lib для VisualStudio. На платформах Windows мы должны избавиться от окна консоли, которое
появляется при запуске программы с помощью main(). Это делается второй прагмой. Мы также могли бы использовать метод WinMain, но тогда потеряли бы
независимость от платформы.
*/
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

struct SAppContext
{
	IrrlichtDevice *device;
};

// Define some values that we'll use to identify individual GUI controls.
enum 
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,

	GUI_ID_DIALOG_ROOT_WINDOW,
	GUI_ID_DIALOG_ROOT_2_WINDOW,
	GUI_ID_DIALOG_ROOT_3_WINDOW,

	GUI_ID_X_SCALE,
	GUI_ID_Y_SCALE,
	GUI_ID_Z_SCALE,

	GUI_ID_BUTTON_SET_SCALE,
	GUI_ID_BUTTON_SCALE_MUL10,
	GUI_ID_BUTTON_SCALE_DIV10,

	// Buttons
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
	GUI_ID_ASK_BUTTON,

	GUI_ID_SIMPLE_BUTTON,
	GUI_ID_BBOX_BUTTON,
	GUI_ID_WIREFRAME_BUTTON,

	// Topbar
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

#define EXPLORER_WINDOW_POS_Y BUTTON_SIZE + (OFFSET * 2)// 
#define EXPLORER_WINDOW_POS_H ((Height / 2) - (EXPLORER_WINDOW_POS_Y * 2 - OFFSET))

#define PROPERTIES_WINDOW_POS_H EXPLORER_WINDOW_POS_Y + EXPLORER_WINDOW_POS_H

class MyEventReceiver : public IEventReceiver

{
private:
	SAppContext & Context;

public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)		// Оставляем события относящиеся к приложению интерфейса
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:{
					break;
				}
					
				case GUI_ID_NEW_WINDOW_BUTTON:{
					break;
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;

				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the model filename, selected in the file dialog
					IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
					
				}
				break;
			}
		}

		return false;
	}

};

void createButtonsField(IrrlichtDevice *device, IVideoDriver* driver){
	 // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_2_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, OFFSET, Width, BUTTON_SIZE + (OFFSET * 2)), false, L"qwer", 0, GUI_ID_DIALOG_ROOT_2_WINDOW);
	// wnd->setDraggable(false);
	wnd->setDrawTitlebar(false);

	// create a buttons
	env->addButton(rect<s32>(OFFSET, OFFSET / 2, BUTTON_SIZE + OFFSET, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_ADD_BUTTON, L"ADD", L"Add Fille");
	env->addButton(rect<s32>((OFFSET * 2) + BUTTON_SIZE, OFFSET / 2, (BUTTON_SIZE + OFFSET) * 2, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SAVE_BUTTON, L"SAVE", L"Save project");
	env->addButton(rect<s32>((OFFSET * 3) + (BUTTON_SIZE * 2), OFFSET / 2, (BUTTON_SIZE + OFFSET) * 3, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_DELETE_BUTTON, L"DELETE", L"Deletes the selected element");

	env->addButton(rect<s32>((OFFSET * 5) + (BUTTON_SIZE * 3), OFFSET / 2, (BUTTON_SIZE * 4) + (OFFSET * 5), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SELECT_BUTTON, L"SELECT", L"Object selection");
	env->addButton(rect<s32>((OFFSET * 6) + (BUTTON_SIZE * 4), OFFSET / 2, (BUTTON_SIZE * 5) + (OFFSET * 6), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_BRUSH_BUTTON, L"BRUSH", L"Exits Program");
	env->addButton(rect<s32>((OFFSET * 7) + (BUTTON_SIZE * 5), OFFSET / 2, (BUTTON_SIZE * 6) + (OFFSET * 7), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_MOVE_BUTTON, L"MOVE", L"Moving an object");

	env->addButton(rect<s32>((OFFSET * 9) + (BUTTON_SIZE * 6), OFFSET / 2, (BUTTON_SIZE * 7) + (OFFSET * 9), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_PERSPECTIVE_BUTTON, L"PERSPECTIVE", L"Displaying the scene in the projection window as a perspective.");
	env->addButton(rect<s32>((OFFSET * 10) + (BUTTON_SIZE * 7), OFFSET / 2, (BUTTON_SIZE * 8) + (OFFSET * 10), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_TOP_BUTTON, L"TOP", L"View from above");
	env->addButton(rect<s32>((OFFSET * 11) + (BUTTON_SIZE * 8), OFFSET / 2, (BUTTON_SIZE * 9) + (OFFSET * 11), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_FRONT_BUTTON, L"FRONT", L"Front view");
	env->addButton(rect<s32>((OFFSET * 12) + (BUTTON_SIZE * 9), OFFSET / 2, (BUTTON_SIZE * 10) + (OFFSET * 12), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_LEFT_BUTTON, L"LEFT", L"Left side view");
	env->addButton(rect<s32>((OFFSET * 13) + (BUTTON_SIZE * 10), OFFSET / 2, (BUTTON_SIZE * 11) + (OFFSET * 13), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_ASK_BUTTON, L"SPLIT", L"Exits Program");

	env->addButton(rect<s32>((OFFSET * 15) + (BUTTON_SIZE * 11), OFFSET / 2, (BUTTON_SIZE * 12) + (OFFSET * 15), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SIMPLE_BUTTON, L"EDIT", L"Exits Program");
	env->addButton(rect<s32>((OFFSET * 16) + (BUTTON_SIZE * 12), OFFSET / 2, (BUTTON_SIZE * 13) + (OFFSET * 16), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_BBOX_BUTTON, L"BBoX", L"Exits Program");
	env->addButton(rect<s32>((OFFSET * 17) + (BUTTON_SIZE * 13), OFFSET / 2, (BUTTON_SIZE * 14) + (OFFSET * 17), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_WIREFRAME_BUTTON, L"WIREFRAME", L"Exits Program");
	
	/*driver->draw2DLine(position2d<s32>( 300, 300 ),
	 position2d<s32>( 600, 600) ,
	  SColor(255,0,0,255));*/
}

void createToolBox(IrrlichtDevice *device)
{
    // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window   Toolset   от края     отступ с верху   ширина    длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, EXPLORER_WINDOW_POS_Y, 300, PROPERTIES_WINDOW_POS_H), false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);


    // create tab control and tabs
    // IGUITabControl* tab = env->addTabControl(
    //     rect<s32>(2,20,800-602,480-7), wnd, true, true);

    // IGUITab* t1 = tab->addTab(L"Config");

    // add some edit boxes and a button to tab one
    env->addStaticText(L"X:", rect<s32>(22,48,40,66), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(40,46,130,66), true, wnd, GUI_ID_X_SCALE);

    env->addStaticText(L"Y:", rect<s32>(22,82,40,96), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(40,76,130,96), true, wnd, GUI_ID_Y_SCALE);

    env->addStaticText(L"Z:", rect<s32>(22,108,40,126), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(40,106,130,126), true, wnd, GUI_ID_Z_SCALE);

    env->addButton(rect<s32>(10,134,85,165), wnd, GUI_ID_BUTTON_SET_SCALE, L"Set");

    // quick scale buttons
    env->addButton(rect<s32>(65,20,95,40), wnd, GUI_ID_BUTTON_SCALE_MUL10, L"* 10");
    env->addButton(rect<s32>(100,20,130,40), wnd, GUI_ID_BUTTON_SCALE_DIV10, L"* 0.1");

    //updateScaleInfo(Model);

}

void createExplorer(IrrlichtDevice *device)
{
    // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_3_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window			 от края     отступ с верху   ширина    длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, PROPERTIES_WINDOW_POS_H, 300, PROPERTIES_WINDOW_POS_H * 2), false, L"Explorer", 0, GUI_ID_DIALOG_ROOT_3_WINDOW);


    // create tab control and tabs
    // IGUITabControl* tab = env->addTabControl(
    //     rect<s32>(2,20,800-602,480-7), wnd, true, true);

    // IGUITab* t1 = tab->addTab(L"Config");


    //updateScaleInfo(Model);

}

int main(int argc,char **argv){
	/*
	1. deviceType рендер с помощью которого будет отрисовывать можно заменить на:
	EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8, EDT_DIRECT3D9 или EDT_OPENGL.
	
	*/
	glutInit(&argc,argv);
	Width = glutGet(GLUT_SCREEN_WIDTH);
    Height = glutGet(GLUT_SCREEN_HEIGHT);

	IrrlichtDevice *device =
		createDevice(video::EDT_SOFTWARE, dimension2d<u32>(Width, Height), 16,
			true, false, false, 0);

	if (!device)
		return 1;

	/*
	Обратите внимание, что перед строкой стоит буква «L». Irrlicht Engine использует широкие строки символов при отображении текста.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	// device->setResizable(true);

	/*
	Создаем указатели на VideoDriver, SceneManager и  GUI
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();




	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	// create menu
	gui::IGUIContextMenu* menu = guienv->addMenu();
	menu->addItem(L"File", -1, true, true);		// 0	
	menu->addItem(L"Edit", -1, true, true);		// 1
	menu->addItem(L"View", -1, true, true);		// 2
	menu->addItem(L"Help", -1, true, true);		// 3

	// File
	gui::IGUIContextMenu* submenu;
	submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open Model File", GUI_ID_OPEN_MODEL);
	submenu->addItem(L"Save", GUI_ID_SAVE_MODEL);
	submenu->addItem(L"Delete", GUI_ID_DELETE_MODEL);
	submenu->addSeparator();
	submenu->addItem(L"Quit", GUI_ID_QUIT);

	// Edit
	submenu = menu->getSubMenu(1);
	submenu->addItem(L"Select", GUI_ID_SELECT);
	submenu->addItem(L"Brush", GUI_ID_BRUSH);
	submenu->addItem(L"Move", GUI_ID_MOVE);

	// // View
	submenu = menu->getSubMenu(2);
	submenu->addItem(L"View", GUI_ID_VIEW, true, true);
	submenu->addItem(L"Camera", GUI_ID_CAMERA, true, true);

		submenu = menu->getSubMenu(2)->getSubMenu(0);
		submenu->addItem(L"Solid", GUI_ID_SOLID);
		submenu->addItem(L"Wareframe", GUI_ID_WAREFRANE);
		submenu->addItem(L"Reflection", GUI_ID_REFLECATION);

		submenu = menu->getSubMenu(2)->getSubMenu(1);
		submenu->addItem(L"Perspective", GUI_ID_PERSPECTIVE);
		submenu->addItem(L"Top", GUI_ID_TOP);
		submenu->addItem(L"Left", GUI_ID_LEFT);
		submenu->addItem(L"Front", GUI_ID_FRONT);
		submenu->addItem(L"Back", GUI_ID_BACK);
		submenu->addItem(L"Bottom", GUI_ID_BOTTOM);
		submenu->addItem(L"Right", GUI_ID_RIGHT);

	// // About
	submenu = menu->getSubMenu(3);
	submenu->addItem(L"About", GUI_ID_ABOUT);

	//
	SAppContext context;
	context.device = device;
	 

	IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	if (!mesh){
		device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node){
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_RUN);
		node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	createButtonsField(device, driver);
	createToolBox(device);
	createExplorer(device);

	while(device->run()){
		// if (device->isWindowActive()) {
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	// }
	}

	device->drop();

	return 0;
}
