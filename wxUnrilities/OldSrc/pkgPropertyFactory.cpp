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
//	pkgPropertyFactory.cpp
// ============================================================================
#include "prec.h"
#include "pkgPropertyFactory.h"
#include "pkgProperty.h"
#include "pkgProperties.h"


// ============================================================================
// pkgPropertyFactory - Static
// ============================================================================
pkgPropertyFactory pkgPropertyFactory::Global;


// ============================================================================
// pkgPropertyFactory
// ============================================================================
void pkgPropertyFactory::RegisterDefault()
{
    Register( 1, &pkgpByte::Create );
    Register( 2, &pkgpInt::Create );
    Register( 3, &pkgpBool::Create );
    Register( 4, &pkgpFloat::Create );
    Register( 5, &pkgpObject::Create );
    Register( 6, &pkgpName::Create );
    Register( 7, &pkgpString::Create );
    Register( 8, &pkgpClass::Create );
    Register( 9, &pkgpArray::Create );
    Register( 10, &pkgpStruct::Create );
    Register( 11, &pkgpVector::Create );
    Register( 12, &pkgpRotator::Create );
    Register( 13, &pkgpStr::Create );
    Register( 14, &pkgpMap::Create );
    Register( 15, &pkgpFixedArray::Create );
}

pkgProperty* pkgPropertyFactory::Create( pkgSerializer& S, pkgArchive& A )
{
    try
    {
        pkgProperty pheader(S,A);

        fn_PropertyCreate fn = PropertyTypes.at( pheader.GetType() );
        if( fn == NULL )
            throw exFName( wxString::Format( wxT("Unknown property type: %d"), pheader.GetType() ).c_str() );

        return fn(S,A,pheader);
    }
    catch( exSafe& )
    {
        return NULL;
    }
}


// ============================================================================