//@	{
//@	 "targets":{"name":"filenamepicker.hpp","type":"include"}
//@	,"dependencies_extra":[{"ref":"filenameselect.o","rel":"implementation"}]
//@	}

#ifndef TIGER_FILENAMEPICKER_HPP
#define TIGER_FILENAMEPICKER_HPP

#include <utility>
#include <string>

namespace Tiger
	{
	class Container;

	enum class FileselectMode:int{OPEN,SAVE};

	bool filenameSelect(Container& cnt,std::string& filename_in,FileselectMode mode);
	}

#endif