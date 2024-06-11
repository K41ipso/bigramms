#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "../headers/list_func.h"
#include "../headers/frequency_dict.h"
#include "../headers/bigramm_dict.h"
#include "../headers/main_backend.h"

using namespace std;

map<string, int> read_freq_dict(string file_name) {

	string line;
	string full_txt_path = "../txt/";
	full_txt_path += file_name;
	ifstream in(full_txt_path);

	map<string, int> result_map;
	char sym;
	bool word = false;
	bool count = false;
	string localsave_ = "";
	string localcount_ = "";

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = line.at(i);
				if (sym == ' ') {
					if (word) {
						count = true;
						word = false;
					}
					else {
						word = true;
					}
				}
				if (word && sym != ' ') {
					localsave_ += sym;
				}
				if (count && sym != ' ' && sym != ';') {
					localcount_ += sym;
				}
			}
			result_map[localsave_] = stoi(localcount_);
			count = false;

			//cout << localsave_ << "\t" << stoi(localcount_) << endl; //otladka

			localsave_ = "";
			localcount_ = "";
		}
	}
	in.close();
	
	return result_map;
}

main_dict<bigramm_dict> read_bigram_dict(string file_name) {

	string line;
	string full_txt_path = "../txt/";
	full_txt_path += file_name;
	ifstream in(full_txt_path);

	main_dict<bigramm_dict> result_bigram_dict;
	bigramm_dict local_line_;

	char sym;
	bool word = false;
	bool count = false;
	bool next_word = false;
	bool bracket = false;
	bool used_localname = false;
	string localsave_ = "";
	string local_next_word_ = "";
	string localcount_ = "";

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = line.at(i);
				if (sym == ' ' || sym == ';') {
					if (word) {
						if (!used_localname) {
							//cout << localsave_ << "\t"; // otladka
							used_localname = true;
						}
						
						if (count) {
							if (next_word) {
								//cout << " | " << local_next_word_ << " (" << localcount_ << ")"; //otladka

								local_line_.next_words.push_back(local_next_word_);
								local_line_.count_next_words.push_back(stoi(localcount_));
								local_next_word_ = "";
								localcount_ = "";
								next_word = false;
								count = false;
								if (sym == ';') {
									local_line_.name_word = localsave_;
									localsave_ = "";
									cout << endl; //otladka
									used_localname = false;
									result_bigram_dict.line.push_back(local_line_);
									local_line_.next_words.clear();
									local_line_.count_next_words.clear();
									//local_line_.name_word = "";
									//local_line_.next_words.clear();
									//local_line_.count_next_words.clear();
								}
							}
							else {
								next_word = true;
							}
						}
						else {
							count = true;
						}
					}
					else {
						word = true;
					}
				}
				if (word && !count && sym != ' ' && sym != ';') {
					localsave_ += sym;
				}
				if (count && word && sym != ' ' && sym != ';') {
					if (sym == '(') {
						bracket = true;
					}
					else {
						if (sym == ')') {
							bracket = false;
						}
						else {
							if (bracket) {
								localcount_ += sym;
							}
							else {
								local_next_word_ += sym;
							}
						}
					}
				}
			}
			word = false;
			count = false;
		}
	}
	in.close();

	return result_bigram_dict;
}

