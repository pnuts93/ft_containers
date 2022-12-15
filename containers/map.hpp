/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 08:43:37 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 16:02:32 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

#include "bidirectional_iterator.hpp"
#include "reverse_iterator.hpp"
#include "compare.hpp"

namespace ft
{
	template < typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<T> >
	class map
	{
		public:
			typedef Key														key_type;
			typedef T														mapped_type;
			typedef	pair<const key_type, mapped_type>						value_type;
			typedef Compare													key_compare;
			typedef Alloc													allocator_type;
			typedef typename allocator_type::reference						reference;
			typedef typename allocator_type::const_reference				const_reference;
			typedef typename allocator_type::pointer						pointer;
			typedef typename allocator_type::const_pointer					const_pointer;
			typedef ft::bidirectional_iterator<key_type, mapped_type, false, key_compare>	iterator;
			typedef ft::bidirectional_iterator<key_type, mapped_type, true, key_compare>	const_iterator;
			typedef ft::reverse_iterator<iterator>							reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;
			typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
			typedef size_t													size_type;
			class value_compare
			{
				friend class map;
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {};
				public:
					typedef bool result_type;
					typedef value_type first_argument_type;
					typedef value_type second_argument_type;
					bool operator() (const value_type& x, const value_type& y) const
					{
						return comp(x.first, y.first);
					};
			};

		private:
			typedef typename RBT<Key, T, Compare>::node_pointer			node_pointer;
			ft::RBT<Key, T, key_compare>								_bin_tree;
			allocator_type												_allocator;

		public:
			map() : _bin_tree(key_compare()),
					_allocator(allocator_type())
			{};
			
			explicit map(const key_compare& comp, const allocator_type& alloc = allocator_type()) : _bin_tree(comp),
																									_allocator(alloc)
			{};
			
			template <class InputIterator>
			map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _bin_tree(comp),
																																					_allocator(alloc)
			{
				while (first != last)
				{
					_bin_tree.insert(*first);
					first++;
				}
			};
			
			map(const map &other) : _allocator(other._allocator)
			{
				*this = other;
			};
			
			~map() {};
			
			map& operator=(const map &other)
			{
				_bin_tree = other._bin_tree;
				return (*this);
			};
			
			// Iterators

			iterator				begin(void)
			{
				if (_bin_tree.getRoot())
					return (iterator(RBT<Key, T, Compare>::minValueNode(_bin_tree.getRoot())));
				return (iterator(_bin_tree.getEnd()));
			};
			const_iterator			begin(void) const
			{
				if (_bin_tree.getRoot())
					return (const_iterator(RBT<Key, T, Compare>::minValueNode(_bin_tree.getRoot())));
				return (const_iterator(_bin_tree.getEnd()));
			};
			iterator				end(void)
			{
				return (iterator(_bin_tree.getEnd()));
			};
			const_iterator			end(void) const
			{
				return (const_iterator(_bin_tree.getEnd()));
			};
			reverse_iterator		rbegin(void) { return (reverse_iterator(end())); };
			const_reverse_iterator	rbegin(void) const { return (const_reverse_iterator(end())); };
			reverse_iterator		rend(void) { return (reverse_iterator(begin())); };
			const_reverse_iterator	rend(void) const { return (const_reverse_iterator(begin())); };

			// Capacity

			bool		empty(void) const { return (!_bin_tree.getRoot()); };
			size_type	size(void) const { return (RBT<Key, T, Compare>::countChildren(_bin_tree.getRoot())); };
			size_type	max_size(void) const { return (_bin_tree.max_size()); };

			// Element Access

			mapped_type& operator[] (const key_type& k) { return (((_bin_tree.insert(ft::make_pair(k, mapped_type()))).first)->data.second); };

			// Modifiers

