// ============================================================================
//  Unrilities :: Cross-platform toolkit for Unreal Engine packages
// ============================================================================
//  Copyright (C) 2005  Roman Dzieciol <neai at o2.pl>
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// ============================================================================
//	pkgPackageFormat.cpp
// ============================================================================
#include "prec.h"
#include "pkgPackageFormat.h"
#include "cfgFormatFlags.h"

#include "pkgFlags.h"
#include "pkgTypeBase.h"
#include "pkgType.h"
#include "pkgTypeFlow.h"
#include "pkgTypeRegistry.h"

#include "pkgObject.h"
#include "cfgFormatHeader.h"
#include "pkgNativeFunc.h"
#include "cfgNativeFunc.h"

// ============================================================================
// pkgPackageFormat
// ============================================================================
pkgPackageFormat::pkgPackageFormat()
{
    Flags           = new pkgFlags;
    BaseTypes       = new pkgTypeBaseRegistry;
    FlowTypes       = new pkgTypeFlowRegistry;
    Functions       = new pkgNativeFuncMap;
    HeaderTypes     = new pkgType;
    Objects         = new pkgType;
}

pkgPackageFormat::~pkgPackageFormat()
{
    delete Flags;
    delete BaseTypes;
    delete FlowTypes;
    delete HeaderTypes;
    delete Objects;
    delete Functions;
}

void pkgPackageFormat::OnReload()
{
    BaseTypes->OnReload();
    FlowTypes->OnReload();

    cfgFormatFlags  ( wxFileName(wxT(".\\Config\\UT2004\\Format\\Flags.xml")), Flags );
    cfgNativeFunc   ( wxFileName(wxT(".\\Config\\UT2004\\Format\\Functions.xml")), Functions );
    cfgFormatHeader ( wxFileName(wxT(".\\Config\\UT2004\\Format\\Header.xml")), this, HeaderTypes );
    cfgFormatHeader ( wxFileName(wxT(".\\Config\\UT2004\\Format\\Objects.xml")), this, HeaderTypes );
}


// ============================================================================