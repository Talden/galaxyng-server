/* Diagnostic.cpp -- display formatted error diagnostics of varying severity
   Copyright 2004 Kenneth D. Weinert
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "common.h"
#include "Diagnostic.h"


Diagnostic::Diagnostic(string programName)
{
	m_programName = programName;
}

Diagnostic::~Diagnostic()
{
}

ostream&
Diagnostic::output_message(ostream& os, const char *mode,
			   const char *message, va_list ap)
{
  os << m_programName << ": " << mode << ": ";
  const char *p = message;
  int num;
  double d;
  char *s;

  while (*p) {
    switch (*p) {
    case '%':
      p++;
      switch (*p) {
      case 'd':
	num = va_arg(ap, int);
	os << num;
	break;
	
      case 'f':
	d = va_arg(ap, double);
	os << d;
	break;

      case 's':
	s = va_arg(ap, char *);
	os << s;
	break;

      default:
	os << "(unknown formatter '" << *p << "')";
	break;
      }
      break;
      
    case '\\':
      p++;
      switch(*p) {
      case '\\':
	os << "\\";
	break;
	
      case 't':
	os << "\t";
	break;
	
      case 'n':
	os << "\n";
	break;
	
      default:
	os << "(unknown escape: '" << *p << "')";
	break;
      }
      break;
      
    default:
      os << *p;
    }
    p++;
  }
  
  return os;
}

ostream&
Diagnostic::Warning(ostream& os, const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  output_message(os, "WARNING", message, ap);
  va_end (ap);
  return os;
}

ostream&
Diagnostic::Error(ostream& os, const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  output_message(os, "ERROR", message, ap);
  va_end (ap);
  return os;
}

ostream&
Diagnostic::Fatal(ostream& os, const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  (void)output_message(os, "FATAL", message, ap);
  va_end (ap);
  exit(EXIT_FAILURE);
}
