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
//@	{"targets":[{"name":"filtercompile.o","type":"object","pkgconfig_libs":["uuid"]}]}

#include "filtercompile.hpp"
#include "blob.hpp"
#include "sinkstdio.hpp"

#include <uuid.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <array>
#include <string>
#include <stack>

using namespace Tiger;

TIGER_BLOB(char,filterstate,"engine/filterstate.hpp");
TIGER_BLOB(char,filterstateclient,"engine/filterstateclient.hpp");
TIGER_BLOB(char,pluginmain,"engine/pluginmain.hpp");

static std::array<char,37> uuid_generate() noexcept
	{
	std::array<char,37> ret;
	uuid_t id;
	::uuid_generate(id);
	uuid_unparse_upper(id,ret.begin());
	return ret;
	}

namespace
	{
	class DirectoryGuard
		{
		public:
			DirectoryGuard& operator=(const DirectoryGuard&)=delete;
			DirectoryGuard(const DirectoryGuard&)=delete;

			DirectoryGuard(const std::string& dir):m_dir(dir)
				{mkdir(m_dir.c_str(),S_IRWXU);}

			~DirectoryGuard()
				{	
				while(!m_items.empty())
					{
					auto x=m_items.top();
					m_items.pop();
					remove(x.c_str());
					}
				rmdir(m_dir.c_str());
				}

			DirectoryGuard& itemAdd(const char* name)
				{
				m_items.push(m_dir + '/' + name);
				return *this;
				}

		private:
			std::stack<std::string> m_items;
			std::string m_dir;
		};
	}


static void sigpipe(int signal,siginfo_t* info, void* context)
	{
	}

static void sigpipeSetFilter()
	{
	struct sigaction sa;
	sa.sa_sigaction=sigpipe;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_SIGINFO;
	sa.sa_restorer=nullptr;

	struct sigaction sa_old;

	sigaction(SIGPIPE,&sa,&sa_old);
	}


static size_t read_helper(int fd,void* buffer, size_t count)
	{
	auto pos=reinterpret_cast<uint8_t*>(buffer);
	size_t n_read=0;
	while(n_read!=count)
		{
		auto n=::read(fd,pos,count-n_read);
		if(n==0)
			{return n_read;}
		if(n==-1)
			{
			if(!( errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK))
				{throw Error("I/O error");}
			n=0;
			}
		pos+=n;
		n_read+=n;
		}
	return n_read;
	}

static constexpr auto PIPE_READ_END=0;
static constexpr auto PIPE_WRITE_END=1;

void Tiger::filterCompile(const char* filename,const char* target_file,DataSink& sink_stderr)
	{
	auto tmp=std::string("/dev/shm/");
	tmp+=uuid_generate().data();
	DirectoryGuard g(tmp);
	printRange(filterstate_begin,filterstate_end,SinkStdio((tmp+"/filterstate.hpp").c_str()));
	g.itemAdd("filterstate.hpp");
	printRange(filterstateclient_begin,filterstateclient_end,SinkStdio((tmp+"/filterstateclient.hpp").c_str()));
	g.itemAdd("filterstateclient.hpp");
	printRange(pluginmain_begin,pluginmain_end,SinkStdio((tmp+"/pluginmain.hpp").c_str()));
	
	std::string iquote("-iquote");
	iquote+=tmp;

	const char* const argv[]=
		{
		 "g++","-std=c++14","-O3","--fast-math"
		,"-march=native","-fno-stack-protector"
		,"-Wconversion","-Wall",iquote.c_str()
		,"-include","pluginmain.hpp","-include","filterstateclient.hpp"
		,"-fpic","-shared","-o",target_file,filename,nullptr
		};

	sigpipeSetFilter();
	int pipe_stderr[2];
	pipe(pipe_stderr);
	int pipe_execerror[2];
	pipe(pipe_execerror);
	auto pid=fork();
	if(pid==0)
		{
		close(pipe_execerror[PIPE_READ_END]);
		fcntl(pipe_execerror[PIPE_WRITE_END],F_SETFD,FD_CLOEXEC);
		close(pipe_stderr[PIPE_READ_END]);
		dup2(pipe_stderr[PIPE_WRITE_END],STDERR_FILENO);
		if(execvp(argv[0],const_cast<char* const*>(argv))==-1)
			{
			int status=errno;
			auto res=write(pipe_execerror[PIPE_WRITE_END],&status,sizeof(status));
			_exit(static_cast<int>(res));
			}
		}
	
	int status=0;
	close(pipe_execerror[PIPE_WRITE_END]);
	close(pipe_stderr[PIPE_WRITE_END]);
	if(read(pipe_execerror[PIPE_READ_END],&status,sizeof(status))==sizeof(status))
		{
		close(pipe_execerror[PIPE_READ_END]);
		close(pipe_stderr[PIPE_READ_END]);
		int s;
		waitpid(static_cast<int>(pid),&s,0);
		throw Error("It was not possible to compile filter ",filename);
		}

	constexpr size_t N=4096;
	size_t n=0;
	char buffer[N];
	do
		{
		n=read_helper(pipe_stderr[PIPE_READ_END],buffer,N);
		write(STDERR_FILENO,buffer,n);
		sink_stderr.write(buffer,n);
		}
	while(n==N);
	
	close(pipe_execerror[PIPE_READ_END]);
	close(pipe_stderr[PIPE_READ_END]);
	int s;
	waitpid(static_cast<int>(pid),&s,0);
	s=WEXITSTATUS(s);
	if(s!=0)
		{throw Error("It was not possible to compile filter ",filename);}
	}
