/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RBT.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 15:26:02 by pnuti             #+#    #+#             */
/*   Updated: 2022/12/14 16:41:37 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBT_HPP
#define RBT_HPP

#define C_RED "\033[31m"
#define C_BLACK "\033[30;1m"
#define C_RESET "\033[0m"

/*  
    1. Every node has a color either red or black.
    2. The root of the tree is always black.
    3. There are no two adjacent red nodes (A red node cannot have a red parent or red child).
    4. Every path from a node (including root) to any of its descendants NULL nodes has the same number of black nodes.
    5. All leaf nodes are black nodes.
*/

#include <iostream>
#include "pair.hpp"

namespace ft
{
	enum
	{
		RED,
		BLACK,
		DOUBLE_BLACK
	};

	template <typename T>
		struct node
		{
			typedef T type;
			T		data;
			int		color;
			bool	isEnd;
			node	*parent;
			node	*lChild;
			node	*rChild;
		};

	template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<pair<Key,T> > >
	class RBT
	{
		public:
			typedef pair<Key, T>									value_type;
			typedef Alloc											allocator_type;
			typedef Compare											key_compare;
			typedef typename allocator_type::reference				reference;
			typedef typename allocator_type::const_reference		const_reference;
			typedef typename allocator_type::pointer				pointer;
			typedef typename allocator_type::const_pointer			const_pointer;
			typedef node<value_type>*								node_pointer;

		private:
			node_pointer											_root;
			node_pointer											_end;
			allocator_type											_data_alloc;
			std::allocator<node<value_type> >						_node_alloc;
			key_compare												_comp;


		public:
			RBT() : _root(NULL),
					_data_alloc(allocator_type()),
					_node_alloc(std::allocator<node<value_type> >()),
					_comp(key_compare())
			{
				_end = _node_alloc.allocate(1);
				initEnd();
			};
			RBT(key_compare const comp) :	_root(NULL),
											_data_alloc(allocator_type()),
											_node_alloc(std::allocator<node<value_type> >()),
											_comp(comp)
			{
				_end = _node_alloc.allocate(1);
				initEnd();
			};
			RBT(RBT const &other) : _root(NULL),
									_data_alloc(other._data_alloc),
									_node_alloc(other._node_alloc),
									_comp(other._comp)
			{
				_end = _node_alloc.allocate(1);
				initEnd();
				*this = other;
			};
			~RBT()
			{
				if (_root)
					_end->parent->rChild = NULL;
				_node_alloc.deallocate(_end, 1);
				purge(_root);
			};

			RBT& operator=(RBT const &other)
			{
				if (_root)
				{
					purge(_root);
					_root = NULL;
					initEnd();
				}
				node_pointer other_ptr = minValueNode(other.getRoot());
				while (other_ptr && !other_ptr->isEnd)
				{
					insert(other_ptr->data);
					other_ptr = forward(other_ptr, _comp);
				}
				return (*this);
			};

			node_pointer getRoot(void) const { return (_root); };
			static node_pointer getRoot(node_pointer a_node)
			{
				while (a_node->parent)
					a_node = a_node->parent;
				return (a_node); 
			};

			node_pointer getEnd(void) const { return (_end); };

			size_t max_size(void) const { return (_node_alloc.max_size()); };

			void initEnd(void)
			{
					_node_alloc.construct(_end, node<value_type>());
					_end->parent = NULL;
					_end->lChild = NULL;
					_end->rChild = NULL;
					_end->isEnd = true;
					_end->color = BLACK;
					_data_alloc.construct(&_end->data, value_type());
			};

			node_pointer search(value_type const element) const
			{
				node_pointer tmp(_root);
				while (tmp && !tmp->isEnd)
				{
					if (_comp(element.first, tmp->data.first))
						tmp = tmp->lChild;
					else if (_comp(tmp->data.first, element.first))
						tmp = tmp->rChild;
					else
						return (tmp);
				}
				return (_end);
			};

