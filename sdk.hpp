//@	{"targets":[{"name":"sdk.hpp","type":"include"}]}

#ifndef TIGER_SDK_HPP
#define TIGER_SDK_HPP

#include <utility>

namespace Tiger
	{
	struct ProcessData
		{
		public:
			ProcessData(const ProcessData&)=delete;
			ProcessData& operator=(const ProcessData&)=delete;

			ProcessData(const float* params,float* src,float* d
				,int w,int h) noexcept:m_params(params),m_src(src),m_dest(d)
				,m_width(w),m_height(h)
				{}

			template<int n>
			float& dest(int x,int y) const noexcept
				{
				static_assert(n>=0,"Channel not found");
				return m_dest[y*m_width + x + n];
				}

			template<int n>
			float source(int x,int y) const noexcept
				{
				static_assert(n>=0,"Channel not found");
				return m_dest[y*m_height + x + n];
				}

			int width() const noexcept
				{return m_width;}

			int height() const noexcept
				{return m_height;}

			template<int n>
			float param() const noexcept
				{
				static_assert(n>=0,"Parameter not found");
				return m_params[n];
				}

			void swap() noexcept
				{std::swap(m_src,m_dest);}
				
		private:
			const float* m_params;
			float* m_src;
			float* m_dest;
			int m_width;
			int m_height;
		};
	}

const char* const* parameters() noexcept;
const char* const* channels() noexcept;

template<int ch>
inline float value(const float* ptr)
	{return ptr[ch];}

template<int ch>
inline float& value(float* ptr)
	{return ptr[ch];}

void process(const Tiger::ProcessData& data);

#define PARAMETERS(...) static constexpr const char* __param_names[]\
	={ __VA_ARGS__,nullptr};\
	const char* const* parameters() noexcept {return __param_names;}\
	static constexpr int P(const char* x) {return map(__param_names,x);}

#define CHANNELS(...) static constexpr const char* __channel_names[]\
	={ __VA_ARGS__,nullptr};\
	const char* const* channels() noexcept {return __channel_names;}\
	static constexpr int C(const char* x){return map(__channel_names,x);}

static constexpr bool equals(const char* a,const char* b)
	{
	while(1)
		{
		if(*a=='\0')
			{break;}
		if(*b=='\0')
			{break;}
		if(*a!=*b)
			{return 0;}
		++a;
		++b;
		}
	return *a=='\0' && *b=='\0';
	}

static constexpr int map(const char* const* x,const char* name)
	{
	auto ptr=x;
	while((*ptr)!=nullptr)
		{
		if(equals(*ptr,name))
			{return ptr-x;}
		++ptr;
		}
	return -1;
	}
#endif
