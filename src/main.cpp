#include "headers/Header.hpp"
#include "headers/Model.hpp"

int Width;
int Height;
/*
По этим подключениям остались вопросы

Чтобы использовать файл Irrlicht.DLL, нам нужно связать его с Irrlicht.lib. Для этого мы используем библиотеку pragma comment lib для VisualStudio. 
На платформах Windows мы должны избавиться от окна консоли, котороепоявляется при запуске программы с помощью main(). Это делается второй прагмой.
 Мы также могли бы использовать метод WinMain, но тогда потеряли бы независимость от платформы.
*/
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


const wchar_t* _path;
char buffer[BUFSIZE];
int ret;
ISceneNode** selectedModel;
Model CurrentObject;
std::vector<Model> Objects;

ISceneNode* node;


// Макросы для работы с окнами
#define EXPLORER_WINDOW_POS_Y BUTTON_SIZE + (OFFSET * 2)// 
#define EXPLORER_WINDOW_POS_H ((Height / 2) - (EXPLORER_WINDOW_POS_Y * 2 - OFFSET))
#define PROPERTIES_WINDOW_POS_H EXPLORER_WINDOW_POS_Y + EXPLORER_WINDOW_POS_H
	int itemCounter = 1;	

//Функция выводит элемент загруженный на сцену
void addSceneTreeItem( ISceneNode * parent, IGUITreeViewNode* nodeParent){

	IGUITreeViewNode* node;
	wchar_t msg[128];
	

	s32 imageIndex;
	list<ISceneNode*>::ConstIterator it = parent->getChildren().begin();
	
	for (; it != parent->getChildren().end(); ++it){
	
		switch ( (*it)->getType () )
		{
			case ESNT_Q3SHADER_SCENE_NODE: imageIndex = 0; (*it)->setName("Q3Shader"); break;
			case ESNT_CAMERA: imageIndex = 1; (*it)->setName("Camera"); break;
			case ESNT_EMPTY: imageIndex = 2; (*it)->setName("Empty"); break;
			case ESNT_MESH: imageIndex = 3; (*it)->setName("Mesh"); break;
			case ESNT_OCTREE: imageIndex = 3; (*it)->setName("Octree"); break;
			case ESNT_ANIMATED_MESH: imageIndex = 4; (*it)->setName("AninMesh"); break;
			case ESNT_SKY_BOX: imageIndex = 5; (*it)->setName("Skybox"); break;
			case ESNT_BILLBOARD: imageIndex = 6; (*it)->setName("Billboard"); break;
			case ESNT_PARTICLE_SYSTEM: imageIndex = 7; (*it)->setName("Particle_System"); break;
			case ESNT_TEXT: imageIndex = 8; (*it)->setName("Text"); break;
			default:imageIndex = -1; break;
		}

		if ( imageIndex < 0 )
		{
			swprintf ( msg, 128, L"%hs,%hs_%d", device->getSceneManager()->getSceneNodeTypeName ( (*it)->getType () ),(*it)->getName(), itemCounter);
		}
		else
		{
			swprintf ( msg, 128, L"%hs_%d",(*it)->getName(), itemCounter);
			// printf("\n\tname: %s\n", (*it)->getName());
		}
		
		// cout << "\n\tname_split:  " << (*it)->getName() << endl;
		node = nodeParent->addChildBack( msg, 0, imageIndex ); //Добавляет ребенка в конец списка элеметов родтеля(Список является частью дерева)
		

		// Отностся к анимированным элементам
		// list<ISceneNodeAnimator*>::ConstIterator ait = (*it)->getAnimators().begin();
		// for (; ait != (*it)->getAnimators().end(); ++ait)
		// {
		// 	imageIndex = -1;
		// 	swprintf ( msg, 128, L"%hs", device->getSceneManager()->getAnimatorTypeName ( (*ait)->getType () ));

		// 	switch ( (*ait)->getType() )
		// 	{
		// 		case ESNAT_FLY_CIRCLE:
		// 		case ESNAT_FLY_STRAIGHT:
		// 		case ESNAT_FOLLOW_SPLINE:
		// 		case ESNAT_ROTATION:
		// 		case ESNAT_TEXTURE:
		// 		case ESNAT_DELETION:
		// 		case ESNAT_COLLISION_RESPONSE:
		// 		case ESNAT_CAMERA_FPS:
		// 		case ESNAT_CAMERA_MAYA:
		// 		default:
		// 			break;
		// 	}
		// 	node->addChildBack( msg, 0, imageIndex );
		// }

		addSceneTreeItem ( *it, node);
	}
	itemCounter++;
	// cout << "\t ItemCounter    " << itemCounter << endl;

}

