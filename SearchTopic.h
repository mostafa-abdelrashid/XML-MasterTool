#ifndef SEARCHTOPIC_H
#define SEARCHTOPIC_H

#include <string>
#include <vector>
#include "SearchWord.h"

using namespace std; 

// Search the XML content for posts where a <topic> equals 'topic' (case-insensitive).
// Returns a vector of PostMatch for matching posts.
vector<PostMatch> searchPostsByTopic(const string& xmlContent, const string& topic);

#endif
