// Copyright (c) 2008 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#pragma once

#include "has_member.h"
#include <type_traits>

namespace detail {
	
	template <typename, typename = void>
	struct is_allocator
	{
		static const bool value = false;
	};

	template <typename class_type>
	struct is_allocator<class_type, typename std::enable_if<std::is_class<class_type>::value, void>::type>
	{
		DECLARE_HAS_MEMBER(has_allocate, allocate);
		DECLARE_HAS_MEMBER(has_deallocate, deallocate);

		static const bool value = has_allocate<class_type>::value && has_deallocate<class_type>::value;
	};
} // namespace detail
