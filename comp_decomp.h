#ifndef COMP_DECOMP
#define COMP_DECOMP

#include <string>
#include <vector>
#include <map>
#include <cstdint>
using namespace std;

bool writeFile(const string &filename, const string &content);
string readFile(const string& filename);

vector<uint16_t> stringToU16(const string& s);
string u16ToString(const vector<uint16_t>& v);

void bytePairEncode(vector<uint16_t>& data, map<uint16_t, pair<uint16_t,uint16_t>>& dict, int maxSymbols = 512);
void bytePairDecode(vector<uint16_t>& data, const map<uint16_t, pair<uint16_t,uint16_t>>& dict);

void writeCompressed(const string& filename, const vector<uint16_t>& data, const map<uint16_t, pair<uint16_t,uint16_t>>& dict);
void readCompressed(const string& filename, vector<uint16_t>& data, map<uint16_t, pair<uint16_t,uint16_t>>& dict);

void compress(const string& inputFile, const string& outputFile);
void decompress(const string& inputFile, const string& outputFile);

#endif
