
#include <unistd.h>

void    ft_putnbr_fd(int n, int fd)
{
  char  c;
  int   sign;

  sign = ((n < 0) ? -1 : 1);
  if (n < 0)
    write(fd, "-", 1);
  if (n / 10)
    ft_putnbr_fd((n / 10) * sign, fd);
  c = ((n % 10) * sign) +'0';
  write(fd, &c, 1);
}