//Создание подокна отображеия на сцене Scene Explorer
void addSceneExplorerTree(IGUITab* t1){	

    SceneTree = device->getGUIEnvironment()->addTreeView(rect<s32>( 0, OFFSET, 300, PROPERTIES_WINDOW_POS_H ), t1, -1, true, true, false );
    SceneTree->setToolTipText ( L"Show the current Scenegraph" );
    SceneTree->getRoot()->clearChildren();
	itemCounter = 1;
    addSceneTreeItem (device->getSceneManager()->getRootSceneNode(), SceneTree->getRoot());

	IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "../media/iconlist.png" ), dimension2di( 32, 32 ), true );

    if ( imageList ){
        SceneTree->setImageList( imageList );
        imageList->drop ();
    }

}




// Создаем класс для отлавливания обработки всех событий
class MyEventReceiver : public IEventReceiver
{

public:
	MyEventReceiver()  { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)		// Оставляем события относящиеся к приложению интерфейса
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = device->getGUIEnvironment(); //Это окружение графического интерфейса пользователя(Список виджетов ползунков
																		//которые харнятся в движке)

			// int x, y, z;
			IGUIElement* toolboxWnd;
			const wchar_t* text;
			wchar_t* end;

			switch(event.GUIEvent.EventType)
			{
			case EGET_TREEVIEW_NODE_SELECT:
				{
					// ISceneNode* tmp = (ISceneNode*) SceneTree->getSelected();
					ISceneNode* tmp = (ISceneNode*) SceneTree->getSelected();

					for (int i = 0; i < Objects.size(); i++) {
						// cout << tmp.getPosition().X << endl;
						// if (Objects[i].getCoord()->X == tmp->getPosition().X){
						// 	if (Objects[i].getCoord()->Y == tmp->getPosition().Y){
						// 		if (Objects[i].getCoord()->Z == tmp->getPosition().Z){
									// CurrentObject = Objects[i];
						// 		}
						// 	}
						// }
					}
					
					
					// cout << "\t\tid: " << tmp->getID() << endl;
				}
				break;

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

				case GUI_ID_ADD_BUTTON:{
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;
				}
				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					Model* tmp = new Model; 
					IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
					tmp->LoadModel(core::stringc(dialog->getFileName()).c_str());
					Objects.push_back(*tmp);
					SceneTree->getRoot()->clearChildren();
					itemCounter = 1;
					addSceneTreeItem(device->getSceneManager()->getRootSceneNode(), SceneTree->getRoot());




				}
				break;
			case EGET_EDITBOX_ENTER:
				switch(id){
					case GUI_ID_X_POS:
					    toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_X_POS, true)->getText();
						
