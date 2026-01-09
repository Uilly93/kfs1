
#include "../kernel.h"

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
