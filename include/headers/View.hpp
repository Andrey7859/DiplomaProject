#pragma once
#include "headers/Camera.hpp"
#include "headers/Model.hpp"

int Width;
int Height;
int itemCounter = 1;
IGUIListBox* MapList; // Структура данных список
bool showDebug = false; // Для Wareframe и  BBox

// Макросы для работы с окнами
#define EXPLORER_WINDOW_POS_Y (BUTTON_SIZE + (OFFSET * 2)) //Отступ с верху 70
#define EXPLORER_WINDOW_POS_H ( Height - EXPLORER_WINDOW_POS_Y - OFFSET ) / 2//Длина Explorer 468
#define PROPERTIES_WINDOW_POS_H EXPLORER_WINDOW_POS_Y + EXPLORER_WINDOW_POS_H // Длина Prorerties 538
#define EXPLORER_WINDOW_WIDTH ((Width / 5) - BUTTON_SIZE) // ширина Explorer и Toolset 304
#define WINDOW_SPLIT_WIDTH (( Width - EXPLORER_WINDOW_WIDTH ) / 2 ) + EXPLORER_WINDOW_WIDTH// Ширина серидины окна сплит 992
#define WINDOW_SPLIT_HEIGHT ((Height - EXPLORER_WINDOW_POS_Y ) / 2 ) + EXPLORER_WINDOW_POS_Y// Высота серидины окна сплит 548

void createCameras();
void splitscreen();
void cameraViewState();