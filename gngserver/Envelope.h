/* Envelope.h -- read and store envelope data 
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

#if !defined(GNGS_ENVELOPE_H)
#define GNGS_ENVELOPE_H

#include <string>
#include <vector>
#include <iostream>

class Envelope
{
 public:
  Envelope(istream& is);
  ~Envelope();

  int size() { return lines.size(); };

 private:
  vector<string> lines;
};

#endif
