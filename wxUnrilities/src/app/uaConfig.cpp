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
//	uaConfig.cpp
// ============================================================================
#include "prec.h"
#include "uaConfig.h"
#include "unTextPrinterFactory.h"
#include "unTextPrinterPresets.h"
#include "dtComparerFactory.h"
#include "asvNamedSize.h"
#include "asvNamedAlign.h"
#include "asvNamedFormat.h"
#include "asvNamedPath.h"
#include "cfgUnrilities.h"



// ============================================================================
// uaConfig
// ============================================================================
uaConfig::uaConfig()
{
}

void uaConfig::OnReload()
{
    wxFileName filepath(wxT(".\\Config\\UT2004\\Unrilities\\Unrilities.xml"));
    filepath.Normalize();

    try
    {
        SW_DEF(sw);

        GNamedFormat.OnReload();
        GNamedAlign.OnReload();
        GNamedSize.OnReload();
        GNamedPath.OnReload();
        GComparerFactory.OnReload();
        GTextPrinterFactory.OnReload();

        cfgUnrilities unrilities ( filepath );

        GTextPrinterPresets.OnReload();

        SW_LOGF(sw);
    }
    catch (exception& e) 
    {
        throw exception( wxString::Format( wxT("[%s] %s"), filepath.GetFullPath().c_str(), e.what() ) );
    }
}


// ============================================================================