/* gngObjectTest.h -- Test suite using cppunit (a port of JUnit) for
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

#if !defined(GNGOBJECTTEST_H)
#define GNGOBJECTTEST_H

#include <iostream>
using namespace std;

#include <cppunit/extensions/HelperMacros.h>
#include "gngObject.h"

///\brief test suite using cppunit (a port of JUnit) for running unit tests
///
/// I am using cppunit for generating tests for the code as I go
/// along. CPPUnit is a port of JUnit.
class gngObjectTest : public CppUnit::TestFixture
{
  gngObject* go;

  CPPUNIT_TEST_SUITE(gngObjectTest);
  CPPUNIT_TEST(testConstructor);
  CPPUNIT_TEST_SUITE_END();

 public:
  /// setUp is to set up common variables for the suite of tests.
  void setUp();

  /// tearDown is to clean up common variables for the suite of tests.
  void tearDown();

  /// builds up the tests for the suite.
  void testConstructor();
};

#endif

