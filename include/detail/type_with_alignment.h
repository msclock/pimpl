//  (C) Copyright John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.
#pragma once

#include <cstddef> // size_t
#include <type_traits>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4121) // alignment is sensitive to packing
#endif

namespace pimpl {

	namespace detail {

		union max_align
		{
			char c;
			short s;
			int i;
			long l;
			long long ll;
			float f;
			double d;
			long double ld;
		};

		template <std::size_t Target, bool check>
		struct long_double_alignment
		{
			typedef long double type;
		};
		template <std::size_t Target>
		struct long_double_alignment<Target, false>
		{
			typedef detail::max_align type;
		};

		template <std::size_t Target, bool check>
		struct double_alignment
		{
			typedef double type;
		};
		template <std::size_t Target>
		struct double_alignment<Target, false>
		{
			typedef typename long_double_alignment<Target, std::alignment_of<long double>::value >= Target>::type type;
		};

		template <std::size_t Target, bool check>
		struct long_long_alignment
		{
			typedef long long type;
		};
		template <std::size_t Target>
		struct long_long_alignment<Target, false>
		{
			typedef typename double_alignment<Target, std::alignment_of<double>::value >= Target>::type type;
		};

		template <std::size_t Target, bool check>
		struct long_alignment
		{
			typedef long type;
		};
		template <std::size_t Target>
		struct long_alignment<Target, false>
		{
			typedef typename long_long_alignment<Target, std::alignment_of<long long>::value >= Target>::type type;
		};

		template <std::size_t Target, bool check>
		struct int_alignment
		{
			typedef int type;
		};
		template <std::size_t Target>
		struct int_alignment<Target, false>
		{
			typedef typename long_alignment<Target, std::alignment_of<long>::value >= Target>::type type;
		};

		template <std::size_t Target, bool check>
		struct short_alignment
		{
			typedef short type;
		};
		template <std::size_t Target>
		struct short_alignment<Target, false>
		{
			typedef typename int_alignment<Target, std::alignment_of<int>::value >= Target>::type type;
		};

		template <std::size_t Target, bool check>
		struct char_alignment
		{
			typedef char type;
		};
		template <std::size_t Target>
		struct char_alignment<Target, false>
		{
			typedef typename short_alignment<Target, std::alignment_of<short>::value >= Target>::type type;
		};

	} // namespace detail

	template <std::size_t Align>
	struct type_with_alignment
	{
		typedef typename detail::char_alignment<Align, std::alignment_of<char>::value >= Align>::type type;
	};

#if defined(__GNUC__)
	namespace tt_align_ns {

		struct __attribute__((__aligned__(2))) a2
		{
		};
		struct __attribute__((__aligned__(4))) a4
		{
		};
		struct __attribute__((__aligned__(8))) a8
		{
		};
		struct __attribute__((__aligned__(16))) a16
		{
		};
		struct __attribute__((__aligned__(32))) a32
		{
		};
		struct __attribute__((__aligned__(64))) a64
		{
		};
		struct __attribute__((__aligned__(128))) a128
		{
		};
	} // namespace tt_align_ns

	template <>
	struct type_with_alignment<1>
	{
	public:
		typedef char type;
	};
	template <>
	struct type_with_alignment<2>
	{
	public:
		typedef tt_align_ns::a2 type;
	};
	template <>
	struct type_with_alignment<4>
	{
	public:
		typedef tt_align_ns::a4 type;
	};
	template <>
	struct type_with_alignment<8>
	{
	public:
		typedef tt_align_ns::a8 type;
	};
	template <>
	struct type_with_alignment<16>
	{
	public:
		typedef tt_align_ns::a16 type;
	};
	template <>
	struct type_with_alignment<32>
	{
	public:
		typedef tt_align_ns::a32 type;
	};
	template <>
	struct type_with_alignment<64>
	{
	public:
		typedef tt_align_ns::a64 type;
	};
	template <>
	struct type_with_alignment<128>
	{
	public:
		typedef tt_align_ns::a128 type;
	};

#endif
} // namespace pimpl

#if defined(__GNUC__)
namespace std {

	// forward declaration
	template <typename T>
	struct is_pod;

	template <>
	struct is_pod<pimpl::tt_align_ns::a2> : public true_type
	{
	};

	template <>
	struct is_pod<pimpl::tt_align_ns::a4> : public true_type
	{
	};
	template <>
	struct is_pod<pimpl::tt_align_ns::a8> : public true_type
	{
	};
	template <>
	struct is_pod<pimpl::tt_align_ns::a16> : public true_type
	{
	};
	template <>
	struct is_pod<pimpl::tt_align_ns::a32> : public true_type
	{
	};
	template <>
	struct is_pod<pimpl::tt_align_ns::a64> : public true_type
	{
	};
	template <>
	struct is_pod<pimpl::tt_align_ns::a128> : public true_type
	{
	};
} // namespace std

#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
