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
//  uaPackageMenu.h
// ============================================================================
#ifndef UAPACKAGEMENU_H
#define UAPACKAGEMENU_H

#include "uwContextMenu.h"

class pkgPackage;
class unWindowFactory;
class unTextPrinterGroup;
class uwTreeBook;
class wxListEvent;
class unDataTree;
class uwTreeList;


// ============================================================================
// uaMenuExportsList
// ============================================================================
class uaMenuExportsList : public uwContextMenu
{
public:
    uaMenuExportsList( pkgPackage* package, uwTreeBook* treebook );
    virtual void PopupMenu( wxWindow* parent, wxListEvent& event );

protected:
    void OnMenuFindInExportTree( wxCommandEvent& event );
    void OnMenuInspectObject( wxCommandEvent& event );

    unDataTree* GetDataTreeNode( const wxString& name, unDataTree* datatree, uwTreeList* treelist );

protected:
    pkgPackage* Package;
    unWindowFactory* Factory;
    int ItemIndex;
    int ListIndex;
    unTextPrinterGroup* Group;
    uwTreeBook* Treebook;

private:
    DECLARE_EVENT_TABLE()
};


#endif UAPACKAGEMENU_H
// ============================================================================