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
//  uwListFactory.h
// ============================================================================
#ifndef UWLISTFACTORY_H
#define UWLISTFACTORY_H

#include "unWindowFactory.h"

class uwList;
class uwContextMenu;


// ============================================================================
// uwListFactory
// ============================================================================
class uwListFactory : public unWindowFactory
{
public:
    // Special
    uwListFactory( wxWindow* parent, void* clientdata, const wxString& name=wxEmptyString, uwContextMenu* menu=NULL )
    :   unWindowFactory(parent,clientdata,name)
    ,   Window(NULL)
    ,   Menu(menu)
    {
    }

    virtual ~uwListFactory();

    // Accessors
    uwList* GetList() { return Window; }

    // WindowFactory
    virtual wxWindow* Create();

    // ListFactory
    virtual void AddColumns() = 0;
    virtual void AddData() = 0;

protected:
    uwList* Window;
    uwContextMenu* Menu;
};


#endif UWLISTFACTORY_H
// ============================================================================