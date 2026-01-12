#include "../includes/kernel.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	char				*dest2;
	const unsigned char	*src2;

	if (!dest && !src)
		return ((void *)0);
	if (n != 0)
	{
		src2 = src;
		dest2 = dest;
		i = 0;
		while (i < n)
		{
			dest2[i] = src2[i];
			i++;
		}
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const char	*ptr;
	char		*ptr2;

	ptr = src;
	ptr2 = dest;
	if (!dest && !src)
		return ((void *)0);
	if (dest > src)
	{
		while (n > 0)
		{
			ptr2[n - 1] = ptr[n - 1];
			n--;
		}
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}

size_t ft_strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int isprintable(char c){
	return (c > 32 && c < 127);
}