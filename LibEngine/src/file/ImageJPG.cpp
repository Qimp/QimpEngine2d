#include "common/Qimp.h"
#include "file/Image.h"
#include "file/FileManager.h"
#include "jpeglib.h"

QIMP_BEGIN_NAMESPACE

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);
	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

bool Image::LoadJPG(FileReader* reader)
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	memset(&cinfo, 0, sizeof(cinfo));
	/* We use our private extension JPEG error handler.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct my_error_mgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1] = { 0 };
	unsigned long location = 0;

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		* We need to clean up the JPEG object, close the input file, and return.
		*/
		jpeg_destroy_decompress(&cinfo);
		return false;
	}
	uint8* data = reader->GetPointer();
	uint32 dataLen = reader->Size();

	/* setup decompression process and source, then read JPEG header */
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char *)data, dataLen);
#if (JPEG_LIB_VERSION >= 90)
	jpeg_read_header(&cinfo, TRUE);
#else
	jpeg_read_header(&cinfo, true);
#endif

	Assert(cinfo.jpeg_color_space != JCS_RGB);
	Assert(cinfo.jpeg_color_space == JCS_GRAYSCALE || cinfo.jpeg_color_space == JCS_YCbCr);
	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);
	Assert(cinfo.output_components == 3);
	m_Channel = 3;
	m_Width = (uint32)(cinfo.output_width);
	m_Height = (uint32)(cinfo.output_height);
	row_pointer[0] = new unsigned char[cinfo.output_width * 3];
	m_Data = new unsigned char[cinfo.output_width * cinfo.output_height * 3];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (unsigned int i = 0; i < cinfo.output_width; i++)
		{
			m_Data[location + i * 3 + 0] = row_pointer[0][i * 3 + 0];
			m_Data[location + i * 3 + 1] = row_pointer[0][i * 3 + 1];
			m_Data[location + i * 3 + 2] = row_pointer[0][i * 3 + 2];
		}
		location += cinfo.output_width * 3;
	}
	jpeg_destroy_decompress(&cinfo);
	delete row_pointer[0];
	m_Format = TexFormat::RGB8;
	return true;
}

bool Image::LoadJPGx2(FileReader* reader)
{
	uint8* data = reader->GetPointer();
	uint32 dataLen = reader->Size();

	// 先验证一下是不是 jpg 文件
	if (data[0] != 0xFF || data[1] != 0xD8 || data[2] != 0xFF)
		return false;

	const int EMPTY_LINE = 2;
	
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	memset(&cinfo, 0, sizeof(cinfo));
	/* We use our private extension JPEG error handler.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct my_error_mgr jerr;
	
	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		* We need to clean up the JPEG object, close the input file, and return.
		*/
		jpeg_destroy_decompress(&cinfo);
		return false;
	}
	
	/* setup decompression process and source, then read JPEG header */
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char *)data, dataLen);
#if (JPEG_LIB_VERSION >= 90)
	jpeg_read_header(&cinfo, TRUE);
#else
	jpeg_read_header(&cinfo, true);
#endif

	Assert(cinfo.jpeg_color_space != JCS_RGB);
	Assert(cinfo.jpeg_color_space == JCS_GRAYSCALE || cinfo.jpeg_color_space == JCS_YCbCr);
	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);
	Assert(cinfo.output_components == 3);

	m_Channel = 4;
	m_Format = TexFormat::RGBA8;
	m_Width = (uint32)(cinfo.output_width);
	m_Height = (uint32)( (cinfo.output_height - EMPTY_LINE) / 2);

	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1] = { 0 };
	row_pointer[0] = new unsigned char[cinfo.output_width * 3];
	m_Data = new unsigned char[m_Width * m_Height * m_Channel];

	unsigned long location = 0;
	for (unsigned int h = 0; h < m_Height; h++)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (unsigned int w = 0; w < cinfo.output_width; w++)
		{
			location = (h * m_Width + w) * m_Channel;
			m_Data[location + 0] = row_pointer[0][w * 3 + 0];
			m_Data[location + 1] = row_pointer[0][w * 3 + 1];
			m_Data[location + 2] = row_pointer[0][w * 3 + 2];
		}
	}
	for (int i = 0; i < EMPTY_LINE; i++)
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
	for (unsigned int h = 0; h < m_Height; h++)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (unsigned int w = 0; w < cinfo.output_width; w++)
		{
			location = (h * m_Width + w) * m_Channel;
			m_Data[location + 3] = row_pointer[0][w * 3 + 0];
		}
	}

	jpeg_destroy_decompress(&cinfo);
	delete row_pointer[0];
	return true;
}

bool Image::SaveJPG(const char* szFilePath)
{
	bool bRet = false;

	if (szFilePath == NULL) return false;
	
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;					/* target file */
	JSAMPROW row_pointer[1];		/* pointer to JSAMPLE row[s] */
	int row_stride;					/* physical row width in image buffer */

	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	outfile = fopen(szFilePath, "wb");
	if (outfile == NULL) return false;
	
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = m_Width;	/* image width and height, in pixels */
	cinfo.image_height = m_Height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB;	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 95, true);
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = m_Width * 3; /* JSAMPLEs per row in image_buffer */
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &m_Data[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return true;
}



QIMP_END_NAMESPACE
