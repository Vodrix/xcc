#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <windows.h>
#include "crc.h"
#include "fname.h"
#include "file32.h"
#include "mix_file.h"
#include "multi_line.h"
#include "string_conversion.h"
#include "vartypes.h"
#include "xcc_dirs.h"

using namespace std;

struct t_idinfo
{
	string name;
	string description;
};

typedef map<int, t_idinfo> t_idlist;

void parse_line_normal(string& line, t_idinfo& idinfo)
{
	Cmulti_line mline = to_lower(line);
	idinfo.name = mline.get_next_line('\t');
	idinfo.description = mline.get_next_line('\t');
}

void add_file(const string& fname, t_idlist& id_list)
{
	cout << "Adding " + fname << endl;
	ifstream inf(fname.c_str());
	if (!inf)
	{
		cerr << "Error opening file" << endl;
		return;
	}
	int lineindex = 0;
	string line;
	t_idinfo idinfo;
	while (getline(inf, line))
	{
		lineindex++;
		parse_line_normal(line, idinfo);
		if (!idinfo.name.empty())
			id_list[Cmix_file::get_id(game_unknown, idinfo.name)] = idinfo;
	}
}

void write_list(t_idlist& id_list, Cfile32& f1, ofstream& f2)
{
	t_idinfo idinfo;
	const char* v_name[] = {"TEST"};
	int size = id_list.size();
	f1.write(&size, 4);
	for (auto& i : id_list)
	{
		f1.write(i.second.name.c_str(), i.second.name.size() + 1);
		f1.write(i.second.description.c_str(), i.second.description.size() + 1);
		f2 << nh(8, Cmix_file::get_id(game_unknown, i.second.name)) << '\t' << v_name << '\t' << i.second.name << '\t' << i.second.description << endl;
	}
}

void write_database(const string& ifname, t_idlist& test_list)
{
	Cfile32 f1;
	Cfname fname = ifname;
	ofstream f2;
	fname.set_ext(".dat");
	if (f1.open(fname.get_all().c_str(), GENERIC_WRITE))
	{
		cerr << "error opening binary output file" << endl;
		return;
	}
	fname.set_ext(".txt");
	f2.open(fname.get_all().c_str());
	if (f2.fail())
	{
		cerr << "error opening text output file" << endl;
		return;
	}
	write_list(test_list, f1, f2);
}

int main()
{
	t_idlist test_list;
	const string indir = xcc_dirs::get_data_dir();
	const string filenameend = " description.txt";

	add_file(indir + "mega" + filenameend, test_list);

	//only use one descriptor, the game it's assigned to wont matter as it's handled by mixer
	//cache works just as previous with game assignments
	//this will, in short, mean that there wont be duplicates between games
	
	write_database(indir + "global mix database", test_list);
	return 0;
}
