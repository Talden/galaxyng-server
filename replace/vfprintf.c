/* Provide a version vfprintf in terms of fprintf.
   By Kaveh Ghazi  (ghazi@caip.rutgers.edu)  3/29/98
   Copyright (C) 1998 Free Software Foundation, Inc.
 */

#include "config.h"
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <stdio.h>

#define COPY_VA_INT \
  do { \
	 const int value = abs (va_arg (ap, int)); \
	 char buf[32]; \
	 ptr++; /* Go past the asterisk. */ \
	 *sptr = '\0'; /* NULL terminate sptr. */ \
	 sprintf(buf, "%d", value); \
	 strcat(sptr, buf); \
	 while (*sptr) sptr++; \
     } while (0)

#define PRINT_CHAR(CHAR) \
  do { \
	 putc(CHAR, stream); \
	 ptr++; \
	 total_printed++; \
	 continue; \
     } while (0)

#define PRINT_TYPE(TYPE) \
  do { \
	int result; \
	TYPE value = va_arg (ap, TYPE); \
	*sptr++ = *ptr++; /* Copy the type specifier. */ \
	*sptr = '\0'; /* NULL terminate sptr. */ \
	result = fprintf(stream, specifier, value); \
	if (result == -1) \
	  return -1; \
	else \
	  { \
	    total_printed += result; \
	    continue; \
	  } \
      } while (0)

int
vfprintf (stream, format, ap)
  FILE * stream;
  const char * format;
  va_list ap;
{
#ifdef HAVE_DOPRNT
  return _doprnt (format, ap, stream);
#else /* ! HAVE_DOPRNT */
   
  const char * ptr = format;
  char specifier[128];
  int total_printed = 0;
  
  while (*ptr != '\0')
    {
      if (*ptr != '%') /* While we have regular characters, print them. */
	PRINT_CHAR(*ptr);
      else /* We got a format specifier! */
	{
	  char * sptr = specifier;
	  int long_width = 0, short_width = 0, double_width = 0;
	  
	  *sptr++ = *ptr++; /* Copy the % and move forward. */

	  while (strchr ("-+ #0", *ptr)) /* Move past flags. */
	    *sptr++ = *ptr++;

	  if (*ptr == '*')
	    COPY_VA_INT;
	  else
	    while (isdigit(*ptr)) /* Handle explicit numeric value. */
	      *sptr++ = *ptr++;
	  
	  if (*ptr == '.')
	    {
	      *sptr++ = *ptr++; /* Copy and go past the period. */
	      if (*ptr == '*')
		COPY_VA_INT;
	      else
		while (isdigit(*ptr)) /* Handle explicit numeric value. */
		  *sptr++ = *ptr++;
	    }
	  while (strchr ("hlL", *ptr))
	    {
	      switch (*ptr)
		{
		case 'h':
		  short_width = 1;
		  break;
		case 'l':
		  if (*(ptr+1) == 'l')     /* test for 'll' == double width */
		    {
		       double_width = 1;
		       ptr++;
		    }
		  else
		    long_width++;
		  break;
		case 'L':
		  double_width = 1;
		  break;
		default:
		  abort();
		}
	      *sptr++ = *ptr++;
	    }

	  switch (*ptr)
	    {
	    case 'd':
	    case 'i':
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
	    case 'c':
	      {
		if (short_width)
		  PRINT_TYPE(short);
		else
		  {
		    switch (long_width)
		      {
		      case 0:
			PRINT_TYPE(int);
			break;
		      case 1:
			PRINT_TYPE(long);
			break;
		      case 2:
#if defined(__GNUC__) || defined(HAVE_LONG_LONG)
			PRINT_TYPE(long long);
#else
			PRINT_TYPE(long); /* Fake it and hope for the best. */
#endif
			break;
		      default:
			abort();
		      } /* End of switch (int_width) */
		  } /* End of else statement */
	      } /* End of integer case */
	      break;
	    case 'f':
	    case 'e':
	    case 'E':
	    case 'g':
	    case 'G':
	      {
		if (double_width == 0)
		  PRINT_TYPE(double);
		else
		  {
#if defined(__GNUC__) || defined(HAVE_LONG_DOUBLE)
		    PRINT_TYPE(long double);
#else
		    PRINT_TYPE(double); /* Fake it and hope for the best. */
#endif
		  }
	      }
	      break;
	    case 's':
	      PRINT_TYPE(char *);
	      break;
	    case 'p':
	      PRINT_TYPE(void *);
	      break;
	    case '%':
	      PRINT_CHAR('%');
	      break;
	    default:
	      abort();
	    } /* End of switch (*ptr) */
	} /* End of else statement */
    }

    return total_printed;
#endif /* HAVE_DOPRNT */
}

