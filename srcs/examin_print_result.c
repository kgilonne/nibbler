
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "ares.h"
#include "dom_ex.h"

static void	print_txt_result(struct ares_txt_reply *txt)
{
  write(1, "\t\"TXT records\":[\n", 17);
  while (txt)
  {
      write(1, "\t\t\"", 3);
      write(1, txt->txt, txt->length);
      write(1, "\"", 1);
      if (txt->next)
        write(1, ",\n", 2);
      txt = txt->next;
  }
  write(1, "\n\t],\n", 5);
}

static void	print_ns_result(struct hostent *host)
{
  int		i = 0;

  write(1, "\t\"NS records\":[\n", 16);
  while (host && host->h_aliases[i])
  {
      write(1, "\t\t\"", 3);
      write(1, host->h_aliases[i], strlen(host->h_aliases[i]));
      write(1, "\"", 1);
      if (host->h_aliases[i + 1])
        write(1, ",\n", 2);
      i++;
  }
  write(1, "\n\t],\n", 5);
}

static void	print_dmarc_result(struct ares_txt_reply *dmarc)
{
  write(1, "\t\"DMARC record\":\"", 17);
  while (dmarc)
  {
      write(1, dmarc->txt, dmarc->length);
      dmarc = dmarc->next;
  }
  write(1, "\",\n", 3);
}

static void	print_dkim_result(struct ares_txt_reply *dkim)
{
  if (dkim == NULL)
    return ;
  write(1, "\t\"DKIM record\":\"", 16);
  while (dkim)
  {
      write(1, dkim->txt, dkim->length);
      dkim = dkim->next;
  }
  write(1, "\",\n", 3);
}

static void	print_mx_result(struct ares_mx_reply *mx)
{
  write(1, "\t\"MX records\":[\n", 16);
  while (mx)
  {
      write(1, "\t\t\"", 3);
      ft_putnbr_fd(mx->priority, 1);
      write(1, " ", 1);
      write(1, mx->host, strlen(mx->host));
      write(1, "\"", 1);
      if (mx->next)
        write(1, ",\n", 2);
      mx = mx->next;
  }
  write(1, "\n\t]", 3);
}

void		examin_print_result(t_res res)
{
  write(1, "{\n", 2);

  print_txt_result(res.txt);
  ares_free_data(res.txt);

  print_ns_result(res.host);
  ares_free_hostent(res.host);

  print_dmarc_result(res.dmarc);
  ares_free_data(res.dmarc);

  print_dkim_result(res.dkim);
  ares_free_data(res.dkim);

  print_mx_result(res.mx);
  ares_free_data(res.mx);
}
