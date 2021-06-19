// EoSLib.h: interface for the Manifold library.
// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
/////////////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_GAS_EOSLIB_H__INCLUDED_)
#define MATERIAL_GAS_EOSLIB_H__INCLUDED_

#pragma once

#include "./EoS.h"
#include "./Factory.h"
#include "./Exponential/Exponential.h"
#include "./Ideal/Factory.h"
#include "./BlatzKo/Factory.h"
#include "./Polytropic/Factory.h"
#include "./Quadratic/Factory.h"
#include "./FK2LK/Factory.h"
#include "./Vinet/Vinet.h"
#include "./Regression/Regression.h"
#include "./Multiphase/Multiphase.h"
#include "./PengRobinson/PengRobinson.h"
#include "./NoTension/Factory.h"
#include "./FKQuadratic/Factory.h"
#include "./LK2FK/Factory.h"
#include "./ThermalLK2FK/Factory.h"
#include "./PressureInterpolation/Factory.h"
#include "./PressureRegression/Factory.h"
#include "./SolidPolytropic/Factory.h"
#include "./TaitMurnaghan/Factory.h"
#include "./TwoStages/Factory.h"
#include "./JWL/Factory.h"
#include "./MieGruneisen/Factory.h"
#include "./LKMieGruneisen/Factory.h"

#endif // !defined(MATERIAL_GAS_EOSLIB_H__INCLUDED_)
