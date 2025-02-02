#pragma once

#include "cc_file.h"
#include "palette.h"
#include "virtual_file.h"

class Cvirtual_image
{
public:
	const Cvirtual_image& palette(const t_palette_entry* palette, bool inflate = false);
	void remove_alpha();
	void add_alpha();
	void cb_pixel(int cb_pixel, const t_palette_entry* palette = NULL);
	void decrease_color_depth(int cb_pixel, const t_palette_entry* palette = NULL);
	void increase_color_depth(int cb_pixel);
	void increase_palette_depth();
	void flip();
	int get_clipboard();
	int set_clipboard() const;
	int load();
	int save() const;
	void load(const Cvirtual_binary& image, int cx, int cy, int cb_pixel, const t_palette_entry* palette, bool inflate = false);
	void load(const void* image, int cx, int cy, int cb_pixel, const t_palette_entry* palette, bool inflate = false);
	int load(const Cvirtual_binary& s);
	int load(const Cvirtual_file& f);
	int load(const string& fname);
	int save(Cvirtual_file& f, t_file_type ft) const;
	Cvirtual_file save(t_file_type ft) const;
	int save(const string& fname, t_file_type ft) const;
	void swap_rb();
	Cvirtual_image() = default;
	Cvirtual_image(const Cvirtual_binary& image, int cx, int cy, int cb_pixel, const t_palette_entry* palette = NULL, bool inflate = false);
	Cvirtual_image(const void* image, int cx, int cy, int cb_pixel, const t_palette_entry* palette = NULL, bool inflate = false);

	const byte* image() const
	{
		return m_image.data();
	}

	byte* image_edit()
	{
		return m_image.data_edit();
	}

	int cx() const
	{
		return m_cx;
	}

	int cy() const
	{
		return m_cy;
	}

	int cb_pixel() const
	{
		return mcb_pixel;
	}

	const t_palette_entry* palette() const
	{
		return reinterpret_cast<const t_palette_entry*>(m_palette.data());
	}

	int cb_image() const
	{
		return m_cx * m_cy * mcb_pixel;
	}

	int ofs8(int x, int y) const
	{
		return x + m_cx * y;
	}

	int ofs24(int x, int y) const
	{
		return ofs8(x, y) * 3;
	}

	int pixel8(int x, int y) const
	{
		return m_image.data()[ofs8(x, y)];
	}

	void pixel8(int x, int y, int v)
	{
		m_image.data_edit()[ofs8(x, y)] = v;
	}

	t_palette_entry pixel24(int x, int y) const
	{
		return reinterpret_cast<const t_palette_entry*>(m_image.data())[ofs8(x, y)];
	}
	
	void pixel24(int x, int y, t_palette_entry v)
	{
		reinterpret_cast<t_palette_entry*>(m_image.data_edit())[ofs8(x, y)] = v;
	}
private:
	Cvirtual_binary m_image;
	Cvirtual_binary m_palette;
	int m_cx = 0;
	int m_cy = 0;
	int mcb_pixel = 0;
};
