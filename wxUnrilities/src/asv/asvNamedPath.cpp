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
//	asvNamedPath.cpp
// ============================================================================
#include "prec.h"
#include "asvNamedPath.h"

// ============================================================================
// asvNamedPath - Global
// ============================================================================
asvNamedPath& asvNamedPath::Global()
{
    static asvNamedPath cofu;
    return cofu;
}


// ============================================================================
// asvNamedPath
// ============================================================================
asvNamedPath::asvNamedPath()
{
}

void asvNamedPath::OnReload()
{
}

void asvNamedPath::Add( const wxString& name, const wxString& format )
{
    Map[name] = format;
}

wxString asvNamedPath::Get( const wxString& name )
{
    unStringMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return (*it).second;

    throw exception(wxString::Format( wxT("Unknown named path requested NAME=%s"), name.c_str() ).c_str());
}


// ============================================================================