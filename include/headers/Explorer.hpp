#pragma once
#include "headers/View.hpp"

// Добавляет элемент сцены (модель) в список
void addMapListItem(){
    wchar_t msg[128];
	s32 imageIndex;
    ISceneNode* node;

	for (int i = 0; i < Objects.size(); i++){
		node = Objects[i].getModel();

		switch (node->getType()){
			case ESNT_Q3SHADER_SCENE_NODE: imageIndex = 0; node->setName("Q3Shader"); ; break;
			case ESNT_CAMERA: imageIndex = 1; node->setName("Camera"); break;
			case ESNT_EMPTY: imageIndex = 2; node->setName("Empty"); break;
			case ESNT_MESH: imageIndex = 3; node->setName("Model"); break;
			case ESNT_OCTREE: imageIndex = 3; node->setName("Octree"); break;
			case ESNT_ANIMATED_MESH: imageIndex = 4; node->setName("AninMesh"); break;
			case ESNT_SKY_BOX: imageIndex = 5; node->setName("Skybox"); break;
			case ESNT_BILLBOARD: imageIndex = 6; node->setName("Billboard"); break;
			case ESNT_PARTICLE_SYSTEM: imageIndex = 7; node->setName("Particle_System"); break;
			case ESNT_TEXT: imageIndex = 8; node->setName("Text"); break;
			default:imageIndex = -1; break;
		}

    	swprintf ( msg, 128, L"%hs_%d", Objects[i].getModel()->getName(),itemCounter); // Копирует строку в msg
		itemCounter++;
	}

	MapList->addItem(msg);
}

//Создание подокна отображеия на сцене Scene Explorer
void addSceneExplorerTree(IGUITab* t1){
    MapList = env->addListBox ( rect<s32>( 0, 0, EXPLORER_WINDOW_WIDTH , PROPERTIES_WINDOW_POS_H), t1, -1, true  );
	MapList->setToolTipText ( L"Show the current Scenegraph" );
	addMapListItem();
	itemCounter = 1;

	// IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "../media/iconlist.png" ), dimension2di( 32, 32 ), true );

    // if ( imageList ){
    //     // SceneTree->setImageList( imageList );
    //     imageList->drop ();
    // }

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
	IGUITreeViewNode** node = new IGUITreeViewNode*[size];		//  Выделение динамической памяти массива

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

    SceneTree = device->getGUIEnvironment()->addTreeView(rect<s32>( 0, OFFSET, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H ), t2, -1, true, true, false );
    SceneTree->setToolTipText ( L"Show all files and folders into project" );
    SceneTree->getRoot()->clearChildren();
    addContentBrowserTreeItem (SceneTree->getRoot(), ".");

	IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "media/iconlist.png" ), dimension2di( 32, 32 ), true );

    if ( imageList ){
        SceneTree->setImageList( imageList );
        imageList->drop ();
    }
}

//Создание окна explorer
void createExplorer()
{
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_3_WINDOW, true);
    
	if (e)
        e->remove();

    // create the toolbox window			 от края     отступ с верху  		 ширина    					длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, PROPERTIES_WINDOW_POS_H, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H * 2), false, L"Explorer", 0, GUI_ID_DIALOG_ROOT_3_WINDOW);

    //create tab control and tabs
    IGUITabControl* tab = env->addTabControl(
        rect<s32>(0, OFFSET, 300, PROPERTIES_WINDOW_POS_H - (3 * OFFSET)), wnd, true, true);

    IGUITab* t1 = tab->addTab(L"Scene explorer");
	addSceneExplorerTree(t1);					// Объекты на сцене 

    IGUITab* t2 = tab->addTab(L"Content browser");
	addContentBrowserTree(t2);					// Список файлов в папке проекта
}