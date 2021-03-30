//-----------------------------------------------------------------------------
// boost aligned_storage.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef> // for std::size_t
#include <type_traits>
#include "type_with_alignment.h"
namespace pimpl {
	namespace detail {

		namespace aligned_storage {

			static const std::size_t alignment_of_max_align = std::alignment_of<detail::max_align>::value;

			//
			// To be TR1 conforming this must be a POD type:
			//
			template <
				std::size_t size_,
				std::size_t alignment_>
			struct aligned_storage_imp
			{
				union data_t
				{
					char buf[size_];
					typename pimpl::type_with_alignment<alignment_>::type align_;
				} data_;
				void* address() const { return const_cast<aligned_storage_imp*>(this); }
			};
			template <std::size_t size>
			struct aligned_storage_imp<size, std::size_t(-1)>
			{
				union data_t
				{
					char buf[size];
					pimpl::detail::max_align align_;
				} data_;
				void* address() const { return const_cast<aligned_storage_imp*>(this); }
			};

			template <std::size_t alignment_>
			struct aligned_storage_imp<0u, alignment_>
			{
				/* intentionally empty */
				void* address() const { return 0; }
			};

		} // namespace aligned_storage
	}	  // namespace detail

	template <
		std::size_t size_,
		std::size_t alignment_ = std::size_t(-1)>
	class aligned_storage : private pimpl::detail::aligned_storage::aligned_storage_imp<size_, alignment_>
	{
	public: // constants
		using type = pimpl::detail::aligned_storage::aligned_storage_imp<size_, alignment_>;

		static const std::size_t size = size_;
		static const std::size_t alignment = (alignment_ == std::size_t(-1)
												  ? pimpl::detail::aligned_storage::alignment_of_max_align
												  : alignment_);

	private: // noncopyable
		aligned_storage(const aligned_storage&);
		aligned_storage& operator=(const aligned_storage&);

	public: // structors
		aligned_storage()
		{
		}

		~aligned_storage()
		{
		}

	public: // accessors
		void* address()
		{
			return static_cast<type*>(this)->address();
		}

		const void* address() const
		{
			return static_cast<const type*>(this)->address();
		}
	};
} // namespace pimpl

namespace std {

	// forward declaration,  needed by 'aligned_storage_imp' template below
	template <typename T>
	struct is_pod;

	template <std::size_t size_, std::size_t alignment_>
	struct is_pod<pimpl::detail::aligned_storage::aligned_storage_imp<size_, alignment_>> : public true_type
	{
	};
} // namespace std