			pair<iterator,bool>	insert(const value_type& val)
			{
				node_pointer existing_node = _bin_tree.search(val);
				if (existing_node != _bin_tree.getEnd())
					return (ft::make_pair(iterator(existing_node), false));
				pair<node_pointer, bool> ret = _bin_tree.insert(val);
				return (ft::make_pair(iterator(ret.first), ret.second));
			};
			iterator			insert(iterator position, const value_type& val)
			{
				(void)position;
				return (insert(val).first);
			};
			template <class InputIterator>
			void				insert(InputIterator first, InputIterator last)
			{
				while (first != last)
					insert(*(first++));
			};
			void				erase(iterator position) { _bin_tree.deleteNode(*position); };
			size_type			erase(const key_type& k)
			{
				node_pointer to_delete = _bin_tree.search(ft::make_pair(k, mapped_type()));
				if (to_delete && !to_delete->isEnd)
				{
					_bin_tree.deleteNode(to_delete->data); 
					return (1);
				}
				return (0);
			};
			void				erase(iterator first, iterator last)
			{
				size_t n_erased(0);
				iterator first_tmp(first);
				int i(0);

				while (first++ != last)
					n_erased++;
				key_type *to_erase = new key_type[n_erased];
				while (first_tmp != last)
					to_erase[i++] = (first_tmp++)->first;
				while (i > 0)
					_bin_tree.deleteNode(ft::make_pair(to_erase[--i], mapped_type()));
				delete [] to_erase;
			};
			void				swap(map& x) { _bin_tree.swap(x._bin_tree); };
			void				clear(void) { erase(begin(), end()); };

			// Observers

			key_compare		key_comp(void) const { return (key_compare()); };
			value_compare	value_comp(void) const { return (value_compare(key_comp())); };

			// Operations

			iterator							find(const key_type& k) { return (iterator(_bin_tree.search(ft::make_pair(k, mapped_type())))); };
			const_iterator						find(const key_type& k) const { return (const_iterator(_bin_tree.search(ft::make_pair(k, mapped_type())))); };
			size_type							count(const key_type& k) const
			{
				if (!_bin_tree.search(ft::make_pair(k, mapped_type()))->isEnd)
					return (1);
				return (0);
			};
			iterator							lower_bound(const key_type& k)
			{
				iterator start = begin();
				iterator finish = end();
				while (start != finish && key_comp()(start->first, k))
					++start;
				return (start);
			};
			const_iterator						lower_bound(const key_type& k) const
			{
				const_iterator start = begin();
				const_iterator finish = end();
				while (start != finish && key_comp()(start->first, k))
					++start;
				return (start);
			};
			iterator							upper_bound(const key_type& k)
			{
				iterator start = begin();
				iterator finish = end();
				while (start != finish && !key_comp()(k, start->first))
					++start;
				return (start);
			};
			const_iterator						upper_bound(const key_type& k) const
			{
				const_iterator start = begin();
				const_iterator finish = end();
				while (start != finish && !key_comp()(k, start->first))
					++start;
				return (start);
			};
			pair<iterator,iterator>				equal_range(const key_type& k) { return (ft::make_pair(lower_bound(k), upper_bound(k))); };
			pair<const_iterator,const_iterator>	equal_range(const key_type& k) const { return (ft::make_pair(lower_bound(k), upper_bound(k))); };

			// Allocator

			allocator_type get_allocator(void) const { return (_allocator); };

			// Non-member relational operators

			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator==(const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator!=(const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator< (const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator<=(const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator> (const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
			template <class Key_, class T_, class Compare_, class Alloc_>
			friend bool operator>=(const map<Key_,T_,Compare_,Alloc_>& lhs, const map<Key_,T_,Compare_,Alloc_>& rhs );
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator==(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		return (!(lhs == rhs));
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator< (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		return ((lhs == rhs) || (lhs < rhs));
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator> (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		return (!(lhs <= rhs));
	};
	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs)
	{
		return (!(lhs < rhs));
	};
}

#endif