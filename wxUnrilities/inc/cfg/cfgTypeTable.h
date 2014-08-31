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
//  cfgTypeTable.h
// ============================================================================
#ifndef CFGTYPETABLE_H
#define CFGTYPETABLE_H

#include "uxHeaders.h"

class pkgPackage;
class unTypeTable;
class unTextColumn;
class dtComparer;
class unTextPrinter;


// ============================================================================
// cfgTypeTable
// ============================================================================
class cfgTypeTable : public uxParser
{
public:
    // State variables
    UXP_VARIABLE( unTypeTable*,   CurrentTable );
    UXP_VARIABLE( unTextColumn*,    CurrentColumn );
    UXP_VARIABLE( dtComparer*,  CurrentSorter );
    UXP_VARIABLE( unTextPrinter*,   BasePrinter );
    UXP_VARIABLE( unTextPrinter*,   CurrentPrinter );
    UXP_VARIABLE( unTextPrinter*,   NextPrinter );
    UXP_VARIABLE( wxString,   PrinterName );
    UXP_VARIABLE( wxString,   PrinterParams );
    UXP_VARIABLE( wxString,   TablePath );
    UXP_VARIABLE( dword,   ColumnWidth );
    UXP_VARIABLE( dword,   ColumnAlign );

public:
    // Special
    cfgTypeTable( wxFileName filename, pkgPackage* package, vector<unTypeTable*>& tables );
};


#endif CFGTYPETABLE_H
// ============================================================================