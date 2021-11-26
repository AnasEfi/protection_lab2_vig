#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <windows.h>
#include <vector>
using namespace std;

class Efimenko_vigener
{
private:
	string key;
	int mod;
	string word;
public:
	Efimenko_vigener(string key_input, int size_N = 255)
	{
		key = key_input;
		mod = size_N;
	}
	Efimenko_vigener()
	{
	}
	string encrypt(string word)
	{
		string output = "";
		int j = 0;

		for (int i = 0;i < word.size();i++)
		{
			if (j >= key.size())
				j = 0;

			unsigned char ch = word[i] + key[j];
			ch = ch % mod;
			output += ch;
			j++;
		}
		return output;
	}

	string decrypt(string data)
	{
		string output;
		int j = 0;
		for (int i = 0;i < data.size();i++)
		{
			if (j >= key.size())
				j = 0;

			unsigned char ch = data[i] - key[j];
			ch = (ch + mod) % mod;
			output += ch;
			j++;
		}
		return output;
	}

	vector<pair<unsigned char, int>> get_sorted_symbols_count(string text)
	{
		bool chars_nullable[255]; 
		vector<pair<unsigned char, int>> chars_counter;
		for (auto& nullable : chars_nullable)
			nullable = false;

		for (int i = 0; i < text.size(); i++)
		{
			for (int j = 0; j < 255; j++)
			{
				if (chars_nullable[j] == false)
				{
					pair<unsigned char, int> c_counter_pair = pair<unsigned char, int>((text[i]), 1);
					chars_counter.push_back(c_counter_pair);
					chars_nullable[j] = true;
					break;
				}
				else if ((text[i]) == (chars_counter.at(j).first))
				{
					chars_counter.at(j).second++;
					break;
				}
				//cout << chars_counter.at(j).first << " " << chars_counter.at(j).second<<endl;
				continue;
			}
		}
		return chars_counter;
	}

	vector<pair<unsigned char, double>> get_sorted_symbols_countd(string text)
	{
		bool chars_nullable[255];
		vector<pair<unsigned char, double>> chars_counter;
		for (auto& nullable : chars_nullable)
			nullable = false;

		for (int i = 0; i < text.size(); i++)
		{
			for (int j = 0; j < 255; j++)
			{
				if (chars_nullable[j] == false)
				{
					pair<unsigned char, double> c_counter_pair = pair<unsigned char, double>((text[i]), 1);
					chars_counter.push_back(c_counter_pair);
					chars_nullable[j] = true;
					break;
				}
				else if ((text[i]) == (chars_counter.at(j).first))
				{
					chars_counter.at(j).second++;
					break;
				}
				//cout << chars_counter.at(j).first << " " << chars_counter.at(j).second<<endl;
				continue;
			}
		}
		return chars_counter;
	}
	double findEC(vector<pair<unsigned char, int>>& sub)
	{
		double index=0;
		double index_final = 0;
		int N = 0;
		for (auto pair : sub)
			N += pair.second;

		for (auto pair : sub)
		{
			double add = 0;
			int num = pair.second*(pair.second-1);
			int k = N * (N - 1);
			add = (double)num /(double)k;
			index += add;
			/*int num = pair.second * (pair.second - 1);
			index += (double)num;*/
		}
		/*int k = N * (N - 1);
		index = index / double(k);*/
		index_final = index;
		return index_final;
	}

	pair<unsigned char, double> decrypt_by_average_symbol_value(string ref_text)
	{
		// Тут частоты каждого символа эталонного текст в виде вектор пары (буква, частота)
		vector<pair<unsigned char, double>> standart_text_symbols_data=get_sorted_symbols_countd(ref_text);
		vector<pair<unsigned char, double>> frequency_vector;
		pair<unsigned char, double>letter_frequency;
		for (auto &pair : standart_text_symbols_data)
		{
			double frequency = (double)pair.second / (double)ref_text.size();
			letter_frequency.first = pair.first;
			letter_frequency.second = frequency;
			frequency_vector.push_back(letter_frequency);
		}
		pair< unsigned char, double > buf (frequency_vector.at(0).first, frequency_vector.at(0).second);
		for (auto &pair : frequency_vector)
		{
			if (pair.second > buf.second)
				buf = pair;
		}
		cout << "Letter: " << buf.first << "  frequency:" << buf.second << endl;
		return buf;
	}

