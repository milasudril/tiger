//@	{"targets":[{"name":"pluginmain.hpp","type":"object"}]}

namespace Tiger
	{class FilterState;}

const char* const* parameters() noexcept;
const char* const* channels() noexcept;

void process(const Tiger::FilterState& data,unsigned long long frame_count);
