#include <tchar.h>
#include <string>
#include "zlib.h"

//����gzipѹ��
//args
//inBuffer:Ҫѹ��������	inSize:Ҫѹ�����ݴ�С	outBuffer:ѹ���������	outSize:ѹ�������ݻ�������С	zipSize:ѹ��������ʵ�ʴ�С
bool gzip(const char* inBuffer, int inSize, char* outBuffer, int outSize, int *zipSize)
{
	//��ʼ׼������
	z_stream d_stream;
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	if (deflateInit2(&d_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
		return false;

	d_stream.next_in = (Byte*)inBuffer;
	d_stream.avail_in = inSize;
	d_stream.next_out = (Byte*)outBuffer;

	while (d_stream.avail_in != 0 && d_stream.total_out < outSize)
	{
		if(deflate(&d_stream, Z_NO_FLUSH) != Z_OK)
			return false;
	}

	if (d_stream.avail_in != 0)
		return false;

	if (deflate(&d_stream, Z_FINISH) != Z_STREAM_END)
		return false;

	if (deflateEnd(&d_stream) != Z_OK)
		return false;

	*zipSize = d_stream.total_out;	//����ѹ�����С

	return true;
}

//��GZIPѹ��
//args
//inBuffer:Ҫ��ѹ��������	inSize:Ҫ��ѹ�����ݴ�С	outBuffer:��ѹ���������	outSize:��ѹ�������ݻ�������С	zipSize:��ѹ��������ʵ�ʴ�С
bool unGzip(const char* inBuffer, int inSize, char* outBuffer, int outSize, int *unzipSize)
{
	z_stream c_stream;
	c_stream.zalloc = (alloc_func)0;
	c_stream.zfree = (free_func)0;
	c_stream.opaque = (voidpf)0;
	c_stream.next_in = (Byte*)inBuffer;
	c_stream.avail_in = 0;
	c_stream.next_out = (Byte*)outBuffer;

	if (inflateInit2(&c_stream, MAX_WBITS + 16) != Z_OK)
		return false;

	while (c_stream.total_out < outSize && c_stream.total_in < inSize)
	{
		c_stream.avail_in = c_stream.avail_out = 1;
		int err = inflate(&c_stream,Z_NO_FLUSH);
		if(err == Z_STREAM_END) break;
		if (err != Z_OK)
			return false;
	}

	if (inflateEnd(&c_stream) != Z_OK)
		return false;

	*unzipSize = c_stream.total_out;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string str = "asjkhjwlkuboskljiodjbklcjblkxcbjosifudopgusdopgudopfgdfuifgopdfg";
	int size = str.size() + 1;

	int outSize = size * 2;
	char *outBuffer = new char[outSize];
	memset(outBuffer, 0, outSize);

	int zipSize = outSize;
	bool err = gzip(str.c_str(), size, outBuffer, outSize, &zipSize);

	char *zipBuffer = new char[zipSize];
	memset(zipBuffer, 0, zipSize);
	memcpy(zipBuffer, outBuffer, zipSize);

	delete[] outBuffer;
	
	outSize = zipSize * 2;
	outBuffer = new char[outSize];
	memset(outBuffer, 0, outSize);

	int unzipSize = outSize;
	err = unGzip(zipBuffer, zipSize, outBuffer, outSize, &unzipSize);

	char *unzipBuffer = new char[unzipSize];
	memset(unzipBuffer, 0, unzipSize);
	memcpy(unzipBuffer, outBuffer, unzipSize);

	delete[] outBuffer;
	delete[] zipBuffer;
	delete[] unzipBuffer;

	return 0;
}