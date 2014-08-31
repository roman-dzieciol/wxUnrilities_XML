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
//  uwHexTreeFactory.h
// ============================================================================
#ifndef UWHEXTREEFACTORY_H
#define UWHEXTREEFACTORY_H

#include "unWindowFactory.h"

class uwHexTree;
class pkgPackage;


// ============================================================================
// uwHexTreeFactory
// ============================================================================
class uwHexTreeFactory : public unWindowFactory
{
public:
    // Special
    uwHexTreeFactory
    (   wxWindow* parent
    ,   unWindowFactory* clientdata
    ,   const wxString& name
    ,   pkgPackage* package
    ,   wxFileOffset offset 
    ,   dword size 
    )
    :   unWindowFactory(parent,clientdata,name)
    ,   HexTree(NULL)
    ,   Factory(clientdata)
    ,   Package(package)
    ,   DataOffset(offset)
    ,   DataSize(size)
    {
    }

    virtual ~uwHexTreeFactory();

    // Factory
    virtual wxWindow* Create();

protected:
    uwHexTree* HexTree; 
    unWindowFactory* Factory;
    pkgPackage* Package;
    wxFileOffset DataOffset;
    dword DataSize;
};


#endif UWHEXTREEFACTORY_H
// ============================================================================