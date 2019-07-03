//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2019 developers.
//
// File developed by: Peter Doak, doakpw@ornl.gov, Oak Ridge National Laboratory
//
// File refactored from VMC.h
//////////////////////////////////////////////////////////////////////////////////////

#ifndef QMCPLUSPLUS_QMCDRIVERINTERFACE_H
#define QMCPLUSPLUS_QMCDRIVERINTERFACE_H

//#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
//#include <libxml/xpath.h>

namespace qmcplusplus
{

/** Creates a common base class pointer for QMCDriver and QMCDriverNew
 *  to share.
 *
 *  Once the "unified" driver is done this should be refactored away
 */

class QMCDriverInterface
{
public:
  virtual bool run() = 0;

  virtual bool put(xmlNodePtr cur) = 0;

  virtual void resetComponents(xmlNodePtr cur) = 0;

  virtual void recordBlock(int block) = 0;

  virtual ~QMCDriverInterface() {}
};

}

#endif
