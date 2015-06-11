
#ifndef DOM_EX_H
# define DOM_EX_H

#include "ares.h"

# define SYS_FAIL	-1
# define NOT_EXIST	1
# define TRY_AGAIN	2

# define TXT_ID		0
# define DKIM_ID	1
# define NS_ID		2
# define DMARC_ID	3
# define MX_ID		4

# define BUFF_SZ	128

typedef struct		s_res
{
  int			status[5];

  struct ares_txt_reply *txt;
  struct ares_txt_reply *dkim;
  struct hostent	*host;
  struct ares_txt_reply *dmarc;
  struct ares_mx_reply  *mx;
}			t_res;

void	domain_examiner(char *dkim_selector, char *sender, char *domain);
void	examin_result_loop(ares_channel channel);
void	examin_perform_records(ares_channel channel, char *dkim_selector, char *domain);
void	examin_print_result(t_res res);
void	examin_check_error(t_res res);
void	examin_check_spf(char *sender, char *domain);

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_strsplit(char const *s, char c);
void    ft_putnbr_fd(int n, int fd);
void	ft_ctabdel(char ***ctab);
void	ft_strdel(char **as);

#endif /* DOM_EX_H */
