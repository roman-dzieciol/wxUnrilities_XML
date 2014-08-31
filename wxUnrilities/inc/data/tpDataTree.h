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
//  tpDataTree.h
// ============================================================================
#ifndef TPDATATREE_H
#define TPDATATREE_H

//
// TODO:: scrap/refactor
//

#include "unTextPrinter.h"
#include "utTypeCastor.h"
#include "pkgFlags.h"
#include "pkgType.h"
#include "pkgPackage.h"
#include "pkgPackageData.h"
#include "unDataTree.h"


// ============================================================================
// tpAccessor
// ============================================================================
template<class TH, class TF>
class tpAccessor : public unTextPrinter
{
public:
    tpAccessor( TF function ) : Function(function)
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        TH* host = static_cast<TH*>(ptr);
        return ((*host).*Function)();
    }

protected:
    TF Function;
};

template<class FH, class FR> inline tpAccessor<FH,FR(FH::*)()>* new_tpAccessor( FR(FH::*function )()  )
{  return new tpAccessor<FH,FR(FH::*)()>(function); }

template<class FH, class FR> inline tpAccessor<FH,FR(FH::*)()const>* new_tpAccessor( FR(FH::*function )()const  )
{ return new tpAccessor<FH,FR(FH::*)()const>(function); }


// ============================================================================
// tpFlags
// ============================================================================
template<class TH, class TF>
class tpFlags : public unTextPrinter
{
public:
    tpFlags( TF function, pkgFlagGroup* flaggroup ) : Function(function), FlagGroup(flaggroup)
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        TH* host = static_cast<TH*>(ptr);
        return FlagGroup->PrintFlagNames(((*host).*Function)());
    }

protected:
    TF Function;
    pkgFlagGroup* FlagGroup;
};

template<class FH, class FR> inline tpFlags<FH,FR(FH::*)()>* new_tpFlags( FR(FH::*function )(), pkgFlagGroup* flaggroup  )
{ return new tpFlags<FH,FR(FH::*)()>(function,flaggroup); }

template<class FH, class FR> inline tpFlags<FH,FR(FH::*)()const>* new_tpFlags( FR(FH::*function )()const, pkgFlagGroup* flaggroup  )
{ return new tpFlags<FH,FR(FH::*)()const>(function,flaggroup); }


// ============================================================================
// tpTypeChild
// ============================================================================
class tpTypeChild : public unTextPrinter
{
public:
    tpTypeChild( const wxString& name, unTextPrinter* printer ) 
    : Name(name), TextPrinter(printer)
    {
    }

    virtual ~tpTypeChild()
    {
        delete TextPrinter;
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        pkgType* type = static_cast<pkgType*>(ptr);
        pkgType* child = GetCachedPath(type);
        return TextPrinter->PrintText(child);
    }

    pkgType* GetCachedPath( pkgType* item )
    {
        if( CachedPath.empty() )
            CachedPath = item->FindTypePathByNamePath(Name);
        return item->FindTypeByTypePath(CachedPath, true); 
    }

protected:
    wxString Name;
    unTextPrinter* TextPrinter;
    pkgTypePath CachedPath;
};

// Creator
inline tpTypeChild* new_tpTypeChild( const wxString& name, unTextPrinter* printer ) 
{ return new tpTypeChild(name,printer); }


// ============================================================================
// tpPkgType
// ============================================================================
class tpPkgType : public unTextPrinter
{
public:
    tpPkgType( pkgPackage& package, const wxString& name, unTextPrinter* printer ) 
    : TextPrinter(printer)
    {
        Type = package.GetData().GetRoot().FindTypeByNamePath(name);
    }

    virtual ~tpPkgType()
    {
        delete TextPrinter;
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        return TextPrinter->PrintText(Type);
    }

protected:
    unTextPrinter* TextPrinter;
    pkgType* Type;
};

// Creator
inline tpPkgType* new_tpPkgType( pkgPackage& package, const wxString& name, unTextPrinter* printer ) 
{
    return new tpPkgType(package,name,printer);
}



// ============================================================================
// tpFormat
// ============================================================================
template<class TH, class TP>
class tpFormat : public unTextPrinter
{
public:
    tpFormat( const wxString& format, const TP& printer ) 
    :   Format(format), Printer(printer)
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        return GetPrinter(ptr,Printer);
    }

    //
    template<class T, class R> wxString GetPrinter( void* ptr, R(T::*func)() )
    {
        return GetText( ((*GetPointer(ptr)).*func)() );
    }

    template<class T, class R> wxString GetPrinter( void* ptr, R(T::*func)()const )
    {
        return GetText( ((*GetPointer(ptr)).*func)() );
    }

    //
    template<class R> wxString GetText( R param )
    {
        return wxString::Format( Format, param );
    }

    wxString GetText( wxString param )
    {
        return wxString::Format( Format, param.c_str() );
    }

    // returns typecasted pointer
    TH* GetPointer( void* ptr )
    {
        return static_cast<TH*>(ptr);
    }

protected:
    wxString Format;
    TP Printer;
};


//template<class TH, class TP> inline tpFormat<TH,TP>* new_tpFormat( const wxString& format, const TP& printer )
//{ return new tpFormat<TH,TP>(format,printer); }

template<class TH, class TR> inline tpFormat<TH,TR(TH::*)()>* new_tpFormat( const wxString& format, TR(TH::*printer)()  )
{ return new tpFormat<TH,TR(TH::*)()>(format,printer); }

