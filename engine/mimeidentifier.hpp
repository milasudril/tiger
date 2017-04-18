//@	{
//@	 "targets":[{"name":"mimeidentifier.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"mimeidentifier.o","rel":"implementation"}]
//@	}

#ifndef TIGER_MIMEIDENTIFIER_HPP
#define TIGER_MIMEIDENTIFIER_HPP

#include <cstdint>

namespace Tiger
	{
	class MimeIdentifier
		{
		public:
			MimeIdentifier(const MimeIdentifier&)=delete;
			MimeIdentifier& operator=(const MimeIdentifier&)=delete;

			MimeIdentifier();
			~MimeIdentifier();

			const char* identify(const char* filename) const noexcept;

		private:
			void* m_handle;
		};
	}

#endif
