//@	{
//@	 "targets":[{"name":"filter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filter.o","rel":"implementation"}]
//@	}

#ifndef TIGER_FILTER_HPP
#define TIGER_FILTER_HPP

#include "plugin.hpp"
#include "parameter.hpp"
#include "host.hpp"
#include <map>
#include <vector>

namespace Tiger
	{
	class Filter:public Plugin
		{
		public:
			explicit Filter(const char* src,const char* objdir);
			void paramSet(const Parameter& param);
			void paramsList(FILE* output) const;
			void channelsList(FILE* output) const;
			unsigned int channelIndex(const std::string& name) const;
			unsigned int channelCount() const noexcept;
			const float* parameters() const noexcept
				{return m_params.data();}
			void run(const ProcessData& procdata) const
				{m_process(procdata);}

		private:
			decltype(&process) m_process;
			std::vector<float> m_params;
			std::map<std::string,unsigned int> m_param_index;
			std::map<std::string,unsigned int> m_channel_index;
		};
	}

#endif // TIGER_FILTER_HPP
