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
//  unTextPrinterPresets.h
// ============================================================================
#ifndef UNTEXTPRINTERPRESETS_H
#define UNTEXTPRINTERPRESETS_H

class unTextPrinter;
class pkgPackage;


// ============================================================================
// unTextPrinterPresetItem
// ============================================================================
class unTextPrinterPresetItem
{
public:
    unTextPrinterPresetItem( const wxString& name, const wxString& params ) 
    : Name(name), Params(params) {}

    wxString Name;
    wxString Params;
};

// ============================================================================
// unTextPrinterPreset
// ============================================================================
class unTextPrinterPreset
{
public:
    unTextPrinterPreset()
    {
    }

    unTextPrinterPreset( const wxString& name, const unTextPrinterPresetItem& item )
    :   Name(name)
    {
        Presets.push_back(item);
    }

    void SetName( const wxString& name ) { Name = name; }
    wxString GetName() { return Name; }

    void Add( const wxString& name, const wxString& params );
    unTextPrinter* Get( pkgPackage* package );

protected:
    wxString Name;
    vector<unTextPrinterPresetItem> Presets;
};


// ============================================================================
// unTextPrinterPresets
// ============================================================================
class unTextPrinterPresets
{
protected:
    WX_DECLARE_STRING_HASH_MAP( unTextPrinterPreset*, unTextPrinterPresetsMap );

public:
    static unTextPrinterPresets& Global();
    #define GTextPrinterPresets unTextPrinterPresets::Global() 

public:
    unTextPrinterPresets();
    ~unTextPrinterPresets();

    void Add( unTextPrinterPreset* preset );
    void AddFlag( const wxString& name );
    void Add( const wxString& preset, const wxString& name, const wxString& param=wxEmptyString );
    unTextPrinter* Get( const wxString& name, pkgPackage* package );
    unTextPrinter* operator()( const wxString& name, pkgPackage* package ) { return Get(name,package); }

    void OnReload();

protected:
    unTextPrinterPresetsMap Map;
};


#endif UNTEXTPRINTERPRESETS_H
// ============================================================================