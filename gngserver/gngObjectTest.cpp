/* Testing.cpp -- Test suite using cppunit (a port of JUnit) for
                  running unit tests
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

#include "gngObjectTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( gngObjectTest );

void
gngObjectTest::setUp()
{
  go = new gngObject();
  go->init(std::cin);
}

void
gngObjectTest::tearDown()
{
  delete go;
}

void
gngObjectTest::testConstructor()
{
  CPPUNIT_ASSERT(go->envelopeLines() > 0);
  CPPUNIT_ASSERT(go->ordersLines() > 0);
}
