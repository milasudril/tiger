//@	{
//@	"targets":
//@		[{
//@		 "name":"filter.o","type":"object"
//@		,"pkgconfig_libs":["uuid"]
//@		,"dependencies":[{"ref":"magic","rel":"external"}]
//@		}]
//@	}

#include "filter.hpp"
#include "error.hpp"
#include <magic.h>
#include <uuid.h>
#include <cstring>
#include <array>

using namespace Tiger;

namespace
	{
	class MagicHandle
		{
		public:
			MagicHandle(const MagicHandle&)=delete;
			MagicHandle& operator=(const MagicHandle&)=delete;

			MagicHandle():m_handle(magic_open(MAGIC_ERROR|MAGIC_MIME))
				{magic_load(m_handle,nullptr);}
			~MagicHandle()
				{magic_close(m_handle);}

			const char* identify(const char* filename) noexcept
				{return magic_file(m_handle,filename);}

		private:
			magic_t m_handle;
		};
	}

inline bool begins_with(const char* str,const char* str2)
	{return strncmp(str,str2,strlen(str2))==0;}

static std::array<char,37> uuid_generate() noexcept
	{
	std::array<char,37> ret;
	uuid_t id;
	::uuid_generate(id);
	uuid_unparse_upper(id,ret.begin());
	return ret;
	}

static void compile(const char* src,const char* dest)
	{
//TODO: Use libmaike (somewhat overkill here) or fork/exec pair
	std::string cmdbuff("g++ -std=c++14 -O3 --fast-math -march=native -fno-stack-protector "
		"-fpic -shared -o '");
	cmdbuff+=dest;
	cmdbuff+="' '";
	cmdbuff+=src;
	cmdbuff+="'";

	if(system(cmdbuff.c_str())!=0)
		{throw Error("It was not possible to compile filter ",src);}
	}

static std::string objectGenerate(const char* src)
	{
	MagicHandle m;
	auto mime=m.identify(src);
	if(mime==nullptr)
		{throw Error(src," has unknown MIME type");}
	if(begins_with(mime,"text/x-c"))
		{
		std::string ret("/tmp/tiger_");
		ret+=uuid_generate().begin();
		ret+='_';
		ret+=src;
		ret+=".cpp.so";
		compile(src,ret.c_str());
		return ret;
		}
	if(begins_with(mime,"application/x-sharedlib"))
		{return std::string(src);}
	throw Error("Tiger cannot use a filter of type ",mime);
	}

static void lookup_fill(const char* const* names
	,std::map<std::string,unsigned int>& index_map)
	{
	if(names==nullptr)
		{return;}
	auto N=0;
	while((*names)!=nullptr)
		{
		auto ip=index_map.insert({*names,N});
		if(!ip.second)
			{throw Error(*names," is already used within this context");}
		++names;
		++N;
		}
	}

Filter::Filter(const char* src):Plugin(objectGenerate(src))
	{
		{
		auto fn=entryPoint<decltype(&::parameters)>("_Z10parametersv");
		lookup_fill(fn(),m_param_index);
		m_params.resize(m_param_index.size());
		}

		{
		auto fn=entryPoint<decltype(&channels)>("_Z8channelsv");
		lookup_fill(fn(),m_channel_index);
		}

	m_process=entryPoint<decltype(m_process)>("_Z7processRKN5Tiger11ProcessDataE");
	}

void Filter::paramSet(const Parameter& param)
	{
	auto i=m_param_index.find(param.name());
	if(i==m_param_index.end())
		{throw Error(name()," does not take a parameter with name ",param.name());}
	m_params[i->second]=param.value();
	}

void Filter::paramsList(FILE* output) const
	{
	auto ptr=m_param_index.begin();
	auto ptr_end=m_param_index.end();
	fprintf(output,"\nParameters\n----------\n");
	while(ptr!=ptr_end)
		{
		fprintf(output," * `%s`=%.9g\n",ptr->first.c_str(),m_params[ptr->second]);
		++ptr;
		}
	}

void Filter::channelsList(FILE* output) const
	{
	auto ptr=m_channel_index.begin();
	auto ptr_end=m_channel_index.end();
	fprintf(output,"\nChannels\n--------\n");
	while(ptr!=ptr_end)
		{
		fprintf(output," * `%s`\n",ptr->first.c_str());
		++ptr;
		}
	}

unsigned int Filter::channelIndex(const std::string& ch) const
	{
	auto i=m_channel_index.find(ch);
	if(i==m_channel_index.end())
		{throw Error(name()," does use a channel with name ",ch.c_str());}
	return i->second;
	}

unsigned int Filter::channelCount() const noexcept
	{return m_channel_index.size();}
