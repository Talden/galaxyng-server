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

/** 
 * \brief The gngObject class encompasses input to the server, whether
 * that be from email, direct connection, html form, etc.
 *
 * The GNGServer will allow several methods of contact for receiving
 * commands from players and the GM and the subsequent sending of
 * feedback to the contact entity. This object is meant to encompass
 * all of the defined methods so the remainder of the program can
 * remain ignorant of how the data is sent and received.
 */
class gngObject
{
 public:
  /// \brief the constructor takes an istream&
  ///
  /// The constructor takes an istream& and then reads and stores the
  /// lines into the envelope and orders objects.
  gngObject(istream& is);

  ~gngObject();

  /// \brief report on the number of lines in the envelope object
  int  envelopeLines() { return m_env->size(); };

  /// \brief report on the number of lines in the orders object
  int  ordersLines() { return m_orders->size(); };

 private:
  Envelope *m_env;
  Orders   *m_orders;
};

#endif
