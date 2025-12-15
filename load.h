#ifndef LOAD_H
#define LOAD_H

#include <string>
#include "myClass.h"

// reads a score file and fills a myClass object
// returns false if file cannot be opened
bool loadScoresFile(const std::string& filename, myClass& cls);

#endif