						 // wcstol осуществляет перевод из const wchar_t в int
						CurrentObject.getCoord()->X = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setCoord(*CurrentObject.getCoord());
						break;
					case GUI_ID_Y_POS:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Y_POS, true)->getText();
						
						CurrentObject.getCoord()->Y = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setCoord(*CurrentObject.getCoord());
						break;
					case GUI_ID_Z_POS:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Z_POS, true)->getText();
						
						CurrentObject.getCoord()->Z = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setCoord(*CurrentObject.getCoord());
						break;

					case GUI_ID_X_ROT:
					    toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_X_ROT, true)->getText();
						
						CurrentObject.getRotation()->X = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setRotation(*CurrentObject.getRotation());
						break;
					case GUI_ID_Y_ROT:
					    toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Y_ROT, true)->getText();
						
						CurrentObject.getRotation()->Y = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setRotation(*CurrentObject.getRotation());
						break;
					case GUI_ID_Z_ROT:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Z_ROT, true)->getText();
						
						CurrentObject.getRotation()->Z = wcstol(text, NULL, 10); // осуществляет перевод из const wchar_t в int
						CurrentObject.setRotation(*CurrentObject.getRotation());
					break;

					case GUI_ID_X_SCALE:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_X_SCALE, true)->getText();
						
						CurrentObject.getScale()->X = wcstof(text, &end); // осуществляет перевод из const wchar_t в float
						CurrentObject.setScale(*CurrentObject.getScale());
					break;
					case GUI_ID_Y_SCALE:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Y_SCALE, true)->getText();
						
						CurrentObject.getScale()->Y = wcstof(text, &end); // осуществляет перевод из const wchar_t в float
						CurrentObject.setScale(*CurrentObject.getScale());
					break;
					case GUI_ID_Z_SCALE:
						toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
						text = toolboxWnd->getElementFromId(GUI_ID_Z_SCALE, true)->getText();
						
						CurrentObject.getScale()->Z = wcstof(text, &end); // осуществляет перевод из const wchar_t в float
						CurrentObject.setScale(*CurrentObject.getScale());
					break;
				}

				CurrentObject.updatePosInfo();
			}

		}

		return false;
	}

};

//Функция создание поля кнопок

