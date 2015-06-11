
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "dom_ex.h"

#if __linux
static double	timespec_to_ms(struct timespec ts)
{
  return (( ((double) ts.tv_sec) * 1000 ) + ( ((double) ts.tv_nsec) / 1000000 ));
}
#endif

int		main(int argc, char **argv)
{
#if __linux
  struct timespec	start;
  struct timespec	end;
#endif

	char *dkim_selector;
	char *sender;
	char *domain;

	int i = 1;
	dkim_selector = NULL;
	if (argc > i + 1 && strcmp(argv[i], "-dkim") == 0) {
		dkim_selector = argv[i+1];
		i += 2;
	}
	sender = NULL;
	if (argc > i + 1 && strcmp(argv[i], "-spf") == 0) {
		sender = argv[i+1];
		i += 2;
	}

  if (argc < i + 1 || argv[i][0]  == '\0')
  {
    printf ("Usage: %s -dkim < selector > -spf < sender > < domain >\n", argv[0]);
    return (SYS_FAIL);
  }
	domain = argv[i];

#if __linux
  clock_gettime(CLOCK_REALTIME, &start);
#endif

  domain_examiner(dkim_selector, sender, domain);

#if __linux
  if (clock_gettime(CLOCK_REALTIME, &end) == -1)
    printf(",\n\t\"Time\":\"none\"");
  else
  {
    double	total;

    total = ( timespec_to_ms(end) - timespec_to_ms(start) );
    printf(",\n\t\"Time\":\"%.2fms\"", total);
  }
#endif

  printf("\n}\n");
  return (EXIT_SUCCESS);
}
