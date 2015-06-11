
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ares.h"
#include "dom_ex.h"

static void	check_txt_error(int status, int *err_count)
{
  char      *error;

	if (status != ARES_SUCCESS)
  {
    error = (char *)ares_strerror(status);
    write(1, "\t\t\"TXT records\":\"", 17);
    write(1, error, strlen(error));
    write(1, "\"", 1);
    if (*err_count)
      write(1, ",", 1);
    write(1, "\n", 1);
    *err_count -= 1;
    if (status == ARES_ETIMEOUT || status == ARES_ECONNREFUSED
     || status == ARES_ESERVFAIL || status == ARES_EREFUSED)
      exit(TRY_AGAIN);
    else if (status == ARES_ENOMEM)
      exit(SYS_FAIL);
  }
}

static void	check_ns_error(int status, int *err_count)
{
  char		*error;

  if (status != ARES_SUCCESS)
  {
      error = (char *)ares_strerror(status);
      write(1, "\t\t\"NS records\":\"", 16);
      write(1, error, strlen(error));
      write(1, "\"", 1);
      *err_count -= 1;
      if (*err_count)
        write(1, ",", 1);
      write(1, "\n", 1);
      if (status == ARES_ETIMEOUT || status == ARES_ECONNREFUSED
       || status == ARES_ESERVFAIL || status == ARES_EREFUSED)
        exit(TRY_AGAIN);
      else if (status == ARES_ENOMEM)
      	exit(SYS_FAIL);
  }
}

static void	check_dmarc_error(int status, int *err_count)
{
  char		*error;

  if (status != ARES_SUCCESS
   && status != ARES_ENOTFOUND && status != ARES_ENODATA)
  {
      error = (char *)ares_strerror(status);
      write(1, "\t\t\"DMARC record\":\"", 19);
      write(1, error, strlen(error));
      write(1, "\"", 1);
      *err_count -= 1;
      if (*err_count)
        write(1, ",", 1);
      write(1, "\n", 1);
      if (status == ARES_ETIMEOUT || status == ARES_ECONNREFUSED
       || status == ARES_ESERVFAIL || status == ARES_EREFUSED)
        exit(TRY_AGAIN);
      else if (status == ARES_ENOMEM)
      	exit(SYS_FAIL);
  }
}

static void	check_mx_error(int status, int *err_count)
{
  char		*error;

  if (status != ARES_SUCCESS)
  {
      error = (char *)ares_strerror(status);
      write(1, "\t\t\"MX records\":\"", 16);
      write(1, error, strlen(error));
      write(1, "\"", 1);
      *err_count -= 1;
      if (*err_count)
        write(1, ",", 1);
      write(1, "\n", 1);
      if (status == ARES_ETIMEOUT || status == ARES_ECONNREFUSED
       || status == ARES_ESERVFAIL || status == ARES_EREFUSED)
        exit(TRY_AGAIN);
      else if (status == ARES_ENOMEM)
      	exit(SYS_FAIL);
  }
}

static void	check_dkim_error(int status)
{
  char		*error;

  if (status != ARES_SUCCESS)
  {
    error = (char *)ares_strerror(status);
    write(1, "\t\t\"DKIM record\":\"", 17);
    write(1, error, strlen(error));
    write(1, "\"\n", 2);
    if (status == ARES_ETIMEOUT || status == ARES_ECONNREFUSED
	|| status == ARES_ESERVFAIL || status == ARES_EREFUSED)
      exit(TRY_AGAIN);
    else if (status == ARES_ENOMEM)
      exit(SYS_FAIL);
  }
}

void		examin_check_error(t_res res)
{
  int		i = 0;
  int		err_count = 0;

  while (i < 5)
  {
    if (res.status[i] != ARES_SUCCESS)
      err_count++;
    i++;
  }

  // These errors don't need to be printed
  if (res.status[DMARC_ID] == ARES_ENOTFOUND
   || res.status[DMARC_ID] == ARES_ENODATA)
    err_count--;

  if (!err_count)
    return ;
  write(1, ",\n\t\"Errors\":{\n", 14);
  check_txt_error(res.status[TXT_ID], &err_count);
  check_ns_error(res.status[NS_ID], &err_count);
  check_dmarc_error(res.status[DMARC_ID], &err_count);
  check_mx_error(res.status[MX_ID], &err_count);
  check_dkim_error(res.status[DKIM_ID]);
  write(1, "\t}", 2);
}
