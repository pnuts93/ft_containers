/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 11:22:22 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 16:47:55 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator.hpp"

namespace ft
{
	template <class Iterator>
		class reverse_iterator
	{
		public:
			typedef Iterator													iterator_type;
			typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename ft::iterator_traits<Iterator>::value_type			value_type;
			typedef typename ft::iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename ft::iterator_traits<Iterator>::pointer				pointer;
			typedef typename ft::iterator_traits<Iterator>::reference			reference;

		protected:
			iterator_type	_current;
		
		public:
			reverse_iterator() : _current(iterator_type()) {};
			explicit reverse_iterator(iterator_type const &other) : _current(other) {};
			template<typename Iter>
			reverse_iterator(reverse_iterator<Iter> const &other) : _current(other.base()) {};

			reverse_iterator& operator=(reverse_iterator<Iterator> const &other)
			{
				_current = other.base();
				return (*this);
			}

			Iterator			base(void) const { return (_current); };
			reference			operator*(void) const
			{
				Iterator tmp(_current);
				return (*(--tmp));
			};
			reference			operator[](difference_type n) const { return (_current[-n -1]); };
			pointer				operator->(void) const { return (&(operator*())); };
			reverse_iterator&	operator++(void)
			{
				--_current;
				return (*this);
			};
			reverse_iterator	operator++(int)
			{
				reverse_iterator	t(_current);
				--_current;
				return (t);
			};
			reverse_iterator&	operator--(void)
			{
				++_current;
				return (*this);
			};
			reverse_iterator	operator--(int)
			{
				reverse_iterator	t(_current);
				++_current;
				return (t);
			};
			reverse_iterator	operator+=(difference_type n)
			{
				_current -= n;
				return (*this);
			};
			reverse_iterator	operator-=(difference_type n)
			{
				_current += n;
				return (*this);
			};

			reverse_iterator	operator+(difference_type n) { return reverse_iterator(_current - n); };
			template<typename Iterator_>
			friend reverse_iterator<Iterator_>	operator+(typename reverse_iterator<Iterator_>::difference_type n, reverse_iterator<Iterator_> const &rit);

			reverse_iterator	operator-(difference_type n) { return reverse_iterator(_current + n); };

			template< class Iterator1, class Iterator2>
			friend typename reverse_iterator<Iterator1>::difference_type operator-( const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs);
	};

	template<typename Iter1, typename Iter2>
	bool	operator==(const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() == rhs.base()); };
	template<typename Iter1, typename Iter2>
	bool	operator!=(const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() != rhs.base()); };
	template<typename Iter1, typename Iter2>
	bool	operator< (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() >  rhs.base()); };
	template<typename Iter1, typename Iter2>
	bool	operator> (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() <  rhs.base()); };
	template<typename Iter1, typename Iter2>
	bool	operator<=(const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() >= rhs.base()); };
	template<typename Iter1, typename Iter2>
	bool	operator>=(const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs) { return (lhs.base() <= rhs.base()); };

	template<typename Iterator_>
	reverse_iterator<Iterator_>	operator+(typename reverse_iterator<Iterator_>::difference_type n, reverse_iterator<Iterator_> const &rit)
	{
		return reverse_iterator<Iterator_>(rit.base() - n);
	};

	template< class Iterator1, class Iterator2>
	typename reverse_iterator<Iterator1>::difference_type operator-(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return (rhs.base() - lhs.base());
	};
}

#endif