#pragma once

#include <type_traits>
#include <memory>

namespace pimpl { // FUNCTION TEMPLATE to_address

	template <class T, class = void>
	constexpr bool can_apply_to_address = false; // determines whether _Ptr has pointer_traits<_Ptr>::to_address(p)

	template <class T>
	constexpr bool can_apply_to_address<T, std::void_t<decltype(std::pointer_traits<T>::to_address(std::declval<const T&>()))>> = true;

	template <class T>
	constexpr T* to_address(T* const _Val) noexcept
	{
		static_assert(!std::is_function<T>::value,
					  "N4810 20.10.4 [pointer.conversion]/2: The program is ill-formed if T is a function type.");
		return _Val;
	}

	template <class Ptr>
	constexpr auto to_address(const Ptr& _Val) noexcept
	{
		if (can_apply_to_address<Ptr>)
		{
			return std::pointer_traits<Ptr>::to_address(_Val);
		}
		else
			return ::to_address(_Val.operator->()); // plain pointer overload must come first
	}
} // namespace pimpl