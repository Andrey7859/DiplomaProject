#pragma once
#include "headers/Model.hpp"
#include "headers/Header.hpp"

//Удлаяет модель со сцены
inline void deleteModel(){
	s32 selected = MapList->getSelected();
	swap(Objects[selected], Objects.back());
	CurrentObject = Objects[0];
	Objects.back().getModel()->remove();
	Objects.erase(Objects.end()); // Удаляет элемент из вектора
	
	MapList->removeItem(selected); // Удаляет элемент из GUI
	MapList->setSelected(0);
}

//Обработчик событий нажатий на пункты меню
void OnMenuItemSelected( IGUIContextMenu* menu ){
	s32 id = menu->getItemCommandId(menu->getSelectedItem());

	switch(id){
	// Блок меню File
	case GUI_ID_OPEN_MODEL: // Добавить новую модель
		env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
	break;
	
	case GUI_ID_LOAD_SCENE: // Загрузить сцену
		env->addFileOpenDialog(L"Please choose scene.", true, 0, 1, true);
	
	break;

	case GUI_ID_SAVE_MODEL: // Сохранение
		if (false == smgr->saveScene("../save/SaveProject.irr", 0, 0)){
			cout << "Error. Can't save" << endl;}	
	break;

	case GUI_ID_DELETE_MODEL: // Удалить модель
		deleteModel();
	break;

	case GUI_ID_QUIT: // Выход из программы
		device->closeDevice();
	break;

	// Блок меню Edit
	case GUI_ID_BRUSH: // Удалить модель 
		env->addFileOpenDialog(L"Please choose scene.", true, 0, 2, true);

	break;

	// Подблок меню View 1
	case GUI_ID_PERSPECTIVE: // Вид перспективы
		currentViewState = GUI_ID_PERSPECTIVE_BUTTON; break;
	case GUI_ID_TOP: // Вид сверху
		currentViewState = GUI_ID_TOP_BUTTON; break;
	case GUI_ID_FRONT: // Вид фронт 
		currentViewState = GUI_ID_FRONT_BUTTON; break;
	case GUI_ID_LEFT: // Вид слева
		currentViewState = GUI_ID_LEFT_BUTTON; break;
	case GUI_ID_SPLIT: // Вид слева
		currentViewState = GUI_ID_LEFT_BUTTON; break;

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
class MyEventReceiver : public IEventReceiver{
	public:
		// Конструктор
		MyEventReceiver()  { } 
		
		virtual bool OnEvent(const SEvent& event){	
			// Отлавливаем события относящиеся к приложению интерфейса
			if (event.EventType == EET_GUI_EVENT){
				s32 id = event.GUIEvent.Caller->getID();
				IGUIElement* toolboxWnd;
				const wchar_t* text;
				wchar_t* end;
				
				// Выбирает объект при нажатии
				if ( event.GUIEvent.Caller == MapList && event.GUIEvent.EventType == gui::EGET_LISTBOX_SELECTED_AGAIN ){
					s32 selected = MapList->getSelected();
					CurrentObject = Objects[selected];
					CurrentObject.updatePosInfo();
				}

				switch(event.GUIEvent.EventType){
					// Скрул бар	
					case EGET_SCROLL_BAR_CHANGED:
						switch (id){
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
						OnMenuItemSelected((IGUIContextMenu*)event.GUIEvent.Caller);
					break;

					// Кнопки 
					case EGET_BUTTON_CLICKED:
						switch(id){
							case GUI_ID_ADD_BUTTON:{
								env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);

								return true;
								break;
							}
							
							case GUI_ID_LOAD_BUTTON:{
								env->addFileOpenDialog(L"Please choose scene.", true, 0, 1, true);

								return true;
								break;
							}
							
							case GUI_ID_SAVE_BUTTON:{
								if (false == smgr->saveScene("../save/SaveProject.irr", 0, 0)){
								cout << "Error. Can't save" << endl;}	

								return true;
								break;
							}

							case GUI_ID_DELETE_BUTTON:{
								deleteModel();
							
								return true;
								break;
							}
							case GUI_ID_BRUSH_BUTTON: // Удалить модель 
								env->addFileOpenDialog(L"Please choose texture.", true, 0, 2, true);
								
								return true;
								break;

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
								CurrentObject.getModel()->setMaterialFlag(video::EMF_WIREFRAME, !CurrentObject.getModel()->getMaterial(0).Wireframe);
								CurrentObject.getModel()->setMaterialFlag(video::EMF_POINTCLOUD, false);

								return true;
								break;
							}

							default:
								return false;
						}
					break;
					
					case EGET_FILE_SELECTED:{
						IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;

						if (id == -1){
							Model* tmp = new Model; 
							tmp->LoadModel(core::stringc(dialog->getFileName()).c_str());
							Objects.push_back(*tmp);
							addMapListItem();
							itemCounter++;
							CurrentObject = Objects[Objects.size()-1]; // выбор последнего объекта
							MapList->setSelected(MapList->getChildren().getSize()); // выделяет объект
						}
						
						if (id == 1){
							Objects.clear();
							smgr->clear();
							itemCounter = 0;
							MapList->clear();
							core::array<scene::ISceneNode*> outNodes;
							device->getSceneManager()->loadScene(core::stringc(dialog->getFileName()).c_str());
							device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, outNodes);

							if (outNodes.size()){
								for (int i = 0; i < outNodes.size(); i++){
									Model* tmp = new Model; 
									tmp->setModel(outNodes[i]);
									Objects.push_back(*tmp);
									itemCounter++;
									addMapListItem();
								}
							}
							CurrentObject = Objects[Objects.size()-1]; // выбор последнего объекта
							MapList->setSelected(MapList->getChildren().getSize()); // выделяет объект
						}
						
						//Загрузка текстура
						if (id == 2){
							video::ITexture * texture = device->getVideoDriver()->getTexture( core::stringc(dialog->getFileName()).c_str() );
							if (texture && CurrentObject.getModel()){ // Модель = объемная фигура
								// Перезагружает текстуру
								device->getVideoDriver()->removeTexture(texture);
								texture = device->getVideoDriver()->getTexture( core::stringc(dialog->getFileName()).c_str() );
								// Устанавливаем новую текстуру
								CurrentObject.getModel()->setMaterialTexture(0, texture);
							}						
						}
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
					break;
				}

			}

		return false;
	}
};
