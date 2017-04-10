/*
Tiger
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//@	{
//@	"targets":
//@		[{
//@		 "name":"image.o","type":"object"
//@		,"pkgconfig_libs":["libpng"]
//@		}]
//@	}

#include "image.hpp"
#include "error.hpp"
#include "cpuinfo.hpp"
#include "datasource.hpp"
#include "datasink.hpp"

#include <png.h>
#include <cmath>

using namespace Tiger;

namespace
	{
	class PNGReader
		{
		public:
			explicit inline PNGReader(DataSource& src);
			inline ~PNGReader();

			uint32_t width() const noexcept
				{return m_width;}

			uint32_t height() const noexcept
				{return m_height;}

			uint32_t channelCount() const noexcept
				{return m_n_channels;}

			enum class ColorType:unsigned int
				{
				 UNKNOWN
				,INFORMATION_MISSING
				,SRGB
				,GAMMACORRECTED
				};

			inline ColorType colorType() const noexcept
				{return m_color_type;}

			inline double gamma() const noexcept
				{return m_gamma;}

			uint32_t sampleSize() const noexcept
				{return m_sample_size;}

			inline void headerRead();

			inline void pixelsRead(float* pixels_out);


		private:
			[[noreturn]] static void on_error(png_struct* pngptr,const char* message);

			static void on_warning(png_struct* pngptr,const char* message);

			static void on_read(png_struct* pngptr,uint8_t* data,png_size_t N);

			void channelBitsConversionSetup();

			png_struct* m_handle;
			png_info* m_info;
			png_info* m_info_end;

			uint32_t m_width;
			uint32_t m_height;
			uint32_t m_n_channels;
			uint32_t m_sample_size;
			double m_gamma;

			ColorType m_color_type;
			};
	};

void PNGReader::on_warning(png_struct* pngptr,const char* message)
	{
//	Ignore warnings
	}

void PNGReader::on_error(png_struct* pngptr,const char* message)
	{
	auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(pngptr));
	throw Error(source->name(),": An error occured while decoding the image. "
		,message);
	}

void PNGReader::on_read(png_struct* pngptr,uint8_t* data,png_size_t N)
	{
	auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(pngptr));
	if(source->read(data,N)!=N)
		{
		throw Error(source->name(),": An error occured while decoding the image. ");
		}
	}

void PNGReader::channelBitsConversionSetup()
	{
	auto n_bits=png_get_bit_depth(m_handle,m_info);

	if(n_bits < 8)
		{
		m_sample_size=1;
		png_set_packing(m_handle);
		}

	if(n_bits>=8)
		{m_sample_size=n_bits/8;}

	if(n_bits > 8)
		{
		if(!CPUInfo::bigEndianIs())
			{png_set_swap(m_handle);}
		}

	switch(png_get_color_type(m_handle,m_info))
		{
		case PNG_COLOR_TYPE_GRAY:
			m_n_channels=1;
			if(n_bits<8)
				{png_set_expand_gray_1_2_4_to_8(m_handle);}
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			m_n_channels=2;
			break;

		case PNG_COLOR_TYPE_RGB:
			m_n_channels=3;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			m_n_channels=4;
			break;

		case PNG_COLOR_TYPE_PALETTE:
			if(png_get_valid(m_handle,m_info,PNG_INFO_tRNS))
				{
				png_set_tRNS_to_alpha(m_handle);
				m_n_channels=4;
				}
			else
				{m_n_channels=3;}
			png_set_palette_to_rgb(m_handle);
			break;

		default:
			{
			auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(m_handle));
			throw Error(source->name(),": Unknown number of channels in image.");
			}
		}
	}


PNGReader::PNGReader(DataSource& source)
	{
	m_info=nullptr;
	m_info_end=nullptr;
	m_handle=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,on_error,on_warning);
	png_set_read_fn(m_handle,&source,on_read);
	m_info=png_create_info_struct(m_handle);
	m_color_type=ColorType::INFORMATION_MISSING;
	m_gamma=1.0;
	}

PNGReader::~PNGReader()
	{
	png_destroy_read_struct(&m_handle,&m_info,&m_info_end);
	}

void PNGReader::headerRead()
	{
	png_set_sig_bytes(m_handle,8);
	png_read_info(m_handle,m_info);
	m_width=static_cast<uint32_t>( png_get_image_width(m_handle,m_info) );
	m_height=static_cast<uint32_t>( png_get_image_height(m_handle,m_info) );
	channelBitsConversionSetup();

	if(png_get_valid(m_handle,m_info,PNG_INFO_sRGB))
		{m_color_type=ColorType::SRGB;}
	else
	if(png_get_valid(m_handle,m_info,PNG_INFO_gAMA))
		{
		png_get_gAMA(m_handle,m_info,&m_gamma);
		m_color_type=ColorType::GAMMACORRECTED;
		}
	}

template<class T>
static void pixelsScale(const T* pixels_in,Image::SampleType* pixels_out,uint32_t N)
	{
	auto factor=static_cast<float>( ( 1L<<(8L*sizeof(T)) ) - 1 );

	while(N)
		{
		*pixels_out=static_cast<float>(*pixels_in)/factor;
		--N;
		++pixels_in;
		++pixels_out;
		}
	}

void PNGReader::pixelsRead(Image::SampleType* pixels_out)
	{
	auto width=m_width;
	auto height=m_height;
	auto sample_size=m_sample_size;
	auto n_channels=m_n_channels;
	std::unique_ptr<uint8_t[]> buffer_temp(new uint8_t[width*height*sample_size*n_channels]);

		{
		auto row=buffer_temp.get();
		std::unique_ptr<uint8_t*[]> rows(new uint8_t*[height]);
		for(uint32_t n=0;n<height;++n)
			{rows.get()[n]=row + n*width*sample_size*n_channels;}

		png_read_image(m_handle,rows.get());
		}

	switch(sample_size)
		{
		case 1:
			pixelsScale(buffer_temp.get(),pixels_out,width*height*n_channels);
			break;

		case 2:
			pixelsScale(reinterpret_cast<const uint16_t*>(buffer_temp.get())
				,pixels_out,width*height*n_channels);
			break;
		case 4:
			pixelsScale(reinterpret_cast<const uint32_t*>(buffer_temp.get())
				,pixels_out,width*height*n_channels);
			break;
		default:
			{
			auto source=reinterpret_cast<DataSource*>(png_get_io_ptr(m_handle));
			throw Error(source->name(),": Unsupported sample size.");
			}
		}
	}

static float fromSRGB(float x)
	{
	return x<=0.04045f? x/12.92f : std::pow( (x + 0.055f)/(1.0f + 0.055f),2.4f);
	}

static float fromGamma(float x,float gamma)
	{
	return std::pow(x,gamma);
	}

static void fromGamma(Image& image,float gamma)
	{
	auto ptr=image.pixels();
	auto n_ch=image.channelCount();
	auto N=image.width() * image.height() * n_ch;

	while(N!=0)
		{
		*ptr=fromGamma(*ptr,gamma);
		++ptr;
		--N;
		}
	}

static void fromSRGB(Image& image)
	{
	auto ptr=image.pixels();
	auto n_ch=image.channelCount();
	auto N=image.width() * image.height() * n_ch;

	while(N!=0)
		{
		*ptr=fromSRGB(*ptr);
		++ptr;
		--N;
		}
	}

static void convert(Image& img,const PNGReader& reader)
	{
	switch(reader.colorType())
		{
		case PNGReader::ColorType::UNKNOWN:
			return;

		case PNGReader::ColorType::INFORMATION_MISSING:
			fromSRGB(img);
			return;

		case PNGReader::ColorType::GAMMACORRECTED:
			fromGamma(img,static_cast<float>(1.0/reader.gamma()));
			return;

		case PNGReader::ColorType::SRGB:
			fromSRGB(img);
			return;

		default:
			return;
		}
	}



Image::Image(DataSource& source)
	{
		{
		uint8_t magic[8]="xxxxxxx";
		if(source.read(magic,8)!=8)
			{
			throw Error(source.name(),": An I/O error occured while reading the image "
				"magic number.");
			}

		if(png_sig_cmp(magic,0,8))
			{
			throw Error(source.name(),": The image file has an unknown encoding.");
			}
		}

	PNGReader reader(source);
	reader.headerRead();
	m_width=reader.width();
	m_height=reader.height();
	m_channel_count=reader.channelCount();
	

	m_data.reset(new SampleType[reader.width()*reader.height()
		*reader.channelCount()]);

	reader.pixelsRead(m_data.get());
	convert(*this,reader);
	}

Image::Image(uint32_t width,uint32_t height,uint32_t n_channels):
	m_data(new SampleType[width*height*n_channels])
	,m_width(width),m_height(height),m_channel_count(n_channels)
	{
	}


namespace
	{
	class PNGWriter
		{
		public:
			explicit inline PNGWriter(DataSink& dest);
			inline ~PNGWriter();

			PNGWriter& width(uint32_t w) noexcept
				{
				m_width=w;
				return *this;
				}

			PNGWriter& height(uint32_t h) noexcept
				{
				m_height=h;
				return *this;
				}

			PNGWriter& channelCount(uint32_t ch) noexcept
				{
				m_n_channels=ch;
				return *this;
				}

			inline void headerWrite();

			inline void pixelsWrite(const float* pixels_out);


		private:
			[[noreturn]] static void on_error(png_struct* pngptr,const char* message);

			static void on_warning(png_struct* pngptr,const char* message);

			static void on_write(png_struct* pngptr,uint8_t* data,png_size_t N);

			static void on_flush(png_struct* pngptr);

			static int colorType(uint32_t channel_count);

			png_struct* m_handle;
			png_info* m_info;
			png_info* m_info_end;

			uint32_t m_width;
			uint32_t m_height;
			uint32_t m_n_channels;
			};
	};


void PNGWriter::on_warning(png_struct* pngptr,const char* message)
	{
//	Ignore warnings
	}

void PNGWriter::on_error(png_struct* pngptr,const char* message)
	{
	auto sink=reinterpret_cast<DataSink*>(png_get_io_ptr(pngptr));
	throw Error(sink->name(),": An error occured while decoding the image. "
		,message);
	}

void PNGWriter::on_write(png_struct* pngptr,uint8_t* data,png_size_t N)
	{
	auto sink=reinterpret_cast<DataSink*>(png_get_io_ptr(pngptr));
	if(sink->write(data,N)!=N)
		{
		throw Error(sink->name(),": An error occured while writing the image. ");
		}
	}

void PNGWriter::on_flush(png_struct* pngptr)
	{
	auto sink=reinterpret_cast<DataSink*>(png_get_io_ptr(pngptr));
	sink->flush();
	}

int PNGWriter::colorType(uint32_t channel_count)
	{
	switch(channel_count)
		{
		case 1:
			return PNG_COLOR_TYPE_GRAY;
		case 2:
			return PNG_COLOR_TYPE_GRAY_ALPHA;
		case 3:
			return PNG_COLOR_TYPE_RGB;
		case 4:
			return PNG_COLOR_TYPE_RGBA;
		default:
			throw Error("The current image has an invalid channel count");
		}
	}

PNGWriter::PNGWriter(DataSink& dest)
	{
	m_handle=png_create_write_struct(PNG_LIBPNG_VER_STRING
		,&dest,on_error,on_warning);
	png_set_write_fn(m_handle,&dest,on_write,on_flush);
	m_info=png_create_info_struct(m_handle);
	png_set_compression_level(m_handle,Z_BEST_COMPRESSION);
	png_set_compression_strategy(m_handle,Z_DEFAULT_STRATEGY);
	}

PNGWriter::~PNGWriter()
	{
	png_write_end(m_handle,m_info);
	png_destroy_write_struct(&m_handle, &m_info);
	}

void PNGWriter::headerWrite()
	{
	png_set_IHDR(m_handle,m_info,m_width,m_height,16,colorType(m_n_channels)
		,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(m_handle,Z_BEST_COMPRESSION);
	png_set_compression_strategy(m_handle,Z_DEFAULT_STRATEGY);
	png_set_gAMA(m_handle,m_info,1.0);

	png_write_info(m_handle,m_info);
	}

void PNGWriter::pixelsWrite(const Image::SampleType* pixels_in)
	{
	std::unique_ptr<uint16_t[]> buffer_temp(new uint16_t[m_width*m_height*m_n_channels]);

	if(!CPUInfo::bigEndianIs())
		{png_set_swap(m_handle);}

		{
		auto ptr=pixels_in;
		auto ptr_out=buffer_temp.get();
		auto N=m_width*m_height*m_n_channels;
		while(N!=0)
			{
			*ptr_out=static_cast<uint16_t>( 0xffff* (std::min( std::max(*ptr,0.0f),1.0f)));
			++ptr;
			++ptr_out;
			--N;
			}
		}

		{
		auto row=buffer_temp.get();
		std::unique_ptr<uint16_t*[]> rows(new uint16_t*[m_height]);
		for(uint32_t n=0;n<m_height;++n)
			{rows.get()[n]=row + n*m_width*m_n_channels;}

		png_write_image(m_handle,reinterpret_cast<uint8_t**>(rows.get()));
		}
	}

void Image::store(DataSink& sink) const
	{
	PNGWriter writer(sink);
	writer.channelCount(m_channel_count)
		.height(m_height)
		.width(m_width)
		.headerWrite();
	writer.pixelsWrite(m_data.get());
	}
