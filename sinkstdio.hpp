//@	{
//@	 "targets":[{"name":"sinkstdio.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sinkstdio.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SINKSTDIO_HPP
#define TIGER_SINKSTDIO_HPP

#include "datasink.hpp"
#include "error.hpp"
#include <cstdio>
#include <string>

namespace Tiger
	{
	class SinkStdio final: public DataSink
		{
		public:
			SinkStdio(const SinkStdio& rhs)=delete;
			SinkStdio& operator=(const SinkStdio& rhs)=delete;
			explicit SinkStdio(const char* filename)
				{
				m_sink=fopen(filename,"wb");
				if(m_sink==NULL)
					{throw Error("It was not possible to open ",filename,".");}
				m_name=filename;
				}

			~SinkStdio()
				{
				if(m_sink!=NULL)
					{
					fflush(m_sink);
					fclose(m_sink);
					}
				}

			const char* name() const noexcept
				{return m_name.c_str();}

			size_t write(const void* buffer, size_t length)
				{return fwrite(buffer,1,length,m_sink);}

			void flush() 
				{fflush(m_sink);}

			FILE* handle() noexcept
				{return m_sink;}

		private:
			FILE* m_sink;
			std::string m_name;
		};

	void printRange(const char* begin,const char* end,FILE* sink);

	inline void printRange(const char* begin,const char* end,SinkStdio&& sink)
		{printRange(begin,end,sink.handle());}

	}

#endif // TIGER_SINKSTDIO_HPP
