#ifndef TIGER_SRCSTDIO_HPP
#define TIGER_SRCSTDIO_HPP

#include "datasource.hpp"
#include <cstdio>

namespace Tiger
	{
	class SrcStdio final: public DataSource
		{
		public:
			SrcStdio(const SrcStdio& rhs)=delete;
			SrcStdio& operator=(const SrcStdio& rhs)=delete;
			explicit SrcStdio(const char* filename)
				{
				m_src=fopen(filename,"rb");
				if(m_src==NULL)
					{throw Error("It was not possible to open ",filename,".");}
				m_name=filename;
				}

			~SrcStdio()
				{
				if(m_src!=NULL)
					{fclose(m_src);}
				}

			const char* name() const noexcept
				{return m_name.c_str();}

			size_t read(void* buffer, size_t length)
				{return fread(buffer,1,length,m_src);}

		private:
			FILE* m_src;
			std::string m_name;
		};
	}

#endif // TIGER_SRCSTDIO_HPP