			pair<node_pointer, bool> insert(value_type element)
			{
				node_pointer tmp(_root);
				while (tmp)
				{
					if (_comp(element.first, tmp->data.first))
					{
						if (tmp->lChild)
							tmp = tmp->lChild;
						else
						{
							tmp->lChild = _node_alloc.allocate(1);
							_node_alloc.construct(tmp->lChild, node<value_type>());
							tmp->lChild->parent = tmp;
							tmp = tmp->lChild;
							tmp->rChild = NULL;
							break;
						}
					}
					else if (_comp(tmp->data.first, element.first))
					{
						if (tmp->rChild && !tmp->rChild->isEnd)
							tmp = tmp->rChild;
						else
						{
							tmp->rChild = _node_alloc.allocate(1);
							_node_alloc.construct(tmp->rChild, node<value_type>());
							tmp->rChild->parent = tmp;
							tmp = tmp->rChild;
							if (tmp->parent == _end->parent)
							{
								tmp->rChild = _end;
								_end->parent = tmp;
							}
							else
								tmp->rChild = NULL;
							break;
						}
					}
					else
						return (ft::make_pair(tmp, false));
				}
				if (!_root)
				{
					tmp = _node_alloc.allocate(1);
					_node_alloc.construct(tmp, node<value_type>());
					tmp->parent = NULL;
					_end->parent = tmp;
					_root = tmp;
					tmp->rChild = _end;
				}
				_data_alloc.construct(&(tmp->data), element);
				tmp->lChild = NULL;
				tmp->isEnd = false;
				tmp->color = RED;
				recoloringInsert(tmp);
				return (ft::make_pair(tmp, true));
			};

/*
	Red Black Tree deletion
	
	Principles:
	1. Color
		1.a Deleting a Red Node does not require further adjustments
		1.b Deleting a Black Node alters the balance of the RBT
	2. Position
		2.a If the node is external it can be deleted
		2.b If the node is internal, it has to be substituted with its Inorder Predecessor or Inorder Successor, then it can be deleted
*/

			void deleteNode(value_type element)
			{
				node_pointer to_delete = search(element);
				if (to_delete == _end)
					return ;
				else if (!to_delete->lChild || (!to_delete->rChild || to_delete->rChild->isEnd))
					delete_case1(to_delete);
				else
					delete_case2(to_delete);
			};

			void delete_case1(node_pointer to_delete)
			{
				if (to_delete == _root && !to_delete->lChild && to_delete->rChild->isEnd)
					_end->parent = NULL;
				if (to_delete->color == RED)
				{
					if (to_delete->lChild)
						substitute(to_delete, to_delete->lChild);
					else
						substitute(to_delete, to_delete->rChild);
				}
				else
				{
					if (to_delete->lChild)
					{
						swapValues(to_delete, to_delete->lChild);
						to_delete = to_delete->lChild;
						to_delete->parent->lChild = NULL;
					}
					else if (to_delete->rChild && !to_delete->rChild->isEnd)
					{
						swapValues(to_delete, to_delete->rChild);
						to_delete = to_delete->rChild;
						to_delete->parent->rChild = NULL;
					}
					if (to_delete->color == BLACK)
					{
						to_delete->color = DOUBLE_BLACK;
						solveDoubleBlack(to_delete);
						if (to_delete == _root)
							_root = NULL;
						else
							substitute(to_delete, to_delete->rChild);
					}
				}
				_node_alloc.deallocate(to_delete, 1);
			};

			void delete_case2(node_pointer to_delete)
			{
				node_pointer tmp = maxValueNode(to_delete->lChild);
				swapValues(to_delete, tmp);
				delete_case1(tmp);
			};

			void solveDoubleBlack(node_pointer double_black)
			{
				node_pointer s = sibling(double_black);
				node_pointer p = double_black->parent;
				if (!p)
				{
					double_black->color = BLACK;
					return ;
				}
				else if (!s)
				{
					if (p->color == RED)
					{
						double_black->color = BLACK;
						p->color = BLACK;
					}
					else
					{
						double_black->color = BLACK;
						p->color = DOUBLE_BLACK;
						solveDoubleBlack(p);
					}
				}
				else if (s->color == BLACK)
				{
					if ((!s->lChild || s->lChild->color == BLACK) && (!s->rChild || s->rChild->color == BLACK))
					{
						double_black->color = BLACK;
						s->color = RED;
						if (p->color == BLACK)
						{
							p->color = DOUBLE_BLACK;
							solveDoubleBlack(p);
						}
						else
							p->color = BLACK;
					}
					else if (nearNephew(double_black) && nearNephew(double_black)->color == RED)
					{
						nearNephew(double_black)->color = BLACK;
						s->color = RED;
						if (isRightChild(s))
							right_rotate(s);
						else
							left_rotate(s);
						solveDoubleBlack(double_black);
					}
					else
					{
						farNephew(double_black)->color = BLACK;
						if (isRightChild(s))
						{
							swapColor(s, p);
							left_rotate(p);
						}
						else
						{
							swapColor(s, p);
							right_rotate(p);
						}
						double_black->color = BLACK;

					}
				}
				else if (s->color == RED)
				{
					s->color = BLACK;
					p->color = RED;
					if (isLeftChild(double_black))
						left_rotate(p);
					else
						right_rotate(p);
					solveDoubleBlack(double_black);
				}
			}

