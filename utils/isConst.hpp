/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isConst.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnuti <pnuti@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:03:55 by pnuti             #+#    #+#             */
/*   Updated: 2022/11/22 18:11:17 by pnuti            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISCONST_HPP
#define ISCONST_HPP

#include <cstddef>

/* 	Template defined to avoid duplication of code when dealing with const
	Source: https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators */

template <bool flag, class IsTrue, class IsFalse>
	struct chooseConst;

template <class IsTrue, class IsFalse>
	struct chooseConst<true, IsTrue, IsFalse>
{
	typedef IsTrue type;
};

template <class IsTrue, class IsFalse>
	struct chooseConst<false, IsTrue, IsFalse>
{
	typedef IsFalse type;
};

#endif