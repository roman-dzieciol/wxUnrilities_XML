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
//  dtPrinters.h
// ============================================================================
#ifndef DTPRINTERS_H
#define DTPRINTERS_H



#include "dtPrinter.h"
#include "pkgType.h"
#include "pkgFlags.h"

class pkgFlagGroup;





// ============================================================================
// tprTypeArray
// ============================================================================
class tprTypeArray : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeArray );
    DATA_PRINTER_CREATOR( tprTypeArray, dtPrinter );

    // Special
    tprTypeArray( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprTypeArray( const tprTypeArray& c )
    :   dtPrinter(c)
    ,   Type(c.Type)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return NextPrinter->GetString( GetTypeChild(reinterpret_cast<dword>(ptr)) );
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return NextPrinter->GetUnsigned( GetTypeChild(reinterpret_cast<dword>(ptr)) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return NextPrinter->GetSigned( GetTypeChild(reinterpret_cast<dword>(ptr)) );
    }

    virtual wxString PrintText( void* ptr )
    {
        return GetString( ptr );
    }

    virtual wxString PrintText( dword index )
    {
        return GetString( reinterpret_cast<void*>(index) );
    }

    // Internal
    pkgType* GetTypeChild( dword index )
    {
        return Type->FindTypeByIndex(index,false);
    }

protected:
    pkgType* Type;
};





// ============================================================================
// tprFormatText
// ============================================================================
class tprFormatText : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprFormatText );
    DATA_PRINTER_CREATOR( tprFormatText, dtPrinter );

    // Special
    tprFormatText( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprFormatText( const tprFormatText& c )
    :   dtPrinter(c)
    ,   Format(c.Format)
    {
    }


    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetFormattedString(ptr);
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    // Internal
    wxString GetFormattedString( void* ptr )
    {
        return wxString::Format( Format, NextPrinter->GetString(ptr).c_str() );
    }

protected:
    wxString Format;
};


// ============================================================================
// tprFormatSigned
// ============================================================================
class tprFormatSigned : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprFormatSigned );
    DATA_PRINTER_CREATOR( tprFormatSigned, dtPrinter );

    // Special
    tprFormatSigned( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprFormatSigned( const tprFormatSigned& c )
    :   dtPrinter(c)
    ,   Format(c.Format)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetFormattedString(ptr);
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    // Internal
    wxString GetFormattedString( void* ptr )
    {
        return wxString::Format( Format, NextPrinter->GetSigned(ptr) );
    }

protected:
    wxString Format;
};


// ============================================================================
// tprFormatUnsigned
// ============================================================================
class tprFormatUnsigned : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprFormatUnsigned );
    DATA_PRINTER_CREATOR( tprFormatUnsigned, dtPrinter );

    // Special
    tprFormatUnsigned( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprFormatUnsigned( const tprFormatUnsigned& c )
    :   dtPrinter(c)
    ,   Format(c.Format)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetFormattedString(ptr);
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    // Internal
    wxString GetFormattedString( void* ptr )
    {
        return wxString::Format( Format, NextPrinter->GetUnsigned(ptr) );
    }

protected:
    wxString Format;
};


// ============================================================================
// tprFlags
// ============================================================================
class tprFlags : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprFlags );
    DATA_PRINTER_CREATOR( tprFlags, dtPrinter );

    // Special
    tprFlags( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprFlags( const tprFlags& c )
    :   dtPrinter(c)
    ,   Flags(c.Flags)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetFlagString(ptr);
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetFlagString(ptr) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetFlagString(ptr) );
    }

    // Internal
    wxString GetFlagString( void* ptr )
    {
        return Flags->PrintFlagNames( NextPrinter->GetUnsigned(ptr) );
    }

protected:
    pkgFlagGroup* Flags;
};


