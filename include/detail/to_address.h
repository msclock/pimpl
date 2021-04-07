#pragma once

#include <type_traits>
#include <memory>

namespace pimpl {

	// FUNCTION TEMPLATE to_address

	/// A copy of std::void_t from C++17 (helper for C++11 and C++14)
	template <typename... Ts>
	struct make_void
	{
		using type = void;
	};

	/// A copy of std::void_t from C++17 - same reasoning as enable_if_t, it does not hurt to redefine
	template <typename... Ts>
	using void_t = typename make_void<Ts...>::type;

	template <typename T, class = void>
	constexpr bool can_apply_to_address = false; // determines whether _Ptr has pointer_traits<_Ptr>::to_address(p)

	template <typename T>
	constexpr bool can_apply_to_address<T, void_t<decltype(std::pointer_traits<T>::to_address(std::declval<const T&>()))>> = true;

	template <typename T>
	constexpr T* to_address(T* const _Val) noexcept
	{
		static_assert(!std::is_function<T>::value,
					  "N4810 20.10.4 [pointer.conversion]/2: The program is ill-formed if T is a function type.");
		return _Val;
	}

	template <typename Ptr>
	constexpr auto to_address(const Ptr& _Val) noexcept
	{
		if (can_apply_to_address<Ptr>)
		{
			return std::pointer_traits<Ptr>::to_address(_Val);
		}
		else
			return pimpl::to_address(_Val.operator->()); // plain pointer overload must come first
	}
} // namespace pimpl