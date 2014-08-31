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
//	dtPrinterGroup.cpp
// ============================================================================
#include "prec.h"
#include "dtPrinterGroup.h"



// ============================================================================
// dtPrinterSort
// ============================================================================
class dtPrinterSort
{
public:
    dtPrinterSort( dtPrinter& printer, dtComparer& comparer ) 
    : Printer(printer), Comparer(comparer)
    { 
    }

    bool operator()( dword a, dword b )
    {
        return Comparer.Compare( Printer.PrintText(a), Printer.PrintText(b) );
    }

protected:
    dtPrinter&	Printer;
    dtComparer&	Comparer;
};

// ============================================================================
// dtPrinterCachedSort
// ============================================================================
class dtPrinterCachedSort
{
public:
    dtPrinterCachedSort( dtPrinter& printer, dtComparer& comparer, dtTableCacheColumn& column ) 
        : Printer(printer), Comparer(comparer), Column(column)
    { 
    }

    bool operator()( dword a, dword b )
    {
        wxString& left = Column AT(a);
        if( left.empty() )
            left = Printer.PrintText(a);

        wxString& right = Column AT(b);
        if( right.empty() )
            right = Printer.PrintText(b);

        return Comparer.Compare( left, right );
    }

protected:
    dtPrinter&	Printer;
    dtComparer&	Comparer;
    dtTableCacheColumn& Column;
};



// ============================================================================
// dtPrinterGroup
// ============================================================================
void dtPrinterGroup::Sort( dword column, bool bforce )
{
    SW_DEF( sw );
    if( column != SortedColumn || bforce )
    {
        SortedColumn = column;
        if( bUseCache )
            stable_sort( IdxRegEx.begin(), IdxRegEx.end(), dtPrinterCachedSort( *Printers.at(column), *Comparers.at(column), TableCache AT(column)  ) );
        else
            stable_sort( IdxRegEx.begin(), IdxRegEx.end(), dtPrinterSort( *Printers.at(column), *Comparers.at(column)  ) );

    }
    else
    {
        reverse( IdxRegEx.begin(), IdxRegEx.end() );
    }
    SW_LOG( sw, "Sorted in %s." );
}




// ============================================================================