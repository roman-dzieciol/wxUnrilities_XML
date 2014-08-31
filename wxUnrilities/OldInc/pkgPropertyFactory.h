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
//  pkgPropertyFactory.h
// ============================================================================
#ifndef PKGPROPERTYFACTORY_H
#define PKGPROPERTYFACTORY_H

#include "pkgSerializer.h"

class pkgProperty;


// ============================================================================
// pkgPropertyFactory
// ============================================================================
class pkgPropertyFactory
{
public:
    static pkgPropertyFactory Global;
    #define GPropertyFactory pkgPropertyFactory::Global

private:
    typedef pkgProperty* (*fn_PropertyCreate)( pkgSerializer& S, pkgArchive& A, const pkgProperty& P );
    typedef vector<fn_PropertyCreate> TypeMap;

public:
    void OnReload();
    void Register( dword i, fn_PropertyCreate fn );
    void RegisterDefault();
    void Unregister( dword i );
    void UnregisterAll();
    pkgProperty* Create( pkgSerializer& S, pkgArchive& A );

private:
    TypeMap PropertyTypes;
};


// ============================================================================
// pkgPropertyFactory - inline
// ============================================================================
inline void pkgPropertyFactory::OnReload()
{
    UnregisterAll();
    RegisterDefault();
}

inline void pkgPropertyFactory::Register( dword i, fn_PropertyCreate fn )
{
    if( i+1 > PropertyTypes.size() )
        PropertyTypes.resize(i+1,NULL);

    if( PropertyTypes.at(i) != NULL )
        throw exFName( wxString::Format( wxT("Type %d already registered."), i ).c_str() );

    PropertyTypes.at(i) = fn;
}

inline void pkgPropertyFactory::Unregister( dword i )
{
    PropertyTypes.at(i) = NULL;
}

inline void pkgPropertyFactory::UnregisterAll()
{
    PropertyTypes.clear();
}


#endif PKGPROPERTYFACTORY_H
// ============================================================================