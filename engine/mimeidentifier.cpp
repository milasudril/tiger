//@	{
//@	 "targets":
//@		[{
//@		"name":"mimeidentifier.o","type":"object"
//@		,"dependencies":[{"ref":"magic","rel":"external"}]
//@		}]
//@	}

#include "mimeidentifier.hpp"
#include <magic.h>

using namespace Tiger;

MimeIdentifier::MimeIdentifier():m_handle(magic_open(MAGIC_ERROR|MAGIC_MIME))
	{magic_load(reinterpret_cast<magic_t>(m_handle),nullptr);}

MimeIdentifier::~MimeIdentifier()
	{magic_close(reinterpret_cast<magic_t>(m_handle));}

const char* MimeIdentifier::identify(const char* filename) const noexcept
	{return magic_file(reinterpret_cast<magic_t>(m_handle),filename);}
