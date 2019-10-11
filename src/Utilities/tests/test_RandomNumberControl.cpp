//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2017 Jeongnim Kim and QMCPACK developers.
//
// File developed by:  Mark Dewing, mdewing@anl.gov Argonne National Laboratory
//
// File created by: Mark Dewing, mdewing@anl.gov Argonne National Laboratory
//////////////////////////////////////////////////////////////////////////////////////


#include "catch.hpp"


//#include "Utilities/RandomGenerator.h"
#include "Message/Communicate.h"
#include "OhmmsData/Libxml2Doc.h"
#include "Utilities/RandomNumberControl.h"


#include <stdio.h>
#include <string>


namespace qmcplusplus
{
TEST_CASE("RandomNumberControl make_seeds", "[ohmmsapp]")
{
  Communicate* c;
  OHMMS::Controller->initialize(0, NULL);
  c = OHMMS::Controller;

  RandomNumberControl random_control{c,8};
  //random_control.make_seeds();

  REQUIRE(random_control.Children.size() > 0);
}

TEST_CASE("RandomNumberControl stack instantiation", "[ohmmsapp]")
{
  Communicate* c;
  OHMMS::Controller->initialize(0, NULL);
  c = OHMMS::Controller;

  RandomNumberControl random_control{c,1};

  double rd = (random_control.get_random())();
  REQUIRE(random_control.Children.size() > 0);
  REQUIRE(rd >= 0.0);
  REQUIRE(rd < 1.0);

  RandomNumberControl random_control2{c,2};

  double rd2 = (random_control2.get_random())();
  REQUIRE(random_control.Children.size() > 0);
  REQUIRE(rd2 >= 0.0);
  REQUIRE(rd2 < 1.0);

}

TEST_CASE("RandomNumberControl no random in xml", "[ohmmsapp]")
{
  Communicate* c;
  OHMMS::Controller->initialize(0, NULL);
  c = OHMMS::Controller;

  const char* xml_input = "<tmp></tmp>";

  Libxml2Document doc;
  bool okay = doc.parseFromString(xml_input);
  REQUIRE(okay);

  RandomNumberControl rnc{c};

  xmlXPathContextPtr context = doc.getXPathContext();
  rnc.initialize(context);
}

TEST_CASE("RandomNumberControl random in xml", "[ohmmsapp]")
{
  Communicate* c;
  OHMMS::Controller->initialize(0, NULL);
  c = OHMMS::Controller;

  const char* xml_input = "<tmp><random seed='0'></random></tmp>";

  Libxml2Document doc;
  bool okay = doc.parseFromString(xml_input);
  REQUIRE(okay);

  RandomNumberControl rnc{c};

  xmlXPathContextPtr context = doc.getXPathContext();
  rnc.initialize(context);


  rnc.write("rng_out", c);

  RandomNumberControl rnc2{c};
  rnc2.read("rng_out", c);
  // not sure what to test here - for now make sure it doesn't crash.
}
} // namespace qmcplusplus
