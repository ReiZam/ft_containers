/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 05:47:49 by kmazier           #+#    #+#             */
/*   Updated: 2021/11/24 05:40:55 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

#include <uchar.h>

namespace ft
{
	template<bool B, class T = void>
	struct enable_if {};
	
	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template<typename T, T v>
    struct integral_constant
    {
      	static const T                  		value = v;
      	typedef T                           	value_type;
      	typedef integral_constant<T, v>			type;
    
		operator value_type() const
	  	{
		  return value;
		}
	};

 	template<typename T, T v>
    const T integral_constant<T, v>::value;

	typedef integral_constant<bool, true>	true_type;
	
  	typedef integral_constant<bool, false>	false_type;
	
	template<typename>
    struct remove_cv;

	template<typename>
	struct __is_integral_helper : public false_type {};

	template<>
	struct __is_integral_helper<bool> : public true_type {};

	template<>
	struct __is_integral_helper<char> : public true_type {};

	template<>
	struct __is_integral_helper<char16_t> : public true_type {};

	template<>
	struct __is_integral_helper<char32_t> : public true_type {};

	template<>
	struct __is_integral_helper<wchar_t> : public true_type {};

	template<>
	struct __is_integral_helper<short> : public true_type {};

	template<>
	struct __is_integral_helper<int> : public true_type {};

	template<>
	struct __is_integral_helper<long> : public true_type {};

	template<>
	struct __is_integral_helper<long long> : public true_type {};

	template<typename T>
    struct is_integral : public __is_integral_helper<typename remove_cv<T>::type>::type {};

	template<typename T>
    struct remove_const { typedef T     type; };

  	template<typename T>
    struct remove_const<T const> { typedef T     type; };
	
  	template<typename T>
    struct remove_volatile { typedef T     type; };

  	template<typename T>
    struct remove_volatile<T volatile> { typedef T     type; };

	template<typename T>
    struct remove_cv
    {
      	typedef typename remove_const<typename remove_volatile<T>::type>::type	type;
    };
	
	template<class T, class U>
	struct are_same
	{
		enum { __value = 0 };
		typedef false_type type;
	};
	
	template<class T>
	struct are_same<T, T> : true_type
	{
		enum { __value = 1 };
		 typedef true_type type;
	};
};

#endif