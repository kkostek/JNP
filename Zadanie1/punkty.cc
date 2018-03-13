/*
 *  Zadanie punkty
 *  Jakub Sieron 360326
 *  Konstanty Kostrzewski 382605
 */
#include <iostream>
#include <regex>
#include <map>
#include <fstream>

using namespace std;

/*
 * map of identifiers
 * string: key - identifier
 * int: value - it is easier to use int then string as an index etc.
 */
map<string, int> IDs;

/*
 * int: key; When new element is being added to map, it gets key as value
 * and key increases;
 */
int key = 0;

inline bool isInMap(const string &ID) {
    return (IDs.count(ID) > 0);
}

inline void errorParameters(const string &param) {
    cerr << "Usage: " << param << " file" << endl;
}

inline void errorFileProblem(const string &file) {
    cerr << "Error: problem with file " << file << endl;
}

inline void errorRepeatedID(int lineNum, const string &file, const string &line) {
    cerr << "Error in " << file << ", line " << lineNum << ": " << line << endl;
}

inline void errorIncorrectID(int lineNum, const string &file, const string &line) {
    cerr << "Error in " << file << ", line " << lineNum << ": " << line << endl;
}

bool readFile(const string &path) {
    fstream studentsIDs; //file contains students' IDs
    studentsIDs.open(path, ios::in);

    if (!studentsIDs.is_open()) {
        errorFileProblem(path);
        return false;
    }

    regex pattern("[a-z]{2}[0-9]{6}|[a-z]{2}m-[0-9]{4}");
    string line;
    int lineCounter = 1;

    while (!studentsIDs.eof()) {
        getline(studentsIDs, line);
        if (!line.empty()) {
            if (regex_match(line, pattern)) {
                if (isInMap(line)) {
                    errorRepeatedID(lineCounter, path, line);
                }
                else {
                    IDs.insert(pair<string, int>(line, key++));
                }
            }
            else {
                errorIncorrectID(lineCounter, path, line);
            }
        }
        lineCounter++;
    }

    studentsIDs.close();
    return true;
}

inline void errorLine(int lineNum, const string &line) {
    cerr << "Error in cin, line " << lineNum << ": " << line << endl;
}

inline void errorID(int lineNum, const string &ID) {
    cerr << "Error in cin, line " << lineNum << ": " << ID << endl;
}

bool isCorrectIndexList(const string &word) {
    string idRegEx = "(([a-z]{2}[0-9]{6})|([a-z]{2}m-[0-9]{4}))";
    regex pattern(idRegEx + "([+]" + idRegEx + "){1,2}");
    return regex_match(word, pattern);
}

bool isCorrectPath(const string &word) {
    regex path("grupa[1-8]/zadanie[1-6]");
    return regex_match(word, path);
}

vector<int> checkIDListCorectness(const string &idents, int lineNum) {
    vector<int> result;
    unsigned long firstPlus = idents.find_first_of('+'), lastPlus = idents.find_last_of('+');

    if (firstPlus == lastPlus) { //when two IDs
        string ID1 = idents.substr(0, firstPlus);
        string ID2 = idents.substr(firstPlus + 1);

        if (isInMap(ID1)) {
            result.push_back(IDs[ID1]);
        }
        else {
            errorID(lineNum, ID1);
        }

        if (ID2 != ID1 && isInMap(ID2)) {
            result.push_back(IDs[ID2]);
        }
        else {
            errorID(lineNum, ID2);
        }

    }
    else { //when three IDs
        string ID1 = idents.substr(0, firstPlus);
        string ID2 = idents.substr(firstPlus + 1, lastPlus - (firstPlus + 1));
        string ID3 = idents.substr(lastPlus + 1);

        if (isInMap(ID1)) {
            result.push_back(IDs[ID1]);
        }
        else {
            errorID(lineNum, ID1);
        }

        if (ID2 != ID1 && isInMap(ID2)) {
            result.push_back(IDs[ID2]);
        }
        else {
            errorID(lineNum, ID2);
        }

        if (ID3 != ID1 && ID3 != ID2 && isInMap(ID3)) {
            result.push_back(IDs[ID3]);
        }
        else {
            errorID(lineNum, ID3);
        }
    }
    return result;
}

vector<int> checkInputReturnIDs(const string &line, int lineNum) {
    vector<int> empty;

    string prefix = line.substr(0, line.find_last_of('/'));
    string students = line.substr(line.find_last_of('/') + 1);

    if (!isCorrectPath(prefix)) {
        errorLine(lineNum, line);
        return empty;
    }
    else if (!isCorrectIndexList(students)) {
        errorLine(lineNum, line);
        return empty;
    }

    return checkIDListCorectness(students, lineNum);
}

void addCooperations(vector<int> keys, int **cooperations) {

    for (auto &it1 : keys) {
        for (auto &it2 : keys) {
            if (it1 != it2) {
                cooperations[it1][it2]++;
            }
        }
    }
}

vector<int> getPoints(int **cooperations, unsigned long size) {
    vector<int> result;

    for (unsigned long i = 0; i < size; i++) {
        int sum = 0;
        for (unsigned long j = 0; j < size; j++) {
            sum += (cooperations[i][j] - 1) * cooperations[i][j] / 2;
        }
        result.push_back(sum);
    }
    return result;
}

inline string getIDNum(const string &ID) {
    return ID.substr(2);
}

vector<pair<string, int>> getResults(vector<int> points) {
    vector<pair<string, int>> results;

    for (auto &ID : IDs) {
        results.emplace_back(make_pair(getIDNum(ID.first), points[ID.second]));
    }

    sort(results.begin(), results.end());
    return results;
}

void printResults(vector<pair<string, int>> results) {

    for (auto &value : results) {
        if (value.second > 0) {
            cout << value.first << ";" << value.second << ";" << endl;
        }
    }
}

int main(int argc, char **argv) {

    if (argc != 2) {
        errorParameters(argv[0]);
        return 1;
    }

    if (!readFile(argv[1])) {
        return 1;
    }

    string line;
    int lineNum = 1;

    unsigned long numOfIDs = IDs.size();
    auto **cooperations = new int *[numOfIDs];

    for (unsigned long i = 0; i < numOfIDs; i++) {
        cooperations[i] = new int[numOfIDs];
    }

    for (unsigned long i = 0; i < numOfIDs; i++) {
        for (unsigned long j = 0; j < numOfIDs; j++) {
            cooperations[i][j] = 0;
        }
    }

    while (getline(cin, line)) {
        if (!line.empty()) {
            vector<int> keys = checkInputReturnIDs(line, lineNum);
            addCooperations(keys, cooperations);
        }
        lineNum++;
    }

    vector<int> points = getPoints(cooperations, numOfIDs);
    vector<pair<string, int>> results = getResults(points);

    printResults(results);

    return 0;
}
