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
//  dtPrinterGroup.h
// ============================================================================
#ifndef DTPRINTERGROUP_H
#define DTPRINTERGROUP_H

#include "dtComparer.h"
#include "dtPrinter.h"


// ============================================================================
// dtTextColumn
// ============================================================================
class dtTextColumn
{
public:
    dtTextColumn( const wxString& name=wxEmptyString, dword align=wxALIGN_LEFT, dword width=-1 )
    : Name(name), Align(align), Width(width)
    {
    }

    wxString GetName() const { return Name; }
    dword GetAlign() const { return Align; }
    dword GetWidth() const { return Width; }

    void SetName( const wxString& p ) { Name = p; }
    void SetAlign( dword p ) { Align = p; }
    void SetWidth( dword p ) { Width = p; }

protected:
    wxString Name;
    dword Align;
    dword Width;
};


// ============================================================================
// dtIndexArray
// ============================================================================
typedef vector<dword> dtIndexArray;


// ============================================================================
// dtIndexGenerator
// ============================================================================
struct dtIndexGenerator
{
    dtIndexGenerator() : counter(-1) {}
    dword counter;
    dword operator()() { return ++counter; }
};

// ============================================================================
// Cache
// ============================================================================
typedef vector<wxString>            dtTableCacheColumn;
typedef vector<dtTableCacheColumn>  dtTableCache;


// ============================================================================
// dtPrinterGroup
// ----------------------------------------------------------------------------
// Holds a number of printers
// For use in list controls
// Refactor?
// ============================================================================
class dtPrinterGroup
{
public:
    // Special
    dtPrinterGroup()
    :	bUseCache(false)
    ,	ClientData(NULL)
    {
    }

    ~dtPrinterGroup()
    {
        for( vector<dtTextColumn*>::iterator it=Columns.begin(); it!=Columns.end(); ++it )
            delete (*it);

        for( vector<dtPrinter*>::iterator it=Printers.begin(); it!=Printers.end(); ++it )
            delete (*it);

        for( vector<dtComparer*>::iterator it=Comparers.begin(); it!=Comparers.end(); ++it )
            delete (*it);
    }


    // Accessors
    vector<dtTextColumn*>& GetColumns() { return Columns; }
    wxString GetName() const { return Name; }
    void* GetClientData() { return ClientData; }

    // Mutators
    void SetName( const wxString& p ) { Name = p; }
    void SetClientData( void* p ) { ClientData = p; }


    // Printing
    wxString Print( void* ptr, dword column ) const
    {
        return Printers AT(column)->Print(ptr);
    }

    wxString Print( dword index, dword column ) 
    {
        index = IdxRegEx AT(index);
        if( bUseCache )
        {
            wxString& text = TableCache AT(column) AT(index);
            if( text.empty() )
                text = Printers AT(column)->Print(reinterpret_cast<void*>(index));
            return text;
        }
        else
        {
            return Printers AT(column)->Print(reinterpret_cast<void*>(index));
        }
    }

    dword TranslateIndex( dword index )
    {
        return IdxRegEx AT(index);
    }


    // Population
    dtPrinter* Add( dtPrinter* printer )
    {
        Printers.push_back(printer);
        return Printers.back();
    }

    dtPrinter* AddPrinter( dtPrinter* printer=NULL )
    {
        if( printer == NULL )
            printer = new dtPrinter;

        Printers.push_back(printer);
        return Printers.back();
    }

    dtPrinter* AddEmpty()
    {
        Printers.push_back(new dtPrinter);
        return Printers.back();
    }

    dtComparer* AddComparer( dtComparer* comparer )
    {
        Comparers.push_back(comparer);
        return Comparers.back();
    }

    dtTextColumn* AddColumn( dtTextColumn* column )
    {
        Columns.push_back(column);
        return Columns.back();
    }


    // Sorting
    void Sort( dword column, bool bforce=false );

    void SetItemCount( dword count )
    {
        ItemCount = count;

        IdxDefault.reserve(ItemCount);
        IdxDefault.resize(ItemCount);
        generate(IdxDefault.begin(), IdxDefault.end(), dtIndexGenerator());
        IdxRegEx = IdxDefault;

        SortedColumn = 0;
    }

    dword GetItemCount() const
    {
        return ItemCount;
    }



    // Cache
    void EnableCache()
    {
        bUseCache = true;

        // get column count
        dword width = Printers.size();

        // make column template
        dtTableCacheColumn column;
        column.reserve(ItemCount);
        column.resize(ItemCount);

        // init cache table
        TableCache.clear();
        TableCache.reserve(width);
        TableCache.resize(width, column);
    }

protected:
    vector<dtTextColumn*> Columns;
    vector<dtPrinter*> Printers;
    vector<dtComparer*> Comparers;
    dtIndexArray IdxDefault;
    dtIndexArray IdxRegEx;
    dword SortedColumn;
    dword ItemCount;
    wxString Name;

    dtTableCache TableCache;
    bool bUseCache;
    void* ClientData;
};


#endif DTPRINTERGROUP_H
// ============================================================================