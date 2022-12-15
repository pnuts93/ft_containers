/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 08:55:08 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 09:27:26 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft
{
	template <class T, class Container = vector<T> >
	class stack
	{
		public:
			typedef T			value_type;
			typedef Container	container_type;
			typedef size_t		size_type;
		
		protected:
			container_type	c;

		public:
			explicit stack(const container_type& ctnr = container_type()) : c(ctnr) {};
			~stack() {};

			stack& operator=(const container_type& c)
			{
				this->c = c; 
				return (*this);
			};

			bool				empty(void) const { return (c.empty()); };
			size_type			size(void) const { return (c.size()); };
			value_type&			top(void) { return (c.back()); };
			const value_type&	top(void) const { return (c.back()); };
			void				push(const value_type& val) { c.push_back(val); };
			void				pop(void) { c.pop_back(); };

			template <class T_, class Container_>
			friend bool operator== (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
			template <class T_, class Container_>
			friend bool operator!= (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
			template <class T_, class Container_>
			friend bool operator<  (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
			template <class T_, class Container_>
			friend bool operator<= (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
			template <class T_, class Container_>
			friend bool operator>  (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
			template <class T_, class Container_>
			friend bool operator>= (const stack<T_,Container_>& lhs, const stack<T_,Container_>& rhs);
	};
	template <class T, class Container>
	bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c == rhs.c); };
	template <class T, class Container>
	bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c != rhs.c); };
	template <class T, class Container>
	bool operator<  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c <  rhs.c); };
	template <class T, class Container>
	bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c <= rhs.c); };
	template <class T, class Container>
	bool operator>  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c >  rhs.c); };
	template <class T, class Container>
	bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) { return (lhs.c >= rhs.c); };
}

#endif