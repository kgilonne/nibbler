
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "ares.h"
#include "dom_ex.h"

void		domain_examiner(char *selector, char *sender, char *domain)
{
  int                   status;
  ares_channel          channel;
  struct ares_options	options;
  int                   optmask;

  options.flags = ARES_FLAG_USEVC|ARES_FLAG_NOCHECKRESP;
  optmask = ARES_OPT_FLAGS;
  status = ares_init_options(&channel, &options, optmask);
  if (status != ARES_SUCCESS)
  {
    printf("ares_init_options: %s\n", ares_strerror(status));
    exit(SYS_FAIL);
  }

  examin_perform_records(channel, selector, domain);
  ares_destroy(channel);
  if (sender != NULL)
    examin_check_spf(sender, domain);
}
