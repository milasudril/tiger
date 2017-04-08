//@	{"targets":[{"name":"channel.hpp","type":"include"}]}

#ifndef TIGER_CHANNEL_HPP
#define TIGER_CHANNEL_HPP

#include <string>

namespace Tiger
	{
	class Channel
		{
		public:
			Channel(){}

			explicit Channel(const char* keyval_string)
				{load(keyval_string);}

			void load(const char* keyval_string);

			const char* name() const noexcept
				{return m_name.c_str();}

			const char* filename() const noexcept
				{return m_filename.c_str();}


		private:
			std::string m_name;
			std::string m_filename;
		};

	}

#endif // TIGER_CHANNEL_HPP
