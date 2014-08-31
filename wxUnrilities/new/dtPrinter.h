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
//  dtPrinter.h
// ============================================================================
#ifndef DTPRINTER_H
#define DTPRINTER_H

//#include "dtComparer.h"

class pkgPackage;


// ============================================================================
// dtPrinter
// ----------------------------------------------------------------------------
// Polymorphic primitive for translating data into string
// ============================================================================
class dtPrinter
{
public:
    // Special
    dtPrinter( const wxString& params=wxEmptyString, pkgPackage* package=NULL )
    :   NextPrinter(NULL)
    {
    }

    dtPrinter( const dtPrinter& c )
    {
        NextPrinter = c.NextPrinter->Clone();
    }

    virtual ~dtPrinter()
    {
        delete NextPrinter;
    }

    virtual dtPrinter* Clone() { return new dtPrinter(*this); }

    // Printing
    virtual wxString Print( void* ptr=NULL ) { return wxEmptyString; }

    // Accessors
    dtPrinter* GetNextPrinter() { return NextPrinter; }

    // Accessors
    virtual wxString GetName() { exNO_IMPL; }
    //virtual wxString GetString( void* ptr ) { return wxEmptyString; }
    //virtual dword GetUnsigned( void* ptr ) { exNO_IMPL; }
    //virtual int GetSigned( void* ptr ) { exNO_IMPL; }
    //virtual void* GetPtr( void* ptr ) { exNO_IMPL; }

    // Mutators
    void SetNextPrinter( dtPrinter* printer ) { NextPrinter = printer; }

protected:
    dtPrinter* NextPrinter;
};


// ============================================================================
// dtPrinter - macros
// ============================================================================
#define DATA_PRINTER_CTOR(BASE,SUPER) \
BASE( const wxString& params = wxEmptyString, pkgPackage* package = NULL ) \
: SUPER(params,package)

#define DATA_PRINTER_COMMON(NAME) \
protected: \
    static const wxString Name; \
public: \
    virtual wxString GetName() { return GetStaticName(); } \
    static wxString GetStaticName() { return Name; } \
    virtual NAME* Clone() { return new NAME(*this); }

#define DATA_PRINTER_CREATOR(NAME,BASE) \
    static BASE* Create( const wxString& params, pkgPackage* package ) \
    { return new NAME(params,package); }


#endif DTPRINTER_H
// ============================================================================