	vector<unsigned char> frequency_analysis(int key_length, string textEnc, pair<unsigned char, double> max_fr_letter)
	{
		/// Получаем все L-граммы в зависимости от длины ключа
		vector<unsigned char> result;
		string result2="";
		int _LGrammCount = key_length;
		vector<string>_substrs(_LGrammCount);
		int _tmp = 0;
		for (int i = 0; i < textEnc.size(); i++)
		{
			_substrs[_tmp].push_back(textEnc[i]);
			_tmp++;
			if (_tmp >= key_length)
				_tmp = 0;
		}
		vector<vector<pair<unsigned char, double>>> sorted_LGramms;
		for (int j = 0; j < _LGrammCount; j++)
		{
			cout << "L-gramma [" << j << "]: " << endl;
			//cout << _substrs[j] << endl;
			pair<unsigned char, int> high_freq_sub = decrypt_by_average_symbol_value(_substrs[j]);
			int i1 = (int)high_freq_sub.first;
			int i2 = (int)max_fr_letter.first;
			int res = i1 - i2;
			unsigned char l = 'д';
			unsigned char l2 = 'о';
			unsigned char l3 = 'м';
			
			unsigned char difference = (unsigned char)res;

			result.push_back(difference);
			result2 =+ difference;
		}
		return result;
	}

	void findIndexs(string word2)
	{
		vector <double> indexes;
		vector<string> substring;
		vector<pair<unsigned char, int>> collect_index;
		//find length
		//find substrings
		int tmp_counter = 0;
		int key_length = 2;
		string substr_result="";
		do
		{
			for (int i = 0; i < word2.size(); i++)
			{
				if (++tmp_counter % key_length == 0) {
					substr_result += word2[i];
				}
			}
			substring.push_back(substr_result);
			//cout << substr_result;
			collect_index=get_sorted_symbols_count(substr_result);
			indexes.push_back(findEC(collect_index));
			key_length++;
			substr_result = "";
			tmp_counter = 0;
		} while (key_length < 20);
	}
			// find indexes

	};

void PrintMenu()
{
	cout << "1.Encrypt/Decrypt" << endl
		<< "2.Decrypt without key" << endl
		<< "3.Exit" << endl
		<< "Choice:";
}

int main()

{
	while (1) {
		PrintMenu();
		int x;
		cin >> x;
		switch (x) {
		case 1:
		{
			string filename, string, word, key, count;
			cout << "Enter name of file: ";
			cin >> filename;
			ifstream inf(filename, ios::binary);
			while (!inf.eof())
			{
				getline(inf, count);
				word += count;
			}
			inf.close();

			cout << "Enter key: ";
			cin >> key;
			Efimenko_vigener chiefr(key, 255);
			ofstream fout;
			fout.open("data6.txt", ios::binary);
			fout << chiefr.encrypt(word);
			fout.close();
			fout.open("data7.txt", ios::binary);
			fout << chiefr.decrypt(chiefr.encrypt(word));
			fout.close();
			break;
		}
		case 2:
		{
			string filename, string, word_reference,word2, key, count;
			cout << "Enter name of file: ";
			cin >> filename;
			ifstream inf(filename, ios::binary);
			word2 = "";
			while (!inf.eof())
			{
				getline(inf, count);
				word2 += count;
			}
			inf.close();
			Efimenko_vigener chiefr2;
			chiefr2.findIndexs(word2);
			
			cout << "Enter name reference text: "; //Input ideal text and count frequency 
			cin >> filename;
			ifstream fin(filename, ios::binary);
			word_reference = "";
			while (!fin.eof())
			{
				getline(fin, count);
				word_reference += count;
			}
			fin.close(); //begin frequency analyse

			pair<unsigned char, double> high_freq_ideal = chiefr2.decrypt_by_average_symbol_value(word_reference); //самый частовстречаемый символ в эталонном тексте
		
			vector<unsigned char> keyChange = chiefr2.frequency_analysis(3, word2, high_freq_ideal); //find key_name
			ofstream fout;
			fout.open("key.txt", ios::binary);
			for (auto pos : keyChange)
			{
				fout << pos;
				cout << pos;
			}
			fout.close();
			break;
		}
		case 3:
		{
			return 0;
		}
		}
		cout << '\n';
	}
	return 0;
}
