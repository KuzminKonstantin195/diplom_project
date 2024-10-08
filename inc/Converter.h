#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "SearchServer.h"

#pragma once

using namespace std;
using namespace nlohmann;

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
public:

	ConverterJSON() = default;
	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	vector<string> GetTextDocuments();
	/**
	* Метод считывает поле max_responses для определения предельного
	* количества ответов на один запрос
	* @return
	*/
	int GetResponsesLimit();
	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	vector<string> GetRequests();
	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(vector<vector<RelativeIndex>>);
};