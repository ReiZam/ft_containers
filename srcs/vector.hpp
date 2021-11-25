/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 01:43:47 by kmazier           #+#    #+#             */
/*   Updated: 2021/11/25 07:35:36 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

#include <memory>
#include <exception>
#include <stdexcept>
#include "iterator.hpp"
#include "algorithm.hpp"
#include "type_traits.hpp"

namespace ft
{
	template<class T, class Allocator = std::allocator<T> >
	class	vector
	{
		public:
			typedef T													value_type;
			typedef typename Allocator::template rebind<T>::other		allocator_type;
			typedef size_t												size_type;
			typedef __gnu_cxx::ptrdiff_t								difference_type;
			typedef typename allocator_type::reference					reference;
			typedef typename allocator_type::const_reference			const_reference;
			typedef typename allocator_type::pointer					pointer;
			typedef typename allocator_type::const_pointer				const_pointer;
			typedef ft::normal_iterator<pointer, vector> 				iterator;
			typedef ft::normal_iterator<const_pointer, vector>			const_iterator;
			typedef ft::reverse_iterator<iterator>						reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
		public:
			// MEMBER FUNCTIONS
			vector(void) :  start(), finish(), end_of_storage(), allocator() {}

			explicit vector(const Allocator &_allocator) : start(), finish(), end_of_storage(), allocator(_allocator) {}
			
			explicit vector(size_type size, const T &value = T(), const Allocator &_allocator = Allocator()) : allocator(_allocator)
			{
				this->fill_initialize(size, value, true_type());
			}

			template<class InputIt>
			vector(InputIt first, InputIt last, const Allocator &_allocator = Allocator()) : allocator(_allocator)
			{
				typedef typename ft::is_integral<InputIt>::type integral;
				
				this->fill_initialize(first, last, integral());
			}

			vector(const vector& other)
			{
				if (*this != other)
					this->copy(other, false);
			}

			~vector(void)
			{
				this->delete_storage();
			}

			allocator_type	get_allocator_type(void) const
			{
				return (this->allocator);
			}

			vector	&		operator=(const vector &other)
			{
				if (*this == other)
					return (*this);
				this->copy(other, true);
				return (*this);
			}

			void			assign(size_type count, const_reference value) 
			{
				this->assign_range(count, value, true_type());
			}

			template< class InputIt >
			void			assign(InputIt first, InputIt last)
			{
				typedef typename ft::is_integral<InputIt>::type integral;
				
				this->assign_range(first, last, integral());
			}

			// ELEMENT ACCESS
			reference			operator[](size_type index)
			{
				return (*(this->start + index));
			}

			const_reference		operator[](size_type index) const
			{
				return (*(this->start + index));
			}
			
			reference			at(size_type index)
			{
				if (index >= this->size())
					throw std::out_of_range("vector: out of range");
				return ((*this)[index]);
			}

			const_reference		at(size_type index) const
			{
				if (index >= this->size())
					throw std::out_of_range("vector: out of range");
				return ((*this)[index]);
			}

			reference			front(void)
			{
				return (*(this->begin()));
			}

			const_reference		front(void) const
			{
				return (*(this->begin()));
			}

			reference			back(void)
			{
				return (*(this->end() - 1));
			}

			const_reference		back(void) const
			{
				return (*(this->end() - 1));
			}

			pointer					data(void)
			{
				return (this->empty() ? NULL : this->start);
			}

			const pointer			data(void) const
			{
				return (this->empty() ? NULL : this->start);
			}

			// ITERATORS
			iterator 			begin()
			{
				return iterator(this->start);
			}

			const_iterator		begin() const
			{
				return const_iterator(this->start);
			}
			
			iterator 			end()
			{
				return iterator(this->finish);
			}

			const_iterator		end() const
			{
				return const_iterator(this->finish);
			}
			
			reverse_iterator		rend()
			{
				return reverse_iterator(begin());
			}
			
			const_reverse_iterator 	rend() const
			{
				return const_reverse_iterator(begin());
			}

			reverse_iterator 	rbegin() 
			{
				return reverse_iterator(end());
			}

			const_reverse_iterator 	rbegin() const
			{
				return const_reverse_iterator(end());
			}

			// CAPACITY
			bool				empty(void) const
			{
				return (this->finish - this->start == 0);
			}

			size_type			size(void) const
			{	
				return (this->finish - this->start);
			}

			size_type			max_size(void) const
			{
				return (this->allocator.max_size());
			}

			void	reserve(size_type new_cap)
			{
				if (new_cap > this->max_size())
					throw std::length_error("vector::reserve");
				if (capacity() < new_cap)
					reallocate(new_cap);
			}

			size_type			capacity(void) const
			{
				return (size_type(this->end_of_storage - this->start));
			}

			// MODIFIERS
			void				clear(void)
			{
				if (this->size() == 0) return ;

				this->finish = this->start;
			}

			iterator			insert(iterator pos, const T &value)
			{
				size_type diff = pos - this->begin();
				
				this->insert_value(diff, value);
				return (this->begin() + diff);
			}

			void				insert(iterator pos, size_type amount, const_reference value)
			{
				size_type diff = pos - this->begin();

				if (amount == 0) return ;

				this->insert_range(diff, amount, value, true_type());
			}

			template<class InputIt>
			void				insert(iterator pos, InputIt first, InputIt last)
			{
				typedef typename ft::is_integral<InputIt>::type integral;
				size_type diff = pos - this->begin();

				this->insert_range(diff, first, last, integral());
			}

			iterator			erase(iterator pos)
			{
				size_type diff = pos - this->begin();

				this->erase_value(diff);
				return (this->begin() + diff);
			}

			iterator			erase(iterator first, iterator last)
			{
				return (this->erase_iterator_value(first, last));
			}

			void				push_back(const_reference value)
			{
				if (this->finish == this->end_of_storage)
					this->reallocate(this->new_size());
				this->allocator.construct(this->finish, value);
				++this->finish;
			}

			void				pop_back(void)
			{
				if (this->size() > 0)
					this->erase_value(this->size() - 1);
			}

			void			resize(size_type count, value_type value = value_type())
			{
				if (count < this->size())
					this->finish = this->start + count;
				else if (count > this->size())
					this->reallocate_and_set_finish(new_size(count), count, value);
			}

			void			swap(vector &other)
			{
				if (*this == other) return ;
				
				this->swap_vector(other);
			}
		private:
			pointer			start;
			pointer 		finish;
			pointer 		end_of_storage;
			allocator_type	allocator;

			void	create_storage(size_type size)
			{
				this->start = this->allocator.allocate(size);
				this->finish = this->start;
				this->end_of_storage = this->start + size;
			}

			void	delete_storage(void)
			{
				this->allocator.deallocate(this->start, this->capacity());
			}
			
			void	swap_pointer(pointer *a, pointer *b)
			{
				pointer tmp;

				tmp = *a;
				*a = *b;
				*b = tmp;
			}

			void	swap_vector(vector &other)
			{
				swap_pointer(&other.start, &this->start);
				swap_pointer(&other.finish, &this->finish);
				swap_pointer(&other.end_of_storage, &this->end_of_storage);
			}

			void	fill_initialize(size_type size, const value_type &value, true_type)
			{
				this->create_storage(size);
				for (size_type i = 0;i < size;i++)
				{
					this->allocator.construct(this->finish, value);
					this->finish++;
				}
			}

			template<class InputIt>
			void	fill_initialize(InputIt first, InputIt last, false_type)
			{
				this->create_storage(last - first);
				for (InputIt it = first;it != last;it++)
					this->allocator.construct(this->finish++, *it);
			}
			
			void	reallocate(size_type size)
			{
				size_type		current_size = this->size();
				pointer 		_start 		= this->allocator.allocate(size);
				pointer 		_finish		= _start;
				
				for (size_type i = 0;i < current_size;i++)
				{
					if (i < size)
					{
						*_finish = *(this->start + i);
						// this->allocator.construct(_finish, *(this->start + i));
						++_finish;
					}
				}
				this->delete_storage();
				this->start = _start;
				this->finish = _finish;
				this->end_of_storage = _start + size;
			}

			void	reallocate(size_type size, T value)
			{
				size_type	before_size = this->size();
				pointer 	_start = this->allocator.allocate(size);
				pointer 	_finish = _start;

				for (size_type i = 0;i < size;i++)
				{
					*_finish = i < before_size ? *(this->start + i) : value;
					++_finish;
				}
				this->delete_storage();
				this->start = _start;
				this->finish = _finish;
				this->end_of_storage = _start + size;
			}

			void	reallocate_and_set_finish(size_type size, size_type finish_index)
			{
				this->reallocate(size);
				this->finish = this->start + finish_index;
			}

			void	reallocate_and_set_finish(size_type size, size_type finish_index, T value)
			{
				this->reallocate(size, value);
				this->finish = this->start + finish_index;
			}

			void	copy(const vector &other, bool del)
			{
				pointer 	_start = this->allocator.allocate(other.size());
				pointer 	_finish = _start;

				for (size_type i = 0;i < other.size();i++)
				{
					this->allocator.construct(_start + i, other[i]);
					_finish++;
				}
				if (del)
					this->delete_storage();
				this->start = _start;
				this->finish = _finish;
				this->end_of_storage = _finish;
			}

			size_t	new_size()
			{
				size_t size = this->size();

				return (size == 0 ? 1 : size * 2);
			}

			size_t	new_size(size_t n)
			{
				size_t size = this->size();

				return (n < size * 2 ? size * 2 : n);
			}

			void	insert_value(size_type pos, const_reference value)
			{
				value_type	tmp;
				value_type	tmp2;

				if (this->finish == this->end_of_storage)
					this->reallocate(new_size());
				tmp = *(this->start + pos);
				*(this->start + pos) = value;
				for (size_type i = pos + 1;i < this->size() + 1;i++)
				{
					tmp2 = *(this->start + i);
					*(this->start + i) = tmp;
					tmp = tmp2;
				}
				++this->finish;
			}

			void	insert_range(size_type pos, size_type amount, const_reference value, true_type)
			{
				if (amount == 0) return ;
				
				if (amount > this->capacity() - this->size())
					this->reallocate_and_set_finish(new_size(amount + this->size()), this->size(), value);
				for (size_type i = 0;i < amount;i++)
					this->insert_value(pos, value);
			}

			template<class InputIt>
			void	insert_range(size_type pos, InputIt first, InputIt last, false_type)
			{
				size_type range_size = last - first;

				if (range_size == 0) return ;
				
				if (range_size > this->capacity() - this->size())
					this->reallocate_and_set_finish(new_size(range_size + this->size()), this->size());
				for (InputIt it = first;it != last;++it)
					this->insert_value(pos, *(first + (last - it - 1)));
			}

			void	assign_range(size_type count, const_reference value, true_type)
			{
				if (count == 0) return ;
				
				if (count > this->capacity())
					this->reallocate(count);
				this->finish = this->start;
				for (size_type i = 0;i < count;i++)
				{
					*(this->start + i) = value;
					this->finish++;
				}
			}

			template<class InputIt>
			void	assign_range(InputIt first, InputIt last, false_type)
			{
				size_type count = last - first;

				if (count == 0) return ;
				if (count > this->capacity())
					this->reallocate(count);
				this->finish = this->start;
				for (size_type i = 0;i < count;i++)
				{
					*(this->start + i) = *(first + i);
					++this->finish;
				}
			}

			void	erase_value(size_type pos)
			{
				size_type current_size = this->size();

				for (size_type i = pos;i < current_size - 1;i++)
					this->allocator.construct(this->start + i, *(this->start + i + 1));
				--this->finish;
			}

			iterator	erase_iterator_value(iterator first, iterator last)
			{
				size_type start = first - this->begin();
				size_type end = last - this->begin();
				
				if (first - last == 0)
					return (last);
				if (end - start == size())
					this->clear();
				else
					for (size_type i = start;i < end;i++)
						this->erase_value(start);
				return (this->begin() + start);
			}
	};
	
	template<class T, class Allocator>
	bool	operator==(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (a.size() == b.size() && ft::equal(a.begin(), a.end(), b.begin()));
	}

	template<class T, class Allocator>
	bool	operator!=(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (!(a == b));
	}

	template<class T, class Allocator>
	bool	operator<(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (ft::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()));
	}

	template<class T, class Allocator>
	bool	operator>(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (b < a);
	}

	template<class T, class Allocator>
	bool	operator<=(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (!(b < a));
	}

	template<class T, class Allocator>
	bool	operator>=(const ft::vector<T, Allocator>& a, const ft::vector<T, Allocator>& b)
	{
		return (!(a < b));
	}

	template< class T, class Alloc >
	void swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}


#endif