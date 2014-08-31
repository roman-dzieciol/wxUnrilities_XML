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
//	pkgNativeFunc.cpp
// ============================================================================
#include "prec.h"
#include "pkgNativeFunc.h"


// ============================================================================
// pkgNativeFunc
// ============================================================================
pkgNativeFunc::pkgNativeFunc()
:   Class(NULL)
,   Package(NULL)
{
}


// ============================================================================
// pkgNativeFuncClass
// ============================================================================
pkgNativeFuncClass::~pkgNativeFuncClass()
{
    for( pkgNativeFunc::Array::iterator it=Functions.begin(); it!=Functions.end(); ++it )
    {
        delete *it;
    }
}

pkgNativeFunc::Array& pkgNativeFuncClass::GetFunctions()
{
    return Functions;
}

void pkgNativeFuncClass::AddFunction( pkgNativeFunc* p )
{
    Functions.push_back(p);
}


// ============================================================================
// pkgNativeFuncPackage
// ============================================================================
pkgNativeFuncPackage::~pkgNativeFuncPackage()
{
    for( pkgNativeFuncClass::Array::iterator it=Classes.begin(); it!=Classes.end(); ++it )
    {
        delete *it;
    }
}

pkgNativeFuncClass::Array& pkgNativeFuncPackage::GetClasses()
{
    return Classes;
}

void pkgNativeFuncPackage::AddClass( pkgNativeFuncClass* p )
{
    Classes.push_back(p);
}


// ============================================================================
// pkgNativeFuncMap
// ============================================================================
pkgNativeFuncMap::~pkgNativeFuncMap()
{
    for( pkgNativeFuncPackage::Array::iterator it=Packages.begin(); it!=Packages.end(); ++it )
    {
        delete *it;
    }
}

pkgNativeFuncPackage::Array& pkgNativeFuncMap::GetPackages()
{
    return Packages;
}

void pkgNativeFuncMap::AddPackage( pkgNativeFuncPackage* p )
{
    Packages.push_back(p);
}


// ============================================================================