#include <math.h>
#ifndef M_PI
#define M_PI (3.1415926535897932385)
#endif

#define RESULT(x) printf ("printed %d characters\n", (x));

#ifndef PRINTF_ATTRIBUTE
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#  define PRINTF_ATTRIBUTE(m, n)
# else
#  define PRINTF_ATTRIBUTE(m, n) __attribute__ ((format (__printf__, m, n)))
# endif
#endif

#define PRINTF_ATTRIBUTE_1 PRINTF_ATTRIBUTE(1, 2)
#define PRINTF_ATTRIBUTE_2 PRINTF_ATTRIBUTE(2, 3)
#define PRINTF_ATTRIBUTE_3 PRINTF_ATTRIBUTE(3, 4)

static int checkit (const char * format, ...) PRINTF_ATTRIBUTE_1;

static int
checkit (const char* format, ...)
{
  va_list args;
  int result;

#ifndef __STDC__
  char *format;
#endif

  va_start (args, format);

#ifndef __STDC__
  format = va_arg (args, char *);
#endif

  result = vfprintf(stdout, format, args);
  va_end(args);

  return result;
}

#if defined(VFPRINTF_MAIN_NEEDED)
int
main ()
{
  RESULT(checkit ("<%d>\n", 0x12345678));
  RESULT(printf ("<%d>\n", 0x12345678));

  RESULT(checkit ("<%200d>\n", 5));
  RESULT(printf ("<%200d>\n", 5));

  RESULT(checkit ("<%.300d>\n", 6));
  RESULT(printf ("<%.300d>\n", 6));

  RESULT(checkit ("<%100.150d>\n", 7));
  RESULT(printf ("<%100.150d>\n", 7));

  RESULT(checkit ("<%s>\n",
		  "jjjjjjjjjiiiiiiiiiiiiiiioooooooooooooooooppppppppppppaa\n\
777777777777777777333333333333366666666666622222222222777777777777733333"));
  RESULT(printf ("<%s>\n",
		 "jjjjjjjjjiiiiiiiiiiiiiiioooooooooooooooooppppppppppppaa\n\
777777777777777777333333333333366666666666622222222222777777777777733333"));

  RESULT(checkit ("<%f%s%d%s>\n",
		  1.0, "foo", 77, "asdjffffffffffffffiiiiiiiiiiixxxxx"));
  RESULT(printf ("<%f%s%d%s>\n",
		 1.0, "foo", 77, "asdjffffffffffffffiiiiiiiiiiixxxxx"));

  RESULT(checkit ("<%4f><%.4f><%%><%4.4f>\n", M_PI, M_PI, M_PI));
  RESULT(printf ("<%4f><%.4f><%%><%4.4f>\n", M_PI, M_PI, M_PI));

  RESULT(checkit ("<%*f><%.*f><%%><%*.*f>\n", 3, M_PI, 3, M_PI, 3, 3, M_PI));
  RESULT(printf ("<%*f><%.*f><%%><%*.*f>\n", 3, M_PI, 3, M_PI, 3, 3, M_PI));

  RESULT(checkit ("<%d><%i><%o><%u><%x><%X><%c>\n",
		  75, 75, 75, 75, 75, 75, 75));
  RESULT(printf ("<%d><%i><%o><%u><%x><%X><%c>\n",
		 75, 75, 75, 75, 75, 75, 75));

  RESULT(checkit ("<%d><%ld><%hd><%d>\n", 123, (long)234, (short)345, 456));
  RESULT(printf ("<%d><%ld><%hd><%d>\n", 123, (long)234, (short)345, 456));

#if defined(__GNUC__) || defined (HAVE_LONG_LONG)
  RESULT(checkit ("<%d><%lld><%d>\n", 123, 234234234234234234LL, 345));
  RESULT(printf ("<%d><%lld><%d>\n", 123, 234234234234234234LL, 345));
#endif

#if defined(__GNUC__) || defined (HAVE_LONG_DOUBLE)
  RESULT(checkit ("<%.20f><%.20Lf><%.20f>\n",
		  1.23456, 1.234567890123456789L, 1.23456));
  RESULT(printf ("<%.20f><%.20Lf><%.20f>\n",
		 1.23456, 1.234567890123456789L, 1.23456));
#endif

  return 0;
}
#endif
