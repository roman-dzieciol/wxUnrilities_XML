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
//  cfgUnrilities.h
// ============================================================================
#ifndef CFGUNRILITIES_H
#define CFGUNRILITIES_H

#include "uxHeaders.h"

class unTextPrinterPreset;


// ============================================================================
// cfgUnrilities
// ============================================================================
class cfgUnrilities : public uxParser
{
public:
    // state variables
    UXP_VARIABLE( dword, SizeWidth );
    UXP_VARIABLE( wxString, FormatString );
    UXP_VARIABLE( wxString, PathName );
    UXP_VARIABLE( unTextPrinterPreset*, PrinterPreset );
    UXP_VARIABLE( wxString, PrinterName );
    UXP_VARIABLE( wxString, PrinterParam );

public:
    cfgUnrilities( const wxFileName& filename );
};



#endif CFGUNRILITIES_H
// ============================================================================