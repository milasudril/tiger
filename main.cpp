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
#include <cstdio>
#include <alice/alice.hpp>

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
		typedef Tiger::Parameter Type;
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

ALICE_OPTION_DESCRIPTOR(OptionDescriptor
	,{"","help","Print usage information","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","filter","Name of filter","string",Alice::Option::Multiplicity::ONE}
	,{"","params","Filter parameters","Parameter",Alice::Option::Multiplicity::ZERO_OR_MORE}
	,{"","params-list","Lists all parameters availible for *filter*","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","channels-list","Lists all channel names used by *filter","string",Alice::Option::Multiplicity::ZERO_OR_ONE}
	,{"","source","Selects static source image","Channel mapping",Alice::Option::Multiplicity::ZERO_OR_MORE}
	,{"","init","Selects initial image","Channel mapping",Alice::Option::Multiplicity::ZERO_OR_MORE}
	,{"","output","Selects output image","Channel mapping",Alice::Option::Multiplicity::ZERO_OR_MORE}
	,{"","iterations","Selects the number of iterations to apply the filter","unsigned int",Alice::Option::Multiplicity::ONE}
	);

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

static void simulation_run(unsigned int N,Tiger::Filter& filter)
	{
	while(N!=0)
		{
		--N;
		}
	}

int main(int argc,char** argv)
	{
	try
		{
		Alice::CommandLine<OptionDescriptor> cmdline{argc,argv};

		if(cmdline.get<Alice::Stringkey("help")>())
			{
			cmdline.help(0);
			return 0;
			}

		Tiger::Filter filter(cmdline.get<Alice::Stringkey("filter")>().valueGet().c_str());
		params_set(cmdline.get<Alice::Stringkey("params")>().valueGet(),filter);
		if(cmdline.get<Alice::Stringkey("params-list")>())
			{
		//TODO: If set, use argument value as filename
			filter.paramsList(stdout);
			}

		if(cmdline.get<Alice::Stringkey("channels-list")>())
			{
		//TODO: If set, use argument value as filename
			filter.channelsList(stdout);
			}

		simulation_run(cmdline.get<Alice::Stringkey("iterations")>().valueGet(),filter);
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
