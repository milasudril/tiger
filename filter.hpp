//@	{
//@	 "targets":[{"name":"filter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filter.o","rel":"implementation"}]
//@	}

#ifndef TIGER_FILTER_HPP
#define TIGER_FILTER_HPP

#include "plugin.hpp"
#include "parameter.hpp"
#include "sdk.hpp"
#include <map>
#include <vector>

namespace Tiger
	{
	class Filter:public Plugin
		{
		public:
			explicit Filter(const char* src);
			void paramSet(const Parameter& param);

		private:
			std::vector<float> m_params;
			std::map<std::string,unsigned int> m_param_index;
			std::map<std::string,unsigned int> m_channel_index;
		};
	}

#endif // TIGER_FILTER_HPP
