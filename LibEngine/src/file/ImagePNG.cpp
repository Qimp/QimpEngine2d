#include "common/Qimp.h"
#include "file/Image.h"
#include "file/FileManager.h" 
#include "png.h"

QIMP_BEGIN_NAMESPACE

#define PNGSIGSIZE  8

struct ImageSource
{
	uint8*	Data;
	int32	Size;
	int32	Offset;
};

static void PngReadCallback(png_structp pngPtr, png_bytep data, png_size_t length)
{
	ImageSource* isource = (ImageSource*)png_get_io_ptr(pngPtr);

	if ((int)(isource->Offset + length) <= isource->Size)
	{
		memcpy(data, isource->Data + isource->Offset, length);
		isource->Offset += length;
	}
	else
	{
		png_error(pngPtr, "pngReaderCallback failed");
	}
}

bool Image::LoadPNG(FileReader* reader)
{
	// length of bytes to check if it is a valid png file
	bool		result = false;
	png_byte	header[PNGSIGSIZE] = { 0 };
	png_structp	pngPtr = 0;
	png_infop	infoPtr = 0;

	uint8* data = reader->GetPointer();
	uint32 dataLen = reader->Size();
	do
	{
		// png header len is 8 bytes
		Assert(dataLen > PNGSIGSIZE);

		// check the data is png or not
		memcpy(header, data, PNGSIGSIZE);
		if (png_sig_cmp(header, 0, PNGSIGSIZE))
			break;

		// init png_struct
		pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (!pngPtr)
			break;

		// init png_info
		infoPtr = png_create_info_struct(pngPtr);
		if (!infoPtr)
			break;

		if (setjmp(png_jmpbuf(pngPtr)))
			break;

		// set the read call back function
		ImageSource imageSource;
		imageSource.Data = (unsigned char*)data;
		imageSource.Size = dataLen;
		imageSource.Offset = 0;
		png_set_read_fn(pngPtr, &imageSource, PngReadCallback);

		uint32 bitDepth = 8;

		// read png file info
		png_read_info(pngPtr, infoPtr);

		m_Width = png_get_image_width(pngPtr, infoPtr);
		m_Height = png_get_image_height(pngPtr, infoPtr);
		bitDepth = png_get_bit_depth(pngPtr, infoPtr);
		png_uint_32 colorType = png_get_color_type(pngPtr, infoPtr);

		png_set_strip_16(pngPtr); // strip 16 bit channels to 8 bit
		png_set_packing(pngPtr);  // separate palettized channels


		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (colorType == PNG_COLOR_TYPE_PALETTE)
		{
			//png_set_expand(pngPtr);
			png_set_palette_to_rgb(pngPtr);
		}

		// grayscale -> 8 bits
		if (!(colorType & PNG_COLOR_MASK_COLOR) && bitDepth < 8)
			png_set_expand(pngPtr);

		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
			png_set_expand(pngPtr);

		// expand grayscale images to RGB
		if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(pngPtr);

		// read png data
		png_uint_32 rowbytes;
		png_bytep* rowPointer = new png_bytep[m_Height];

		png_read_update_info(pngPtr, infoPtr);

		rowbytes = png_get_rowbytes(pngPtr, infoPtr);

		m_Data = new uint8[rowbytes * m_Height];
		if (!m_Data)
			break;

		for (uint32 i = 0; i < m_Height; ++i)
			rowPointer[i] = m_Data + i*rowbytes;

		png_read_image(pngPtr, rowPointer);
		png_read_end(pngPtr, NULL);

		m_Channel = rowbytes / m_Width;
		m_Format = (m_Channel == 3) ? TexFormat::RGB8 : TexFormat::RGBA8;

		delete [] rowPointer;

		result = true;
	} while (0);

	if (pngPtr)
	{
		png_destroy_read_struct(&pngPtr, (infoPtr) ? &infoPtr : 0, 0);
	}
	return result;
}

bool Image::SavePNG(const char* file)
{
	if (file == NULL) return false;
	Assert(m_Format == TexFormat::RGBA8);

	FILE*		fp;
	png_structp	png_ptr;
	png_infop	info_ptr;
	png_colorp	palette;
	png_bytep*	row_pointers;

	fp = fopen(file, "wb");
	if (fp == NULL) return false;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return false;
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, m_Width, m_Height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
	png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);
	row_pointers = (png_bytep *)malloc(m_Height * sizeof(png_bytep));
	if (row_pointers == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
	}
	{
		for (int i = 0; i < (int)m_Height; i++)
			row_pointers[i] = (png_bytep)m_Data + i * m_Width * 4;
		png_write_image(png_ptr, row_pointers);
		free(row_pointers);
		row_pointers = NULL;
	}
	png_write_end(png_ptr, info_ptr);
	png_free(png_ptr, palette);
	palette = NULL;
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return true;
}

QIMP_END_NAMESPACE
