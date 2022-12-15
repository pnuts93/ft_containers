/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 06:58:56 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 11:40:43 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <sstream>
#include <memory>
#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "is_integral.hpp"
#include "compare.hpp"

namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T											value_type;
			typedef Alloc										allocator_type;
			typedef value_type&									reference;
			typedef const value_type&							const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef ft::random_access_iterator<pointer, false>	iterator;
			typedef ft::random_access_iterator<pointer, true>	const_iterator;
			typedef ft::reverse_iterator<iterator>				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;
			typedef ptrdiff_t									difference_type;
			typedef size_t										size_type;

		protected:
			value_type		*_arr;
			allocator_type	_alloc;
			size_type		_capacity;
			size_type		_size;

		private:
			value_type *mem_realloc(value_type *old_ptr, size_type old_size, size_type new_size)
			{
				value_type *res = _alloc.allocate(new_size);
				_alloc.construct(res, value_type());
				int i(0);
				while (old_size - i > 0 && new_size - i > 0)
				{
					res[i] = old_ptr[i];
					i++;
				}
				if (old_ptr)
					_alloc.deallocate(old_ptr, old_size);
				return (res);
			}

			std::string to_string(size_type n) const
			{
				std::stringstream res;
				res << n;
				return (res.str());
			}

			std::string _M_range_check(size_type n) const
			{
				std::string res;
				res.append("vector::_M_range_check: __n (which is ");
				res.append(to_string(n));
				res.append(") >= this->size() (which is ");
				res.append(to_string(_size));
				res.append(")");
				return (res);
			}

		public:
			explicit vector(const allocator_type& alloc = allocator_type()) :
			_arr(NULL),
			_alloc(alloc),
			_capacity(0),
			_size(0)
			{};
			
			explicit vector(size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) :
			_alloc(alloc),
			_capacity(n)
			{
				_arr = _alloc.allocate(n);
				_size = 0;
				while (_size < n)
				{
					_alloc.construct(_arr + _size, val);
					_size++;
				}
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL) :
			_alloc(alloc),
			_capacity(ft::distance(first, last))
			{
				_arr = _alloc.allocate(_capacity);
				_size = 0;
				while (first != last)
				{
					_alloc.construct(_arr + _size, *first);
					first++;
					_size++;
				}
			};

			vector(const vector &other) :
			_alloc(other._alloc),
			_capacity(other._capacity),
			_size(other._size)
			{
				_arr = _alloc.allocate(_capacity);
				size_type i = 0;
				while (i < _size)
				{
					_alloc.construct(_arr + i, other._arr[i]);
					i++;
				}
			};

			~vector()
			{
				_alloc.deallocate(_arr, _capacity);
			};
			
			vector&	operator=(const vector &other)
			{
				clear();
				if (_capacity < other.size())
					reserve(other.size());
				_size = 0;
				while (_size < other._size)
				{
					_arr[_size] = other._arr[_size];
					_size++;
				}
				return (*this);
			};

			const_iterator			begin(void) const { return (const_iterator(_arr)); };
			iterator				begin(void) { return (iterator(_arr)); };
			const_iterator			end(void) const { return (const_iterator(_arr + _size)); };
			iterator				end(void) { return (iterator(_arr + _size)); };
			reverse_iterator		rbegin(void) { return ( reverse_iterator(iterator(_arr + _size))); };
			const_reverse_iterator	rbegin(void) const { return (const_reverse_iterator(const_iterator(_arr + _size))); };
			reverse_iterator		rend(void) { return (reverse_iterator(iterator(_arr))); };
			const_reverse_iterator	rend(void) const { return (const_reverse_iterator(const_iterator(_arr))); };

			size_type		size(void) const { return (_size); };
			size_type		max_size(void) const { return (_alloc.max_size()); };
			size_type		capacity() const { return (_capacity); };

			bool			empty() const { return (_size == 0); };

			void			resize(size_type n, value_type val = value_type())
			{
				if (n < _size)
				{
					while (n < _size)
						pop_back();
				}
				else if (n > _size)
				{
					if (_capacity < n)
						reserve(n);
					while (_size < n)
						_arr[_size++] = val;
				}
			};

			void			reserve(size_type n)
			{
				if (n > max_size())
					throw std::length_error(_M_range_check(n));
				else if (n > _capacity)
				{
					_arr = mem_realloc(_arr, _capacity, n);
					_capacity = n;
				}
			};

			reference		operator[](size_type n) { return (_arr[n]); };
			const_reference	operator[](size_type n) const { return (_arr[n]); };
			reference		at(size_type n)
			{
				if (n >= _size)
					throw std::out_of_range(_M_range_check(n));
				else
					return (_arr[n]);
			};
			const_reference	at(size_type n) const
			{
				if (n >= _size)
					throw std::out_of_range(_M_range_check(n));
				else
				{
					return (_arr[n]);
				}
			};
			reference		front(void) { return (_arr[0]); };
			const_reference	front(void) const { return (_arr[0]); };
			reference		back(void) { return (_arr[_size - 1]); };
			const_reference	back(void) const { return (_arr[_size - 1]); };

			template <class InputIterator>
				void		assign(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			{
				if (_size > 0)
				{
					for (size_type i = 0; i < _size; i++)
						_alloc.destroy(_arr + i);
				}
				size_type n = ft::distance(first, last);
				if (_capacity < n)
				{
					_alloc.deallocate(_arr, _capacity);
					_arr = _alloc.allocate(n);
					_capacity = n;
				}
				_size = 0;
				while (first != last)
				{
					_alloc.construct(_arr + _size, *first);
					first++;
					_size++;
				}
			};
			void			assign(size_type n, const value_type& val)
			{
				if (_size > 0)
				{
					for (size_type i = 0; i < _size; i++)
						_alloc.destroy(_arr + i);
				}
				if (_capacity < n)
				{
					_alloc.deallocate(_arr, _capacity);
					_arr = _alloc.allocate(n);
					_capacity = n;
				}
				for (size_type i = 0; i < n; i++)
					_alloc.construct(_arr + i, val);
				_size = n;
			}
			void			push_back(const value_type& val)
			{
				if (_size == _capacity)
					reserve(_capacity * 2 + (_capacity == 0));
				_arr[_size++] = val;
			};
			void		pop_back(void)
			{
				_size--;
				_alloc.destroy(_arr + _size);
			};
			iterator		insert(iterator position, const value_type& val)
			{
				size_type ret = position - begin();
				insert(position, 1, val);
				return (iterator(_arr + ret));
			};
			void			insert(iterator position, size_type n, const value_type& val)
			{
				vector<value_type>	tmp;
				if ((_size + n) > _capacity)
					tmp.reserve(_size + n);
				else
					tmp.reserve(_capacity);
				iterator	new_it = tmp.begin();
				iterator	old_it = begin();
				while (old_it != position)
				{
					*new_it = *old_it;
					new_it++;
					old_it++;
					tmp._size++;
				}
				while (n-- > 0)
				{
					*new_it = val;
					new_it++;
					tmp._size++;
				}
				while (old_it != end())
				{
					*new_it = *old_it;
					new_it++;
					old_it++;
					tmp._size++;
				}
				swap(tmp);
			};
			template<class InputIterator>
				void		insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			{
				vector<value_type> tmp;
				size_type n = ft::distance(first, last);
				if ((_size + n) > _capacity)
					tmp.reserve(_size + n);
				else
					tmp.reserve(_capacity);
				iterator	new_it = tmp.begin();
				iterator	old_it = begin();
				while (old_it != position)
				{
					*new_it = *old_it;
					new_it++;
					old_it++;
					tmp._size++;
				}
				while (first != last)
				{
					*new_it = *first;
					new_it++;
					first++;
					tmp._size++;
				}
				while (old_it != end())
				{
					*new_it = *old_it;
					new_it++;
					old_it++;
					tmp._size++;
				}
				swap(tmp);
			};
			iterator		erase(iterator position) { return (erase(position, position + 1)); };
			iterator		erase(iterator first, iterator last)
			{
				iterator it_e = end();
				if (first == it_e)
					return (it_e);
				iterator it = first;
				while (it != last)
				{
					_alloc.destroy((it++).base());
					_size--;
				}
				it = first;
				while (last != it_e)
				{
					_alloc.construct(it.base(), *last);
					_alloc.destroy(last.base());
					it++;
					last++;
				}
				return (first);
			};
			void			swap(vector& x)
			{
				pointer tmp_arr = x._arr;
				size_type tmp_capacity = x._capacity;
				size_type tmp_size = x._size;
				allocator_type tmp_alloc = x._alloc;
				x._arr = _arr;
				x._capacity = _capacity;
				x._size = _size;
				x._alloc = _alloc;
				_arr = tmp_arr;
				_capacity = tmp_capacity;
				_size = tmp_size;
				_alloc = tmp_alloc;
			};
			void			clear(void) { erase(begin(), end()); };
			allocator_type	get_allocator(void) const { return (_alloc); };
	};

	template<class T, class Alloc>
		bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			if (lhs.size() != rhs.size())
				return (false);
			else
				return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
		};

	template<class T, class Alloc>
		bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(lhs == rhs));
		};

	template <class T, class Alloc>
		bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		};

	template <class T, class Alloc>
		bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (lhs == rhs || lhs < rhs);
		};

	template <class T, class Alloc>
		bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(lhs <= rhs));
		};

	template <class T, class Alloc>
		bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
		{
			return (!(lhs < rhs));
		};

	template <class T, class Alloc>
		void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
		{
			x.swap(y);
		};
}

#endif