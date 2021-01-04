#include "twString.h"
#include <vector>

extern "C"
{
    #include "libjpeg/jpeglib.h"
    #include "libjpeg/jerror.h"
}

#include <windows.h>
#include <iostream>
#include "SOIL/SOIL.h"
#include "libpng/png.h"
#include "twImage.h"
#include "twDialog.h"

extern HINSTANCE instance;

using namespace std;

twImage::twImage()
{
	m_width = 0;
	m_height = 0;
	m_texture = 0;
	m_bitmap = NULL;
}

twImage::~twImage()
{

}

twImage::twImage(twString path)
{
	load(path);
	m_bitmap = NULL;
}

twImage::twImage(const unsigned char* data,int size)
{
	load(data, size);
	m_bitmap = NULL;
}

void twImage::load(twString path)
{
	clear();
	m_path = path;
	int channels;
	unsigned char* pixels = SOIL_load_image(path.c_str(), &m_width, &m_height, &channels, SOIL_LOAD_RGBA);
	if (pixels)
	{
		m_pixels.resize(m_width * m_height);
		memcpy(&m_pixels[0], pixels, m_width * m_height * 4);
		SOIL_free_image_data(pixels);
	}
	m_texture = 0;
}

void twImage::load(const unsigned char* data,int size)
{
    clear();
    m_path.clear();
    int channels;
    unsigned char* pixels = SOIL_load_image_from_memory(data, size, &m_width, &m_height, &channels, SOIL_LOAD_RGBA);
	if (pixels)
	{
		m_pixels.resize(m_width * m_height);
		memcpy(&m_pixels[0], pixels, m_width * m_height * 4);
		SOIL_free_image_data(pixels);
	}
	m_texture = 0;
}

void twImage::fromBitmap(HBITMAP &bitmap)
{
	BITMAP bitmap2;
	GetObject(bitmap, sizeof(BITMAP), &bitmap2);
	m_width = bitmap2.bmWidth;
	m_height = bitmap2.bmHeight;
	m_pixels.resize(m_width * m_height);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			COLORREF color = GetPixel(hdc, i, j);
			setPixel(twPosition(i, j), twColor(GetRValue(color), GetGValue(color), GetBValue(color)));
		}
	}
	DeleteObject(bitmap);
	DeleteDC(hdc);
}

bool twImage::save(twString path)
{
	if (path.size() < 4)
		return false;
	twString extension = path.substr(path.size() - 3);
	if (extension == "bmp" || extension == "BMP")
	{
		const unsigned char* pixels = reinterpret_cast<const unsigned char*>(&m_pixels[0]);
		SOIL_save_image(path.c_str(), SOIL_SAVE_TYPE_BMP, m_width, m_height, 4, pixels);
		return true;
	}
	if (extension == "png" || extension == "PNG")
	{
		FILE* file = fopen(path.c_str(), "wb");
		if (!file)
			return false;
		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png)
		{
			fclose(file);
			return false;
		}
		png_infop pngInfo = png_create_info_struct(png);
		if (!pngInfo)
		{
			fclose(file);
			png_destroy_write_struct(&png, NULL);
			return false;
		}
		if (setjmp(png->jmpbuf))
		{
			png_destroy_write_struct(&png, &pngInfo);
			return false;
		}
		png_init_io(png, file);
		png_set_IHDR(png, pngInfo, m_width, m_height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_write_info(png, pngInfo);
		png_byte* pixelsPtr = (png_byte*)&m_pixels[0];
		std::vector<png_byte*> rowPointers(m_height);
		for (unsigned int i = 0; i < m_height; ++i)
		{
			rowPointers[i] = pixelsPtr;
			pixelsPtr += m_width * 4;
		}
		png_set_rows(png, pngInfo, &rowPointers[0]);
		png_write_png(png, pngInfo, PNG_TRANSFORM_IDENTITY, NULL);
		png_write_end(png, pngInfo);
		png_destroy_write_struct(&png, &pngInfo);
		fclose(file);
		return true;
	}
	if (extension == "jpg" || extension == "JPG" || extension == "jpeg" || extension == "JPEG")
	{
		FILE* file = fopen(path.c_str(), "wb");
		if (!file)
			return false;
		jpeg_compress_struct compressInfo;
		jpeg_error_mgr errorManager;
		compressInfo.err = jpeg_std_error(&errorManager);
		jpeg_create_compress(&compressInfo);
		compressInfo.image_width = m_width;
		compressInfo.image_height = m_height;
		compressInfo.input_components = 3;
		compressInfo.in_color_space = JCS_RGB;
		jpeg_stdio_dest(&compressInfo, file);
		jpeg_set_defaults(&compressInfo);
		jpeg_set_quality(&compressInfo, 90, TRUE);
		vector<unsigned char> pixelsBuffer(m_width * m_height * 3);
		for (int i = 0; i < m_pixels.size(); ++i)
		{
			pixelsBuffer[i * 3 + 0] = m_pixels[i].r;
			pixelsBuffer[i * 3 + 1] = m_pixels[i].g;
			pixelsBuffer[i * 3 + 2] = m_pixels[i].b;
		}
		unsigned char* pixelsPtr = &pixelsBuffer[0];
		jpeg_start_compress(&compressInfo, TRUE);
		while (compressInfo.next_scanline < compressInfo.image_height)
		{
			JSAMPROW rowPointer = pixelsPtr + (compressInfo.next_scanline * m_width * 3);
			jpeg_write_scanlines(&compressInfo, &rowPointer, 1);
		}
		jpeg_finish_compress(&compressInfo);
		jpeg_destroy_compress(&compressInfo);
		fclose(file);
		return true;
	}
}

