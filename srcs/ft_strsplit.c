
#include <string.h>
#include <stdlib.h>

static int		fill_ret(char **ret, const char *s, int *tab, char c)
{
	int		j;

	j = 0;
	while (s[tab[0]] != '\0' && s[tab[0]] != c)
	{
		ret[tab[1]][j] = s[tab[0]];
		tab[0] += 1;
		j++;
	}
	ret[tab[1]][j] = '\0';
	return (tab[0]);
}

static int		ft_strleni(const char *str, char target, int k)
{
	int		i;

	i = 0;
	while (str[k] != target && str[k] != '\0')
	{
		k++;
		i++;
	}
	return (i);
}

static int		ft_sub_str_nbr(const char *str, char c)
{
	int		i;
	int		nbr;

	i = 0;
	nbr = 0;
	if (strlen(str) == 0)
		return (0);
	while (str[i])
	{
		while (str[i] == c && str[i])
			i++;
		if (str[i] != c && str[i])
		{
			nbr++;
			while (str[i] != c && str[i])
				i++;
		}
	}
	return (nbr);
}

char			**ft_strsplit(char const *s, char c)
{
	char		**ret;
	int		nbr;
	int		tab[2];

	tab[0] = 0;
	tab[1] = 0;
	if (!s || (nbr = ft_sub_str_nbr(s, c)) < 1)
		return (NULL);
	ret = (char**)malloc(sizeof(char**) * (nbr + 1));
	if (!ret)
		return (NULL);
	while (s[tab[0]] != '\0')
	{
		if (s[tab[0]] != c)
		{
			ret[tab[1]] = (char*)malloc((ft_strleni(s, c, tab[0]) + 1));
			tab[0] = fill_ret(ret, s, tab, c);
			tab[1] += 1;
		}
		else
			tab[0] += 1;
	}
	ret[tab[1]] = NULL;
	return (ret);
}
