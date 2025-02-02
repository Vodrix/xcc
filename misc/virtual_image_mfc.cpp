#include "stdafx.h"

#include <afx.h>
#include <afxdlgs.h>
#include "dds_file.h"
#include "image_file.h"
#include "jpeg_file.h"
#include "pcx_file.h"
#include "pcx_file_write.h"
#include "png_file.h"
#include "tga_file.h"
#include "virtual_image.h"

int Cvirtual_image::get_clipboard()
{
	int error = 0;
	if (!OpenClipboard(NULL))
		return 0x100;
	void* h_mem = GetClipboardData(CF_DIB);
	if (!h_mem)
		error = 0x101;
	else
	{
		byte* mem = reinterpret_cast<byte*>(GlobalLock(h_mem));
		if (!mem)
			error = 0x102;
		else
		{	
			const BITMAPINFOHEADER* header = reinterpret_cast<BITMAPINFOHEADER*>(mem);
			int cb_pixel = header->biBitCount >> 3;
			if (cb_pixel != 1 && cb_pixel != 3)
				error = 0x103;
			else
			{
				t_palette_entry* palette = cb_pixel == 1 ? new t_palette : NULL;
				const RGBQUAD* r = reinterpret_cast<RGBQUAD*>(mem + header->biSize);
				if (palette)
				{
					for (int i = 0; i < (header->biClrUsed ? header->biClrUsed : 256); i++)
					{
						palette[i].r = r->rgbRed;
						palette[i].g = r->rgbGreen;
						palette[i].b = r->rgbBlue;
						r++;
					}
				}
				int cx = header->biWidth;
				int cy = header->biHeight;
				if (cx * cb_pixel & 3)
				{
					int cb_line = cx * cb_pixel;
					byte* d = new byte[cb_line * cy];
					byte* w = d;
					for (int y = 0; y < cy; y++)
					{
						memcpy(w, r, cb_line);
						r += cb_line + 3 >> 2;
						w += cb_line;
					}
					load(d, cx, cy, cb_pixel, palette);
					delete[] d;
				}
				else
					load(r, cx, cy, cb_pixel, palette);
				flip();
				if (cb_pixel == 3)
					swap_rb();
				delete palette;
			}
			GlobalUnlock(h_mem);
		}
	}
	CloseClipboard();
	return error;
}

int Cvirtual_image::set_clipboard() const
{
	int error = 0;
	int cb_line = cx() * cb_pixel();
	void* h_mem = GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + (cb_line + 3 & ~3) * cy());
	if (!h_mem)
		error = 0x100;
	else
	{
		byte* mem = reinterpret_cast<byte*>(GlobalLock(h_mem));
		if (!mem)
			error = 0x101;
		else
		{
			BITMAPINFOHEADER* header = reinterpret_cast<BITMAPINFOHEADER*>(mem);
			ZeroMemory(header, sizeof(BITMAPINFOHEADER));
			header->biSize = sizeof(BITMAPINFOHEADER);
			header->biWidth = cx();
			header->biHeight = cy();
			header->biPlanes = 1;
			header->biBitCount = cb_pixel() << 3;
			header->biCompression = BI_RGB;
			RGBQUAD* palette = reinterpret_cast<RGBQUAD*>(mem + sizeof(BITMAPINFOHEADER));
			if (cb_pixel() == 1)
			{
				for (int i = 0; i < 256; i++)
				{
					palette->rgbBlue = this->palette()[i].b;
					palette->rgbGreen = this->palette()[i].g;
					palette->rgbRed = this->palette()[i].r;
					palette->rgbReserved = 0;
					palette++;
				}
			}
			const byte* r = image() + cb_image();
			byte* w = reinterpret_cast<byte*>(palette);
			for (int y = 0; y < cy(); y++)
			{
				r -= cb_line;
				if (cb_pixel() == 3)
				{
					for (int x = 0; x < cx(); x++)
					{
						const t_palette_entry* v = reinterpret_cast<const t_palette_entry*>(r) + x;
						*w++ = v->b;
						*w++ = v->g;
						*w++ = v->r;
					}
					w -= cb_line;
				}
				else
					memcpy(w, r, cb_line);
				w += cb_line + 3 & ~3;
			}
			GlobalUnlock(h_mem);
			if (!OpenClipboard(NULL))
				error = 0x102;
			else
			{
				if (EmptyClipboard() && SetClipboardData(CF_DIB, h_mem))
					h_mem = NULL;
				else
					error = 0x103;
				CloseClipboard();
			}
		}
		if (h_mem)
			GlobalFree(h_mem);
	}
	return error;
}

int Cvirtual_image::load()
{
	const char* load_filter = "Image files (*.jpeg;*.jpg;*.pcx;*.png)|*.jpeg;*.jpg;*.pcx;*.png|JPEG files (*.jpeg;*.jpg)|*.jpeg;*.jpg|PCX files (*.pcx)|*.pcx|PNG files (*.png)|*.png|";

	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, load_filter, NULL);
	if (IDOK == dlg.DoModal())
		return load(static_cast<string>(dlg.GetPathName()));
	return 2;
}

int Cvirtual_image::save() const
{
	const char* save_filter = "JPEG files (*.jpeg;*.jpg)|*.jpeg;*.jpg|PCX files (*.pcx)|*.pcx|PNG files (*.png)|*.png|";

	CFileDialog dlg(false, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, save_filter, NULL);
	dlg.m_ofn.nFilterIndex = 2;
	if (IDOK == dlg.DoModal())
	{
		t_file_type ft = ft_pcx;
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			ft = ft_jpeg;
			break;
		case 2:
			ft = ft_pcx;
			break;
		case 3:
			ft = ft_png;
			break;
		}
		return save(static_cast<string>(dlg.GetPathName()), ft);
	}
	return 2;
}
