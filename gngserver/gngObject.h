/* gngObject.h -- read and store incoming commands
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

#if !defined(GNGOBJECT_H)
#define GNGOBJECT_H

#include <iostream>
#include <string>
#include <vector>

#include "Envelope.h"
#include "Orders.h"

class gngObject
{
 public:
  gngObject();
  ~gngObject();

  bool init(istream&is);

  int  envelopeLines() { return m_env->size(); };
  int  ordersLines() { return m_orders->size(); };

 private:
  friend bool operator !(gngObject& go);

  Envelope *m_env;
  Orders   *m_orders;
};

#endif
