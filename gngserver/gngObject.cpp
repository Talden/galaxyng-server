/* gngObject.cpp -- read and store incoming orders
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

#include "gngObject.h"

gngObject::gngObject()
{
  m_env = new Envelope();
  m_orders = new Orders();
}

gngObject::~gngObject()
{
  delete m_env;
  delete m_orders;
}

bool
gngObject::init(istream& is)
{
  bool inHeader = true;
  string line;

  while (inHeader) {
    if (getline(is, line, '\n') == NULL) {
      inHeader = false;
      break;
    }
    m_env->m_lines.push_back(line);
  }


  while (getline(is, line, '\n' )) {
    m_orders->m_lines.push_back(line);
  }
}

bool
gngObject::operator !(gngObject& go)
{
  return true;
}
