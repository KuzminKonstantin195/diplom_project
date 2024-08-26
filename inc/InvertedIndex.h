#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <memory>
#include <Windows.h>

#include "nlohmann/json.hpp"

#pragma once

using namespace std;
using namespace nlohmann;

/**
* Хранит кол-во вхрждений слова в конкретный документ по ID
*/
struct Entry {
	size_t doc_id, count;

	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id &&
			count == other.count);
	}
};

/*
* разбиение строки на отдельные слова
*/
vector<string> wordSepar(const string& text);
/*
* 
*/

/*
* удаление повторов слов в массиве
*/
//void deleteRepeat(vector<string>& words);

class InvertedIndex {
public:
	InvertedIndex() = default;
	InvertedIndex(InvertedIndex& idx);
	/**
	* Обновить или заполнить базу документов, по которой будем совершать
	поиск
	* @param texts_input содержимое документов
	*/
	void UpdateDocumentBase(vector<string> input_docs);
	/**
	* * Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	vector<Entry> GetWordCount(const string& word);
private:
	vector<string> docs;											// список содержимого документов
	void correction_ways(vector<string> &ways);
public:
	map<string, vector<Entry>> freq_dictionary;					// частотный словарь
};
