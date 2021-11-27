/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 03:37:48 by kmazier           #+#    #+#             */
/*   Updated: 2021/11/27 05:37:43 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MAP_HPP
# define FT_MAP_HPP

#include <memory>
#include "common.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "tree.hpp"

namespace ft
{

	template<class Key, class T, class Compare = ft::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T>>>
	class map
	{

		public:
			// TODO: replace iterators/size type with the type of the binary tree. 
			typedef Key																		key_type;
			typedef T																		mapped_type;
			typedef ft::pair<const Key, T>													value_type;
			typedef size_t																	size_type;
			typedef ptrdiff_t																difference_type;
			typedef Compare																	key_compare;
			typedef Allocator																allocator_type;
			typedef typename allocator_type::reference										reference;
			typedef typename allocator_type::const_reference								const_reference;
			typedef typename allocator_type::pointer										pointer;
			typedef typename allocator_type::const_pointer									const_pointer;
			typedef ft::normal_iterator<pointer, map>										iterator;
			typedef ft::normal_iterator<const_pointer, map>									const_iterator;
			typedef ft::reverse_iterator<iterator>											reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>									const_reverse_iterator;
		private:
			typedef typename Allocator::template rebind<ft::pair<const Key, T>>::other		allocator_type;
			typedef RBTree<Key, T, Compare, Allocator>				rb_tree;
		public:
			class value_compare : ft::binary_function<value_type, value_type, bool>
			{
				friend class map<Key, T, Compare, Allocator>;
				protected:
					Compare comp;

					value_compare(Compare _comp) : comp(_comp) {}
				public:
					bool operator()(const value_type& x, const value_type& y) const
					{
						return (comp(x.first, y.first));
					}
			};
		public:
			// MEMBERS FUNCTIONS
			map() : tree(), allocator() {}
			
			~map() {}

			allocator_type	get_allocator() const
			{
				return (this->allocator);
			}
		private:
			rb_tree			tree;
			allocator_type	allocator;
	};
};

#endif