			void recoloringInsert(node_pointer new_node)
			{
				if (new_node == _root)
					invertColor(new_node);
				else if (new_node->parent->color == RED)
				{
					if (sibling(new_node->parent) && !sibling(new_node->parent)->isEnd && sibling(new_node->parent)->color == RED)
					{
						invertColor(new_node->parent);
						invertColor(sibling(new_node->parent));
						new_node->parent->parent->color = RED;
						recoloringInsert(new_node->parent->parent);
					}
					else
						dispatchCase(new_node);
				}
			};

			void dispatchCase(node_pointer new_node)
			{
				if (isLeftChild(new_node) && isLeftChild(new_node->parent))
					llcase(new_node);
				else if (isRightChild(new_node) && isLeftChild(new_node->parent))
					lrcase(new_node);
				else if (isRightChild(new_node) && isRightChild(new_node->parent))
					rrcase(new_node);
				else if (isLeftChild(new_node) && isRightChild(new_node->parent))
					rlcase(new_node);
				else
					std::cerr << "Red Black Tree failed insertion of new node" << std::endl;
			};

			void invertColor(node_pointer to_change)
			{
				if (to_change->color == RED)
					to_change->color = BLACK;
				else
					to_change->color = RED;
			};

			node_pointer sibling(node_pointer a_node)
			{
				if (a_node->parent)
				{
					if (isLeftChild(a_node))
						return (a_node->parent->rChild);
					else
						return (a_node->parent->lChild);
				}
				return (NULL);
			};

			node_pointer nearNephew(node_pointer a_node)
			{
				if (a_node->parent)
				{
					if (isLeftChild(a_node) && a_node->parent->rChild)
						return (a_node->parent->rChild->lChild);
					else if (isRightChild(a_node) && a_node->parent->lChild)
						return (a_node->parent->lChild->rChild);
				}
				return (NULL);
			};

			node_pointer farNephew(node_pointer a_node)
			{
				if (a_node->parent)
				{
					if (isLeftChild(a_node) && a_node->parent->rChild)
						return (a_node->parent->rChild->rChild);
					else if (isRightChild(a_node) && a_node->parent->lChild)
						return (a_node->parent->lChild->lChild);
				}
				return (NULL);
			};

			bool isLeftChild(node_pointer a_node)
			{
				if (a_node->parent)
				{
					if (a_node->parent->lChild == a_node)
						return true;
					else
						return false;
				}
				return false;
			};

			bool isRightChild(node_pointer a_node)
			{
				if (a_node->parent)
				{
					if (a_node->parent->rChild == a_node)
						return true;
					else
						return false;
				}
				return false;
			};

			void llcase(node_pointer new_node)
			{
				right_rotate(new_node->parent->parent);
				invertColor(new_node->parent);
				invertColor(sibling(new_node)); //former grandparent of new_node
			};

			void lrcase(node_pointer new_node)
			{
				left_rotate(new_node->parent);
				llcase(new_node->lChild); //former parent of new_node
			};

			void rrcase(node_pointer new_node)
			{
				left_rotate(new_node->parent->parent);
				invertColor(new_node->parent);
				invertColor(sibling(new_node)); //former grandparent of new_node
			};

			void rlcase(node_pointer new_node)
			{
				right_rotate(new_node->parent);
				rrcase(new_node->rChild); //former parent of new_node
			};

			void right_rotate(node_pointer a_node)
			{
				node_pointer tmp = a_node->lChild;
				a_node->lChild = tmp->rChild;
				if (a_node->lChild)
					a_node->lChild->parent = a_node;
				if (!a_node->parent)
					_root = tmp;
				else
				{
					if (isLeftChild(a_node))
						a_node->parent->lChild = tmp;
					else if (isRightChild(a_node))
						a_node->parent->rChild = tmp;
				}
				tmp->rChild = a_node;
				tmp->parent = a_node->parent;
				a_node->parent = tmp;
			};

			void left_rotate(node_pointer a_node)
			{
				node_pointer tmp = a_node->rChild;
				a_node->rChild = tmp->lChild;
				if (a_node->rChild)
					a_node->rChild->parent = a_node;
				if (!a_node->parent)
					_root = tmp;
				else
				{
					if (isLeftChild(a_node))
						a_node->parent->lChild = tmp;
					else if (isRightChild(a_node))
						a_node->parent->rChild = tmp;
				}
				tmp->lChild = a_node;
				tmp->parent = a_node->parent;
				a_node->parent = tmp;
			};

			void substitute(node_pointer old_node, node_pointer new_node)
			{
				if (old_node == _root)
				{
					_root = new_node;
					new_node->parent = NULL;
					new_node->color = BLACK;
				}
				else if (isLeftChild(old_node))
					old_node->parent->lChild = new_node;
				else
					old_node->parent->rChild = new_node;
				if (new_node && new_node != _root)
				{
					new_node->parent = old_node->parent;
					new_node->color = BLACK;
				}
				if (old_node->rChild && old_node->rChild->isEnd && new_node)
				{
					old_node->rChild = NULL;
					new_node->rChild = _end;
				}
			};

