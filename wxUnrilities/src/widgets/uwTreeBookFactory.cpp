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
//	uwTreeBookFactory.cpp
// ============================================================================
#include "prec.h"
#include "uwTreeBookFactory.h"
#include "uwTreeList.h"
#include "unDataTree.h"
#include "tpDataTree.h"
#include "asvNamedSize.h"

namespace tlc = uwTreeListColumnInfos;



// ============================================================================
// uwTreeBookFactory
// ============================================================================
void uwTreeBookFactory::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxEmptyString, GNamedSize(wxT("NavTree"))-4 ) );
    Tree->SetMainColumn(0);
}

void uwTreeBookFactory::AddData()
{
    // create root
    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );
    bVerticalAlign = true;

    // create printers
    unTextPrinterGroup* printer = AddPrinter( new unTextPrinterGroup );
    printer->Add( new_tpFormat( wxT("%s"), &unWindowFactory::GetName ) );

    // create data tree
    AddTree( Factories, DataTree, printer );
}

void uwTreeBookFactory::AddTree( unWindowFactoryArray& array, unDataTree* parent, unTextPrinterGroup* printer )
{
    for( unWindowFactoryArray::iterator it=array.begin(); it!=array.end(); ++it )
    {
        unWindowFactory& factory = *static_cast<unWindowFactory*>(*it);
        unDataTree* tree = parent->AddChild( new unDataTree( factory.GetName(), ID_IGNORE, false, &factory, printer ) );	
        AddTree( factory.GetChildren(), tree, printer );
    }
}

// ============================================================================