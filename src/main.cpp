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

#define BUTTON_SIZE 64
#define OFFSET ((Height / 10) - BUTTON_SIZE) / 2

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
	s32				counter;
	IGUIListBox*	listbox;
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

	//
	GUI_ID_ADD_BUTTON,
	GUI_ID_EDIT_BUTTON
};

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

			/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
			*/
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					Context.listbox->addItem(L"Window created");
					
					Context.counter += 30;
					if (Context.counter > 200)
						Context.counter = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me",
						rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					// There are some options for the file open dialog
					// We set the title, make it a modal window, and make sure
					// that the working directory is restored after the dialog
					// is finished.
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
					
					Context.listbox->addItem(dialog->getFileName());
				}
				break;
			}
		}

		return false;
	}

};

void createButtonsField(IrrlichtDevice *device){
	 // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_2_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, 0, Width, Height / 10), false, L"qwer", 0, GUI_ID_DIALOG_ROOT_2_WINDOW);
	wnd->setDraggable(false);
	wnd->setDrawTitlebar(false);

	// create a buttons
	env->addButton(rect<s32>(OFFSET, OFFSET, BUTTON_SIZE + OFFSET, BUTTON_SIZE + OFFSET), wnd, GUI_ID_ADD_BUTTON, L"ADD", L"Exits Program");
	env->addButton(rect<s32>((OFFSET * 2) + BUTTON_SIZE, OFFSET, (BUTTON_SIZE + OFFSET) * 2, BUTTON_SIZE + OFFSET), wnd, GUI_ID_EDIT_BUTTON, L"EDIT", L"Exits Program");
	env->addButton(rect<s32>((OFFSET * 3) + BUTTON_SIZE, OFFSET, (BUTTON_SIZE + OFFSET) * 3, BUTTON_SIZE + OFFSET), wnd, GUI_ID_EDIT_BUTTON, L"EDIT", L"Exits Program");

}

void createToolBox(IrrlichtDevice *device)
{
    // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, ((Height * 9) / 10) / 2, 300, ((Height * 9) / 10)), false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);


    // create tab control and tabs
    // IGUITabControl* tab = env->addTabControl(
    //     rect<s32>(2,20,800-602,480-7), wnd, true, true);

    // IGUITab* t1 = tab->addTab(L"Config");

    // add some edit boxes and a button to tab one
    env->addStaticText(L"Scale:",
            rect<s32>(10,20,60,45), false, false, 0);
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

    // create the toolbox window
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, Height / 10, 300, ((Height * 9) / 10) / 2), false, L"Explorer", 0, GUI_ID_DIALOG_ROOT_3_WINDOW);


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

	//
	
	guienv->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exits Program");
	guienv->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON, L"New Window", L"Launches a new Window");
	guienv->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON, L"File Open", L"Opens a file");

	guienv->addStaticText(L"Logging ListBox:", rect<s32>(50,110,250,130), true);
	IGUIListBox * listbox = guienv->addListBox(rect<s32>(50, 140, 250, 210));

	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;
	 
	//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
	//	rect<s32>(10,10,260,22), false);

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

	createButtonsField(device);
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
