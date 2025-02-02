#include "stdafx.h"
#include "image_file.h"

#include "jpeg_file.h"
#include "pcx_file_write.h"
#include "png_file.h"
#include "tga_file.h"

int image_file_write(Cvirtual_file& f, t_file_type ft, const byte* image, const t_palette_entry* palette, int cx, int cy, int pixel)
{
	switch (ft)
	{
	case ft_jpeg:
		return jpeg_file_write(f, image, palette, cx, cy, -1, pixel);
	case ft_pcx:
		pcx_file_write(f, image, palette, cx, cy, pixel);
		return 0;
	case ft_tga:
		f = palette
			? tga_file_write(image, cx, cy, palette)
			: tga_file_write(image, cx, cy, pixel);
		return 0;
	default:
		return png_file_write(f, image, palette, cx, cy, pixel);
	}
}

Cvirtual_file image_file_write(t_file_type ft, const byte* image, const t_palette_entry* palette, int cx, int cy, int pixel)
{
	Cvirtual_file f;
	if (image_file_write(f, ft, image, palette, cx, cy, pixel))
		f.clear();
	return f;
}

int image_file_write(const string& name, t_file_type ft, const byte* image, const t_palette_entry* palette, int cx, int cy, int pixel)
{
	switch (ft)
	{
	case ft_jpeg:
		return jpeg_file_write(name, image, palette, cx, cy, -1, pixel);
	case ft_pcx:
		return pcx_file_write(name, image, palette, cx, cy, pixel);
	case ft_tga:
		return palette
			? tga_file_write(image, cx, cy, palette).save(name)
			: tga_file_write(image, cx, cy, pixel).save(name);
	default:
		return png_file_write(name, image, palette, cx, cy, pixel);
	}
}
