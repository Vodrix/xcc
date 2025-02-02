#include "stdafx.h"
#include "jpeg_file.h"
#include "png_file.h"
#include <jpeglib.h>
#include "fname.h"

class Csource_manager
{
public:
	jpeg_source_mgr* attach(const byte* d, int cb_d)
	{
		pub.init_source = init_source;
		pub.fill_input_buffer = fill_input_buffer;
		pub.skip_input_data = skip_input_data;
		pub.resync_to_restart = jpeg_resync_to_restart;
		pub.term_source = term_source;
		pub.bytes_in_buffer = cb_d; 
		pub.next_input_byte = d; 
		return reinterpret_cast<jpeg_source_mgr*>(this);
	}

	static void init_source(j_decompress_ptr cinfo)
	{
	}

	static boolean fill_input_buffer(j_decompress_ptr cinfo)
	{
		// ERREXIT();
		return true;
	}

	static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
	{
		if (num_bytes < 0)
			return;
		cinfo->src->next_input_byte += num_bytes;
		cinfo->src->bytes_in_buffer -= num_bytes;
	}

	static void term_source(j_decompress_ptr cinfo)
	{
	}

	jpeg_source_mgr pub;
};

#ifdef JPEG_SUPPORT
int Cjpeg_file::decode(Cvirtual_image& d) const
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	
	Csource_manager source_manager;
	cinfo.src = source_manager.attach(get_data(), get_size());
	
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	
	const int cx = cinfo.output_width;
	const int cy = cinfo.output_height;
	const int cb_pixel = cinfo.output_components;
	byte* image = new byte[cx * cy * cb_pixel];
	byte* w = image;
	
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		jpeg_read_scanlines(&cinfo, reinterpret_cast<byte**>(&w), 1);
		w += cx * cb_pixel;
	}

	if (cb_pixel == 1)
	{
		t_palette palette;
		d.load(image, cx, cy, cb_pixel, palette);
	}
	else
		d.load(image, cx, cy, cb_pixel, NULL);
	delete[] image;
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	
	return 0;
}

int jpeg_file_write(Cvirtual_file& f, const byte* image, const t_palette_entry* palette, int cx, int cy, int q, int pixel)
{
	string temp_fname = get_temp_fname();
	int error = jpeg_file_write(temp_fname, image, palette, cx, cy, q, pixel);
	if (!error)
	{
		Cvirtual_binary s;
		error = f.load(temp_fname);
	}
	delete_file(temp_fname);
	return error;
}

int jpeg_file_write(const string& name, const byte* image, const t_palette_entry* palette, int cx, int cy, int q, int pixel)
{
	t_palette_entry* s;
	if (palette)
	{
		int count = cx * cy;
		s = new t_palette_entry[count];
		const byte* r = image;
		t_palette_entry* w = s;
		while (count--)
		{
			*w++ = palette[*r++];
		}
	}
	//else
	//	s = const_cast<t_palette_entry*>(reinterpret_cast<const t_palette_entry*>(image));
	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	FILE* f = fopen(name.c_str(), "wb");
	if (!f) 
	  return 1;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, f);

	cinfo.image_width = cx;
	cinfo.image_height = cy;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	if (q != -1)
		jpeg_set_quality(&cinfo, q, true);
	jpeg_start_compress(&cinfo, TRUE);

	if (palette || pixel == 3)
	{
		auto r = reinterpret_cast<const t_palette_entry*>(image);
		while (cinfo.next_scanline < cinfo.image_height)
		{
			jpeg_write_scanlines(&cinfo, const_cast<byte**>(reinterpret_cast<const byte**>(&r)), 1);
			r += cx;
		}
	}
	else if (pixel > 3)
	{
		byte* tmp_line = new byte[3 * cx];
		switch (pixel)
		{
		case 4:
		{
			auto r = reinterpret_cast<const t_palette32_entry*>(image);
			while (cinfo.next_scanline < cinfo.image_height)
			{
				for (int i = 0; i < cx; ++i)
				{
					memcpy(tmp_line + 3 * i, r++, 3);
				}
				auto tmp_line_p = tmp_line;
				jpeg_write_scanlines(&cinfo, reinterpret_cast<byte**>(&tmp_line_p), 1);
			}
			break;
		}
		case 6:
		{
			auto r48 = reinterpret_cast<const t_palette48_entry*>(image);
			while (cinfo.next_scanline < cinfo.image_height)
			{
				for (int i = 0; i < 3 * cx; )
				{
					tmp_line[i++] = linear2sRGB(r48->r);
					tmp_line[i++] = linear2sRGB(r48->g);
					tmp_line[i++] = linear2sRGB(r48->b);
					r48++;
				}
				auto tmp_line_p = tmp_line;
				jpeg_write_scanlines(&cinfo, reinterpret_cast<byte**>(&tmp_line_p), 1);
			}
			break;
		}
		case 8:
		{
			auto r64 = reinterpret_cast<const t_palette64_entry*>(image);
			while (cinfo.next_scanline < cinfo.image_height)
			{
				for (int i = 0; i < 3 * cx; )
				{
					tmp_line[i++] = linear2sRGB(r64->r);
					tmp_line[i++] = linear2sRGB(r64->g);
					tmp_line[i++] = linear2sRGB(r64->b);
					r64++;
				}
				auto tmp_line_p = tmp_line;
				jpeg_write_scanlines(&cinfo, reinterpret_cast<byte**>(&tmp_line_p), 1);
			}
			break;
		}
		default:
			break;
		}
		delete[] tmp_line;
	}


	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

    fclose(f);

	if (palette)
		delete[] s;
	return 0;
}
#else
int Cjpeg_file::decode(Cvirtual_image& d) const
{
	return 1;
}

int jpeg_file_write(Cvirtual_file& f, const byte* image, const t_palette_entry* palette, int cx, int cy, int q)
{
	return 1;
}

int jpeg_file_write(const string& name, const byte* image, const t_palette_entry* palette, int cx, int cy, int q)
{
	return 1;
}
#endif
