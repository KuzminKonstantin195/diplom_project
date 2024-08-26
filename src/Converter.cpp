#pragma once
#include "Converter.h"

vector<string> ConverterJSON::GetTextDocuments()
{
	vector<string> to_return;
	ifstream file_config;
	json js_struct;
	try
	{

		file_config.open("..\\config\\config.json");
		file_config >> js_struct;
		js_struct = js_struct["files"];
		to_return = js_struct;
	}
	catch (const std::exception& ex)
	{
		cerr << "Error open or read config file: " << ex.what() << endl;
	}
	return to_return;
}

int ConverterJSON::GetResponsesLimit()
{
	ifstream file_config;
	json js_struct;
	int limit = 0;
	try
	{
		file_config.open("..\\config\\config.json");
		file_config >> js_struct;
		file_config.close();
		js_struct = js_struct["config"];
		limit = js_struct["max_responses"];
	}
	catch (const std::exception& ex)
	{
		cerr << "Error open or read config file: " << ex.what() << endl;
	}
	return limit;
}

vector<string> ConverterJSON::GetRequests()
{
	ifstream file_req;
	json js_struct;
	vector<string> to_return;

	try
	{
		file_req.open("..\\config\\requests.json");
		file_req >> js_struct;
		file_req.close();
		to_return = js_struct["requests"];
	}
	catch (const std::exception& ex)
	{
		cerr << "Error open or read requests file: " << ex.what() << endl;
	}

	return to_return;
}

void ConverterJSON::putAnswers(vector<vector<RelativeIndex>> answers)
{
	
	json js_all_answer;
	ofstream answers_file("..\\answers.txt");
	//string line;

	int num_req = 1;
	int max_resp = GetResponsesLimit();

	for (auto& answer : answers)
	{
		/*
		* перед выводом/записью проверяем максимальное кол-во результатов в ответе
		* если необходимо - сортируем и оставляем самые "высокие показатели"
		*/
		int number_element = 1, minimum_pos = 0;

		while (answer.size() > max_resp)
		{
			if (answer.at(number_element).rank < answer.at(minimum_pos).rank)
			{
				minimum_pos = number_element;
			}
			if ((number_element + 1) == answer.size())
			{
				try
				{
					answer.erase(answer.begin() + minimum_pos);
				}
				catch (const std::exception&)
				{
					cerr << "Can't delete element in answer" << endl;
				}
				number_element = 1;
				minimum_pos = 1;
				
			}
			else
			{
				number_element++;
			}
			
		}

		for (auto& i : answer)
		{
			js_all_answer[to_string(num_req)][to_string(i.doc_id)] = i.rank;
		}

		num_req++;
		
	}

	answers_file << js_all_answer;
	answers_file.close();
}
