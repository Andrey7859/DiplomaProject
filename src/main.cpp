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

IGUIEditBox* editboxPosX;
IGUIEditBox* editboxPosY;
IGUIEditBox* editboxPosZ;
IGUIEditBox* editboxRotX;
IGUIEditBox* editboxRotY;
IGUIEditBox* editboxRotZ;
IGUIEditBox* editboxScaleX;
IGUIEditBox* editboxScaleY;
IGUIEditBox* editboxScaleZ;

IGUIScrollBar* scrollbarPosX;
IGUIScrollBar* scrollbarPosY;
IGUIScrollBar* scrollbarPosZ;

ICameraSceneNode *camera[4]={0,0,0,0};

const wchar_t* _path;
char buffer[BUFSIZE];
int ret;
ISceneNode** selectedModel;
Model CurrentObject;
std::vector<Model> Objects;

ISceneNode* node;
IGUIListBox* MapList;

int itemCounter = 1;
int objectsCounter = 0;

bool showDebug = false; // Для Wareframe и  BBox

// Макросы для работы с окнами		
#define EXPLORER_WINDOW_POS_Y (BUTTON_SIZE + (OFFSET * 2)) //Отступ с верху 70
#define EXPLORER_WINDOW_POS_H ( Height - EXPLORER_WINDOW_POS_Y - OFFSET ) / 2//Длина Explorer 468
#define PROPERTIES_WINDOW_POS_H EXPLORER_WINDOW_POS_Y + EXPLORER_WINDOW_POS_H // Длина Prorerties 538
#define EXPLORER_WINDOW_WIDTH ((Width / 5) - BUTTON_SIZE) // ширина Explorer и Toolset 304
#define WINDOW_SPLIT_WIDTH (( Width - EXPLORER_WINDOW_WIDTH ) / 2 ) + EXPLORER_WINDOW_WIDTH// Ширина серидины окна сплит 992
#define WINDOW_SPLIT_HEIGHT ((Height - EXPLORER_WINDOW_POS_Y ) / 2 ) + EXPLORER_WINDOW_POS_Y// Высота серидины окна сплит 548
#define OFFSET100 EXPLORER_WINDOW_WIDTH / 3 //101
#define OFF

