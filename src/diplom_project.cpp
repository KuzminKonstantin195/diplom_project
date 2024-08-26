#include "Converter.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <iostream>


using namespace std;

void show_menu();											// показать список доступных команд

int main()
{	
	show_menu();

	ConverterJSON converter;
	InvertedIndex index;
	SearchServer searcher(index);
	int input_command = -1;
	string str;
	vector<string> vec_str;
	vector<string> requests;
	index.UpdateDocumentBase(converter.GetTextDocuments());

	do
	{
		cout << "Enter command: ";
		cin >> input_command;
		switch (input_command)
		{
		case 1:
			index.UpdateDocumentBase(converter.GetTextDocuments());
			cout << "*update complete*\n" << endl;
			break;
		case 2:

			requests = converter.GetRequests();
			converter.putAnswers(searcher.search(requests));
			cout << "*search complete*\n" << endl;
			break;
		case 3:

			try
			{
				cout << "Write you answer (words swparated by space)" << endl;
				getline(cin, str);
				getline(cin, str);
				vec_str = wordSepar(str);
				converter.putAnswers(searcher.search(vec_str));
				cout << "*search complete*\n" << endl;
			}
			catch (const std::exception& ex)
			{
				cerr << ex.what() << endl;
			}
			
			break;
		case 0:
			cout << "\n\tEND!!!" << endl;
			break;
		default:
			break;
		}

		input_command = -1;
	} while (input_command != 0);
}

void show_menu()
{
	cout << "*************************** COMAND LIST ***************************\n" << endl;
	cout << "1 - update document base;\n";
	cout << "2 - run search with \"requests.json\" file;\n";
	cout << "3 - search with console request;\n";

	cout << "\n0 - end program\n\n" << endl;
}