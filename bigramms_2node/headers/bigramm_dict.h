#pragma once
#include <string>
#include "list_func.h"

using namespace std;

struct bigramm_dict {
	string name_word;
	List<string> next_words;
	List<int> count_next_words;
};