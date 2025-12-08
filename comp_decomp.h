#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdint>
#include <string>

using namespace std;

// read/write functions
// ============================================================
bool writeFile(const string &filename, const string &content) {
    ofstream out(filename, ios::binary);
    if (!out.is_open())
        return false;
    out.write(content.data(), content.size());
    out.close();
    return true;
}

string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Error: Cannot open file " + filename);
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}
// ============================================================


// Helper conversion functions
// ============================================================
vector<uint16_t> stringToU16(const string& s) {
    vector<uint16_t> out;
    out.reserve(s.size());
    for (unsigned char c : s)
        out.push_back((uint16_t)c);
    return out;
}

string u16ToString(const vector<uint16_t>& v) {
    string out;
    out.reserve(v.size());
    for (uint16_t x : v)
        out.push_back((char)(x & 0xFF));
    return out;
}
// ============================================================


// BPE FUNCTIONS
// ============================================================
map<pair<uint16_t,uint16_t>, int> getPairs(const vector<uint16_t>& data) {
    map<pair<uint16_t,uint16_t>, int> pairs;
    for (size_t i = 0; i + 1 < data.size(); ++i)
        pairs[{data[i], data[i+1]}]++;
    return pairs;
}

void bytePairEncode(vector<uint16_t>& data, map<uint16_t, pair<uint16_t,uint16_t>>& dict, int maxSymbols = 512) {
    uint16_t nextSymbol = 256;

    while (true) {
        auto pairs = getPairs(data);
        if (pairs.empty()) break;

        auto mostFreq = max_element(pairs.begin(), pairs.end(),
            [](const auto& a, const auto& b){ return a.second < b.second; });

        if (mostFreq->second < 2) break;

        auto pairToReplace = mostFreq->first;
        uint16_t newToken = nextSymbol++;
        dict[newToken] = pairToReplace;

        for (size_t i = 0; i + 1 < data.size();) {
            if (data[i] == pairToReplace.first && data[i+1] == pairToReplace.second) {
                data[i] = newToken;
                data.erase(data.begin() + i + 1);
            } else i++;
        }

        if (dict.size() >= maxSymbols) break;
    }
}

void bytePairDecode(vector<uint16_t>& data, const map<uint16_t, pair<uint16_t,uint16_t>>& dict) {
    bool replaced;
    do {
        replaced = false;
        vector<uint16_t> newData;

        for (auto x : data) {
            auto it = dict.find(x);
            if (it != dict.end()) {
                newData.push_back(it->second.first);
                newData.push_back(it->second.second);
                replaced = true;
            } else {
                newData.push_back(x);
            }
        }

        data = newData;
    } while (replaced);
}
// ============================================================


// COMPRESSED FILE I/O
// ============================================================
void writeCompressed(const string& filename, const vector<uint16_t>& data,
                     const map<uint16_t, pair<uint16_t,uint16_t>>& dict)
{
    ofstream file(filename, ios::binary);

    uint16_t dictSize = dict.size();
    file.write((char*)&dictSize, sizeof(dictSize));

    for (auto& p : dict) {
        file.write((char*)&p.first, sizeof(p.first));
        file.write((char*)&p.second.first, sizeof(p.second.first));
        file.write((char*)&p.second.second, sizeof(p.second.second));
    }

    uint32_t dataSize = data.size();
    file.write((char*)&dataSize, sizeof(dataSize));

    for (auto x : data)
        file.write((char*)&x, sizeof(x));
}

void readCompressed(const string& filename, vector<uint16_t>& data,
                    map<uint16_t, pair<uint16_t,uint16_t>>& dict)
{
    ifstream file(filename, ios::binary);

    uint16_t dictSize;
    file.read((char*)&dictSize, sizeof(dictSize));

    for (uint16_t i = 0; i < dictSize; ++i) {
        uint16_t key, first, second;
        file.read((char*)&key, sizeof(key));
        file.read((char*)&first, sizeof(first));
        file.read((char*)&second, sizeof(second));
        dict[key] = {first, second};
    }

    uint32_t dataSize;
    file.read((char*)&dataSize, sizeof(dataSize));

    data.resize(dataSize);
    file.read((char*)data.data(), dataSize * sizeof(uint16_t));
}
// ============================================================

