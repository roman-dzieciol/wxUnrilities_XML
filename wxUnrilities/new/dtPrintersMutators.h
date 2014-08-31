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
//  dtPrintersMutators.h
// ============================================================================
#ifndef DTPRINTERSMUTATORS_H
#define DTPRINTERSMUTATORS_H

#include "dtPrinter.h"


// ============================================================================
// dpmOptional
// ============================================================================
class dpmOptional : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( dpmOptional );
    DATA_PRINTER_CREATOR( dpmOptional, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( dpmOptional, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
        if( ptr != NULL )   return NextPrinter->GetString(ptr);
        else                return wxEmptyString;
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        if( ptr != NULL )   return NextPrinter->GetUnsigned(ptr);
        else                return 0;
    }

    virtual int GetSigned( void* ptr ) 
    { 
        if( ptr != NULL )   return NextPrinter->GetSigned(ptr);
        else                return 0;
    }
};


// ============================================================================
// dpmBool
// ============================================================================
class dpmBool : public dtPrinter
{
public:
    DATA_PRINTER_COMMON( dpmBool );
    DATA_PRINTER_CREATOR( dpmBool, dtPrinter );

    // Special
    DATA_PRINTER_CTOR( dpmBool, dtPrinter )
    {
    }

    // Printing
    virtual wxString GetString( void* ptr ) 
    { 
		return NextPrinter->GetSigned(ptr) == 0 ? wxT("False") : wxT("True");
    }

    virtual dword GetUnsigned( void* ptr ) 
    { 
        return NextPrinter->GetUnsigned(ptr);
    }

    virtual int GetSigned( void* ptr ) 
    { 
        return NextPrinter->GetSigned(ptr);
    }
};


#endif DTPRINTERSMUTATORS_H
// ============================================================================