void createButtonsField(IVideoDriver* driver){
	IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_2_WINDOW, true);



	if (e)
        e->remove();

    // Создание верхней панели для кнопок
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, OFFSET, Width, BUTTON_SIZE + (OFFSET * 2)), false, L"qwer", 0, GUI_ID_DIALOG_ROOT_2_WINDOW);
	wnd->setDrawTitlebar(false);

	IGUIButton* addButton = env->addButton(rect<s32>(OFFSET, OFFSET / 2, BUTTON_SIZE + OFFSET, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_ADD_BUTTON, L" ",L"Add object");
	addButton->setImage(driver->getTexture("../media/icon/add.jpg"));
	
	IGUIButton* saveButton = env->addButton(rect<s32>((OFFSET * 2) + BUTTON_SIZE, OFFSET / 2, (BUTTON_SIZE + OFFSET) * 2, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SAVE_BUTTON, L" ", L"Save project");
	saveButton->setImage(driver->getTexture("../media/icon/save.jpg"));

	IGUIButton* deleteButton = env->addButton(rect<s32>((OFFSET * 3) + (BUTTON_SIZE * 2), OFFSET / 2, (BUTTON_SIZE + OFFSET) * 3, BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_DELETE_BUTTON, L" ", L"Delete project");
	deleteButton->setImage(driver->getTexture("../media/icon/delete.jpg"));

	IGUIButton* selectButton = env->addButton(rect<s32>((OFFSET * 5) + (BUTTON_SIZE * 3), OFFSET / 2, (BUTTON_SIZE * 4) + (OFFSET * 5), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SELECT_BUTTON, L" ", L"Select object");
	selectButton->setImage(driver->getTexture("../media/icon/select.jpg"));

	IGUIButton* moveButton = env->addButton(rect<s32>((OFFSET * 6) + (BUTTON_SIZE * 4), OFFSET / 2, (BUTTON_SIZE * 5) + (OFFSET * 6), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_BRUSH_BUTTON, L"", L"Move object");
	moveButton->setImage(driver->getTexture("../media/icon/move.jpg"));

	// Под кисть
	// env->addButton(rect<s32>((OFFSET * 7) + (BUTTON_SIZE * 5), OFFSET / 2, (BUTTON_SIZE * 6) + (OFFSET * 7), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_MOVE_BUTTON, L"MOVE", L"Moving an object");
	
	IGUIButton* perspectiveButton = env->addButton(rect<s32>((OFFSET * 9) + (BUTTON_SIZE * 6), OFFSET / 2, (BUTTON_SIZE * 7) + (OFFSET * 9), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_PERSPECTIVE_BUTTON, L" ", L"Perspective view");
	perspectiveButton->setImage(driver->getTexture("../media/icon/perspective.jpg"));

	IGUIButton* topButton = env->addButton(rect<s32>((OFFSET * 10) + (BUTTON_SIZE * 7), OFFSET / 2, (BUTTON_SIZE * 8) + (OFFSET * 10), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_TOP_BUTTON, L" ", L"Top view");
	topButton->setImage(driver->getTexture("../media/icon/top.jpg"));

	IGUIButton* frontButton = env->addButton(rect<s32>((OFFSET * 11) + (BUTTON_SIZE * 8), OFFSET / 2, (BUTTON_SIZE * 9) + (OFFSET * 11), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_FRONT_BUTTON, L" ", L"Front view");
	frontButton->setImage(driver->getTexture("../media/icon/front.jpg"));
	
	IGUIButton* leftButton = env->addButton(rect<s32>((OFFSET * 12) + (BUTTON_SIZE * 9), OFFSET / 2, (BUTTON_SIZE * 10) + (OFFSET * 12), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_LEFT_BUTTON, L" ", L"Left view");
	leftButton->setImage(driver->getTexture("../media/icon/left.jpg"));

	IGUIButton* splitButton = env->addButton(rect<s32>((OFFSET * 13) + (BUTTON_SIZE * 10), OFFSET / 2, (BUTTON_SIZE * 11) + (OFFSET * 13), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_ASK_BUTTON, L" ", L"Split view");
	splitButton->setImage(driver->getTexture("../media/icon/split.png"));

	IGUIButton* simpleButton = env->addButton(rect<s32>((OFFSET * 15) + (BUTTON_SIZE * 11), OFFSET / 2, (BUTTON_SIZE * 12) + (OFFSET * 15), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_SIMPLE_BUTTON, L" ", L"Solid");
	simpleButton->setImage(driver->getTexture("../media/icon/simple.jpg"));

	IGUIButton* bboxButton = env->addButton(rect<s32>((OFFSET * 16) + (BUTTON_SIZE * 12), OFFSET / 2, (BUTTON_SIZE * 13) + (OFFSET * 16), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_BBOX_BUTTON, L" ", L"Bounding Box");
	bboxButton->setImage(driver->getTexture("../media/icon/bbox.png"));

	IGUIButton* wireFrameButton = env->addButton(rect<s32>((OFFSET * 17) + (BUTTON_SIZE * 13), OFFSET / 2, (BUTTON_SIZE * 14) + (OFFSET * 17), BUTTON_SIZE + (OFFSET / 2)), wnd, GUI_ID_WIREFRAME_BUTTON, L" ", L"Wire Frame");
	wireFrameButton->setImage(driver->getTexture("../media/icon/wireFrame.jpg"));

}


// Функция созданя Toolset -> Properties
void createToolBox()
{
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    
	if (e)
        e->remove();

    // Созадния окна Toolset   				  от края     отступ с верху   ширина    длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, EXPLORER_WINDOW_POS_Y, 300, PROPERTIES_WINDOW_POS_H), false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);


    // create tab control and tabs
    // IGUITabControl* tab = env->addTabControl(
    //     rect<s32>(2,20,800-602,480-7), wnd, true, true);

    // IGUITab* t1 = tab->addTab(L"Config");

	int x0 = 100;
	int between= 30;
	int w = 30;

    // Location (Расположение)
    env->addStaticText(L"Location:", rect<s32>(5,5 + OFFSET ,100,25 + OFFSET), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(x0,5+ OFFSET ,x0 + w,25+ OFFSET ), true, wnd, GUI_ID_X_POS);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between, 5 + OFFSET  ,x0 + w + between + w,25 + OFFSET ), true, wnd, GUI_ID_Y_POS);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between + w + between,5 + OFFSET , x0 + w + between + w + between + w  ,25 + OFFSET ), true, wnd, GUI_ID_Z_POS);

	// Location Rotation (Расположение :: Варащение)
    env->addStaticText(L"Rotation:", rect<s32>(5,30 + OFFSET ,100,50 + OFFSET ), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(x0,30+  OFFSET ,x0 + w,50 + OFFSET ), true, wnd, GUI_ID_X_ROT);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between,30 + OFFSET ,x0 + w + between + w,50 + OFFSET ), true, wnd, GUI_ID_Y_ROT);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between + w + between,30 + OFFSET , x0 + w + between + w + between + w  ,50 + OFFSET), true, wnd, GUI_ID_Z_ROT);

	// Location Scale (Расположение :: Маштаб)
    env->addStaticText(L"Scale:", rect<s32>(5,55 + OFFSET ,100,75 + OFFSET ), false, false, wnd);
    env->addEditBox(L"1.0", rect<s32>(x0,55 +  OFFSET ,x0 + w,75 + OFFSET ), true, wnd, GUI_ID_X_SCALE);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between,55 + OFFSET ,x0 + w + between + w,75 + OFFSET ), true, wnd, GUI_ID_Y_SCALE);

    env->addEditBox(L"1.0", rect<s32>(x0 + w + between + w + between,55 + OFFSET , x0 + w + between + w + between + w  ,75 + OFFSET), true, wnd, GUI_ID_Z_SCALE);
    
	CurrentObject.updatePosInfo();
}

// Структура для работы с функцией scaner
typedef struct filesListStruct {
	std::string str;
	int type;
} filesList;










//Функция выводит количество файлов в директории (Нужна для подсчета точного количесво выделяемой оперативной памяти)
int filesCounter(std::string path) {
	DIR *dir;
	struct dirent *ent;
	int filesListSize = 0;

	if ((dir = opendir (path.c_str())) != NULL) { //Сравнивает получилось ли открыть директорию
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0) // ".." / "." - Это ссылки для перехода в другую директрию. Здесь прои
																				//сходит исключение этих ссылкок из общего списка файлов
                continue;

			filesListSize++;
		}

		closedir (dir);
	} 

	return filesListSize;
}

