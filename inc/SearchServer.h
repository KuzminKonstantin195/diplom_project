#include "InvertedIndex.h"

#pragma once
/*
* Хранит валидность документа по поисковому запросу
*/
struct RelativeIndex {
	size_t doc_id;
	double rank;
	bool operator ==(const RelativeIndex& other) const {
		return (doc_id == other.doc_id && rank == other.rank);
	}
	RelativeIndex(int id, double num) : doc_id(id), rank((float)num) {};
};
class SearchServer {
public:
	/**
	* @param idx в конструктор класса передаётся ссылка на класс
	InvertedIndex,
	* чтобы SearchServer мог узнать частоту слов встречаемых в
	запросе

	*/
	//SearchServer(InvertedIndex* idx) : _index(idx) { };
	SearchServer(InvertedIndex &idx) : _index(&idx) { };
	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	vector<vector<RelativeIndex>> search(vector<string> queries_input);
private:
	InvertedIndex* _index;
};