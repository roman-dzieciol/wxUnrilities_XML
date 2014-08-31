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
//	pkgProperty.cpp
// ============================================================================
#include "prec.h"
#include "pkgProperty.h"


// ============================================================================
//	pkgProperty - Static
// ============================================================================
const wxString pkgProperty::PropertyTerminator = wxT("None");


// ============================================================================
//	pkgProperty
// ============================================================================
pkgProperty::pkgProperty( pkgSerializer& S, pkgArchive& A )
{
    // Read Offset
    Offset = A.Tell();

    // Read name index
    // Name "None" marks end of properties block
    Name = ReadNameItem(S);
    if( PropertyTerminator.IsSameAs( GetName() ) )
        throw exSafe();

    // Read property info
    S >> Info;
    Type = Info & PR_MASK_TYPE;
}

pkgProperty::pkgProperty( pkgSerializer& S, pkgArchive& A, const pkgProperty& P )
:   Offset(P.Offset)
,   Name(P.Name)
,   Info(P.Info)
,   Type(P.Type)
{
}

wxString pkgProperty::DumpInfo()
{
    return GetName().c_str();
}


// ============================================================================