// Заполняет массив структур файлами и папками и выводит кол-во файлов в директории
int scaner(filesList filesList[BUFSIZE], std::string path){
	DIR *dir;
	struct dirent *ent;
	int filesListSize = 0;

	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
                continue;

			filesList[filesListSize].str.append(ent->d_name);
			filesList[filesListSize].type = ent->d_type;
			filesListSize++;
		}

		closedir (dir);
	} 

	return filesListSize;
}

//Данная функция позволяет повторно сканировать директории по которым проходимся
void addContentBrowserTreeItem(IGUITreeViewNode* nodeParent, std::string path){
	int counter = filesCounter(path); // Число файлов в директории
	wchar_t wc[counter];
	filesList filesList[counter];

	int size = scaner(filesList, path); //Заполняет количеством файлов, заполняет массив filesList
	IGUITreeViewNode** node = new IGUITreeViewNode*[size];		// !!! Выделение динамической памяти массива

	for(int i = 0; i < size; i++) {
		mbstowcs(wc, filesList[i].str.c_str(), size);	// Данная функция переобразует тип данных char*   в   wchar_t* 
		node[i] = nodeParent->addChildBack(wc, 0);	// Заполняем встроенную струк.данных (дерево) чтобы использовать (gui Обозревателя(Explorer))

		if(filesList[i].type == DT_DIR) // Если текущий элемент папка
			addContentBrowserTreeItem(node[i], path + "/" + filesList[i].str);  // 1 элемент явялется родителем, 2 элемент берет путь добавляет "/"
																				//и добавляет текущий элемент (Рекурсия)
	}
}

