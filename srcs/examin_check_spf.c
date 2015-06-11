
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dom_ex.h"

#define LENGTH 10

static void	malloc_fail(FILE *out)
{
    perror("malloc : ");
    pclose(out);
    exit(SYS_FAIL);
}

static char	*read_cmd_result(FILE *out)
{
  char		*buf, *save, *res = NULL;

  if ((buf = (char *)malloc((LENGTH * sizeof(char)) + 1)) == NULL)
    malloc_fail(out);

  while((fread(buf, sizeof(char), LENGTH, out) != 0) && !feof(out))
    {
      save = res;
      buf[LENGTH] = '\0';
      if (save)
	{
	  if ((res = ft_strjoin(save, buf)) == NULL)
	    malloc_fail(out);
	  free(save);
	}
      else if ((res = strdup(buf)) == NULL)
	malloc_fail(out);
    }
  ft_strdel(&buf);
  return (res);
}

void		examin_check_spf(char *sender, char *domain)
{
  char		**res_split;
  int		len_split;
  char		*res = NULL;
  char		cmd[BUFF_SZ];
  FILE		*out;
  int   	fd;

  memset(cmd, '\0', BUFF_SZ);
  strcpy(cmd, "spfquery -ip=");
  if (sizeof(cmd) - strlen(cmd) < strlen(sender) + 1)
    return ;
  (void)strncat(cmd, sender, sizeof(cmd) - strlen(cmd) - 1);
  if (sizeof(cmd) - strlen(cmd) < 18)
    return ;
  (void)strncat(cmd, " -sender=contact@", sizeof(cmd) - strlen(cmd) - 1);
  if (sizeof(cmd) - strlen(cmd) < strlen(domain) + 1)
    return ;
  (void)strncat(cmd, domain, sizeof(cmd) - strlen(cmd) - 1);

  // Open trash and connect stderr
  fd = open("/dev/null", O_WRONLY);
  dup2(fd, 2);

  if ((out = popen(cmd,"r")) == NULL)
  {
    perror("popen : ");
    close(fd);
    exit(SYS_FAIL);
  }
  close(fd);

  printf(",\n\t\"SPF test\":{\n");
  if ((res = read_cmd_result(out)) != NULL)
  {
    if ((res_split = ft_strsplit(res, '\n')) == NULL)
    {
      perror("malloc : ");
      exit(SYS_FAIL);
    }
    ft_strdel(&res);

    len_split = 0;
    while (res_split[len_split])
    {
	len_split++;
    }
    if (len_split > 2)
    {
      printf("\t\t\"Result\":\"%s\",\n", res_split[0]);
      printf("\t\t\"Comment\":\"%s\"\n", res_split[2]);
    }
    else
      printf("\t\t\"Error\":\"\response size inferior to 3 lines (%d lines)\n", len_split);
    ft_ctabdel(&res_split);
  }
  else
    printf("\t\t\"Error\":\"\%s failed\"\n", cmd);

  printf("\t}");
  pclose(out);
}
