#pragma once
#include "headers/Header.hpp"

#define SIZE_STATIC 17
#define SIZE_ANIM 6

std::string static_str[SIZE_STATIC] = { ".irr", ".irrmesh", ".3ds", ".obj", ".lwo", ".xml", ".dae", ".mesh", ".my3D", ".lmts", 
									".bsp", ".dmf", ".oct", ".csm", ".stl", ".ply"};
std::string anim_str[SIZE_ANIM] = { ".b3d", ".x", ".ms3d", ".md2", ".md3" };

core::stringc StartUpModelFile = "../media/dwarf.x";
core::stringw Caption;
bool Octree=false;
bool UseLight=false;

template<typename T>
T parce(io::path& extension) {
	for (int i = 0; i < SIZE_STATIC; i++)
	{
		if(extension.c_str() == static_str[i]){
			IMesh* m;
			// return m;
		}
	}

	for (int i = 0; i < SIZE_ANIM; i++)
	{
		if(extension.c_str() == anim_str[i]){
			IAnimatedMesh* m;
			// return m;
		}
	}
	
	return 0;
}

class Model
{
private:
	ISceneNode* _Model = 0;
	vector3df Coord;
	vector3df Rotation;
	vector3df Scale;
public:
    Model();
    void LoadModel(const c8* fn);
	ISceneNode* getModel();
	void updatePosInfo();
	vector3df* getCoord();
	vector3df* getRotation();
	vector3df* getScale();
	void setCoord(vector3df NewCoord);
	void setRotation(vector3df NewRot);
	void setScale(vector3df NewScale);
	Model& operator=(const Model& right) {
        //проверка на самоприсваивание
        if (this == &right) {
            return *this;
        }
		_Model = right._Model;
		Coord = right.Coord;
		Rotation = right.Rotation;
		Scale = right.Scale;
        return *this;
    }

    ~Model();
};

Model::Model()
{
	Coord = vector3df (0, 0, 0);
	Rotation = vector3df (0, 0, 0);
	Scale = vector3df (1, 1, 1);
}

// Функция для загрукзки моделии
void Model::LoadModel(const c8* fn) {

	// Модифицирует имя если это файл .pk3
	io::path filename(fn);
	// Расширение
	io::path extension;
	core::getFileNameExtension(extension, filename);
	// Переводит строку в нижний регистр
	extension.make_lower();

	// Если текстура загружена применяем сравниваем форматы
	if (extension == ".jpg" || extension == ".pcx" ||
		extension == ".png" || extension == ".ppm" ||
		extension == ".pgm" || extension == ".pbm" ||
		extension == ".psd" || extension == ".tga" ||
		extension == ".bmp" || extension == ".wal" ||
		extension == ".rgb" || extension == ".rgba")
	{
		video::ITexture * texture = device->getVideoDriver()->getTexture( filename );
		if ( texture && _Model ) // Модель = объемная фигура
		{
			// Перезагружает текстуру
			device->getVideoDriver()->removeTexture(texture);
			texture = device->getVideoDriver()->getTexture( filename );
			// Устанавливаем новую текстуру
			_Model->setMaterialTexture(0, texture);
		}
		return;
	}

	// Если это архив добавляем его FileArchive..
	else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk")
	{
		device->getFileSystem()->addFileArchive(filename.c_str());
		return;
	}

	// Загружаем модель в движок

	if (_Model)
		_Model->remove();

	_Model = 0;

	if (extension==".irr")
	{
		core::array<scene::ISceneNode*> outNodes;
		device->getSceneManager()->loadScene(filename);
		device->getSceneManager()->getSceneNodesFromType(scene::ESNT_ANIMATED_MESH, outNodes);
		if (outNodes.size())
			_Model = outNodes[0];
		return;
	}

	// auto m = parce(extension);

	IAnimatedMesh* m = device->getSceneManager()->getMesh( filename.c_str() );


	// Если модель не может быть загружена
	if (!m)
	{
		if (StartUpModelFile != filename)
			device->getGUIEnvironment()->addMessageBox(Caption.c_str(), L"The model could not be loaded. Maybe it is not a supported file format.");
		return;
	}

	// устанавливаем свойства материала по умолчанию

	if (Octree)
		_Model = device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
	else
	{
		scene::IAnimatedMeshSceneNode* animModel = device->getSceneManager()->addAnimatedMeshSceneNode(m);
		animModel->setAnimationSpeed(30);
		_Model = animModel;
	}
	_Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
	_Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
//	_Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	_Model->setDebugDataVisible(scene::EDS_OFF);

	updatePosInfo();

}

ISceneNode* Model::getModel() {
	return _Model;
}

void Model::updatePosInfo()
{
    IGUIElement* toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    if (!toolboxWnd)
        return;

    if (!_Model)
    {
        toolboxWnd->getElementFromId(GUI_ID_X_POS, true)->setText( L"-" );
        toolboxWnd->getElementFromId(GUI_ID_Y_POS, true)->setText( L"-" );
        toolboxWnd->getElementFromId(GUI_ID_Z_POS, true)->setText( L"-" );
    }
    else
    {
        core::vector3df _position = _Model->getPosition();
        toolboxWnd->getElementFromId(GUI_ID_X_POS, true)->setText( core::stringw(_position.X).c_str() );
        toolboxWnd->getElementFromId(GUI_ID_Y_POS, true)->setText( core::stringw(_position.Y).c_str() );
        toolboxWnd->getElementFromId(GUI_ID_Z_POS, true)->setText( core::stringw(_position.Z).c_str() );
    }
}

vector3df* Model::getCoord(){
	return &Coord;
}

vector3df* Model::getRotation(){
	return &Rotation;
}

vector3df* Model::getScale(){
	return &Scale;
}

void Model::setCoord(vector3df NewCoord){
	Coord.X = NewCoord.X;
	Coord.Y = NewCoord.Y;
	Coord.Z = NewCoord.Z;

	_Model->setPosition(Coord);
}

void Model::setRotation(vector3df NewRot){
	Rotation.X = NewRot.X;
	Rotation.Y = NewRot.Y;
	Rotation.Z = NewRot.Z;

	_Model->setRotation(Rotation);
}

void Model::setScale(vector3df NewScale){
	Scale.X = NewScale.X;
	Scale.Y = NewScale.Y;
	Scale.Z = NewScale.Z;

	_Model->setScale(Scale);
}

Model::~Model()
{
}


