/* error.h -- display formatted error diagnostics of varying severity
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

#if !defined(GNGS_ERROR_H)
#define GNGS_ERROR_H 1

#include <iostream>
#include <string>

#include <gngserver/common.h>

class Diagnostic
{
 public:
  Diagnostic(string progName);
  ~Diagnostic();

  ostream& Warning(const char *message, ...);
  ostream& Error(const char *message, ...);
  ostream& Fatal(const char *message, ...);
  
 private:
  string m_program_name;
  ostream m_os;
  ostream& output_message(const char *tag, const char *message, va_list ap);
};

#endif /* !GNGS_ERROR_H */