List<string> getKeys() {
	string line;
	string full_txt_path = "../txt/";
	full_txt_path += "text.txt";
	ifstream in(full_txt_path);
	string localsave_ = "";
	string last_localsave_ = "";
	bigramm_dict bigramm_iter;
	main_dict<bigramm_dict> bigramm;
	char sym;
	bool j_indicator = false;
	bool l_indicator = false;

	List<string> keys;

	map<string, int> frequency_dict;

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = (char)tolower(line.at(i));
				if ((sym != '"') && (sym != ';') && (sym != ',') && (sym != '.') &&
					(sym != '!') && (sym != '?') && (sym != 'Ф') && (sym != 'У') &&
					(sym != 'Т') && (sym != ' ') && (sym != 'С')) {
					localsave_ += sym;
				}
				else {
					if (((sym == ' ') || (i == line.length())) && (localsave_ != " ") && (localsave_.length() != 0)) {
						if (frequency_dict.count(localsave_)) {
							frequency_dict[localsave_]++;
						}
						else {
							frequency_dict[localsave_] = 1;
							keys.push_back(localsave_);
						}
						for (int j = 0; j < bigramm.line.GetSize(); j++) {
							if (bigramm.line[j].name_word == last_localsave_) {
								j_indicator = true;
								for (int l = 0; l < bigramm.line[j].next_words.GetSize(); l++) {
									if (bigramm.line[j].next_words[l] == localsave_) {
										l_indicator = true;
										bigramm.line[j].count_next_words[l]++;
									}
								}
								if (!l_indicator) {
									bigramm.line[j].next_words.push_back(localsave_);
									bigramm.line[j].count_next_words.push_back(1);
								}
							}
						}
						//cout << bool(j_indicator == false) << "\t" << bool(last_localsave_.length() > 0) << endl;
						if ((j_indicator == false) && (last_localsave_.length() > 0)) {
							bigramm_iter.name_word = last_localsave_;
							//bigramm_iter.next_words.push_back(localsave_);
							//bigramm_iter.count_next_words.push_back(1);
							bigramm.line.push_back(bigramm_iter);
							bigramm.line[bigramm.line.GetSize() - 1].next_words.push_back(localsave_);
							bigramm.line[bigramm.line.GetSize() - 1].count_next_words.push_back(1);
							//bigramm_iter.next_words.clear();
							//bigramm_iter.count_next_words.clear();
						}
						l_indicator = false;
						j_indicator = false;
						last_localsave_ = "";
						last_localsave_ += localsave_;
						localsave_ = "";
					}
					if (localsave_ == " ") {
						localsave_ = "";
					}
				}
			}
			localsave_ = "";
			last_localsave_ = "";
		}
	}
	in.close();
	return keys;
}
map<string, int> getFrequencyDict() {
	string line;
	string full_txt_path = "../txt/";
	full_txt_path += "text.txt";
	ifstream in(full_txt_path);
	string localsave_ = "";
	string last_localsave_ = "";
	bigramm_dict bigramm_iter;
	main_dict<bigramm_dict> bigramm;
	char sym;
	bool j_indicator = false;
	bool l_indicator = false;

	List<string> keys;

	map<string, int> frequency_dict;

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = (char)tolower(line.at(i));
				if ((sym != '"') && (sym != ';') && (sym != ',') && (sym != '.') &&
					(sym != '!') && (sym != '?') && (sym != 'Ф') && (sym != 'У') &&
					(sym != 'Т') && (sym != ' ') && (sym != 'С')) {
					localsave_ += sym;
				}
				else {
					if (((sym == ' ') || (i == line.length())) && (localsave_ != " ") && (localsave_.length() != 0)) {
						if (frequency_dict.count(localsave_)) {
							frequency_dict[localsave_]++;
						}
						else {
							frequency_dict[localsave_] = 1;
							keys.push_back(localsave_);
						}
						for (int j = 0; j < bigramm.line.GetSize(); j++) {
							if (bigramm.line[j].name_word == last_localsave_) {
								j_indicator = true;
								for (int l = 0; l < bigramm.line[j].next_words.GetSize(); l++) {
									if (bigramm.line[j].next_words[l] == localsave_) {
										l_indicator = true;
										bigramm.line[j].count_next_words[l]++;
									}
								}
								if (!l_indicator) {
									bigramm.line[j].next_words.push_back(localsave_);
									bigramm.line[j].count_next_words.push_back(1);
								}
							}
						}
						//cout << bool(j_indicator == false) << "\t" << bool(last_localsave_.length() > 0) << endl;
						if ((j_indicator == false) && (last_localsave_.length() > 0)) {
							bigramm_iter.name_word = last_localsave_;
							//bigramm_iter.next_words.push_back(localsave_);
							//bigramm_iter.count_next_words.push_back(1);
							bigramm.line.push_back(bigramm_iter);
							bigramm.line[bigramm.line.GetSize() - 1].next_words.push_back(localsave_);
							bigramm.line[bigramm.line.GetSize() - 1].count_next_words.push_back(1);
							//bigramm_iter.next_words.clear();
							//bigramm_iter.count_next_words.clear();
						}
						l_indicator = false;
						j_indicator = false;
						last_localsave_ = "";
						last_localsave_ += localsave_;
						localsave_ = "";
					}
					if (localsave_ == " ") {
						localsave_ = "";
					}
				}
			}
			localsave_ = "";
			last_localsave_ = "";
		}
	}
	in.close();
	return frequency_dict;
}
main_dict<bigramm_dict> getBigramm() {
	string line;
	string full_txt_path = "../txt/";
	full_txt_path += "text.txt";
	ifstream in(full_txt_path);
	string localsave_ = "";
	string last_localsave_ = "";
	bigramm_dict bigramm_iter;
	main_dict<bigramm_dict> bigramm;
	char sym;
	bool j_indicator = false;
	bool l_indicator = false;

	List<string> keys;

	map<string, int> frequency_dict;

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = (char)tolower(line.at(i));
				if ((sym != '"') && (sym != ';') && (sym != ',') && (sym != '.') &&
					(sym != '!') && (sym != '?') && (sym != 'Ф') && (sym != 'У') &&
					(sym != 'Т') && (sym != ' ') && (sym != 'С')) {
					localsave_ += sym;
				}
				else {
					if (((sym == ' ') || (i == line.length())) && (localsave_ != " ") && (localsave_.length() != 0)) {
						if (frequency_dict.count(localsave_)) {
							frequency_dict[localsave_]++;
						}
						else {
							frequency_dict[localsave_] = 1;
							keys.push_back(localsave_);
						}
						for (int j = 0; j < bigramm.line.GetSize(); j++) {
							if (bigramm.line[j].name_word == last_localsave_) {
								j_indicator = true;
								for (int l = 0; l < bigramm.line[j].next_words.GetSize(); l++) {
									if (bigramm.line[j].next_words[l] == localsave_) {
										l_indicator = true;
										bigramm.line[j].count_next_words[l]++;
									}
								}
								if (!l_indicator) {
									bigramm.line[j].next_words.push_back(localsave_);
									bigramm.line[j].count_next_words.push_back(1);
								}
							}
						}
						//cout << bool(j_indicator == false) << "\t" << bool(last_localsave_.length() > 0) << endl;
						if ((j_indicator == false) && (last_localsave_.length() > 0)) {
							bigramm_iter.name_word = last_localsave_;
							//bigramm_iter.next_words.push_back(localsave_);
							//bigramm_iter.count_next_words.push_back(1);
							bigramm.line.push_back(bigramm_iter);
							bigramm.line[bigramm.line.GetSize() - 1].next_words.push_back(localsave_);
							bigramm.line[bigramm.line.GetSize() - 1].count_next_words.push_back(1);
							//bigramm_iter.next_words.clear();
							//bigramm_iter.count_next_words.clear();
						}
						l_indicator = false;
						j_indicator = false;
						last_localsave_ = "";
						last_localsave_ += localsave_;
						localsave_ = "";
					}
					if (localsave_ == " ") {
						localsave_ = "";
					}
				}
			}
			localsave_ = "";
			last_localsave_ = "";
		}
		return bigramm;
	}
	in.close();
}

