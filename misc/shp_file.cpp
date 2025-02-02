#include "stdafx.h"
#include "shp_file.h"

#include "image_file.h"
#include "shp_decode.h"
#include "shp_images.h"
#include "string_conversion.h"

class Cshp_decoder : public Cvideo_decoder
{
public:
	int cb_pixel() const
	{
		return m_f.cb_pixel();
	}

	int cf() const
	{
		return m_f.cf();
	}

	int cx() const
	{
		return m_f.cx();
	}

	int cy() const
	{
		return m_f.cy();
	}

	int decode(void* d0)
	{
		if (m_frame_i >= cf())
			return 1;
		memcpy(d0, m_video.data() + cb_image() * m_frame_i, cb_image());
		m_frame_i++;
		return 0;
	}

	const t_palette_entry* palette() const
	{
		return m_palette;
	}

	int seek(int f)
	{
		m_frame_i = f;
		return 0;
	}

	Cshp_decoder(const Cshp_file& f, const t_palette_entry* palette)
	{
		m_f.load(f);
		m_f.decode(m_video.write_start(cb_video()));
		m_frame_i = 0;
		memcpy(m_palette, palette, sizeof(t_palette));
	}
private:
	Cshp_file m_f;
	int m_frame_i;
	t_palette m_palette;
	Cvirtual_binary m_video;
};

Cvideo_decoder* Cshp_file::decoder(const t_palette_entry* palette)
{
	return new Cshp_decoder(*this, palette);
}

bool Cshp_file::is_valid() const
{
	const t_shp_header& h = header();
	int size = get_size();
	if (sizeof(t_shp_header) > size || h.c_images < 1 || h.c_images > 1000 || sizeof(t_shp_header) + 8 * (h.c_images + 2) > size)
		return false;
	return !(get_offset(cf()) != size || get_offset(cf() + 1)) || get_offset(cf() + 1) == size;
}

void Cshp_file::decode(void* d) const
{
	shp_images::t_image_data* p;
	if (shp_images::load_shp(*this, p))
		return;
	byte* w = reinterpret_cast<byte*>(d);
	for (int i = 0; i < cf(); i++)
	{
		memcpy(w, p->get(i), cb_image());
		w += cb_image();
	}
	shp_images::destroy_shp(p);
}

Cvirtual_image Cshp_file::vimage() const
{
	Cvirtual_binary image;
	decode(image.write_start(cb_video()));
	return Cvirtual_image(image, cx(), cf() * cy(), cb_pixel());
}

Cvirtual_binary shp_file_write(const byte* s, int cx, int cy, int c_images)
{
	Cvirtual_binary d;
	const byte* r = s;
	byte* w = d.write_start(sizeof(t_shp_ts_header) + (sizeof(t_shp_ts_image_header) + cx * cy) * c_images);
	t_shp_header& header = *reinterpret_cast<t_shp_header*>(w);
    header.c_images = c_images;
    header.xpos = 0;
    header.ypos = 0;
    header.cx = cx;
    header.cy = cy;
		header.delta = 0;
		header.flags = 0;
	w += sizeof(t_shp_header);
	int* index = reinterpret_cast<int*>(w);
	w += 8 * (c_images + 2);

	const byte* last = r;
	const byte* last40 = nullptr;
	const byte* last80 = r;
	byte* last80w = w;
	int count20 = 0;
	int deltaframe = 0;
	int largest = 0;

	//first frame is always format80(LCW)
	*index++ = 0x80000000 | w - d.data();
	*index++ = 0;
	w += LCWCompress(r, w, cx * cy);
	r += cx * cy;
	largest = w - last80w;

	for (int i = 0; i < c_images; i++)
	{
		int size80;
		int size40;
		int size20;

		// do test encodes of the 3 possible frame formats to see which is
		// smaller.
		if (last40 != nullptr) {
			size20 = GenerateXORDelta(last, r, w, cx * cy);
		}
		else {
			size20 = 0x7FFFFFFF;
		}

		size40 = GenerateXORDelta(last80, r, w, cx * cy);
		size80 = LCWCompress(r, w, cx * cy);

		// if format80 is smallest or equal, do format80
		if (size80 <= size40 && size80 <= size20) {
			*index++ = 0x80000000 | w - d.data();
			*index++ = 0;
			last80 = r;
			last40 = nullptr;
			last = r;
			last80w = w;
			w += LCWCompress(r, w, cx * cy);
			r += cx * cy;

			if (size80 > largest) {
				largest = size80;
			}
		}
		else if (size40 <= size20) {
			*index++ = 0x40000000 | w - d.data();
			*index++ = 0x80000000 | last80w - d.data();
			last = r;
			last40 = r;
			deltaframe = i;
			w += GenerateXORDelta(last80, r, w, cx * cy);
			r += cx * cy;

			if (size40 > largest) {
				largest = size40;
			}
		}
		else {
			*index++ = 0x20000000 | w - d.data();
			*index++ = 0x48000000 | deltaframe;
			w += GenerateXORDelta(last, r, w, cx * cy);
			last = r;
			r += cx * cy;

			if (size20 > largest) {
				largest = size20;
			}
		}
	}

	header.delta = largest + 37;

	*index++ = w - d.data();
	*index++ = 0;
	*index++ = 0;
	*index++ = 0;
	d.set_size(w - d.data());
	return d;
}
