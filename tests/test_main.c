#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char	*foo = "foo";
size_t	len = 0;

int	main(void)
{
	len = strlen(foo);
	void	*ptr = malloc(sizeof(*foo) * len);
	
	memcpy(ptr, foo, len);

	printf("%s\n", foo);
	write(STDOUT_FILENO, (char *)ptr, len);
	
	free(ptr);
	return 0;
}