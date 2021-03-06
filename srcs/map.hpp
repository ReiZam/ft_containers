/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 03:37:48 by kmazier           #+#    #+#             */
/*   Updated: 2021/12/05 15:52:33 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MAP_HPP
# define FT_MAP_HPP

#include <memory>
#include "common.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "avltree.hpp"

namespace ft
{

	template<class Key, class T, class Compare = ft::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
		public:
			typedef Key																		key_type;
			typedef T																		mapped_type;
			typedef ft::pair<Key, T>														value_type;
			typedef size_t																	size_type;
			typedef ptrdiff_t																difference_type;
			typedef Compare																	key_compare;
			typedef ft::AVLTree<value_type, mapped_type, key_type, key_compare>				avl_tree;
			typedef typename avl_tree::node_pointer											node_pointer;
			typedef typename Allocator::template rebind<value_type>::other					allocator_type;
			typedef typename allocator_type::reference										reference;
			typedef typename allocator_type::const_reference								const_reference;
			typedef typename allocator_type::pointer										pointer;
			typedef typename allocator_type::const_pointer									const_pointer;
			typedef typename avl_tree::iterator		 										iterator;
			typedef typename avl_tree::const_iterator	 									const_iterator;
			typedef typename avl_tree::reverse_iterator	 									reverse_iterator;
			typedef typename avl_tree::const_reverse_iterator 								const_reverse_iterator;		
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
			map() : tree(), comp(), allocator() {}
			
			template<class InputIt>
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : tree(), comp(comp), allocator(alloc)
			{
				(void)comp;
				while (first != last)
				{
					this->insert(*first);
					++first;
				}
					
			}

			map(const map& other) : tree(), comp(), allocator()
			{
				if (*this != other)
					*this = other;
			}

			~map() {}

			allocator_type	get_allocator() const
			{
				return (this->allocator);
			}

			map&	operator=(const map& other)
			{
				if (*this != other)
					this->_copy(other.begin(), other.end());
				return (*this);
			}
			
			// ELEMENT ACCESS
			T&			at(const key_type& key)
			{
				node_pointer result = this->tree.find(key);

				if (this->size() == 0 || result == NULL)
					throw std::out_of_range("map::at");
				return (result->value.second);
			}

			const T&	at(const key_type& key) const
			{
				node_pointer result = this->tree.find(key);

				if (this->size() == 0 || result == NULL)
					throw std::out_of_range("map::at");
				return (result->value.second);
			}

			T&			operator[](const key_type& key)
			{
				node_pointer n = this->tree.find(key);

				if (n != NULL)
					return (n->value.second);
				return (this->insert(ft::make_pair(key, T())).first->second);
			}
			
			// CAPACITY
			size_type	size() const
			{
				return (this->tree.size());
			}

			bool		empty() const
			{
				return (this->size() == 0);
			}

			size_type	max_size() const
			{
				return (this->tree.allocator.max_size());
			}
			
			// ITERATORS
			iterator 				begin()
			{
				return iterator(this->tree.left_eot->parent ? this->tree.left_eot->parent : this->tree.right_eot);
			}

			const_iterator			begin() const
			{
				return const_iterator(this->tree.left_eot->parent ? this->tree.left_eot->parent : this->tree.right_eot);
			}
			
			iterator 				end()
			{
				return iterator(this->tree.right_eot->parent ? this->tree.right_eot : this->tree.right_eot);
			}

			const_iterator			end() const
			{
				return const_iterator(this->tree.right_eot->parent ? this->tree.right_eot : this->tree.right_eot);
			}
			
			reverse_iterator		rend()
			{
				return reverse_iterator(this->begin());
			}
			
			const_reverse_iterator 	rend() const
			{
				return const_reverse_iterator(this->begin());
			}

			reverse_iterator 		rbegin() 
			{
				return reverse_iterator(this->end());
			}

			const_reverse_iterator 	rbegin() const
			{
				return const_reverse_iterator(this->end());
			}

			// MODIFIERS
			void						clear()
			{
				this->tree.destroy();
			}
			
			ft::pair<iterator, bool>	insert(const value_type& value)
			{
				bool inserted = this->tree.insert(value) == 1 ? true : false;

				return (ft::make_pair<iterator, bool>(iterator(this->tree.find(value.first)), inserted));
			}

			iterator					insert(iterator hint, const value_type& value)
			{
				(void)hint;
				this->tree.insert(value);
				return (iterator(this->tree.find(value.first)));
			}

			template<class InputIt>
			void						insert(InputIt first, InputIt last)
			{
				while (first != last)
				{
					this->tree.insert(*first);
					++first;
				}
			}

			void						erase(iterator pos)
			{
				this->tree.remove((*pos).first);
			}

			void						erase(iterator first, iterator last)
			{
				this->tree.remove(first, last);
			}

			size_type					erase(const key_type& key)
			{
				return (this->tree.remove(key));	
			}

			void						swap(map &other)
			{
				ft::swap(&tree.root, &other.tree.root);
				ft::swap(&tree.left_eot, &other.tree.left_eot);
				ft::swap(&tree.right_eot, &other.tree.right_eot);
				ft::swap(&tree.nodes_count, &other.tree.nodes_count);
			}
			
			// LOOKUP
			size_type								count(const key_type& key) const
			{
				return (this->find(key) != this->end() ? 1 : 0);
			}

			iterator								find(const key_type& key)
			{
				node_pointer n = this->tree.find(key);

				if (n == NULL)
					return (this->end());
				return (iterator(n));
			}

			
			const_iterator							find(const key_type& key) const
			{
				node_pointer n = this->tree.find(key);
	
				if (n == NULL)
					return (this->end());
				return (const_iterator(n));
			}

			iterator								lower_bound(const Key& key)
			{
				iterator f = this->find(key);

				if (f != this->end())
					return (f);
				for (iterator it = this->begin();it != this->end();++it)
					if (!this->tree.compare(it->first, key))
						return (it);
				return (this->end());
			}	

			const_iterator							lower_bound(const Key& key) const
			{
				const_iterator f = this->find(key);

				if (f != this->end())
					return (f);
				for (const_iterator it = this->begin();it != this->end();++it)
					if (!this->tree.compare(it->first, key))
						return (it);
				return (this->end());
			}

			iterator								upper_bound(const Key& key)
			{
				for (iterator it = this->begin();it != this->end();++it)
					if (this->tree.compare(key, it->first))
						return (it);
				return (this->end());
			}

			const_iterator							upper_bound(const Key& key) const
			{
				for (const_iterator it = this->begin();it != this->end();++it)
					if (this->tree.compare(key, it->first))
						return (it);
				return (this->end());
			}

			ft::pair<iterator,iterator>				equal_range(const Key& key)
			{
				return (ft::make_pair<iterator, iterator>(this->lower_bound(key), this->upper_bound(key)));
			}

			ft::pair<const_iterator,const_iterator>	equal_range(const Key& key) const
			{
				return (ft::make_pair<const_iterator, const_iterator>(this->lower_bound(key), this->upper_bound(key)));
			}
			
			// OBSERVERS
			key_compare key_comp() const
			{
				return (this->comp);
			}

			map::value_compare value_comp() const
			{
				return (map::value_compare(this->comp));
			}
		public:
			avl_tree		tree;
		private:
			key_compare		comp;
			allocator_type	allocator;

			template<class InputIt>
			void			_copy(InputIt first, InputIt last)
			{
				this->tree.destroy();
				while (first != last)
				{
					this->insert(*first);
					++first;
				}
			}
	};

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator==(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (lhs.tree == rhs.tree);
	}

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator<(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (lhs.tree < rhs.tree);
	}

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator!=(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(lhs.tree == rhs.tree));
	}

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator>(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (rhs.tree < lhs.tree);
	}

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator<=(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(rhs.tree < lhs.tree));
	}

	template<class Key, class T, class Compare, class Allocator>
	inline bool	operator>=(const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(lhs.tree < rhs.tree));
	}

	template< class Key, class T, class Compare, class Allocator>
	void swap(ft::map<Key, T, Compare, Allocator>& lhs, ft::map<Key,T,Compare,Allocator>& rhs)
	{
		lhs.swap(rhs);
	}
};

#endif