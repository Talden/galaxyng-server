/* gngObject.cpp -- read and store incoming commands
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

gngObject::gngObject(istream& is)
{
  m_env = new Envelope();
  m_orders = new Orders();

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

/// \brief the destructor cleans up the owned objects.
///
/// we need to free up the envelope and orders upon object deletion.
gngObject::~gngObject()
{
  delete m_env;
  delete m_orders;
}
