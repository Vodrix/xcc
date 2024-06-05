#pragma once

#include "cc_structures.h"
#include "image_file.h"
#include "palette.h"

class Ccps_file : public Cimage_file<t_cps_header>
{
public:
	void decode(void*) const;
	bool is_valid() const;
	Cvirtual_image vimage() const;

	int cb_pixel() const
	{
		return 1;
	}

	int cx() const override
	{
		return 320;
	}

	int cy() const override
	{
		return 200;
	}

	const byte* get_image() const
	{
		return data() + sizeof(t_cps_header) + header().palette_size;
	}

	const t_palette_entry* palette() const
	{
		return header().palette_size ? reinterpret_cast<const t_palette_entry*>(data() + sizeof(t_cps_header)) : NULL;
	}
};

Cvirtual_binary cps_file_write(const byte* s, const t_palette_entry* palette);
