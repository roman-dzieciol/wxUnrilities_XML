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
//	unTextPrinterPresets.cpp
// ============================================================================
#include "prec.h"
#include "unTextPrinterPresets.h"
#include "unTextPrinterFactory.h"
#include "unTextPrinters.h"
#include "pkgFlags.h"



// ============================================================================
// unTextPrinterPreset
// ============================================================================
void unTextPrinterPreset::Add( const wxString& name, const wxString& params )
{
    Presets.push_back( unTextPrinterPresetItem(name,params) );
}

unTextPrinter* unTextPrinterPreset::Get( pkgPackage* package )
{
    unTextPrinter* group = NULL;
    for( vector<unTextPrinterPresetItem>::reverse_iterator it=Presets.rbegin(); it!=Presets.rend(); ++it )
    {
        unTextPrinter* printer = GTextPrinterFactory.Create( (*it).Name, (*it).Params, package );
        printer->SetNextPrinter(group);
        group = printer;
    }

    return group;
}


// ============================================================================
// unTextPrinterPresets - Static
// ============================================================================
unTextPrinterPresets& unTextPrinterPresets::Global()
{
    static unTextPrinterPresets cof;
    return cof;
}


// ============================================================================
// unTextPrinterPresets
// ============================================================================
unTextPrinterPresets::unTextPrinterPresets()
{
}

unTextPrinterPresets::~unTextPrinterPresets()
{
    for( unTextPrinterPresetsMap::iterator it=Map.begin(); it!=Map.end(); ++it )
    {
        delete ((*it).second);
    }
}

void unTextPrinterPresets::OnReload()
{
    Add( tprTypeObjName::GetStaticName(), tprTypeObjName::GetStaticName() );
    Add( tprTypeObjRef::GetStaticName(), tprTypeObjRef::GetStaticName() );
    Add( tprTypeObjPkg::GetStaticName(), tprTypeObjPkg::GetStaticName() );

    Add( tprTypeOffset::GetStaticName(), tprTypeOffset::GetStaticName() );
    Add( tprTypeUnsigned::GetStaticName(), tprTypeUnsigned::GetStaticName() );
    Add( tprTypeSigned::GetStaticName(), tprTypeSigned::GetStaticName() );
    Add( tprTypeString::GetStaticName(), tprTypeString::GetStaticName() );


}

void unTextPrinterPresets::Add( unTextPrinterPreset* preset )
{
    Map[preset->GetName()] = preset;
}

void unTextPrinterPresets::Add( const wxString& preset, const wxString& name, const wxString& param )
{
    Map[preset] = new unTextPrinterPreset( preset, unTextPrinterPresetItem( name, param ));
}

void unTextPrinterPresets::AddFlag( const wxString& name )
{
    unTextPrinterPreset* flag = new unTextPrinterPreset;
    flag->SetName( wxString::Format( wxT("T%sFlags"), name ) );
    flag->Add( wxT("Flags"), name );
    flag->Add( wxT("TUnsigned"), wxEmptyString );
    Add( flag );  
}

unTextPrinter* unTextPrinterPresets::Get( const wxString& name, pkgPackage* package )
{
    unTextPrinterPresetsMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return ((*it).second)->Get(package);

    throw exception(wxString::Format( wxT("Unknown printer preset requested NAME=%s"), name.c_str() ).c_str());
}


// ============================================================================