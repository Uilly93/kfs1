#include "kernel.h"
#include <stddef.h>
#include <stdint.h>

size_t ft_strlen(const char *s) {
	size_t i = 0;
	while (i[s])
		i++;
	return i;
}

void *ft_memset(void *s, int c, size_t n) {
	char *s1 = s;
	size_t i = 0;
	while (i < n)
		s1[i++] = c;
	return s1;
}

void	*memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*dest1;
	const char	*src1;

	if (!dest && !src)
		return (dest);
	i = 0;
	dest1 = dest;
	src1 = src;
	while (i < n)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest1);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dest1;
	unsigned char	*src1;
	size_t			i;

	if (!dest && !src)
		return (dest);
	i = 0;
	dest1 = (unsigned char *)dest;
	src1 = (unsigned char *)src;
	if (dest1 > src1)
	{
		while (n-- > 0)
			dest1[n] = src1[n];
	}
	else
	{
		while (i < n)
		{
			dest1[i] = src1[i];
			i++;
		}
	}
	return (dest);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;
	size_t				i;

	ptr1 = s1;
	ptr2 = s2;
	i = 0;
	while (i < n)
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n - 1) && (s1[i] || s2[i])
		&& (unsigned char)s1[i] == (unsigned char)s2[i])
	{
		i++;
	}
	if ((unsigned char)s1[i] < (unsigned char)s2[i])
		return (-1);
	if ((unsigned char)s1[i] > (unsigned char)s2[i])
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int		i = 0;
	int		sign = 1;
	long	nb = 0;

	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr [i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + nptr[i] - '0';
		if(nb > 125 || nb < 1)
			return 42000;
		i++;
	}
	return (nb * sign);
}