// ============================================================================
// tprTypeChild
// ============================================================================
class tprTypeChild : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeChild );
    DATA_PRINTER_CREATOR( tprTypeChild, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeChild, dtPrinter )
    ,   PathName(params)
    {
		wxLogMessage("tprTypeChild");
    }

    tprTypeChild( const tprTypeChild& c )
    :   dtPrinter(c)
    ,   PathName(c.PathName)
    {
		wxLogMessage("tprTypeChild copy");
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
		pkgType* type = static_cast<pkgType*>(ptr);
        return NextPrinter->GetString( GetTypeChild(type) );
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
		pkgType* type = static_cast<pkgType*>(ptr);
        return NextPrinter->GetUnsigned( GetTypeChild(type) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
		pkgType* type = static_cast<pkgType*>(ptr);
        return NextPrinter->GetSigned( GetTypeChild(type) );
    }

    // Internal
    pkgType* GetTypeChild( pkgType* type )
    {
        if( CachedPath.empty() )
            CachedPath = type->FindTypePathByNamePath(PathName, true);
        return type->FindTypeByTypePath(CachedPath, true);
    }

protected:
    wxString PathName;
    pkgTypePath CachedPath;
};


// ============================================================================
// tprTypeObjName
// ============================================================================
class tprTypeObjName : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeObjName );
    DATA_PRINTER_CREATOR( tprTypeObjName, dtPrinter );

    // Special
    tprTypeObjName( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprTypeObjName( const tprTypeObjName& c )
    :   dtPrinter(c)
    ,   PathName(c.PathName)
    ,   Array(c.Array)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetObjName(static_cast<pkgType*>(ptr));
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetObjName(static_cast<pkgType*>(ptr)) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetObjName(static_cast<pkgType*>(ptr)) );
    }

    // Internal
    wxString GetObjName( pkgType* type )
    {
        type = Array->FindTypeByIndex(type->GetUnsigned());
        if( CachedPath.empty() )
            CachedPath = type->FindTypePathByNamePath(PathName);
        return type->FindTypeByTypePath(CachedPath, true)->GetString(); 
    }

protected:
    pkgType* Array;
    wxString PathName;
    pkgTypePath CachedPath;
};


// ============================================================================
// tprTypeObjRef
// ============================================================================
class tprTypeObjRef : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeObjRef );
    DATA_PRINTER_CREATOR( tprTypeObjRef, dtPrinter );

    // Special
    tprTypeObjRef( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprTypeObjRef( const tprTypeObjRef& c )
    :   dtPrinter(c)
    ,   NameTable(c.NameTable)
    ,   ExportTable(c.ExportTable)
    ,   ImportTable(c.ImportTable)
    ,   Path_ExportItemObject(c.Path_ExportItemObject)
    ,   Path_ExportItemPackage(c.Path_ExportItemPackage)
    ,   Path_ImportItemObject(c.Path_ImportItemObject)
    ,   Path_ImportItemPackage(c.Path_ImportItemPackage)
    ,   Path_NameItemName(c.Path_NameItemName)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetPackagePath(static_cast<pkgType*>(ptr));
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetPackagePath(static_cast<pkgType*>(ptr)) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetPackagePath(static_cast<pkgType*>(ptr)) );
    }

    // Internal
    wxString GetPackagePath( pkgType* type )
    {
        wxString text;
        while( type != NULL )
        {
            type = PrintPackageName(type,text);
        }
        return text;
    }

    pkgType* PrintPackageName( pkgType* type, wxString& text )
    {
        int index = type->GetSigned();

        if( index > 0 )
        {
            if( text != wxEmptyString )
                text.Prepend( wxT('.') );

            pkgType* item = ExportTable->FindTypeByIndex(--index);
            text.Prepend( PrintObjectName( GetExportItemObject(item) ) );
            return GetExportItemPackage(item);
        }
        else if( index < 0 )
        {
            if( text != wxEmptyString )
                text.Prepend( wxT('.') );

            pkgType* item = ImportTable->FindTypeByIndex(-index-1);
            text.Prepend( PrintObjectName( GetImportItemObject(item) ) );
            return GetImportItemPackage(item);
        }

        return NULL;
    }

    wxString PrintObjectName( pkgType* type )
    {
        return GetNameItemName( NameTable->FindTypeByIndex(type->GetUnsigned()) )->GetString();
    }

    DECLARE_NAMEPATH_CACHED_ACCESSOR( ExportItemObject, Path_ExportItemObject );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ExportItemPackage, Path_ExportItemPackage );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ImportItemObject, Path_ImportItemObject );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ImportItemPackage, Path_ImportItemPackage );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( NameItemName, Path_NameItemName );

