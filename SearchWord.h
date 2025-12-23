#ifndef SEARCHWORD_H
#define SEARCHWORD_H

#include <string>
#include <vector>

using namespace std; 

// struct to hold match info
struct PostMatch {
    string id;   // id of post
    string text; // text/body of post
};

// search posts by word inside <text> or <body>
vector<PostMatch> searchPostsByWord(const string& xmlContent,
                                    const string& word);

// print matches to console
void printMatches(const vector<PostMatch>& matches);

#endif
