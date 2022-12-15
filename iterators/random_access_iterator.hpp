/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_access_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 11:14:33 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 11:15:15 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

#include "iterator.hpp"
#include "enable_if.hpp"

namespace ft
{
	/* 	Random Access Iterator
		Source: https://cplusplus.com/reference/iterator/RandomAccessIterator/ */

	template<typename T, bool isConst>
		class random_access_iterator : private ft::iterator<ft::random_access_iterator_tag, T>
	{
	/********** Aliases **********/
		public:
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const T, T>::type >::value_type			value_type;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const T, T>::type >::difference_type		difference_type;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const T, T>::type >::pointer				pointer;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const T, T>::type >::reference			reference;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const T, T>::type >::iterator_category	iterator_category;

		protected:
			pointer	_current;

		public:
			random_access_iterator() : _current(pointer()) {};
			random_access_iterator(pointer const &other) : _current(other) {};
			random_access_iterator(random_access_iterator const &other) : _current(other._current) {};
			
			template<bool WasConst>
			random_access_iterator(random_access_iterator<T, WasConst> const &other, typename ft::enable_if<isConst && !WasConst>::type* = NULL) : _current(other.base()) {};
			
			random_access_iterator const &operator=(random_access_iterator const &other)
			{
				_current = other._current;
				return (*this);
			};

			friend difference_type	operator- (const random_access_iterator &it1, const random_access_iterator &it2) { return (it1._current -  it2._current); };

			reference				operator*(void) const { return *_current; };
			pointer					operator->(void) const { return _current; };
			reference				operator[](difference_type n) const { return _current[n]; };

			random_access_iterator&	operator++(void)
			{
				++_current;
				return (*this);
			};
			random_access_iterator	operator++(int) { return (random_access_iterator(_current++)); };
			random_access_iterator&	operator--(void)
			{
				--_current;
				return (*this);
			};
			random_access_iterator	operator--(int) { return (random_access_iterator(_current--)); };
			random_access_iterator&	operator+=(difference_type n)
			{
				_current += n;
				return (*this);
			};
			random_access_iterator&	operator-=(difference_type n)
			{
				_current -= n;
				return (*this);
			};
			random_access_iterator	operator+(difference_type n) const { return random_access_iterator(_current + n); };
			template<typename T_, bool isConst_>
			friend random_access_iterator<T_, isConst_>	operator+(typename random_access_iterator<T_, isConst_>::difference_type n, random_access_iterator<T_, isConst_> const &it);
			random_access_iterator	operator-(difference_type n) const { return random_access_iterator(_current - n); };

			pointer					base(void) const { return(_current); };
	};

	template<typename T_, bool isConst_>
	random_access_iterator<T_, isConst_>	operator+(typename random_access_iterator<T_, isConst_>::difference_type n, random_access_iterator<T_, isConst_> const &it)
	{
		return random_access_iterator<T_, isConst_>(n + it.base());
	};

	template<typename T, bool b1, bool b2>
	bool	operator==(const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() == rhs.base()); };
	template<typename T, bool b1, bool b2>
	bool	operator!=(const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() != rhs.base()); };
	template<typename T, bool b1, bool b2>
	bool	operator< (const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() <  rhs.base()); };
	template<typename T, bool b1, bool b2>
	bool	operator> (const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() >  rhs.base()); };
	template<typename T, bool b1, bool b2>
	bool	operator<=(const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() <= rhs.base()); };
	template<typename T, bool b1, bool b2>
	bool	operator>=(const random_access_iterator<T, b1> &lhs, const random_access_iterator<T, b2> &rhs) { return (lhs.base() >= rhs.base()); };
}

#endif