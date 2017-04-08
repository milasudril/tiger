//@	{
//@	 "targets":[{"name":"parameter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"parameter.o","rel":"implementation"}]
//@	}

#ifndef TIGER_PARAMETER_HPP
#define TIGER_PARAMETER_HPP

#include <string>

namespace Tiger
	{
	class Parameter
		{
		public:
			Parameter():m_name(""),m_value(0){}

			explicit Parameter(const char* keyval_string)
				{load(keyval_string);}

			void load(const char* keyval_string);

			const char* name() const noexcept
				{return m_name.c_str();}

			float value() const noexcept	
				{return m_value;}

		private:
			std::string m_name;
			float m_value;
		};
	}
	
#endif // TIGER_PARAMETER_HPP
