#include "stdafx.h"
#include "map_ra_ini_reader.h"
#include "xcc/string_view.h"

#include "multi_line.h"
#include "string_conversion.h"

static const char* section_code[] = {"basic", "map", "unknown"};
static const char* basic_code[] = {"name", "player", "unknown"};
static const char* map_code[] = {"x", "y", "width", "height", "theater", "unknown"};

int Cmap_ra_ini_reader::process_section_start(string_view line)
{
	m_section = t_section_id(find_id(line, section_code, sei_unknown));
	return 0;
}

bool Cmap_ra_ini_reader::process_section() const
{
	return m_section != sei_unknown;
}

int Cmap_ra_ini_reader::process_key(string_view name, string_view value)
{
	switch (m_section)
	{
	case sei_basic:
		switch (find_id(name, basic_code, bai_unknown))
		{
		case bai_name:
			m_basic_data.name = value;
			break;
		case bai_player:
			m_basic_data.player = value;
			break;
		}
		break;
	case sei_map:
		switch (find_id(name, map_code, mai_unknown))
		{
		case mai_x:
			m_map_data.x = to_int(value);
			break;
		case mai_y:
			m_map_data.y = to_int(value);
			break;
		case mai_cx:
			m_map_data.cx = to_int(value);
			break;
		case mai_cy:
			m_map_data.cy = to_int(value);
			break;
		case mai_theater:
			m_map_data.theater = value;
			break;
		}
		break;
	}
	return 0;
}

bool Cmap_ra_ini_reader::is_valid() const
{
	return !(m_map_data.x < 1 ||
		m_map_data.y < 1 ||
		m_map_data.x + m_map_data.cx > 127 ||
		m_map_data.y + m_map_data.cy > 127 ||
		m_map_data.theater.empty());
}
