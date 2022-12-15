/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_integral.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 16:27:40 by pnuti             #+#    #+#             */
/*   Updated: 2022/11/21 11:15:04 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IS_INTEGRAL_HPP
#define IS_INTEGRAL_HPP

namespace ft
{
	template <class T, T v>
	struct integral_constant
	{
		static const T value = v;
		typedef T value_type;
		typedef integral_constant<T,v> type;
		operator T() { return v; }
	};

	template <class T, T v> const
		T integral_constant<T, v>::value;

	typedef integral_constant<bool, true>  true_type;
	typedef integral_constant<bool, false> false_type;

	template <class T> struct is_integral : public false_type {};

	template <> struct is_integral<bool>					: public true_type {};
	template <> struct is_integral<char>					: public true_type {};
	//template <> struct is_integral<char16_t>				: public true_type {}; c++11
	//template <> struct is_integral<char32_t>				: public true_type {}; c++11
	template <> struct is_integral<signed char>				: public true_type {};
	template <> struct is_integral<short int>				: public true_type {};
	template <> struct is_integral<int>						: public true_type {};
	template <> struct is_integral<long int>				: public true_type {};
	template <> struct is_integral<long long int>			: public true_type {};
	template <> struct is_integral<unsigned char>			: public true_type {};
	template <> struct is_integral<unsigned short int>		: public true_type {};
	template <> struct is_integral<unsigned int>			: public true_type {};
	template <> struct is_integral<unsigned long int>		: public true_type {};
	template <> struct is_integral<unsigned long long int>	: public true_type {};
}

#endif