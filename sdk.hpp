//@	{"targets":[{"name":"sdk.hpp","type":"include"}]}

#ifndef TIGER_SDK_HPP
#define TIGER_SDK_HPP

#include <utility>

namespace Tiger
	{
	struct ProcessData
		{
		public:
			ProcessData(float* buffer_a,float* buffer_b,const float* src
				,const float* params,int w,int h) noexcept:
				 m_next(buffer_a),m_current(buffer_b),m_src(src),m_params(params)
				,m_width(w),m_height(h)
				{}

			template<int n>
			float& value_next(int x,int y) const noexcept
				{
				static_assert(n>=0,"Channel not found");
				return m_next[y*m_width + x + n];
				}

			template<int n>
			float value_current(int x,int y) const noexcept
				{
				static_assert(n>=0,"Channel not found");
				return m_current[y*m_height + x + n];
				}

			template<int n>
			float source(int x,int y) const noexcept
				{
				static_assert(n>=0,"Channel not found");
				return m_src[y*m_height + x + n];
				}

			const float* buffer_current() const noexcept
				{return m_current;}

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
				{std::swap(m_next,m_current);}
				
		private:
			float* m_next;
			float* m_current;
			const float* m_src;
			const float* m_params;
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
