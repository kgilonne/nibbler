
#include <sys/select.h>
#include <netdb.h>
#include <time.h>
#include "ares.h"

void		examin_result_loop(ares_channel channel)
{
  int			nfds;
  fd_set                readers, writers;
  struct timeval        tv, *tvp;

  while ("infinite loop")
  {
      FD_ZERO(&readers);
      FD_ZERO(&writers);
      nfds = ares_fds(channel, &readers, &writers);
      if (nfds == 0)
        break ;
      tvp = ares_timeout(channel, NULL, &tv);
      select(nfds, &readers, &writers, NULL, tvp);
      ares_process(channel, &readers, &writers);
    }
}
