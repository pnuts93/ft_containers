/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bidirectional_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 16:01:12 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 17:20:41 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIDIRECTIONAL_ITERATOR_HPP
#define BIDIRECTIONAL_ITERATOR_HPP

#include "iterator.hpp"
#include "RBT.hpp"
#include "enable_if.hpp"

namespace ft
{
	template<typename Key, typename T, bool isConst, typename Compare = std::less<Key> >
	class bidirectional_iterator : private ft::iterator<ft::bidirectional_iterator_tag, T>
	{
		public:
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const pair<Key, T>*, pair<Key, T>*>::type >::value_type			value_type;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const pair<Key, T>*, pair<Key, T>*>::type >::difference_type		difference_type;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const pair<Key, T>*, pair<Key, T>*>::type >::pointer				pointer;
			typedef typename ft::iterator_traits<typename chooseConst<isConst, const pair<Key, T>*, pair<Key, T>*>::type >::reference			reference;
			typedef ft::bidirectional_iterator_tag																								iterator_category;

		protected:
			typedef ft::node<pair<Key, T> >*	node_pointer;
			node_pointer						_current;

		public:
			bidirectional_iterator() : _current(NULL) {};
			bidirectional_iterator(node_pointer a_node) : _current(a_node) {};
			bidirectional_iterator(bidirectional_iterator const &other) : _current(other.base()) {};
			template<bool WasConst>
			bidirectional_iterator(bidirectional_iterator<Key, T, WasConst, Compare> const &other, typename ft::enable_if<isConst && !WasConst>::type* = NULL) : _current(other.base()) {};
			
			~bidirectional_iterator() {};

			bidirectional_iterator	&operator=(bidirectional_iterator const &other)
			{
				_current = other._current;
				return (*this);
			};

			node_pointer			base(void) const { return (_current); };
			template<bool b>
			bool					operator==(bidirectional_iterator<Key, T, b, Compare> const &other) const { return (_current == other.base()); };
			template<bool b>
			bool					operator!=(bidirectional_iterator<Key, T, b, Compare> const &other) const { return (!(_current == other.base())); };

			reference				operator*(void) const { return (_current->data); };
			pointer					operator->(void) const { return (&_current->data); };

			bidirectional_iterator&	operator++(void)
			{
				_current = RBT<Key, T, Compare>::forward(_current, Compare());
				return (*this);
			};
			bidirectional_iterator	operator++(int)
			{
				bidirectional_iterator	tmp(_current);
				_current = RBT<Key, T, Compare>::forward(_current, Compare());
				return (tmp);
			};
			bidirectional_iterator&	operator--(void)
			{
				_current = RBT<Key, T, Compare>::back(_current, Compare());
				return (*this);
			};
			bidirectional_iterator	operator--(int)
			{
				bidirectional_iterator	tmp(_current);
				_current = RBT<Key, T, Compare>::back(_current, Compare());
				return (tmp);
			};
	};
}

#endif