//Создание подокна отображеия на сцене Contetnt Browser 
void addContentBrowserTree(IGUITab* t2) {
	IGUITreeView* SceneTree;

    SceneTree = device->getGUIEnvironment()->addTreeView(rect<s32>( 0, OFFSET, 300, PROPERTIES_WINDOW_POS_H ), t2, -1, true, true, false );
    SceneTree->setToolTipText ( L"Show all files and folders into project" );
    SceneTree->getRoot()->clearChildren();
    addContentBrowserTreeItem (SceneTree->getRoot(), ".");

	// IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "media/iconlist.png" ), dimension2di( 32, 32 ), true );

    // if ( imageList ){
    //     SceneTree->setImageList( imageList );
    //     imageList->drop ();
    // }
}






//Создание окна Explorer 
void createExplorer()
{
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_3_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window			 от края     отступ с верху   ширина    длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, PROPERTIES_WINDOW_POS_H, 300, PROPERTIES_WINDOW_POS_H * 2), false, L"Explorer", 0, GUI_ID_DIALOG_ROOT_3_WINDOW);


    //create tab control and tabs
    IGUITabControl* tab = env->addTabControl(
        rect<s32>(0, OFFSET, 300, PROPERTIES_WINDOW_POS_H - (3 * OFFSET)), wnd, true, true);

    IGUITab* t1 = tab->addTab(L"Scene explorer");
	addSceneExplorerTree(t1);					// Объекты на сцене 

    IGUITab* t2 = tab->addTab(L"Content browser");
	addContentBrowserTree(t2);					// Список файлов в папке проекта

    //Model.updatePosInfo(Model);
}

int main(int argc,char **argv){

	glutInit(&argc,argv);
	Width = glutGet(GLUT_SCREEN_WIDTH);
    Height = glutGet(GLUT_SCREEN_HEIGHT);

	device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(Width, Height), 16, true, false, false, 0);

	if (!device)
		return 1;

	/*
	Обратите внимание, что перед строкой стоит буква «L». Irrlicht Engine использует широкие строки символов при отображении текста.
	*/
	device->setWindowCaption(L"Level Editor");
	// device->setResizable(true);

	//IGUITreeViewNode * select = SceneTree->getSelected(); //Получение выбраного из дерева
	//selectedModel = (ISceneNode*)select->getData(); // Преобразование типов данных

	/*
	Создаем указатели на VideoDriver, SceneManager и  GUI
	*/
	IVideoDriver* driver = device->getVideoDriver(); // Видеодрайвер для отрисовки
	ISceneManager* smgr = device->getSceneManager(); // создает объекты управляет перемещает менеджер делает управление
	IGUIEnvironment* guienv = device->getGUIEnvironment(); //Список всех виджитов

	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("../media/fontlucida.png");
	if (font)
		skin->setFont(font); //шрифт
	else
		skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	// Создания меню
	gui::IGUIContextMenu* menu = guienv->addMenu();
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
	submenu->addItem(L"Select", GUI_ID_SELECT);
	submenu->addItem(L"Brush", GUI_ID_BRUSH);
	submenu->addItem(L"Move", GUI_ID_MOVE);

	// Вкладка вид (View)
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

	// Вкладка о прииложении (About)
	submenu = menu->getSubMenu(3);
	submenu->addItem(L"About", GUI_ID_ABOUT);

	 
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
	camera->setName("Camera");

	// Model.LoadModel(StartUpModelFile.c_str());
	Model* tmp = new Model;
	tmp->LoadModel(StartUpModelFile.c_str());
	Objects.push_back(*tmp);
	CurrentObject = Objects[0];





	createButtonsField(driver);
	createToolBox();
	createExplorer();
	
	// Создаем объект receiver на основе класса MyEventReceiver. Передаем структуру Context
					//Накопитель
	MyEventReceiver receiver;

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);






	while(device->run()){
		// if (device->isWindowActive()) {
		driver->beginScene(true, true, SColor(255,100,101,140)); // Отчищает буфер глубина каждый кадр

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	// }
	}

	device->drop();

	return 0;
}
