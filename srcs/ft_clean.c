#include <stdlib.h>

void	ft_ctabdel(char ***ctab)
{
  int	i;
  char	**tmp;

  if (!(*ctab))
    return ;
  tmp = *ctab;
  i = 0;
  while (tmp[i])
    {
      free(tmp[i]);
      i++;
    }
  free(*ctab);
  *ctab = NULL;
}

void	ft_strdel(char **as)
{
  if (*as)
    {
      free(*as);
      *as = NULL;
    }
}
