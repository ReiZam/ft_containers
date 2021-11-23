/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 04:06:24 by kmazier           #+#    #+#             */
/*   Updated: 2021/11/23 05:07:37 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGO_HPP
# define ALGO_HPP

namespace ft
{
	template<class InputIt1, class InputIt2>
	bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)
				return (false);
			++first2;
			++first1;
		}
		return (true);
	}

	template<class InputIt1, class InputIt2, class BinaryPredicate>
	bool 	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p )
	{
		while (first1 != last1)
		{
			if (!bool(p(*first1, *first2)))
				return (false);
			++first2;
			++first1;
		}
		return (true);
	}

	template<class InputIt1, class InputIt2>
	bool	lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		size_t it1_size = last1 - first1;
		size_t it2_size = last2 - first2;

		if (it1_size == 0 && it2_size == 0)
			return (false);
		if (it1_size == 0)
			return (true);
		if (it2_size == 0)
			return (false);
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 != *first2)
				return (*first1 < *first2);
			++first2;
			++first1;
		}
		return (it1_size < it2_size);
	}

	template<class InputIt1, class InputIt2, class Compare>
	bool	lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp)
	{
		size_t it1_size = last1 - first1;
		size_t it2_size = last2 - first2;

		if (it1_size == 0 && it2_size == 0)
			return (false);
		if (it1_size == 0)
			return (true);
		if (it2_size == 0)
			return (false);
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 != *first2)
				return (comp(*first1, *first2));
			++first2;
			++first1;
		}
		return (it1_size < it2_size);
	}
};


// add lexicographical compare, equal

#endif