			void swapValues(node_pointer n1, node_pointer n2)
			{
				value_type tmp(n1->data);

				_data_alloc.destroy(&(n1->data));
				_data_alloc.construct(&(n1->data), n2->data);
				_data_alloc.destroy(&(n2->data));
				_data_alloc.construct(&(n2->data), tmp);
			};

			void purge(node_pointer root)
			{
				if (root && !root->isEnd)
				{
					purge(root->lChild);
					purge(root->rChild);
					_node_alloc.deallocate(root, 1);
				}
			};

			static node_pointer maxValueNode(node_pointer start)
			{
				node_pointer tmp = start;
				if (!start)
					return (NULL);

				while (tmp->rChild && !tmp->rChild->isEnd)
					tmp = tmp->rChild;
				return (tmp);
			};

			static node_pointer minValueNode(node_pointer start)
			{
				node_pointer tmp = start;
				if (!start)
					return (NULL);

				while (tmp->lChild)
					tmp = tmp->lChild;
				return (tmp);
			};

			template<typename CompareType>
			static node_pointer forward(node_pointer a_node, CompareType comp)
			{
				if (!a_node)
					return (NULL);
				else if (a_node->isEnd)
					return (minValueNode(getRoot(a_node)));
				else if (a_node->rChild)
				{
					if (a_node->rChild->isEnd)
						return (a_node->rChild);
					else
						return (minValueNode(a_node->rChild));
				}
				value_type data = a_node->data;
				while (a_node && (comp(a_node->data.first, data.first) || (!comp(a_node->data.first, data.first) && !comp(data.first, a_node->data.first))))
					a_node = a_node->parent;
				return (a_node);
			};

			template<typename CompareType>
			static node_pointer back(node_pointer a_node, CompareType comp)
			{
				if (!a_node)
					return (NULL);
				else if (a_node == minValueNode(getRoot(a_node)))
					return (maxValueNode(getRoot(a_node))->rChild);
				else if (a_node->isEnd)
					return (a_node->parent);
				if (a_node->lChild)
					return (maxValueNode(a_node->lChild));
				value_type data = a_node->data;
				while (a_node && !comp(a_node->data.first, data.first))
					a_node = a_node->parent;
				return (a_node);
			};

			static size_t countChildren(node_pointer parent)
			{
				if (parent && !parent->isEnd)
					return (countChildren(parent->lChild) + countChildren(parent->rChild) + 1);
				return (0);
			};

			static node_pointer rootFromNode(node_pointer a_node)
			{
				if (!a_node)
					return (NULL);
				while (a_node->parent)
					a_node = a_node->parent;
				return (a_node);
			};

			void printBT(const std::string& prefix, const node_pointer node, bool isLeft)
			{
				if( node != NULL && !node->isEnd )
				{
					std::cout << prefix;

					std::cout << (isLeft ? "├──" : "└──" );

					// print the value of the node
					std::cout << (node->color == BLACK ? C_BLACK : C_RED) << node->data.first << C_RESET << std::endl;

					// enter the next tree level - left and right branch
					printBT( prefix + (isLeft ? "│   " : "    "), node->lChild, true);
					printBT( prefix + (isLeft ? "│   " : "    "), node->rChild, false);
				}
			};

			void printBT(const node_pointer node)
			{
				printBT("", node, false);
			};

			void repositionEnd(void)
			{
				node_pointer max_node = maxValueNode(_root);
				if (_root && _end->parent != max_node)
				{
					if (isLeftChild(_end))
						_end->parent->lChild = NULL;
					else
						_end->parent->rChild = NULL;
					_end->parent = max_node;
				}
			};

			void swapColor(node_pointer node1, node_pointer node2)
			{
				if (node1 && node2)
				{
					int tmp(node1->color);
					node1->color = node2->color;
					node2->color = tmp;
				}
				else
				{
					if (node1)
						node1->color = BLACK;
					else if (node2)
						node2->color = RED;
				}
			}

			void swap(RBT &other)
			{
				node_pointer						tmp_root(_root);
				node_pointer						tmp_end(_end);
				allocator_type						tmp_data_alloc(_data_alloc);
				std::allocator<node<value_type> >	tmp_node_alloc(_node_alloc);
				key_compare							tmp_comp(_comp);

				_root = other._root;
				_end = other._end;
				_data_alloc = other._data_alloc;
				_node_alloc = other._node_alloc;
				_comp = other._comp;

				other._root = tmp_root;
				other._end = tmp_end;
				other._data_alloc = tmp_data_alloc;
				other._node_alloc = tmp_node_alloc;
				other._comp = tmp_comp;
			}
	};
}

#endif