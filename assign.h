#pragma once
#include <vector>

const int CIRCLE = -1;
const int CROSS = -2;

std::vector<std::pair<int, int> > assign(std::vector<std::vector<int> >& table, bool maxMode = false);