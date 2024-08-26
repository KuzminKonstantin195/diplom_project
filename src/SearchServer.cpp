#pragma once
#include "SearchServer.h"

//#define DEBUG

using namespace std;


vector<vector<RelativeIndex>> SearchServer::search
( vector<string>& queries_input)
{
	size_t res_size = queries_input.size();

	vector<vector<RelativeIndex>> result;

	//result.resize(res_size);
#ifdef DEBUG
	cout << "All variable in searchServer is initialized!" << endl;
	cout << "queries: " << endl;

	for (auto& i : queries_input)
	{
		cout << "\t" << i << endl;
	}
#endif // DEBUG

	for (auto &querie : queries_input)
	{
		auto words = wordSepar(querie);							// разделяем запрос на отдельные слова
		vector<RelativeIndex> one_question_result;
		int max_count = 0;										// максимальное кол-во вхождений в документ

#ifdef DEBUG
		// вывод списка запросов
		cout << "querie: \n\t";
		for (auto &i : words)
			cout <<"\""<< i << "\" ";
		cout << endl;
#endif // DEBUG
				
		for (auto& word : words)
		{
			auto entr_res = _index->GetWordCount(word);
			
#ifdef DEBUG
			cout << "\"" << word << "\" founded in _index!" << endl;
#endif // DEBUG

			if (one_question_result.empty())
			{
				/*
				* если результат по запросу пуст, то 
				* полнностью переносим данные из _index по искомому слову
				*/
				for (auto &i : entr_res)
				{
					one_question_result.push_back(RelativeIndex(i.doc_id, i.count));
				}
#ifdef DEBUG
				cout << "First fulled one_question_result!" << endl;
				cout << "Now answer is: " << endl;
				for (auto &i : one_question_result)
				{
					cout << "\t " << i.doc_id << ": " << i.rank << endl;
				}
				
#endif // DEBUG
			}
			else 
			{
				/*
				* иначе сравниваем и оставляем совпадающие
				* 
				*/
				vector<RelativeIndex> new_result;

				for (auto &in_result : one_question_result)
				{
					
					for (auto &i : entr_res)
					{
						if (in_result.doc_id == i.doc_id)
						{
							new_result.push_back(RelativeIndex(in_result.doc_id, in_result.rank + i.count));
#ifdef DEBUG
							cout << "Add cont to " << in_result.doc_id << endl;
#endif // DEBUG
						}
					}
				}
				if (new_result.size() != 0)
				{
					one_question_result = new_result;						// обновляем данные по результату
				}
				else
				{
					for (auto& i : new_result)
					{
						one_question_result.push_back(i);
					}
				}
#ifdef DEBUG
				cout << "Add element to result" << endl;
#endif // DEBUG
			}
		}
		 
		for (auto& i : one_question_result)
		{
			if (i.rank > max_count)
			{
				max_count = i.rank;
			}
		}
		for (auto &i : one_question_result)
		{
			i.rank = i.rank / max_count;
		}
		
		result.push_back(one_question_result);
	}
#ifdef DEBUG
	cout << "Searching end" << endl;
#endif // DEBUG

	return result;
	
}