void read_text(string file_name, int type_read) {

	string line;
	string full_txt_path = "../txt/";
	full_txt_path += file_name;
	ifstream in(full_txt_path);
	string localsave_ = "";
	string last_localsave_ = "";
	bigramm_dict bigramm_iter;
	main_dict<bigramm_dict> bigramm;
	char sym;
	bool j_indicator = false;
	bool l_indicator = false;

	List<string> keys;

	map<string, int> frequency_dict;

	if (in.is_open()) {
		while (getline(in, line)) {
			for (int i = 0; i < line.length(); i++) {
				sym = (char)tolower(line.at(i));
				if ((sym != '"') && (sym != ';') && (sym != ',') && (sym != '.') && 
					(sym != '!') && (sym != '?') && (sym != 'Ф') && (sym != 'У') && 
					(sym != 'Т') && (sym != ' ') && (sym != 'С')) {
					localsave_ += sym;
				}
				else {
					if (((sym == ' ') || (i == line.length())) && (localsave_ != " ") && (localsave_.length() != 0)) {
						if (frequency_dict.count(localsave_)) {
							frequency_dict[localsave_]++;
						}
						else {
							frequency_dict[localsave_] = 1;
							keys.push_back(localsave_);
						}
						for (int j = 0; j < bigramm.line.GetSize(); j++) {
							if (bigramm.line[j].name_word == last_localsave_) {
								j_indicator = true;
								for (int l = 0; l < bigramm.line[j].next_words.GetSize(); l++) {
									if (bigramm.line[j].next_words[l] == localsave_) {
										l_indicator = true;
										bigramm.line[j].count_next_words[l]++;
									}
								}
								if (!l_indicator) {
									bigramm.line[j].next_words.push_back(localsave_);
									bigramm.line[j].count_next_words.push_back(1);
								}
							}
						}
						//cout << bool(j_indicator == false) << "\t" << bool(last_localsave_.length() > 0) << endl;
						if ((j_indicator == false) && (last_localsave_.length() > 0)) {
							bigramm_iter.name_word = last_localsave_;
							//bigramm_iter.next_words.push_back(localsave_);
							//bigramm_iter.count_next_words.push_back(1);
							bigramm.line.push_back(bigramm_iter);
							bigramm.line[bigramm.line.GetSize() - 1].next_words.push_back(localsave_);
							bigramm.line[bigramm.line.GetSize() - 1].count_next_words.push_back(1);
							//bigramm_iter.next_words.clear();
							//bigramm_iter.count_next_words.clear();
						}
						l_indicator = false;
						j_indicator = false;
						last_localsave_ = "";
						last_localsave_ += localsave_;
						localsave_ = "";
					}
					if (localsave_ == " ") {
						localsave_ = "";
					}
				}
			}
			localsave_ = "";
			last_localsave_ = "";
		}
	}
	in.close();

	if (type_read == 0) {
		ofstream out;								// поток дл€ записи
		out.open("../txt/frequency_dict.txt");		// открываем файл дл€ записи
		if (out.is_open())
		{
			for (int i = 0; i < keys.GetSize(); i++)
			{
				out << i << " " << keys[i] << " " << frequency_dict[keys[i]] << ";" << endl;
			}
		}
		out.close();								// закрываем поток

		cout << ">>> File 'frequency_dict.txt' has been written." << endl;

		out.open("../txt/bigramm_dict.txt");		// открываем файл дл€ записи
		if (out.is_open())
		{
			for (int i = 0; i < bigramm.line.GetSize(); i++)
			{
				out << i << " " << bigramm.line[i].name_word;
				for (int j = 0; j < bigramm.line[i].next_words.GetSize(); j++) {
					out << " " << bigramm.line[i].next_words[j] << " (" << bigramm.line[i].count_next_words[j] << ")";
				}
				out << ";" << endl;
			}
		}
		out.close();
		cout << ">>> File 'bigramm_dict.txt' has been written." << endl;
	}
}

