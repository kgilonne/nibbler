
#include <arpa/nameser.h>
#include <netdb.h>
#include <string.h>
#include "ares.h"
#include "dom_ex.h"

static t_res		g_res;

static void		callback_txt(void *arg, int status, int timeouts, unsigned char *abuf, int alen)
{
  int			selector;
  struct ares_txt_reply	*txt_out = NULL;

  (void)timeouts;
  selector = *((int *)arg);
  g_res.status[selector] = status;

  if (status != ARES_SUCCESS)
    return ;

  ares_parse_txt_reply(abuf, alen, &txt_out);

  if (selector == TXT_ID)
    g_res.txt = txt_out;
  else if (selector == DMARC_ID)
    g_res.dmarc = txt_out;
  else if (selector == DKIM_ID)
    g_res.dkim = txt_out;
}

static void		callback_ns(void *arg, int status, int timeouts, unsigned char *abuf, int alen)
{
  struct hostent	*host = NULL;

  (void)arg;
  (void)timeouts;
  g_res.status[NS_ID] = status;

  if (status != ARES_SUCCESS)
    return ;

  ares_parse_ns_reply(abuf, alen, &host);
  g_res.host = host;
}

static void		callback_mx(void *arg, int status, int timeouts, unsigned char *abuf, int alen)
{
  struct ares_mx_reply	*mx_out = NULL;

  (void)arg;
  (void)timeouts;
  g_res.status[MX_ID] = status;

  if (status != ARES_SUCCESS)
    return ;

  ares_parse_mx_reply(abuf, alen, &mx_out);
  g_res.mx = mx_out;
}

#include <stdio.h>
void		examin_perform_records(ares_channel channel, char *dkim_selector, char *domain)
{
  int		txt_id = TXT_ID;
  int		dkim_id = DKIM_ID;
  int		dmarc_id = DMARC_ID;
  char		selector[BUFF_SZ];


  // TXT RECORD
  g_res.txt = NULL;
  ares_query(channel, domain, ns_c_in, ns_t_txt, callback_txt, &txt_id);
  // ---------
  // NS RECORD
  g_res.host = NULL;
  ares_query(channel, domain, ns_c_in, ns_t_ns, callback_ns, NULL);
  // ---------
  // DMARC RECORD
  g_res.dmarc = NULL;
  memset(selector, '\0', BUFF_SZ);
  strcpy(selector, "_dmarc.");
  if (strlen(domain) + 1 <= sizeof(selector) - strlen(selector))
  {
    (void)strncat(selector, domain, sizeof(selector) - strlen(selector) - 1);
    ares_query(channel, selector, ns_c_in, ns_t_txt, callback_txt, &dmarc_id);
  }
  // ---------
  // DKIM RECORD
  g_res.dkim = NULL;
  if (dkim_selector != NULL) {
    memset(selector, '\0', BUFF_SZ);
    strcpy(selector, dkim_selector);
    if (sizeof(selector) - strlen(selector) >= 13) {
      strncat(selector, "._domainkey.", sizeof(selector) - strlen(selector) - 1);
      if (sizeof(selector) - strlen(selector) >= strlen(domain) + 1)
  	{
	  (void)strncat(selector, domain, sizeof(selector) - strlen(selector) - 1);
	  ares_query(channel, selector, ns_c_in, ns_t_txt, callback_txt, &dkim_id);
  	}
    }
  }
  // ---------
  // MX RECORD
  g_res.mx = NULL;
  ares_query(channel, domain, ns_c_in, ns_t_mx, callback_mx, NULL);
  // ---------

  examin_result_loop(channel);
  examin_print_result(g_res);
  examin_check_error(g_res);
}
