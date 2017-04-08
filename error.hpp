//@	{"targets":[{"name":"error.hpp","type":"include"}]}

#ifndef TIGER_ERROR_HPP
#define TIGER_ERROR_HPP

namespace Tiger
	{
	class Error
		{
		public:
			template<class T,class...U>
			explicit constexpr Error(const T& first,const U&...next) noexcept:m_buffer{}
				{write(m_buffer,m_buffer+capacity(),first,next...);}

			const char* message() const noexcept
				{return m_buffer;}

			static constexpr size_t capacity() noexcept
				{return s_capacity;}

		private:
			static constexpr void write(char* buffer,const char* buffer_end)
				{*buffer='\0';}

			template<class T,class...U>
			static constexpr void write(char* buffer,const char* buffer_end,const T& first,const U&...next)
				{
				if(buffer!=buffer_end)
					{
					buffer=concat(buffer,buffer_end,first);
					write(buffer,buffer_end,next...);
					}
				else
					{*buffer='\0';}
				}

			static constexpr char* concat(char* buffer,const char* buffer_end,const char* value)
				{
				do
					{
					auto val=*value;
					if(val=='\0')
						{return buffer;}
					*buffer=val;
					++value;
					++buffer;
					}
				while(buffer!=buffer_end);
				return buffer;
				}

			static constexpr char* concat(char* buffer,const char* buffer_end,char value)
				{
				*buffer=value;
				return buffer+1;
				}

			static constexpr size_t s_capacity=1023;
			char m_buffer[s_capacity + 1];
		};
	}

#endif // TIGER_ERROR_HPP
