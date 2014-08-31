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
//	dtComparerFactory.cpp
// ============================================================================
#include "prec.h"
#include "dtComparerFactory.h"
#include "dtComparers.h"


// ============================================================================
// dtComparerFactory - Static
// ============================================================================
dtComparerFactory& dtComparerFactory::Global()
{
	// Create on first use
    static dtComparerFactory p;
    return p;
}


// ============================================================================
// dtComparerFactory
// ============================================================================
void dtComparerFactory::Add( const wxString& name, dtComparerCreator p )
{
    Map[name] = p;
}

dtComparer* dtComparerFactory::Create( const wxString& name )
{
    dtComparerFactoryMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return ((*it).second)();

    throw exception(wxString::Format( wxT("Unknown COMPARER requested NAME=%s"), name.c_str() ).c_str());
}

void dtComparerFactory::OnReload()
{
	Map.clear();
    Add( cmpNumber::Name, &cmpNumber::Create );
    Add( cmpText::Name, &cmpText::Create );
    Add( cmpTextNoCase::Name, &cmpTextNoCase::Create );
}


// ============================================================================