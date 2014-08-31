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
//	dtPrinterFactory.cpp
// ============================================================================
#include "prec.h"
#include "dtPrinterFactory.h"
#include "dtPrinters.h"


// ============================================================================
// dtPrinterFactory - Static
// ============================================================================
dtPrinterFactory& dtPrinterFactory::Global()
{
	// Create on first use
    static dtPrinterFactory p;
    return p;
}


// ============================================================================
// dtPrinterFactory
// ============================================================================
void dtPrinterFactory::Add( const wxString& name, dtPrinterCreator p )
{
    Map[name] = p;
}

dtPrinter* dtPrinterFactory::Create( const wxString& name, const wxString& params, pkgPackage* package )
{
    dtPrinterFactoryMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return ((*it).second)(params,package);

    throw exception(wxString::Format( wxT("Unknown PRINTER requested NAME=%s"), name.c_str() ).c_str());
}

void dtPrinterFactory::OnReload()
{
	Map.clear();
    Add( tprTypeArray::GetStaticName(), &tprTypeArray::Create );
    Add( tprOptional::GetStaticName(), &tprOptional::Create );
    Add( tprFormatText::GetStaticName(), &tprFormatText::Create );
    Add( tprFormatSigned::GetStaticName(), &tprFormatSigned::Create );
    Add( tprFormatUnsigned::GetStaticName(), &tprFormatUnsigned::Create );
    Add( tprFlags::GetStaticName(), &tprFlags::Create );

    Add( tprTypeChild::GetStaticName(), &tprTypeChild::Create );
    Add( tprTypeObjName::GetStaticName(), &tprTypeObjName::Create );
    Add( tprTypeObjRef::GetStaticName(), &tprTypeObjRef::Create );
    Add( tprTypeObjPkg::GetStaticName(), &tprTypeObjPkg::Create );

    Add( tprTypeOffset::GetStaticName(), &tprTypeOffset::Create );
    Add( tprTypeUnsigned::GetStaticName(), &tprTypeUnsigned::Create );
    Add( tprTypeSigned::GetStaticName(), &tprTypeSigned::Create );
    Add( tprTypeString::GetStaticName(), &tprTypeString::Create );

    Add( tprColor::GetStaticName(), &tprColor::Create );

    Add( tprTypeText::GetStaticName(), &tprTypeText::Create );
    Add( tprTypeData::GetStaticName(), &tprTypeData::Create );

    Add( tprBool::GetStaticName(), &tprBool::Create );
}




// ============================================================================