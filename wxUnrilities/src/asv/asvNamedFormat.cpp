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
//	asvNamedFormat.cpp
// ============================================================================
#include "prec.h"
#include "asvNamedFormat.h"



// ============================================================================
// asvNamedFormat - Global
// ============================================================================
asvNamedFormat& asvNamedFormat::Global()
{
    static asvNamedFormat cofu;
    return cofu;
}


// ============================================================================
// asvNamedFormat
// ============================================================================
asvNamedFormat::asvNamedFormat()
{
}

void asvNamedFormat::OnReload()
{
}

void asvNamedFormat::Add( const wxString& name, const wxString& format )
{
    Map[name] = format;
}

wxString asvNamedFormat::Get( const wxString& name )
{
    guard;
    unStringMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return (*it).second;

    throw exception(wxString::Format( wxT("Unknown named format requested NAME=%s"), name.c_str() ).c_str());
    unguard;
    return wxEmptyString;
}

// ============================================================================