//Функция выводит элемент загруженный на сцену
void addSceneTreeItem( ISceneNode * parent, IGUITreeViewNode* nodeParent){ 

	IGUITreeViewNode* node;
	wchar_t msg[128];
	

	s32 imageIndex;
	list<ISceneNode*>::ConstIterator it = parent->getChildren().begin();
	
	for (; it != parent->getChildren().end(); ++it){
		switch ( (*it)->getType () )
		{
			case ESNT_Q3SHADER_SCENE_NODE: imageIndex = 0; (*it)->setName("Q3Shader"); ; break;
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
		// MapList->addItem(msg, imageIndex);
		// wchar_t vOut[12];
		// _itow_s(objectsCounter, vOut, sizeof(vOut)/2, 10);
		
		// node->setText(vOut);
		objectsCounter++;

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
	// cout << "\t ItemCounter SMGR   " << smgr->getSceneNodeFromName("Camera_1", *it) << endl; 

}

void addMapListItem(){
    wchar_t msg[128];
	s32 imageIndex;
    ISceneNode* node;

	for (int i = 0; i < Objects.size(); i++)
	{
		node = Objects[i].getModel();
		switch (node->getType())
		{
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

    	swprintf ( msg, 128, L"%hs_%d", Objects[i].getModel()->getName(),itemCounter);
		cout << "Add map list  " << Objects[i].getModel()->getName() << endl;
		itemCounter++;
	}
	

	MapList->addItem(msg);


}


//Создание подокна отображеия на сцене Scene Explorer
void addSceneExplorerTree(IGUITab* t1){	

    // SceneTree = device->getGUIEnvironment()->addTreeView(rect<s32>( 0, OFFSET, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H ), t1, -1, true, true, false );
    MapList = env->addListBox ( rect<s32>( 0, 0, EXPLORER_WINDOW_WIDTH , PROPERTIES_WINDOW_POS_H), t1, -1, true  );
	MapList->setToolTipText ( L"Show the current Scenegraph" );
	addMapListItem();
    // SceneTree->getRoot()->clearChildren();
	itemCounter = 1;
    // addSceneTreeItem (device->getSceneManager()->getRootSceneNode(), SceneTree->getRoot()); 

	// IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "../media/iconlist.png" ), dimension2di( 32, 32 ), true );

    // if ( imageList ){
    //     // SceneTree->setImageList( imageList );
    //     imageList->drop ();
    // }

}

//Обработчик событий нажатий на пункты меню
void OnMenuItemSelected( IGUIContextMenu* menu ){

	s32 id = menu->getItemCommandId(menu->getSelectedItem());
		IGUIEnvironment* env = device->getGUIEnvironment();

	switch(id){
	// Блок меню File
	case GUI_ID_OPEN_MODEL: // Добавить новую модель
		env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
		break;
	case GUI_ID_SAVE_MODEL: // Сохранение
		break;
	case GUI_ID_DELETE_MODEL: // Удалить модель 
		break;
	case GUI_ID_QUIT: // Выход из программы
		device->closeDevice();
		break;
	// Блок меню Edit
	case GUI_ID_BRUSH: // Удалить модель 
		break;
	// Подблок меню View 1
	case GUI_ID_PERSPECTIVE: // Вид перспективы
		currentViewState = GUI_ID_PERSPECTIVE_BUTTON;
		break;
	case GUI_ID_TOP: // Вид сверху
		currentViewState = GUI_ID_TOP_BUTTON;
		break;
	case GUI_ID_FRONT: // Вид фронт 
		currentViewState = GUI_ID_FRONT_BUTTON;
		break;
	case GUI_ID_LEFT: // Вид слева
		currentViewState = GUI_ID_LEFT_BUTTON;
		break;
	case GUI_ID_SPLIT: // Вид слева
		currentViewState = GUI_ID_LEFT_BUTTON;
		break;
	// Подблок меню View 2
	case GUI_ID_BBOX: // BBox
		showDebug=!showDebug;
		CurrentObject.getModel()->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
		break;
	case GUI_ID_WAREFRANE: // Wareframe
		CurrentObject.getModel()->setMaterialFlag(video::EMF_WIREFRAME,
			!CurrentObject.getModel()->getMaterial(0).Wireframe);
		CurrentObject.getModel()->setMaterialFlag(video::EMF_POINTCLOUD, false);		break;
		break;
	case GUI_ID_ABOUT: // О программе
		break;
	}
}

 // float переводит в const wchar_t
const wchar_t* ftows(float value) {
	char vOutChar [20];
	wchar_t text0 [20];

	sprintf(vOutChar, "%f", value);
	mbstowcs(text0,vOutChar,sizeof(vOutChar));
	
	const wchar_t *text1 = text0;

	return text1;
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
					// void * ptr;
					// int* counter_ptr;
					// unsigned long long int counter;
					
					IGUITreeViewNode* tmp = SceneTree->getRoot()->getFirstChild();
					// ptr = tmp->getData();
					// counter_ptr = (int*)ptr;
					// counter = *counter_ptr;
					// const wchar_t* text = tmp->getText();
					// int counter = _wtoi(text);


					// for (int i = 0; i < SceneTree->getChildren().size(); i++) {
					// 	if(tmp->getData() == SceneTree->getSelected()->getData()) {
					// 		ptr = SceneTree->getSelected()->getData();
					// 		counter_ptr = reinterpret_cast<int*>(ptr);
					// 		CurrentObject = Objects[counter];
					// 		break;
					// 	}

					// 	tmp = tmp->getNextSibling();
					// }

					// cout << "\n\tcounter: " << counter << endl;
					// cout << "\n\tCOORDS: " << CurrentObject.getCoord()->X << " " << CurrentObject.getCoord()->Z << endl;
				}
				break;
			// Скрул бар	
			case EGET_SCROLL_BAR_CHANGED:
				switch (id)
				{
				case GUI_ID_X_SCROLL_POS:{
					CurrentObject.getCoord()->X = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setCoord(*CurrentObject.getCoord());

					editboxPosX->setText(ftows(CurrentObject.getCoord()->X));
					return true;
					break;
				}
				case GUI_ID_Y_SCROLL_POS:{
					CurrentObject.getCoord()->Y = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setCoord(*CurrentObject.getCoord());

					editboxPosY->setText(ftows(CurrentObject.getCoord()->Y));
					return true;
					break;
				}
				case GUI_ID_Z_SCROLL_POS:{
					CurrentObject.getCoord()->Z = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setCoord(*CurrentObject.getCoord());

					editboxPosZ->setText(ftows(CurrentObject.getCoord()->Z));
					return true;
					break;
				}
				case GUI_ID_X_SCROLL_ROT:{
					CurrentObject.getRotation()->X = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setRotation(*CurrentObject.getRotation());

					editboxRotX->setText(ftows(CurrentObject.getRotation()->X));
					return true;
					break;
				}
				case GUI_ID_Y_SCROLL_ROT:{
					CurrentObject.getRotation()->Y = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setRotation(*CurrentObject.getRotation());

					editboxRotY->setText(ftows(CurrentObject.getRotation()->Y));
					return true;
					break;
				}
				case GUI_ID_Z_SCROLL_ROT:{
					CurrentObject.getRotation()->Z = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setRotation(*CurrentObject.getRotation());

					editboxRotZ->setText(ftows(CurrentObject.getRotation()->Z));
					return true;
					break;
				}
				case GUI_ID_X_SCROLL_SCALE:{
					CurrentObject.getScale()->X = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setScale(*CurrentObject.getScale());

					editboxScaleX->setText(ftows(CurrentObject.getScale()->X));
					return true;
					break;
				}
				case GUI_ID_Y_SCROLL_SCALE:{
					CurrentObject.getScale()->Y = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setScale(*CurrentObject.getScale());

					editboxScaleY->setText(ftows(CurrentObject.getScale()->Y));
					return true;
					break;
				}
				case GUI_ID_Z_SCROLL_SCALE:{
					CurrentObject.getScale()->Z = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					CurrentObject.setScale(*CurrentObject.getScale());

					editboxScaleZ->setText(ftows(CurrentObject.getScale()->Z));
					return true;
					break;
				}
				}
			// Элементы меню
			case EGET_MENU_ITEM_SELECTED:
					OnMenuItemSelected( (IGUIContextMenu*)event.GUIEvent.Caller );
				break;
			// Кнопки 
			case EGET_BUTTON_CLICKED:
				switch(id){
				case GUI_ID_ADD_BUTTON:{
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;
				}
				case GUI_ID_PERSPECTIVE_BUTTON:{
					currentViewState = GUI_ID_PERSPECTIVE_BUTTON;
					return true;

					break;
				}
				case GUI_ID_TOP_BUTTON:{
					currentViewState = GUI_ID_TOP_BUTTON;
					return true;

					break;
				}
				case GUI_ID_FRONT_BUTTON:{
					currentViewState = GUI_ID_FRONT_BUTTON;
					return true;

					break;
				}
				case GUI_ID_LEFT_BUTTON:{
					currentViewState = GUI_ID_LEFT_BUTTON;
					return true;

					break;
				}
				case GUI_ID_SPLIT_BUTTON:{
					currentViewState = GUI_ID_SPLIT_BUTTON;
					return true;

					break;
				}
				case GUI_ID_BBOX_BUTTON:{
					showDebug=!showDebug;
					CurrentObject.getModel()->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
					return true;

					break;
				}
				case GUI_ID_WIREFRAME_BUTTON:{
					CurrentObject.getModel()->setMaterialFlag(video::EMF_WIREFRAME,
						!CurrentObject.getModel()->getMaterial(0).Wireframe);
					CurrentObject.getModel()->setMaterialFlag(video::EMF_POINTCLOUD, false);
						return true;

					break;
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
					// SceneTree->getRoot()->clearChildren();
					itemCounter = 1;
					// addSceneTreeItem(device->getSceneManager()->getRootSceneNode(), SceneTree->getRoot());
					addMapListItem();
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


// Функция созданя Toolset -> Properties
void createToolBox()
{
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    
	if (e)
        e->remove();

    // Созадния окна Toolset   				  от края     отступ с верху  		 ширина    			длина вниз
    IGUIWindow* wnd = env->addWindow(rect<s32>(0, EXPLORER_WINDOW_POS_Y, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H), false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);


    // create tab control and tabs
    // IGUITabControl* tab = env->addTabControl(
    //     rect<s32>(2,20,800-602,480-7), wnd, true, true);

    // IGUITab* t1 = tab->addTab(L"Config");

	int offset50 = 50;
	// int offset;


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

	// Location Rotation (Расположение :: Варащение)
    env->addStaticText(L"Rotation:", rect<s32>( 35, 135, 80, 155), false, false, wnd);
    env->addStaticText(L"X:", core::rect<s32>(10,162,18,180), false, false, wnd);
	editboxRotX = env->addEditBox(L"1.0", rect<s32>(25 , 160, 85, 180), true, wnd, GUI_ID_X_ROT);
	env->addStaticText(L"Y:", core::rect<s32>(10,192,18,210), false, false, wnd);
    editboxRotY = env->addEditBox(L"1.0", rect<s32>(25 , 190, 85, 210), true, wnd, GUI_ID_Y_ROT);
	env->addStaticText(L"Z:", core::rect<s32>(10,222,18,240), false, false, wnd);
    editboxRotZ = env->addEditBox(L"1.0", rect<s32>(25 , 220, 85, 240), true, wnd, GUI_ID_Z_ROT);

	IGUIScrollBar* scrollbarRotX = env->addScrollBar(true,core::rect<s32>(100 , 160, 280, 180), wnd, GUI_ID_X_SCROLL_ROT);
	scrollbarRotX->setMin(-360);
	scrollbarRotX->setMax(360);
	scrollbarRotX->setPos(0);
	IGUIScrollBar* scrollbarRotY = env->addScrollBar(true,core::rect<s32>(100 , 190, 280, 210), wnd, GUI_ID_Y_SCROLL_ROT);
	scrollbarRotY->setMin(-360);
	scrollbarRotY->setMax(360);
	scrollbarRotY->setPos(0);
	IGUIScrollBar* scrollbarRotZ = env->addScrollBar(true,core::rect<s32>(100 , 220, 280, 240), wnd, GUI_ID_Z_SCROLL_ROT);
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

    IGUIScrollBar* scrollbarScaleX = env->addScrollBar(true,core::rect<s32>(100 , 270, 280, 290), wnd, GUI_ID_X_SCROLL_SCALE);
	scrollbarScaleX->setMin(-10);
	scrollbarScaleX->setMax(10);
	scrollbarScaleX->setPos(0);
	scrollbarScaleX->setLargeStep(1);
	scrollbarScaleX->setSmallStep(1);
	IGUIScrollBar* scrollbarScaleY = env->addScrollBar(true,core::rect<s32>(100 , 300, 280, 320), wnd, GUI_ID_Y_SCROLL_SCALE);
	scrollbarScaleY->setMin(-10);
	scrollbarScaleY->setMax(10);
	scrollbarScaleY->setPos(0);
	scrollbarScaleY->setLargeStep(1);
	scrollbarScaleY->setSmallStep(1);
	IGUIScrollBar* scrollbarScaleZ = env->addScrollBar(true,core::rect<s32>(100 , 330, 280, 350), wnd, GUI_ID_Z_SCROLL_SCALE);
	scrollbarScaleZ->setMin(-10);
	scrollbarScaleZ->setMax(10);
	scrollbarScaleZ->setPos(0);
	scrollbarScaleZ->setLargeStep(1); // диапазон шага стрелки
	scrollbarScaleZ->setSmallStep(1);

	// IGUIImage ImageTexture(env, wnd, -1,rect<s32>(10,360, 178, 528));
	// IGUIImage ImageTexture()
	// IGUIImage* clown;
	// clown->setImage()
	// ImageTexture.setImage("../media/irrlichtlogo2.png")

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
	// IGUITreeView* SceneTree;

    // SceneTree = device->getGUIEnvironment()->addTreeView(rect<s32>( 0, OFFSET, EXPLORER_WINDOW_WIDTH, PROPERTIES_WINDOW_POS_H ), t2, -1, true, true, false );
    // SceneTree->setToolTipText ( L"Show all files and folders into project" );
    // SceneTree->getRoot()->clearChildren();
    // addContentBrowserTreeItem (SceneTree->getRoot(), ".");

	// IGUIImageList* imageList = device->getGUIEnvironment()->createImageList(device->getVideoDriver()->getTexture ( "media/iconlist.png" ), dimension2di( 32, 32 ), true );

    // if ( imageList ){
    //     SceneTree->setImageList( imageList );
    //     imageList->drop ();
    // }
}




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

//Создание окна Explorer 
void createExplorer()
{
    IGUIEnvironment* env = device->getGUIEnvironment();
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

    //Model.updatePosInfo(Model);
}

int main(int argc,char **argv){

	glutInit(&argc,argv);
	Width = glutGet(GLUT_SCREEN_WIDTH);
    Height = glutGet(GLUT_SCREEN_HEIGHT);

	cout << "\t Widht   :" << Width << endl;
	cout << "\t Height   :" << Height << endl;
	cout << "\t OFFSET100   :" << OFFSET100 << endl;
	cout << "\t OFFSET   :" << OFFSET << endl;



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
	env = device->getGUIEnvironment();


	IGUISkin* skin = guienv->getSkin();
	// IGUIFont* font = guienv->getFont("../media/fontlucida.png");
	IGUIFont* font = guienv->getFont("../media/fonthaettenschweiler.bmp");

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

	 
	// Model.LoadModel(StartUpModelFile.c_str());
	Model* tmp = new Model;
	tmp->LoadModel(StartUpModelFile.c_str());
	Objects.push_back(*tmp);
	CurrentObject = Objects[0];

	//Основная
	// camera[0] = smgr->addCameraSceneNode(0, *(CurrentObject.getCoord()) + vector3df(0, 30, -40), *(CurrentObject.getCoord()));
    // camera[0]->setFarValue(200000.f); Устанавливает значение дальней плоскости отсечения (по умолчанию: 2000.0f).
	camera[0] = smgr->addCameraSceneNodeMaya();
    camera[0]->setTarget(*(CurrentObject.getCoord()));
	// Top
	camera[1] = smgr->addCameraSceneNode(0, vector3df(0,50,0), vector3df(0,0,0));
	//Front
	camera[2] = smgr->addCameraSceneNode(0, vector3df(50,0,0), vector3df(0,0,0));
	//Left
	camera[3] = smgr->addCameraSceneNode(0, vector3df(0,0,50), vector3df(0,0,0));

	


	createButtonsField(driver);
	createToolBox();
	createExplorer();
	
	// Создаем объект receiver на основе класса MyEventReceiver.
					//Накопитель
	MyEventReceiver receiver;

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);
	// cout << "\t Get data    :" << objectsCounter << endl;


	while(device->run()){
		// if (device->isWindowActive()) {
		driver->beginScene(true, true, SColor(255,100,101,140)); // Отчищает буфер глубина каждый кадр


		switch (currentViewState)
		{
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



		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	// }
	}

	device->drop();

	return 0;
}
