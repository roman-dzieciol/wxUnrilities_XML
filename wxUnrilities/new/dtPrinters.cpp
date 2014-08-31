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
//	dtPrinters.cpp
// ============================================================================
#include "prec.h"
#include "dtPrinters.h"
#include "dtPrinterFactory.h"
#include "asvNamedPath.h"
#include "pkgPackage.h"
#include "pkgPackageData.h"
#include "pkgPackageFormat.h"
#include "asvNamedFormat.h"
#include "pkgFlags.h"


const wxString tprTypeArray::Name           = wxT("TArray");
const wxString tprFormatText::Name          = wxT("FormatText");
const wxString tprFormatSigned::Name        = wxT("FormatSigned");
const wxString tprFormatUnsigned::Name      = wxT("FormatUnsigned");
const wxString tprFlags::Name               = wxT("Flags");
const wxString tprColor::Name               = wxT("Color");

const wxString tprTypeChild::Name           = wxT("TChild");
const wxString tprTypeObjName::Name         = wxT("TObjName");
const wxString tprTypeObjRef::Name          = wxT("TObjRef");
const wxString tprTypeObjPkg::Name          = wxT("TObjPkg");
const wxString tprTypeOffset::Name          = wxT("TOffset");
const wxString tprTypeUnsigned::Name        = wxT("TUnsigned");
const wxString tprTypeSigned::Name          = wxT("TSigned");
const wxString tprTypeString::Name          = wxT("TString");
const wxString tprTypeText::Name            = wxT("TText");
const wxString tprTypeData::Name            = wxT("TData");




// ============================================================================
// tprTypeArray
// ============================================================================
tprTypeArray::tprTypeArray( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Type = package->GetData().GetRoot().FindTypeByNamePath(params);
}


// ============================================================================
// tprFormatText
// ============================================================================
tprFormatText::tprFormatText( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Format = GNamedFormat.Get(params);
}


// ============================================================================
// tprFormatSigned
// ============================================================================
tprFormatSigned::tprFormatSigned( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Format = GNamedFormat.Get(params);
}


// ============================================================================
// tprFormatUnsigned
// ============================================================================
tprFormatUnsigned::tprFormatUnsigned( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Format = GNamedFormat.Get(params);
}


// ============================================================================
// tprFlags
// ============================================================================
tprFlags::tprFlags( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Flags = package->GetFormat().GetFlags().FindGroup(params);
}


// ============================================================================
// tprTypeObjName
// ============================================================================
tprTypeObjName::tprTypeObjName( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    PathName = GNamedPath.Get(wxT("I.NameItem.Name"));
    Array = package->GetData().GetRoot().FindTypeByNamePath( GNamedPath.Get(wxT("G.NameTable.Array")) );
}


// ============================================================================
// tprTypeObjRef
// ============================================================================
tprTypeObjRef::tprTypeObjRef( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Path_ExportItemObject   = GNamedPath.Get(wxT("I.ExportItem.ObjectName"));
    Path_ExportItemPackage  = GNamedPath.Get(wxT("I.ExportItem.Package"));
    Path_ImportItemObject   = GNamedPath.Get(wxT("I.ImportItem.ObjectName"));
    Path_ImportItemPackage  = GNamedPath.Get(wxT("I.ImportItem.Package"));
    Path_NameItemName       = GNamedPath.Get(wxT("I.NameItem.Name"));

    pkgType& root = package->GetData().GetRoot();
    NameTable = root.FindTypeByNamePath( GNamedPath.Get(wxT("G.NameTable.Array")) );
    ExportTable = root.FindTypeByNamePath( GNamedPath.Get(wxT("G.ExportTable.Array")) );
    ImportTable = root.FindTypeByNamePath( GNamedPath.Get(wxT("G.ImportTable.Array")) );
}




// ============================================================================
// tprColor
// ============================================================================
tprColor::tprColor( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
    Format = GNamedFormat.Get(params);
}

// ============================================================================
// tprTypeText
// ============================================================================
tprTypeText::tprTypeText( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
,	Package(package)
{
}

wxString tprTypeText::GetString( void* ptr ) 
{ 
	pkgType* type = static_cast<pkgType*>(ptr);
	auto_ptr<dtPrinter> printer( GPrinterFactory.Create(type->GetTextPrinter(),wxEmptyString,Package) );
	return printer->GetString(type);
}

// ============================================================================
// tprTypeData
// ============================================================================
tprTypeData::tprTypeData( const wxString& params, pkgPackage* package )
:   dtPrinter(params,package)
{
}




// ============================================================================