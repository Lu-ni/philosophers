/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicolli <lnicolli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:27:31 by lnicolli          #+#    #+#             */
/*   Updated: 2024/05/24 16:33:05 by lnicolli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int		signe;
	int		number;
	char	*strcopy;

	strcopy = (char *)str;
	number = 0;
	signe = 1;
	while (*strcopy == ' ' | *strcopy == '\t' | *strcopy == '\n'
		| *strcopy == '\r' | *strcopy == '\v' | *strcopy == '\f')
		strcopy++;
	if (*strcopy == '-' || *strcopy == '+')
	{
		if (*strcopy == '-')
			signe *= -1;
		strcopy++;
	}
	while ('0' <= *strcopy && *strcopy <= '9')
	{
		number *= 10;
		number += *strcopy - '0';
		strcopy++;
	}
	return (number * signe);
}
