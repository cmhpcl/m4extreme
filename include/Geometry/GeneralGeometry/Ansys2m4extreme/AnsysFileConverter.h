// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(_M4EXTREME_ANSYSFILECONVERTER_H__INCLUDED_)
#define _M4EXTREME_ANSYSFILECONVERTER_H__INCLUDED_

#pragma once
#include "Ansys.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cassert>

namespace m4extreme {
  void AnsysFileConverter(int dimension, int elementType, 
                const char* inputFileName, const char* outputFileName);
}

#endif
