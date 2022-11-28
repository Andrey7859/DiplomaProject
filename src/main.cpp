#include "headers/Model.hpp"
#include "headers/Camera.hpp"
#include "headers/Explorer.hpp"
#include "headers/Toolset.hpp"
#include "headers/Toolbar.hpp"
#include "headers/Receiver.hpp"

/*
Чтобы использовать файл Irrlicht.DLL, нам нужно связать его с Irrlicht.lib. Для этого мы используем библиотеку pragma comment lib для VisualStudio. 
На платформах Windows мы должны избавиться от окна консоли, котороепоявляется при запуске программы с помощью main(). Это делается второй прагмой.
 Мы также могли бы использовать метод WinMain, но тогда потеряли бы независимость от платформы.
 */
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc,char **argv){
	glutInit(&argc,argv);
	Width = glutGet(GLUT_SCREEN_WIDTH);
    Height = glutGet(GLUT_SCREEN_HEIGHT);

	device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(Width, Height), 16, true, false, false, 0);

	if (!device)
		return 1;

	//Название проекта Обратите внимание, что перед строкой стоит буква «L». Irrlicht Engine использует широкие строки символов при отображении текста. 
	device->setWindowCaption(L"Level Editor");

	driver = device->getVideoDriver(); // Видеодрайвер для отрисовки
	smgr = device->getSceneManager(); // создает объекты управляет перемещает менеджер делает управление
	env = device->getGUIEnvironment();

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("../media/fonthaettenschweiler.bmp");

	//шрифт
	if (font)
		skin->setFont(font); 
	else
		skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

	createMenu();
 	createCameras();
	createButtonsField();
	createToolset();
	createExplorer();
	MapList->setSelected(0); // Выделяет модель при запуске

    // driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
        driver->getTexture("../media/irrlicht2_up.jpg"),
        driver->getTexture("../media/irrlicht2_dn.jpg"),
        driver->getTexture("../media/irrlicht2_lf.jpg"),
        driver->getTexture("../media/irrlicht2_rt.jpg"),
        driver->getTexture("../media/irrlicht2_ft.jpg"),
        driver->getTexture("../media/irrlicht2_bk.jpg"));
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


	// Создаем объект receiver на основе класса MyEventReceiver.
					//Накопитель
	MyEventReceiver receiver;

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);


	while(device->run()){
		// if (device->isWindowActive()) {
		driver->beginScene(true, true, SColor(255 ,64 ,64 ,64)); // Отчищает буфер глубина каждый кадр

		cameraViewState();

		smgr->drawAll();
		env->drawAll();

		driver->endScene();
	// }
	}

	device->drop();

	return 0;
}
