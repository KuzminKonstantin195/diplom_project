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

/*
void word_clean (string word)
{
	int counter = 0;
	for (auto symb =0; symb < word.length(); symb++)
	{
		if ((word[symb] >= 97 && word[symb] <= 122) ||  (word[symb] >= 48 && word[symb] <= 57))
		{
			
		}
		else if (word[symb] >= 65 && word[symb] <= 90)
		{

		}
		else
		{
			word.erase(word.begin() + symb);
			symb--;
		}
	}
}
*/

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
	
	mutex mtx;

	for (auto way_num = 0; way_num < input_docs.size(); way_num++)
	{
		threads_buffer.push_back (make_shared<thread>([&]()
			{
				int doc_num;
				mtx.lock();
				doc_num = (way_num);
				mtx.unlock();

#ifdef DEBUG
				mtx.lock();
				//cout << this_thread::get_id() << " started" << endl;
				mtx.unlock();
#endif // DEBUG
				map<string, Entry> local_freq;

				ifstream file;
#ifdef DEBUG
				mtx.lock();
				//cout << "In " << this_thread::get_id() << " doc num : " << doc_num << endl;
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

				for (auto& i : local_freq)
				{
#ifdef DEBUG
					mtx.lock();
					cout << "now added " << "\"" << i.first << "\"" << endl;
					mtx.unlock();
#endif // DEBUG

					mtx.lock();

					auto find_result = freq_dictionary.find(i.first);

					if (find_result == freq_dictionary.end())
					{
						freq_dictionary[i.first] = vector<Entry>{ i.second };
#ifdef DEBUGA
						//mtx.lock();
						cout << "Now: " << i.first << ": ";
						for (auto &en : freq_dictionary[i.first])
						{
							cout << en.doc_id << "-" << en.count << "; ";
						}
						cout << endl;
						//mtx.unlock();
#endif // DEBUG
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
								freq_dictionary[i.first].insert (freq_dictionary[i.first].begin() + pos, i.second);
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

					this_thread::sleep_for(chrono::microseconds(2));
				}
				file.close();
			})
		);
		/*
		* такая заглушка нужна, чтобы ветка успевала принять для себя номер документа
		* и не возникало конфликта доступа к одному и тому же файлу
		*/
		this_thread::sleep_for(chrono::microseconds(5));					
	}

	for (auto &i : threads_buffer)
	{
#ifdef DEBUG
		auto id = i->get_id();
#endif // DEBUG

		i->join();

#ifdef DEBUG
		mtx.lock();
		cout << id << " joined" << endl;
		mtx.unlock();
#endif // DEBUG
	}
#ifdef DEBUG
	cout << "UpdateBase ending" << endl;

	for (auto &i : freq_dictionary)
	{
		cout << i.first << ": ";
		for (auto &ii : i.second)
		{
			cout << ii.doc_id << " " << ii.count << "; ";
		}
		cout << endl;
	}
#endif // DEBUG
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

