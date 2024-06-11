#pragma once
#include <map>
#include <string>
#include "list_func.h"
#include "bigramm_dict.h"
#include "frequency_dict.h"


template<typename T>
class main_dict {

public:

	List<T> line;
};

map<string, int> read_freq_dict(string file_name);
main_dict<bigramm_dict> read_bigram_dict(string file_name);
List<string> getKeys();
map<string, int> getFrequencyDict();
main_dict<bigramm_dict> getBigramm();
string func_next_word(string last_word, main_dict<bigramm_dict>& bigramm, List<string>& keys, map<string, int> freq_dict);
void read_text(string file_name, int type_read);
void gui();