template<class TH, class TR> inline tpFormat<TH,TR(TH::*)()const>* new_tpFormat( const wxString& format, TR(TH::*printer)()const  )
{ return new tpFormat<TH,TR(TH::*)()const>(format,printer); }


// ============================================================================
// tpDataTreeTypeOffset
// ============================================================================
class tpDataTreeTypeOffset : public unTextPrinter
{
public:
    tpDataTreeTypeOffset() 
    {
    }

    virtual ~tpDataTreeTypeOffset()
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        unDataTree* tree = static_cast<unDataTree*>(ptr);
        unDataTree* first = static_cast<unDataTree*>(tree->at(0));
        pkgType* type = static_cast<pkgType*>(first->GetClientData());
        return wxString::Format(wxT("0x%.8x"),type->GetOffset());
    }

protected:
};



// ============================================================================
// tpTypeParent
// ============================================================================
class tpTypeParent : public unTextPrinter
{
public:
    tpTypeParent( unTextPrinter* printer ) 
    : TextPrinter(printer)
    {
    }

    virtual ~tpTypeParent()
    {
        delete TextPrinter;
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        pkgType* type = static_cast<pkgType*>(ptr);
        return TextPrinter->PrintText(type->GetParent());
    }

protected:
    unTextPrinter* TextPrinter;
};

// Creator
inline tpTypeParent* new_tpTypeParent( unTextPrinter* printer ) 
{ return new tpTypeParent(printer); }


// ============================================================================
// tpNotNull
// ============================================================================
class tpNotNull : public unTextPrinter
{
public:
    tpNotNull( unTextPrinter* printer ) 
    : TextPrinter(printer)
    {
    }

    virtual ~tpNotNull()
    {
        delete TextPrinter;
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        if( ptr != NULL )
            return TextPrinter->PrintText(ptr);
        else
            return wxEmptyString;
    }

protected:
    unTextPrinter* TextPrinter;
};

// Creator
inline tpNotNull* new_tpNotNull( unTextPrinter* printer ) 
{ return new tpNotNull(printer); }


// ============================================================================
// tpNameTable
// ============================================================================
class tpNameTable : public unTextPrinter
{
public:
    tpNameTable( pkgPackage& package ) 
    {
        Array = package.GetData().GetRoot().FindTypeByNamePath(wxT("Header/NameTable/Table"));
    }

    virtual ~tpNameTable()
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        pkgType* type = static_cast<pkgType*>(ptr);
        dword index = type->GetUnsigned();

        pkgType* item = Array->FindTypeByIndex(index);
        return GetCachedPath(item)->GetString();
    }

    DECLARE_NAMEPATH_CACHED_ACCESSOR( CachedPath, wxT("NameItem/Name") );

protected:
    pkgType* Array;
};

// Creator
inline tpNameTable* new_tpNameTable( pkgPackage& package ) 
{ return new tpNameTable(package); }


// ============================================================================
// tpObjectRef
// ============================================================================
class tpObjectRef : public unTextPrinter
{
public:
    tpObjectRef( pkgPackage& package ) 
    {
        NameTable = package.GetData().GetRoot().FindTypeByNamePath(wxT("Header/NameTable/Table"));
        ExportTable = package.GetData().GetRoot().FindTypeByNamePath(wxT("Header/ExportTable/Table"));
        ImportTable = package.GetData().GetRoot().FindTypeByNamePath(wxT("Header/ImportTable/Table"));
    }

    virtual ~tpObjectRef()
    {
    }

    virtual wxString PrintText( void* ptr=NULL )
    {
        pkgType* type = static_cast<pkgType*>(ptr);
        wxString text;
        while( type != NULL )
        {
            type = PrintPackage(type,text);
        }
        return text;
    }

    pkgType* PrintPackage( pkgType* type, wxString& text )
    {
        int index = type->GetSigned();

        if( index > 0 )
        {
            --index;
            pkgType* item = ExportTable->FindTypeByIndex(index);
            if( text != wxEmptyString )
                text.Prepend( wxT('.') );
            text.Prepend(PrintName(GetExportItemObject(item)));
            return GetExportItemPackage(item);
        }
        else if( index < 0 )
        {
            index = -index-1;
            pkgType* item = ImportTable->FindTypeByIndex(index);
            if( text != wxEmptyString )
                text.Prepend( wxT('.') );
            text.Prepend(PrintName(GetImportItemObject(item)));
            return GetImportItemPackage(item);
        }

        return NULL;
    }

    wxString PrintName( pkgType* type )
    {
        pkgType* item = NameTable->FindTypeByIndex(type->GetUnsigned());
        return GetNameItemName(item)->GetString();
    }

    DECLARE_NAMEPATH_CACHED_ACCESSOR( ExportItemObject, wxT("ExportItem/ObjectName") );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ExportItemPackage, wxT("ExportItem/Package") );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ImportItemObject, wxT("ImportItem/ObjectName") );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( ImportItemPackage, wxT("ImportItem/Package") );
    DECLARE_NAMEPATH_CACHED_ACCESSOR( NameItemName, wxT("NameItem/Name") );

protected:
    pkgType* NameTable;
    pkgType* ExportTable;
    pkgType* ImportTable;

};

// Creator
inline tpObjectRef* new_tpObjectRef( pkgPackage& package ) 
{ return new tpObjectRef(package); }







#endif TPDATATREE_H
// ============================================================================