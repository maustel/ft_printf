/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 10:40:07 by maustel           #+#    #+#             */
/*   Updated: 2023/11/08 17:45:36 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_s_c(va_list ap, char c)
{
	char	*s;
	int		i;

	i = 0;
	if (c == 'c')
	{
		c = va_arg(ap, int);
		return (write (1, &c, 1));
	}
	if (c == 's')
	{
		s = va_arg(ap, char *);
		if (!s)
			return (write(1, "(null)", 6));
		while (s[i])
		{
			if (write (1, &s[i], 1) < 0)
				return (-1);
			i++;
		}
	}
	return (i);
}

static int	print_nbr(size_t nbr, char *base, size_t base_len)
{
	char			str[20];
	int				r;
	int				i;

	if (nbr == 0)
		return (write(1, "0", 1));
	i = 0;
	while (nbr > 0)
	{
		r = nbr % base_len;
		str[i++] = base [r];
		nbr = nbr / base_len;
	}
	r = i;
	i--;
	while (i >= 0)
	{
		if (write (1, &str[i--], 1) < 0)
			return (-1);
	}
	return (r);
}

static int	print_int(int nbr, char *base, int base_len)
{
	int	r;

	r = 0;
	if (nbr == -2147483648)
		return (write(1, "-2147483648", 11));
	if (nbr < 0)
	{
		r = write(1, "-", 1);
		if (r < 0)
			return (-1);
		nbr = -nbr;
	}
	r = r + print_nbr(nbr, base, base_len);
	return (r);
}

static int	format(va_list ap, char c)
{
	int	r;

	if (c == 's' || c == 'c')
		r = print_s_c(ap, c);
	if (c == 'p')
	{
		r = write (1, "0x", 2);
		if (r < 0)
			return (-1);
		r = r + print_nbr(va_arg(ap, unsigned long), "0123456789abcdef", 16);
	}
	if (c == 'i' || c == 'd')
		r = print_int(va_arg(ap, int), "0123456789", 10);
	if (c == 'u')
		r = print_nbr(va_arg(ap, unsigned int), "0123456789", 10);
	if (c == 'x')
		r = print_nbr(va_arg(ap, unsigned int), "0123456789abcdef", 16);
	if (c == 'X')
		r = print_nbr(va_arg(ap, unsigned int), "0123456789ABCDEF", 16);
	if (c == '%')
		r = write(1, "%", 1);
	return (r);
}

int	ft_printf(const char *s, ...)
{
	int		i;
	int		n;
	int		pr;
	va_list	ap;

	va_start(ap, s);
	i = 0;
	n = 0;
	while (s[i])
	{
		if (s[i] != '%')
			if (write (1, &s[i], 1) < 0)
				return (-1);
		if (s[i] == '%')
		{
			i++;
			pr = format(ap, s[i]);
			if (pr < 0)
				return (-1);
			n = n + pr - 2;
		}
		i++;
	}
	va_end(ap);
	return (i + n);
}

// int main()
// {
// 	// char c = 'x';
// 	// char s[] = "hallo";
// 	// void *p = &c;
// 	// printf("%i", ft_printf("123456789%s1234%c\n", "hallo", 'z'));
// 	// printf("%i\n", ft_printf("%s\n"));
// // 	printf("%i\n", printf("original: %c, %s, %p, %i, %d, %u, %x,
// // %X, %%\n", c, s, p, 123, -123456789, 10, 12, 12));
// // 	printf("%i\n", ft_printf("meininal: %c, %s, %p, %i, %d, %u, %x,
// // %X, %%\n", c, s, p, 123, -123456789, 10, 12, 12));
// 	printf("%i\n", printf("0x%p-", (void *)ULONG_MAX));
// 	printf("%i\n", ft_printf("0x%p-", (void *)ULONG_MAX));
// 	leak ();
// 	return (0);
// }
