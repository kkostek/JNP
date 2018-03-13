#include <iostream>
#include <unordered_map>
#include <cassert>
#include <cstring>
#include "dict.h"
#include "dictglobal.h"
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

using namespace std;

typedef unordered_map<string, string> dictionary;
typedef unordered_map<unsigned long, dictionary> map_of_dicts;
unsigned long key = 1;


void print_errors(string s) {
    if (debug) {
        cerr << s << endl;
    }
}

static inline string not_exist(unsigned long dict) {
    return "dict " + to_string(dict) + " does not exist";
}

static inline string dict_name(unsigned long id) {
    return id == dict_global() ? "the Global Dictionary" : ("dict " + to_string(id));
}

map_of_dicts& Dicts_map() {
    static map_of_dicts *ptr = nullptr;
    static dictionary global_dict;
    if (!ptr) {
        ptr = new map_of_dicts({{dict_global(), global_dict}});
        print_errors("the Global Dictionary has been created");
    }
    return *ptr;
}

unsigned long dict_new() {
    auto& Dicts = Dicts_map();
    if (debug) {
        assert(key != 0);
    }
    print_errors("dict_new()");
    dictionary new_dict;
    Dicts.insert({key++, new_dict});
    print_errors("dict_new: dict " + to_string(key - 1) + " has been created");
    return key - 1;
}

void dict_delete(unsigned long id) {
    auto& Dicts = Dicts_map();
    print_errors("dict_delete(" + to_string(id) + ")");
    if (Dicts.count(id)) {

        if (id != dict_global()) {
            Dicts.erase(id);
        }
        string s = id == dict_global() ? "an attempt to remove the Global Dictionary"
                                       : ("dict " + to_string(id) + " has been deleted");
        print_errors("dict_delete: " + s);
    }
    else {
        print_errors("dict_delete: " + not_exist(id));
    }
}

size_t dict_size(unsigned long id) {
    auto& Dicts = Dicts_map();
    print_errors("dict_size(" + to_string(id) + ")");

    if (Dicts.count(id)) {
        dictionary curr_dict = Dicts.find(id)->second;
        size_t result = curr_dict.size();
        print_errors("dict_size: " + dict_name(id) + " contains "
                     + to_string(result) + " element(s)");
        return result;

    } else {
        print_errors("dict_size: " + not_exist(id));
        return 0;
    }
}

void dict_insert(unsigned long id, const char* key, const char* value) {
    auto& Dicts = Dicts_map();
    string s_key = key == NULL ? "NULL" : ("\"" + string(key) + "\"");
    string s_value = value == NULL ? "NULL" : ("\"" + string(value) + "\"");
    print_errors("dict_insert(" + to_string(id) + ", "
                 + s_key + ", " + s_value + ")");

    if (Dicts.count(id) && key != NULL && value != NULL) {
        string curr_key = string(key);
        string curr_value = string(value);
        pair<string, string> curr_position = make_pair(curr_key, curr_value);

        if (id != dict_global()) {
            print_errors("dict_insert: dict " + to_string(id)
                         + ", the pair (\"" + key + "\", \""
                         + value + "\")" + " has been inserted");
                         // Druga poprawka - usuwanie klucza przed dodaniem
                         Dicts.find(id)->second.erase(curr_key);
                         Dicts.find(id)->second.insert(curr_position);
        }
        else if (id == dict_global()) {

                if (Dicts.find(id)->second.size() < MAX_GLOBAL_DICT_SIZE) {
                    print_errors("dict_insert: the Global Dictionary, the pair"
                                 + string(" (\"") + string(key) + "\", \""
                                 + string(value) + "\") has been inserted");
                                 // Druga Poprawka - usuwanie klucza przed dodaniem
                                 Dicts.find(id)->second.erase(curr_key);
                                 Dicts.find(id)->second.insert(curr_position);
                }
                else {
                    print_errors("dict_insert: the Global Dictionary, an "
                                 + string("attempt to insert too many ")
                                 + "elements");
		            }
        }
    }

    if (!Dicts.count(id)) {
        print_errors("dict_insert: " + not_exist(id));
    }

    if (key == NULL) {
        print_errors("dict_insert: " + dict_name(id)
                     + ", an attempt to insert NULL key");
    }

    if (value == NULL) {
        print_errors("dict_insert: " + dict_name(id)
                     + ", an attempt to insert NULL value");
    }
}