protected:
    pkgType* NameTable;
    pkgType* ExportTable;
    pkgType* ImportTable;

    wxString Path_ExportItemObject;
    wxString Path_ExportItemPackage;
    wxString Path_ImportItemObject;
    wxString Path_ImportItemPackage;
    wxString Path_NameItemName;
};


// ============================================================================
// tprTypeObjPkg
// ============================================================================
class tprTypeObjPkg : public tprTypeObjRef
{
public:
    DATA_PRINTER_COMMON( tprTypeObjPkg );
    DATA_PRINTER_CREATOR( tprTypeObjPkg, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeObjPkg, tprTypeObjRef )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetPackagePath(static_cast<pkgType*>(ptr));
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetPackagePath(static_cast<pkgType*>(ptr)) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetPackagePath(static_cast<pkgType*>(ptr)) );
    }

    // Internal
    wxString GetPackagePath( pkgType* type )
    {
        int index = type->GetSigned();

        if( index > 0 )
        {
            type = ExportTable->FindTypeByIndex(--index);
            return PrintObjectName( GetExportItemObject(type) );
        }
        else if( index < 0 )
        {
            type = ImportTable->FindTypeByIndex(-index-1);
            return PrintObjectName( GetImportItemObject(type) );
        }

        return wxEmptyString;
    }
};


// ============================================================================
// tprTypeOffset
// ============================================================================
class tprTypeOffset : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeOffset );
    DATA_PRINTER_CREATOR( tprTypeOffset, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeOffset, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return wxString::Format( "%u", static_cast<pkgType*>(ptr)->GetOffset() );
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetOffset();
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetOffset();
    }
};


// ============================================================================
// tprTypeUnsigned
// ============================================================================
class tprTypeUnsigned : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeUnsigned );
    DATA_PRINTER_CREATOR( tprTypeUnsigned, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeUnsigned, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return wxString::Format( "%u", static_cast<pkgType*>(ptr)->GetUnsigned() );
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetUnsigned();
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetSigned();
    }
};


// ============================================================================
// tprTypeSigned
// ============================================================================
class tprTypeSigned : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeSigned );
    DATA_PRINTER_CREATOR( tprTypeSigned, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeSigned, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return wxString::Format( "%d", static_cast<pkgType*>(ptr)->GetSigned() );
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetUnsigned();
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetSigned();
    }
};


// ============================================================================
// tprTypeString
// ============================================================================
class tprTypeString : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeString );
    DATA_PRINTER_CREATOR( tprTypeString, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( tprTypeString, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetString();
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }
};


// ============================================================================
// tprColor
// ============================================================================
class tprColor : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprColor );
    DATA_PRINTER_CREATOR( tprColor, dtPrinter );

    // Special
    tprColor( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprColor( const tprColor& c )
    :   dtPrinter(c)
    ,   Format(c.Format)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return GetFormattedString(ptr);
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return unStringToDword( GetFormattedString(ptr) );
    }

    // Internal
    wxString GetFormattedString( void* ptr )
    {
        return wxString::Format( Format
        , (NextPrinter->GetUnsigned(ptr)&0x000000ff)
        , (NextPrinter->GetUnsigned(ptr)&0x0000ff00)>>8
        , (NextPrinter->GetUnsigned(ptr)&0x00ff0000)>>16
        , (NextPrinter->GetUnsigned(ptr)&0xff000000)>>24 );
    }

protected:
    wxString Format;
};


// ============================================================================
// tprTypeText
// ============================================================================
class tprTypeText : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeText );
    DATA_PRINTER_CREATOR( tprTypeText, dtPrinter );

    // Special
    tprTypeText( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprTypeText( const tprTypeText& c )
    :   dtPrinter(c)
	,	Package(c.Package)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr );

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }

protected:
	pkgPackage* Package;
};


// ============================================================================
// tprTypeData
// ============================================================================
class tprTypeData : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( tprTypeData );
    DATA_PRINTER_CREATOR( tprTypeData, dtPrinter );

    // Special
    tprTypeData( const wxString& params = wxEmptyString, pkgPackage* package = NULL );

    tprTypeData( const tprTypeData& c )
    :   dtPrinter(c)
    //,   Format(c.Format)
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        return static_cast<pkgType*>(ptr)->GetString();
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return ::unStringToDword(static_cast<pkgType*>(ptr)->GetString());
    }
};




#endif DTPRINTERS_H
// ============================================================================