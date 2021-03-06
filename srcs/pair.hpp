/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 03:44:30 by kmazier           #+#    #+#             */
/*   Updated: 2021/12/01 11:29:25 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PAIR_HPP
# define FT_PAIR_HPP

#include "type_traits.hpp"
#include <memory>

namespace ft
{
	template<typename T1, typename T2>
	struct pair
	{
		typedef T1 first_type;
		typedef T2 second_type;

		T1 first;
		T2 second;

		pair() : first(), second() {}
		
		pair(const T1& x, const T2& y) : first(x), second(y) {}

		template<class U1, class U2>
		pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

		pair(const pair& p) : first(p.first), second(p.second) {}
		
		pair&	operator=(const pair& other)
		{
			if (*this != other)
			{
				first = other.first;
				second = other.second;
			}
			return (*this);
		}
	};

	template<typename T1, typename T2>
	inline ft::pair<T1, T2>	make_pair(T1 t1, T2 t2)
	{
		return (ft::pair<T1, T2>(t1, t2));
	}

	template<class T1, class T2>
	bool operator==(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (x.first == y.first && x.second == y.second);
	}
	
	template<class T1, class T2>
	bool operator!=(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (!(x == y));
	}

	template<class T1, class T2>
	bool operator<(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (x.first < y.first || (!(x.first < y.first) && x.second < y.second));
	}

	template<class T1, class T2>
	bool operator>(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (y < x);
	}

	template<class T1, class T2>
	bool operator>=(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (!(x < y));
	}

	template<class T1, class T2>
	bool	operator<=(const ft::pair<T1,T2>& x, const ft::pair<T1,T2>& y)
	{
		return (!(y < x));
	}
}

#endif
