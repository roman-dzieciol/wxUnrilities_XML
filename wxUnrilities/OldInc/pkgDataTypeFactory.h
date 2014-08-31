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
//  pkgDataTypeFactory.h
// ============================================================================
#ifndef PKGDATATYPEFACTORY_H
#define PKGDATATYPEFACTORY_H

#include "pkgSerializer.h"

class pkgDataType;


// ============================================================================
// pkgDataTypes
// ============================================================================
class pkgDataTypeFactory
{
private:
    typedef pkgDataType* (*fn_Create)();
    WX_DECLARE_STRING_HASH_MAP( fn_Create, TypeMap );

public:
    // Global
    static pkgDataTypeFactory Global;
    #define GDataTypeFactory pkgDataTypeFactory::Global

    // Data map
    void OnReload();
    void RegisterDefault();
    void UnregisterAll();
    void Unregister( const wxString& name );
    pkgDataType* Create( const wxString& name );

    template<class T> void Register()
    {
        DataMap[ T::Name ] = &T::Create;
    }

protected:
    TypeMap  DataMap;
};


#endif PKGDATATYPEFACTORY_H
// ============================================================================