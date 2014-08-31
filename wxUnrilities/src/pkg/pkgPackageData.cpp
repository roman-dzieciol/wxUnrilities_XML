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
//	pkgPackageData.cpp
// ============================================================================
#include "prec.h"
#include "pkgPackageData.h"
#include "pkgPackageFormat.h"
#include "pkgPackage.h"
#include "pkgType.h"


// ============================================================================
// pkgPackageData
// ============================================================================
pkgPackageData::pkgPackageData()
:   Root(NULL)
{
}

pkgPackageData::~pkgPackageData()
{
    delete Root;
}


void pkgPackageData::OnReload( pkgPackage& package )
{
    delete Root;
    Root = new pkgType( wxT("root"), wxT("root") );

    pkgType* header = package.GetFormat().GetHeaderTypes().FindTypeByName(wxT("Header"));
    header->OnRead(package,*Root);
}


// ============================================================================