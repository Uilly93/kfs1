
#include "kernel.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <stddef.h>

static int	ft_putchar(char c)
{
	screen_putchar(c);
	return 1;
}

static int	ft_putstr(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
	return (i);
}

static int	ft_putnbr(long int nb)
{
	size_t		len;

	len = 0;
	if (nb < 0)
	{
		len += ft_putchar('-');
		nb *= -1;
	}
	if (nb >= 10)
	{
		len += ft_putnbr(nb / 10);
		len += ft_putnbr(nb % 10);
	}
	else
		len += ft_putchar(nb + 48);
	return (len);
}

static int	ft_putnbr_base_long(unsigned long nb, char *base)
{
	long long		i;
	char			result [100];
	int				len;

	if (nb == 0)
	{
		ft_putchar(base[0]);
		return (1);
	}
	i = 0;
	while (nb > 0)
	{
		result[i] = base[nb % strlen(base)];
		nb /= strlen(base);
		i++;
	}
	len = i;
	while (i > 0)
		ft_putchar(result[i-- - 1]);
	return (len);
}

static int	ft_putnbr_base(unsigned int nb, char *base)
{
	long long		i;
	char			result [100];
	int				len;

	if (nb == 0)
	{
		ft_putchar(base[0]);
		return (1);
	}
	i = 0;
	// if (nb < 0)
	// {
	// 	ft_putchar('-', buffer);
	// 	nb *= -1;
	// }
	while (nb > 0)
	{
		result[i] = base[nb % strlen(base)];
		nb /= strlen(base);
		i++;
	}
	len = i;
	while (i > 0)
		ft_putchar(result[i-- - 1]);
	return (len);
}

static int	ft_is_c(char c)
{
	return (ft_putchar(c));
}

static int	ft_is_di(int nb)
{
	return (ft_putnbr(nb));
}

static int	ft_is_p(void *ptr)
{
	if (!ptr)
		return ft_putstr("(nil)");
	ft_putstr("0x");
	return (ft_putnbr_base_long((unsigned long) ptr, "0123456789abcdef") + 2);
}

static int	ft_is_s(const char *s)
{
	if (!s)
	{
		ft_putstr("(null)");
		return (6);
	}
	else
		ft_putstr((char *)s);
	return (strlen((char *)s));
}

static int	ft_is_u(unsigned int nb)
{
	return (ft_putnbr_base(nb, "0123456789"));
}

static int	ft_is_xl(int hexa)
{
	return (ft_putnbr_base(hexa, "0123456789abcdef"));
}

static int	ft_is_xu(int hexa)
{
	return (ft_putnbr_base(hexa, "0123456789ABCDEF"));
}



static int	ft_format(const char *fmt, va_list *args)
{
	int	len;

	len = 0;
	if (*fmt == 's')
		len += ft_is_s(va_arg(*args, char *));
	else if (*fmt == 'c')
		len += ft_is_c((char)va_arg(*args, int));
	else if (*fmt == 'd' || *fmt == 'i')
		len += ft_is_di(va_arg(*args, int));
	else if (*fmt == 'p')
		len += ft_is_p(va_arg(*args, void *));
	else if (*fmt == 'u')
		len += ft_is_u(va_arg(*args, unsigned int));
	else if (*fmt == 'x')
		len += ft_is_xl(va_arg(*args, unsigned long));
	else if (*fmt == 'X')
		len += ft_is_xu(va_arg(*args, unsigned long));
	else if (*fmt == '%')
		len += ft_putchar('%');
	return (len);
}


int ft_printf(const char *s, ...)
{
	int	i;
	int	len;

	va_list(args);
	i = 0;
	len = 0;
	va_start(args, s);
	if (!s)
		return (-1);
	while (s[i] != '\0')
	{
		if (s[i] == '%')
		{
			i++;
			len += ft_format(s + i++, &args);
		}
		else
		{
			screen_putchar(s[i]);
			i++;
			len++;
		}
	}
	va_end(args);
	//ft_memmove(terminal_buff, buffer, strlen((char *)buffer));
	return (len);
}
