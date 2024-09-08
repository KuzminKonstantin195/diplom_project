#pragma once
#include "InvertedIndex.h"

//#define DEBUG

vector<string> wordSepar(const string& text)
{
	vector<string> bufVec;
	string word;

	for (auto &symbol : text) {

		if ((symbol >=48 && symbol <=57) || (symbol >= 65 && symbol <= 90) || (symbol >= 97 && symbol <= 122)) {
			if (symbol >= 65 && symbol <= 90)
			{
				word.push_back(symbol + (97 - 65));
			}
			else 
			{
				word.push_back(symbol);
			}
			
		}
		else {

			bufVec.push_back(word);
			word.clear();
		}
	}
	if (word != " " && word != "" && word != "-" && word != "+")
	{
		bufVec.push_back(word);
	}
	
	word.clear();

	return bufVec;
}

InvertedIndex::InvertedIndex(InvertedIndex& idx) : docs(idx.docs), freq_dictionary(idx.freq_dictionary) {};

void InvertedIndex::correction_ways (vector<string> &ways)
{
	
	for (auto &way : ways)
	{
		for (auto symbol_num = 0;  symbol_num < way.length(); symbol_num++)
		{
			if (way[symbol_num] == '/')
			{
				way.erase(way.begin() + symbol_num);
				way.insert(symbol_num, "\\");
				//way[symbol_num] = '\\';
			}
		}
	}
	
}

void InvertedIndex::UpdateDocumentBase(vector<string> input_docs)
{
#ifdef DEBUG
	cout << "UpdateBase started" << endl;
#endif // DEBUG

	vector<shared_ptr<thread>> threads_buffer;
	auto max_thread = std::thread::hardware_concurrency() - 2;
	mutex mtx;
	int counter = 0;

	auto creator_thread = make_shared<thread>([&]()
		{
			while (counter < input_docs.size())
			{
				if (threads_buffer.size() < max_thread && threads_buffer.size() < input_docs.size())
				{
					threads_buffer.push_back(make_shared<thread>([&]()
						{
							mtx.lock();
							int doc_num = counter;
							counter++;
							mtx.unlock();

							map<string, Entry> local_freq;

							ifstream file;
#ifdef DEBUG
							mtx.lock();
							cout << "In " << this_thread::get_id() << " doc : " << input_docs[doc_num] << endl;
							mtx.unlock();
#endif // DEBUG
							try
							{

								file.open(input_docs[doc_num]);
#ifdef DEBUG
								mtx.lock();
								//cout << input_docs[doc_num] << " open!" << endl;
								mtx.unlock();
#endif // DEBUG
							}
							catch (const std::exception&)
							{
								cerr << "Error with open " << input_docs[doc_num] << endl;
							}

							string word;

							do
							{
								file >> word;
								word = wordSepar(word).at(0);

								auto find_result = local_freq.find(word);

								if (find_result == local_freq.end())
								{
									Entry entr;
									entr.doc_id = doc_num;
									entr.count = 1;

									local_freq[word] = entr;
#ifdef DEBUG
									mtx.lock();
									cout << "\"" << word << "\"" << " added in local!" << endl;
									mtx.unlock();
#endif // DEBUG
								}
								else
								{
									local_freq[word].count++;
#ifdef DEBUG
									mtx.lock();
									cout << "in local: " << "\"" << word << "\"" << ": " << local_freq[word].doc_id << "-" << local_freq[word].count << endl;
									mtx.unlock();
#endif // DEBUG
								}

							} while (!file.eof());
							file.close();
							for (auto& i : local_freq)
							{
#ifdef DEBUG
								mtx.lock();
								cout << "now added " << "\"" << i.first << "\"" << endl;
								mtx.unlock();
#endif // 
								mtx.lock();
								auto find_result = freq_dictionary.find(i.first);

								if (find_result == freq_dictionary.end())
								{
									freq_dictionary[i.first] = vector<Entry>{ i.second };
								}
								else
								{
									for (int pos = 0; pos < freq_dictionary[i.first].size(); pos++)
									{
										if (freq_dictionary[i.first][pos].doc_id == i.second.doc_id)
										{
											freq_dictionary[i.first][pos].count += i.second.count;
											break;
										}
										else if (freq_dictionary[i.first][pos].doc_id > i.second.doc_id)
										{
											freq_dictionary[i.first].insert(freq_dictionary[i.first].begin() + pos, i.second);
											break;
										}
										else if ((pos + 1) == freq_dictionary[i.first].size())
										{
											freq_dictionary[i.first].push_back(i.second);
											break;
										}
									}
								}
								mtx.unlock();
							}

						}));
				}
			}
		}
	);

	while (counter)
	{

	}

	creator_thread->join();
}

vector<Entry> InvertedIndex::GetWordCount(const string& word)
{
	auto result = freq_dictionary.find(word);
	if (result != freq_dictionary.end())
	{
		return result->second;
	}
	else
	{
		return vector<Entry>{};
	}
}

