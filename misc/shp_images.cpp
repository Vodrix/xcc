#include "stdafx.h"
#include "shp_decode.h"
#include "shp_images.h"

static t_theater_id g_theater = static_cast<t_theater_id>(-1);

int shp_images::load_shp(const Cshp_file& f, shp_images::t_image_data*& data)
{
	data = new t_image_data;
	data->cx = f.cx();
	data->cy = f.cy();
	data->cf = f.cf();
	int cb_data = f.get_offset(data->cf) - f.get_offset(0);
	data->data = new byte[cb_data];
	memcpy(data->data, f.get_image(0), cb_data);
	data->index = new t_image_index_entry[data->cf];
	for (int i = 0; i < data->cf; i++)
	{
		data->index[i].data_in = data->data + f.get_offset(i) - f.get_offset(0);
		data->index[i].data_out = NULL;
		data->index[i].format = f.get_format(i) << 28;
		if (f.get_format(i) == 2)
			data->index[i].format |= i - 1;
		else if (f.get_format(i) == 4)
		{
			for (int j = 0; j < data->cf; j++)
			{
				if (f.get_ref_offset(i) == f.get_offset(j))
				{
					data->index[i].format |= j;
					break;
				}
			}
		}
	}
	return 0;
}

const byte* shp_images::t_image_data::get(int f, int& cx0, int& cy0)
{
	if (this)
	{
		cx0 = cx;
		cy0 = cy;
		return get(f);
	}
	cx0 = 0;
	cy0 = 0;
	return NULL;
}

void shp_images::destroy_shp(t_image_data*& p)
{
	if (!p)
		return;
	delete[] p->data;
	for (int j = 0; j < p->cf; j++)
		delete[] p->index[j].data_out;
	delete[] p->index;
	delete p;
	p = NULL;
};

void shp_images::set_theater(t_theater_id v)
{
	g_theater = v;
};

const byte* shp_images::t_image_data::get(int f)
{
	if (!index[f].data_out && cx && cy)
	{
		int cb_out = cx * cy;
		index[f].data_out = new byte[cb_out];
		if (index[f].format & 8 << 28)
			LCWDecompress(index[f].data_in, index[f].data_out);
		else
		{
			memcpy(index[f].data_out, get(index[f].format & shp_o_mask), cb_out);
			ApplyXORDelta(index[f].data_in, index[f].data_out);
		}
	}
	return index[f].data_out;
}