string func_next_word(string last_word, main_dict<bigramm_dict>& bigramm, List<string>& keys, map<string, int> freq_dict) {
	int max_count_words = 0;
	int index_max_c_w = 0;
	bool bigr_false = true;

	for (int j = 0; j < bigramm.line.GetSize(); j++) {
		if (bigramm.line[j].name_word == last_word) {
			bigr_false = false;
			for (int counts = 0; counts < bigramm.line[j].count_next_words.GetSize(); counts++) {
				if (max_count_words <= bigramm.line[j].count_next_words[counts]) {
					max_count_words = bigramm.line[j].count_next_words[counts];
					index_max_c_w = counts;
				}
			}
			return bigramm.line[j].next_words[index_max_c_w];
		}
	}
	if (bigr_false) {
		for (int i = 0; i < keys.GetSize(); i++) {
			if (max_count_words < freq_dict[keys[i]]) {
				max_count_words = freq_dict[keys[i]];
				index_max_c_w = i;
			}
		}
		return keys[index_max_c_w];
	}
}

void gui() {

	read_text("text.txt", 0);

	string file_name_string = "";
	string count_n_local_ = "";
	int count_n = 0;

	List<string> keys = getKeys();
	map<string, int> freq_dict = getFrequencyDict();
	main_dict<bigramm_dict> bigramm = getBigramm();

	cout << "Please, enter filename where stay quest string:" << endl;
	cin >> file_name_string;

	cout << "Please, enter n count symbols:" << endl;
	cin >> count_n_local_;
	count_n = stoi(count_n_local_);

	string line;
	string full_txt_path = "../txt/";
	full_txt_path += file_name_string;
	ifstream in(full_txt_path);
	string last_word = "";
	string next_w = "";

	int index_last_space = 0;

	if (in.is_open()) {
		while (getline(in, line)) {
			cout << endl << line << " ";
			for (int i = 0; i < line.length(); i++) {
				if ((char)tolower(line.at(i)) == ' ') {
					index_last_space = i;
				}
			}
			for (int i = index_last_space + 1; i < line.length(); i++) {
				last_word += tolower(line.at(i));
			}
			//cout << last_word << endl;
			for (int i = 0; i <= count_n; i++) {
				next_w = func_next_word(last_word, bigramm, keys, freq_dict);
				cout << next_w << " ";
				last_word = next_w;
			}
		}
	}
	in.close();
}
