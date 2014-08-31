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
//  asvNamedPath.h
// ============================================================================
#ifndef ASVNAMEDPATH_H
#define ASVNAMEDPATH_H


// ============================================================================
// asvNamedPath
// ============================================================================
class asvNamedPath
{
public:
    static asvNamedPath& Global();
    #define GNamedPath asvNamedPath::Global()

public:
    // Special
    asvNamedPath();

    // Factory
    void Add( const wxString& name, const wxString& format );
    wxString Get( const wxString& name );
    wxString operator()( const wxString& name ) { return Get(name); }

    // Initialization
    void OnReload();

protected:
    unStringMap Map;
};


#endif ASVNAMEDPATH_H
// ============================================================================