void dict_remove(unsigned long id, const char* key) {
    auto& Dicts = Dicts_map();
    // Pierwsza poprawka - sprawdzanie czy klucz może być nullem
    if (key == NULL) {
        print_errors("dict_remove: an attempt to remove NULL key");
        return;
    }
    print_errors("dict_remove(" + to_string(id) + ", \"" + key + "\")");

    if (Dicts.count(id)) {
        if (Dicts.find(id)->second.count(key)) {
            Dicts.find(id)->second.erase(key);
            print_errors("dict_remove: " + dict_name(id) + ", the key \""
                         + key + "\" has been removed");
        }
        else {
            print_errors("dict_remove: " + dict_name(id)
                         + " does not contain the key \"" + key + "\"");
        }
    }
    else {
        print_errors("dict_remove: " + not_exist(id));
    }
}

static inline const char* dict_find_help(unsigned long id, unsigned long parent, const char* key) {
    auto& Dicts = Dicts_map();

    if (Dicts.count(id) && Dicts.find(id)->second.count(key)) {
        string value = Dicts.find(id)->second.find(key)->second;
        print_errors("dict_find: " + dict_name(parent) + ", the key \"" + key
                     + "\" has the value \"" + value + "\"");
        return value.c_str();
    }
    else {
        string if_true = "not found";
        string if_false = "not found, looking up the Global Dictionary";
        string looking_up = id == dict_global() ? if_true : if_false;
        print_errors("dict_find: the key \"" + string(key) + "\" "
                     + looking_up);
    }

    if (!Dicts.count(id)) {
        print_errors("dict_find: dict " + to_string(id) + " does not exist");
    }
    return NULL;
}

const char* dict_find(unsigned long id, const char* key) {
    // Pierwsza poprawka - sprawdzanie czy klucz może być nullem
    if (key == NULL) {
      print_errors("dict_find: an attempt to find NULL key");
      return NULL;
    }
    print_errors("dict_find(" + to_string(id) + ", \"" + key + "\")");
    const char* ans = dict_find_help(id, id, key);

    if (ans == NULL && id != dict_global()) {
        ans = dict_find_help(dict_global(), id, key);
    }
    return ans;
}

void dict_clear(unsigned long id) {
    auto& Dicts = Dicts_map();
    print_errors("dict_clear(" + to_string(id) + ")");

    if (Dicts.count(id)) {
        Dicts.find(id)->second.clear();
        print_errors("dict_clear: " + dict_name(id) + " has been cleared");
    }
    else {
        print_errors("dict_clear: " + not_exist(id));
    }
}

void dict_copy(unsigned long src_id, unsigned long dst_id) {
    auto& Dicts = Dicts_map();
    print_errors("dict_copy(" + to_string(src_id) + ", " + to_string(dst_id) + ")");
    if (Dicts.count(src_id) && Dicts.count(dst_id)) {

      size_t src_size = Dicts.find(src_id)->second.size();
      size_t dst_size = Dicts.find(dst_id)->second.size();
      if (dst_id == dict_global() && src_size + dst_size >  MAX_GLOBAL_DICT_SIZE) {
          print_errors("dict_copy: the Global Dictionary is too small "
                       + string("to copy all the elements"));
          return;
      }

        for (auto &val : Dicts.find(src_id)->second) {
          // Druga poprawka - usuwanie klucza przed dodaniem
          Dicts.find(dst_id)->second.erase(val.first);
            Dicts.find(dst_id)->second.insert(val);
        }
        print_errors("dict_copy: dict " + to_string(src_id)
                     + " has been copied to dict " + to_string(dst_id));
    }
    else {

        if (!Dicts.count(src_id)) {
            print_errors("dict_copy: " + not_exist(src_id));
        }

        if (!Dicts.count(dst_id)) {
            print_errors("dict_copy: " + not_exist(dst_id));
        }
    }
}