twSize twImage::getSize()
{
	return twSize(m_width, m_height);
}

twColor twImage::getPixel(twPosition position)
{
	if (position.X >= m_width || position.Y >= m_height || position.X < 0 || position.Y < 0)
		return twColor(0, 0, 0, 0);
	return m_pixels[position.X + position.Y * m_width];
}

void twImage::clear()
{
    m_pixels.clear();
}

void twImage::setPixel(twPosition position, twColor color)
{
	m_pixels[position.X + position.Y * m_width] = color;
	m_texture = 0;
}

void twImage::fromClipboard()
{
	if (!IsClipboardFormatAvailable(CF_BITMAP))
		return;
	if (!OpenClipboard(NULL))
		return;
	HBITMAP bitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	PSIZE size;
	GetBitmapDimensionEx(bitmap, size);
	BITMAP bitmap2;
	GetObject(bitmap, sizeof(BITMAP), &bitmap2);
	m_width = bitmap2.bmWidth;
	m_height = bitmap2.bmHeight;
	m_pixels.resize(m_width * m_height);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			COLORREF color = GetPixel(hdc, i, j);
			setPixel(twPosition(i, j), twColor(GetRValue(color), GetGValue(color), GetBValue(color)));
		}
	}
	CloseClipboard();
	DeleteObject(bitmap);
	DeleteDC(hdc);
}

void twImage::toClipboard()
{
	if (!OpenClipboard(NULL))
		return;
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, toBitmap());
	CloseClipboard();
}

bool twImage::isLoaded()
{
	if (m_width == 0 || m_height == 0 || m_pixels.size() != m_width * m_height)
		return false;
	return true;
}

HBITMAP twImage::toBitmap()
{
    if (m_bitmap != NULL)
        return m_bitmap;
	std::vector<unsigned char> iconPixels(m_width * m_height * 4);
    for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
    {
        iconPixels[i * 4 + 0] = m_pixels[i].b;
        iconPixels[i * 4 + 1] = m_pixels[i].g;
        iconPixels[i * 4 + 2] = m_pixels[i].r;
        iconPixels[i * 4 + 3] = m_pixels[i].a;
    }
    HBITMAP bitmap = CreateBitmap(m_width, m_height, 1, 32, &iconPixels[0]);
    m_bitmap = bitmap;
	return bitmap;
}

HICON twImage::toIcon()
{
	ICONINFO iconInfo;
	iconInfo.fIcon = true;
	HBITMAP bitmap = toBitmap();
	iconInfo.hbmMask = bitmap;
	iconInfo.hbmColor = bitmap;
	return CreateIconIndirect(&iconInfo);

}

twString twImage::getPath()
{
	return m_path;
}

void twImage::horizontalMirror()
{
	vector<twColor> pixels(m_pixels.size());
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			pixels[i + j * m_width] = m_pixels[(m_width - i) + j * m_width];
		}
	}
	m_pixels = pixels;
	m_texture = 0;
}

void twImage::setSize(twSize size)
{
	m_width = size.width;
	m_height = size.height;
	m_pixels.resize(m_width * m_height);
}

twImage* twImage::copy()
{
	twImage *image = new twImage();
	image->m_pixels = m_pixels;
	image->setSize(getSize());
	return image;
}
