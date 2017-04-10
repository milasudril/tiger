/*
Tiger
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//@	{
//@	"targets":
//@		[{
//@		 "name":"tiger","type":"application"
//@		,"dependencies":[{"ref":"alice","rel":"external"}]
//@		}]
//@	}

#include "filter.hpp"
#include "parameter.hpp"
#include "channel.hpp"
#include "error.hpp"
#include "image.hpp"
#include "srcstdio.hpp"
#include "sinkstdio.hpp"
#include "host.hpp"
#include "blob.hpp"
#include <alice/alice.hpp>
#include <cassert>
#include <cstdio>

static constexpr const char* PROGRAM_DESCRIPTION
	="TIGER\n"
"=====\n"
"Tiger (Texture/Image GenEatoR) loads a set of images and applies a filter multiple times. "
"The result of the filter, can be descriped by the equation\n"
"\n$$\n"
"    \\vec{y_{k+1}}(x,y) = \\vec{f}(x,y,\\vec{y_k}(x,y),\\vec{g}(x,y),\\vec{p})\n"
"$$\n"
"\n\n";

ALICE_OPTION_DESCRIPTOR(OptionDescriptor
	,{"","help","Prints usage information to either `stdout`, or to the chosen file."
		,"string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","example","Generates source code for an example filter. "
		"The output is printed to either `stdout`, or to the chosen file.","string"
		,Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","filter","Specifies the filter file. The filter is either a shared object, "
		"or C++ source file. A template filter can be extracted with the `--example` "
		"option. When a C++ source file is specified, it will be compiled into a "
		"shared object stored. The location of that object is controlled by the "
		"option `--objdir`.","string",Alice::Option::Multiplicity::ONE}
	,{"","objdir","Specifies where to store any copiled binary, relative to the working directory.","string",Alice::Option::Multiplicity::ONE}
	,{"","params","Sets filter parameters. This is the content of $\\vec{p}$ in the equation above.","Parameter",Alice::Option::Multiplicity::ZERO_OR_MORE}
	,{"","params-list","Lists all parameters availible for *filter*","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","channels-list","Lists all channel names used by *filter","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","source","Selects static source images. These images determines $\\vec{g}$ in the equation above.","Channel mapping",Alice::Option::Multiplicity::ONE_OR_MORE}
	,{"","init","Selects initial images. These images determines $\\vec{y_{0}}$ in the equation above.","Channel mapping",Alice::Option::Multiplicity::ONE_OR_MORE}
	,{"","dest","Selects output images","Channel mapping",Alice::Option::Multiplicity::ONE_OR_MORE}
	,{"","iterations","Selects the number of iterations to apply the filter","unsigned int",Alice::Option::Multiplicity::ONE}
	);

namespace Alice
	{
	template<>
	struct MakeType<Stringkey("Parameter")>
		{
		typedef Tiger::Parameter Type;
		static constexpr const char* descriptionShortGet() noexcept
			{return "Name:Value";}
		static constexpr const char* descriptionLongGet() noexcept
			{
			return "Parameters control the algorithm used by *filter*. "
				"Use --params-list to see a list of parameters availible.";
			}
		};

	template<class ErrorHandler>
	struct MakeValue<Tiger::Parameter,ErrorHandler>
		{
		static Tiger::Parameter make_value(const std::string& str);
		};

	template<class ErrorHandler>
	Tiger::Parameter MakeValue<Tiger::Parameter,ErrorHandler>::make_value(const std::string& str)
		{return Tiger::Parameter(str.c_str());}



	template<>
	struct MakeType<Stringkey("Channel mapping")>
		{
		typedef Tiger::Channel Type;
		static constexpr const char* descriptionShortGet() noexcept
			{return "Channel:Filename";}
		static constexpr const char* descriptionLongGet() noexcept
			{
			return "A Channel mapping maps a channel to a given file. Use "
				"--channels-list to see a list of availible channels.";
			}
		};

	template<class ErrorHandler>
	struct MakeValue<Tiger::Channel,ErrorHandler>
		{
		static Tiger::Channel make_value(const std::string& str);
		};

	template<class ErrorHandler>
	Tiger::Channel MakeValue<Tiger::Channel,ErrorHandler>::make_value(const std::string& str)
		{return Tiger::Channel(str.c_str());}
	}

static void params_set(const std::vector<Tiger::Parameter>& params,Tiger::Filter& filter)
	{
	auto ptr=params.begin();
	auto ptr_end=params.end();
	while(ptr!=ptr_end)
		{
		filter.paramSet(*ptr);
		++ptr;
		}
	}

static void imageSoA2AoS(const Tiger::Image& src,unsigned int ch,Tiger::Image& dest)
	{
	assert(src.height()==dest.height() && src.width()==dest.width() 
		&& src.channelCount()==1 && ch<dest.channelCount() );

	auto w=dest.width();
	auto h=dest.height();
	auto N=w*h;
	auto ch_count=dest.channelCount();
	auto pixels_in=src.pixels();
	auto pixels_out=dest.pixels() + ch;
	while(N)
		{
		*pixels_out=*pixels_in;
		pixels_out+=ch_count;
		++pixels_in;
		--N;
		}
	}

static Tiger::Image imagesLoad(const std::vector<Tiger::Channel>& files,const Tiger::Filter& f)
	{
	auto ptr=files.begin();
	auto ptr_end=files.end();
	Tiger::Image ret;
	while(ptr!=ptr_end)
		{
		Tiger::Image src{Tiger::SrcStdio(ptr->filename())};
		if(src.channelCount()!=1)
			{throw Tiger::Error(ptr->filename(),": Only grayscale images are supported.");}
		if(!ret.valid())
			{ret=Tiger::Image(src.width(),src.height(),f.channelCount());}
		if( ret.width()!=src.width() || ret.height()!=src.height() )
			{
			throw Tiger::Error(ptr->filename()
				,": Image has a different size comared to other loaded images.");
			}

		auto ch=f.channelIndex(ptr->name());
		imageSoA2AoS(src,ch,ret);
		++ptr;
		}
	return std::move(ret);
	}

static void imageAoS2SoA(const Tiger::ProcessData& src,unsigned int ch_count
	,unsigned int ch,Tiger::Image& dest)
	{
	assert(src.height()==static_cast<int>(dest.height())
		&& src.width()==static_cast<int>(dest.width())
		&& dest.channelCount()==1 && ch<ch_count );;

	auto w=dest.width();
	auto h=dest.height();
	auto N=w*h;
	auto pixels_in=src.buffer_current() + ch;
	auto pixels_out=dest.pixels();
	while(N)
		{
		*pixels_out=*pixels_in;
		pixels_in+=ch_count;
		++pixels_out;
		--N;
		}
	}

static void imagesStore(const Tiger::Filter& f,const Tiger::ProcessData& d
	,const std::vector<Tiger::Channel>& files)
	{
	auto ptr=files.begin();
	auto ptr_end=files.end();
	Tiger::Image img_out(d.width(),d.height(),1);
	while(ptr!=ptr_end)
		{
		auto ch=f.channelIndex(ptr->name());
		imageAoS2SoA(d,f.channelCount(),ch,img_out);
		img_out.store(Tiger::SinkStdio{ptr->filename()});
		++ptr;
		}
	}


static void simulation_run(unsigned int N,const Tiger::Filter& filter
	,Tiger::ProcessData& data)
	{
	while(N!=0)
		{
		filter.run(data);
		data.swap();
		--N;
		}
	}

TIGER_BLOB(char,example,"example.cpp");

int main(int argc,char** argv)
	{
	try
		{
		Alice::CommandLine<OptionDescriptor> cmdline{argc,argv};

		if(cmdline.get<Alice::Stringkey("help")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("help")>().valueGet();
			if(v.size()!=0)
				{
				Tiger::SinkStdio s(v[0].c_str());
				fprintf(s.handle(),"%s\n",PROGRAM_DESCRIPTION);
				cmdline.help(0,s.handle());
				}
			else
				{
				fprintf(stdout,"%s\n",PROGRAM_DESCRIPTION);
				cmdline.help(0,stdout);
				}
			return 0;
			}

		if(cmdline.get<Alice::Stringkey("example")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("example")>().valueGet();
			if(v.size()!=0)
				{printRange(example_begin,example_end,Tiger::SinkStdio(v[0].c_str()));}
			else
				{Tiger::printRange(example_begin,example_end,stdout);}
			return 0;
			}

		Tiger::Filter filter(cmdline.get<Alice::Stringkey("filter")>().valueGet().c_str()
			,cmdline.get<Alice::Stringkey("objdir")>()?
				cmdline.get<Alice::Stringkey("objdir")>().valueGet().c_str():".");
		params_set(cmdline.get<Alice::Stringkey("params")>().valueGet(),filter);
		if(cmdline.get<Alice::Stringkey("params-list")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("params-list")>().valueGet();
			if(v.size()!=0)
				{filter.paramsList(Tiger::SinkStdio(v[0].c_str()).handle());}
			else
				{filter.paramsList(stdout);}
			}

		if(cmdline.get<Alice::Stringkey("channels-list")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("channels-list")>().valueGet();
			if(v.size()!=0)
				{filter.channelsList(Tiger::SinkStdio(v[0].c_str()).handle());}
			else
				{filter.channelsList(stdout);}
			}

		if(!(cmdline.get<Alice::Stringkey("source")>()
			|| cmdline.get<Alice::Stringkey("init")>()
			|| cmdline.get<Alice::Stringkey("dest")>() ) )
			{return 0;}

		if(!cmdline.get<Alice::Stringkey("init")>())
			{throw Tiger::Error("Initial condition is missing");}

		if(!cmdline.get<Alice::Stringkey("dest")>())
			{throw Tiger::Error("List of destination files is missing");}

		auto img_src=imagesLoad(cmdline.get<Alice::Stringkey("source")>().valueGet(),filter);
		auto img_init=imagesLoad(cmdline.get<Alice::Stringkey("init")>().valueGet(),filter);
		if(!img_src.valid())
			{img_src=layoutClone(img_init);}

		if(!layoutSame(img_src,img_init))
			{throw Tiger::Error("Source image and initial image must have the same size.");}

		auto img_next=layoutClone(img_init);

		Tiger::ProcessData procdata
			{
			 img_next.pixels()
			,img_init.pixels()
			,img_src.pixels()
			,filter.parameters()
			,static_cast<int>( img_init.width() )
			,static_cast<int>( img_init.height() )
			};

		simulation_run(cmdline.get<Alice::Stringkey("iterations")>().valueGet()
			,filter,procdata);

		imagesStore(filter,procdata,cmdline.get<Alice::Stringkey("dest")>().valueGet());
		}
	catch(const Alice::ErrorMessage& message)
		{
		fprintf(stderr,"%s\n",message.data);
		return -1;
		}
	catch(const Tiger::Error& e)
		{
		fprintf(stderr,"%s\n",e.message());
		return -1;
		}
	return 0;
	}
