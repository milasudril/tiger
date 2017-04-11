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

#include "simulation.hpp"
#include "channel.hpp"
#include "blob.hpp"
#include "sinkstdio.hpp"
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
	,{"","channels-list","Lists all channel names used by *filter*","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
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

TIGER_BLOB(char,example,"example.cpp");

int main(int argc,char** argv)
	{
	try
		{
		Alice::CommandLine<OptionDescriptor> cmdline{argc,argv};

		if(cmdline.get<Alice::Stringkey("help")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("help")>().valueGet();
			Tiger::SinkStdio s(v.size()!=0?v[0].c_str():nullptr);
			fprintf(s.handle(),"%s\n",PROGRAM_DESCRIPTION);
			cmdline.help(0,s.handle());
			return 0;
			}

		if(cmdline.get<Alice::Stringkey("example")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("example")>().valueGet();
			Tiger::SinkStdio s(v.size()!=0?v[0].c_str():nullptr);
			printRange(example_begin,example_end,s);
			return 0;
			}

		Tiger::Simulation sim(cmdline.get<Alice::Stringkey("filter")>().valueGet().c_str()
			,cmdline.get<Alice::Stringkey("objdir")>()?
				cmdline.get<Alice::Stringkey("objdir")>().valueGet().c_str():".");
		sim.paramsLoad(cmdline.get<Alice::Stringkey("params")>().valueGet());
		if(cmdline.get<Alice::Stringkey("params-list")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("params-list")>().valueGet();
			Tiger::SinkStdio dest(v.size()!=0?v[0].c_str():nullptr);
			sim.paramsList([&dest](const Tiger::Simulation& sim,const char* name,float value)
				{fprintf(dest.handle()," * `%s`=%.9g\n",name,value);});
			}

		if(cmdline.get<Alice::Stringkey("channels-list")>())
			{
			auto& v=cmdline.get<Alice::Stringkey("channels-list")>().valueGet();
			Tiger::SinkStdio dest(v.size()!=0?v[0].c_str():nullptr);
			sim.channelsList([&dest](const Tiger::Simulation& sim,const char* name)
				{fprintf(dest.handle()," * %s\n",name);});
			}

		if(!(cmdline.get<Alice::Stringkey("source")>()
			|| cmdline.get<Alice::Stringkey("init")>()
			|| cmdline.get<Alice::Stringkey("dest")>() ) )
			{return 0;}

		if(!cmdline.get<Alice::Stringkey("init")>())
			{throw Tiger::Error("Initial condition is missing");}

		if(!cmdline.get<Alice::Stringkey("dest")>())
			{throw Tiger::Error("List of destination files is missing");}

		auto iter_count=cmdline.get<Alice::Stringkey("iterations")>().valueGet();
		sim.imagesLoad(cmdline.get<Alice::Stringkey("source")>().valueGet()
				,cmdline.get<Alice::Stringkey("init")>().valueGet())
			.run([iter_count](const Tiger::Simulation& sim,unsigned long long n)
				{return n!=iter_count;})
			.imagesStore(cmdline.get<Alice::Stringkey("dest")>().valueGet());
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
