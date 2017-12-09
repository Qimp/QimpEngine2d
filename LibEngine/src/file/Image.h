#ifndef __IMAGE__H__
#define __IMAGE__H__

QIMP_BEGIN_NAMESPACE

class FileReader;

class Image : public Object
{

	DEFINE_CLASS(Image)

public:
	static Image*				Create(const char* file);

	Image();
	Image(uint32 w, uint32 h, TexFormat fmt, uint8* data);
	~Image();

	bool						Load(const char* file);

	void						GetSubData(vector<uint8>& data, Rect rect);
	uint8*						GetData()									{ return m_Data; };
	uint8						GetData(uint32 i);
	uint32						GetWidth()									{ return m_Width; };
	uint32						GetHeight()									{ return m_Height; };
	uint32						GetDepth()									{ return m_Depth; };
	uint32						GetChannel()								{ return m_Channel; };
	uint32						GetMemorySize()								{ return m_Width * m_Height * m_Channel; };
	TexFormat					GetFormat()									{ return m_Format; };
	Color32						GetPixelColor(float u, float v);

	bool						SavePNG(const char* szFilePath);
	bool						SaveTGA(const char* file);
	bool						SaveJPG(const char* szFilePath);
	bool						SaveETC(const char* file);
	void						SetDataNullAndNoRelease()					{ m_Data = NULL; };

protected:
	bool						LoadPNG(FileReader* reader);
	bool						LoadTGA(FileReader* reader);
	bool						LoadJPG(FileReader* reader);
	bool						LoadJPGx2(FileReader* reader);
	bool						LoadDDS(FileReader* reader);
	bool						LoadETC(FileReader* reader);
	bool						LoadETC_Zlib(FileReader* reader);
	bool						LoadPVR(FileReader* reader);
	bool						LoadPVR_Zlib(FileReader* reader);
	bool						UnpackPVRv2Data(FileReader* reader);
	bool						UnpackPVRv3Data(FileReader* reader);

private:
	uint32						m_Width;
	uint32						m_Height;
	uint32						m_Depth;
	uint32						m_Channel;
	TexFormat					m_Format;
	uint8*						m_Data;
};

QIMP_END_NAMESPACE

#endif