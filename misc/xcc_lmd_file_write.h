#pragma once

#include <vector>
#include "cc_structures.h"
#include <virtual_binary.h>

using namespace std;

class Cxcc_lmd_file_write  
{
public:
	void add_fname(const string& fname);
	Cvirtual_binary write(t_game game) const;
private:
	using t_index = vector<string>;
	t_index m_index;
};
