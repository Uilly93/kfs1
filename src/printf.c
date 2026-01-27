
#include "kernel.h"

static int	ft_putchar(char c, int16_t *idx)
{
	screen_putchar(c, idx);
	return 1;
}

static int	ft_putstr(char *s, int16_t *idx)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		ft_putchar(s[i], idx);
		i++;
	}
	return (i);
}

static int	ft_putnbr(long int nb, int16_t *idx)
{
	size_t		len;

	len = 0;
	if (nb < 0)
	{
		len += ft_putchar('-', idx);
		nb *= -1;
	}
	if (nb >= 10)
	{
		len += ft_putnbr(nb / 10, idx);
		len += ft_putnbr(nb % 10, idx);
	}
	else
		len += ft_putchar(nb + 48, idx);
	return (len);
}

static int	ft_putnbr_base_long(unsigned long nb, char *base, int16_t *idx)
{
	long long		i;
	char			result [100];
	int				len;

	if (nb == 0)
	{
		ft_putchar(base[0], idx);
		return (1);
	}
	i = 0;
	while (nb > 0)
	{
		result[i] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
		i++;
	}
	len = i;
	while (i > 0)
		ft_putchar(result[i-- - 1], idx);
	return (len);
}

static int	ft_putnbr_base(unsigned int nb, char *base, int16_t *idx)
{
	long long		i;
	char			result [100];
	int				len;

	if (nb == 0)
	{
		ft_putchar(base[0], idx);
		return (1);
	}
	i = 0;
	while (nb > 0)
	{
		result[i] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
		i++;
	}
	len = i;
	while (i > 0)
		ft_putchar(result[i-- - 1], idx);
	return (len);
}

static int	ft_is_c(char c, int16_t *idx)
{
	return (ft_putchar(c, idx));
}

static int	ft_is_di(int nb, int16_t *idx)
{
	return (ft_putnbr(nb, idx));
}

static int	ft_is_p(void *ptr, int16_t *idx)
{
	if (!ptr)
		return ft_putstr("(nil)", idx);
	ft_putstr("0x", idx);
	return (ft_putnbr_base_long((unsigned long) ptr, "0123456789abcdef", idx) + 2);
}

static int	ft_is_s(const char *s, int16_t *idx)
{
	if (!s)
	{
		ft_putstr("(null)", idx);
		return (6);
	}
	else
		ft_putstr((char *)s, idx);
	return (ft_strlen((char *)s));
}

static int	ft_is_u(unsigned int nb, int16_t *idx)
{
	return (ft_putnbr_base(nb, "0123456789", idx));
}

static int	ft_is_xl(int hexa, int16_t *idx)
{
	return (ft_putnbr_base(hexa, "0123456789abcdef", idx));
}

static int	ft_is_xu(int hexa, int16_t *idx)
{
	return (ft_putnbr_base(hexa, "0123456789ABCDEF", idx));
}



static int	ft_format(const char *fmt, va_list *args, int16_t *idx)
{
	int	len;

	len = 0;
	if (*fmt == 's')
		len += ft_is_s(va_arg(*args, char *), idx);
	else if (*fmt == 'c')
		len += ft_is_c((char)va_arg(*args, int), idx);
	else if (*fmt == 'd' || *fmt == 'i')
		len += ft_is_di(va_arg(*args, int), idx);
	else if (*fmt == 'p')
		len += ft_is_p(va_arg(*args, void *), idx);
	else if (*fmt == 'u')
		len += ft_is_u(va_arg(*args, unsigned int), idx);
	else if (*fmt == 'x')
		len += ft_is_xl(va_arg(*args, unsigned long), idx);
	else if (*fmt == 'X')
		len += ft_is_xu(va_arg(*args, unsigned long), idx);
	else if (*fmt == '%')
		len += ft_putchar('%', idx);
	return (len);
}


int ft_printf(int16_t idx, const char *s, ...)
{
	int	i;
	int	len;
	int16_t index = 0;

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
			if(idx == -1)
				len += ft_format(s + i++, &args, &idx);
			else
				len += ft_format(s + i++, &args, &idx);
		}
		else
		{
			if(idx == -1)
				screen_putchar(s[i], &idx);
			else
				screen_putchar(s[i], &idx);
			index++;
			i++;
			len++;
		}
	}
	va_end(args);
	return (len);
}
