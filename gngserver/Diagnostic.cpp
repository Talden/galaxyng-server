/* error.cpp -- display formatted error diagnostics of varying severity
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


Diagnostic::Diagnostic()
{
}

Diagnostic::~Diagnostic()
{
}

ostream&
Diagnostic::output_message(const char *mode, const char *message, va_list ap)
{
  m_os << m_program_name << ": " << mode << ": ";
  const char *p = message;
  int num;
  double d;

  while (*p) {
    switch (*p) {
    case '%':
      p++;
      switch (*p) {
      case 'd':
	num = va_arg(ap, int);
	m_os << num;
	break;

      case 'f':
	d = va_arg(ap, double);
	m_os << d;
	break;

      default:
	m_os << "(unknown formatter '" << *p << "')";
	break;
      }
      break;

    case '\\':
      p++;
      switch(*p) {
      case '\\':
	m_os << "\\";
	break;

      case 't':
	m_os << "\t";
	break;

      case 'n':
	m_os << "\n";
	break;

      default:
	m_os << "(unknown escape: '" << *p << "')";
	break;
      }
      break;
      
    default:
      m_os << *p;
    }
  }
  return m_os;
}

ostream&
Diagnostic::Warning(const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  ostream& os = output_message("WARNING", message, ap);
  va_end (ap);
  return os;
}

ostream&
Diagnostic::Error(const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  ostream& os = output_message("ERROR", message, ap);
  va_end (ap);
  return os;
}

ostream&
Diagnostic::Fatal(const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  (void)output_message("FATAL", message, ap);
  va_end (ap);
  exit(EXIT_FAILURE);
}
