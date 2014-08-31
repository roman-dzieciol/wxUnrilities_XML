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
//	asvNamedAlign.cpp
// ============================================================================
#include "prec.h"
#include "asvNamedAlign.h"


// ============================================================================
// asvNamedAlign - Global
// ============================================================================
asvNamedAlign& asvNamedAlign::Global()
{
    static asvNamedAlign cofu;
    return cofu;
}


// ============================================================================
// asvNamedAlign
// ============================================================================
asvNamedAlign::asvNamedAlign()
{
}

void asvNamedAlign::OnReload()
{
    Add( wxT("left"), wxALIGN_LEFT );
    Add( wxT("right"), wxALIGN_RIGHT );
    Add( wxT("center"), wxALIGN_CENTER );
}

void asvNamedAlign::Add( const wxString& name, dword align )
{
    Map[name] = align;
}

dword asvNamedAlign::Get( const wxString& name )
{
    guard;
    unUnsignedMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return (*it).second;

    throw exception(wxString::Format( wxT("Unknown named align requested NAME=%s"), name.c_str() ).c_str());
    unguard;
    return 0;
}

// ============================================================================