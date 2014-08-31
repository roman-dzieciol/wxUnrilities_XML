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
//  dtPrinterFactory.h
// ============================================================================
#ifndef DTPRINTERFACTORY_H
#define DTPRINTERFACTORY_H

class dtPrinter;
class pkgPackage;


// ============================================================================
// dtPrinterFactory
// ----------------------------------------------------------------------------
// Holds printer name and printer pointer
// Creates printer instances
// ----------------------------------------------------------------------------
// Global reference: GPrinterFactory 
// ============================================================================
class dtPrinterFactory
{
private:
    typedef dtPrinter* (*dtPrinterCreator)( const wxString& params, pkgPackage* package );
    WX_DECLARE_STRING_HASH_MAP( dtPrinterCreator, dtPrinterFactoryMap );

public:
    static dtPrinterFactory& Global();
    #define GPrinterFactory dtPrinterFactory::Global() 

public:
    void Add( const wxString& name, dtPrinterCreator p );
    dtPrinter* Create( const wxString& name, const wxString& params, pkgPackage* package );

    void OnReload();

protected:
    dtPrinterFactoryMap Map;
};


#endif DTPRINTERFACTORY_H
// ============================================================================