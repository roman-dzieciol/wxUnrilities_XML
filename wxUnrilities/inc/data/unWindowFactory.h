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
//  unWindowFactory.h
// ============================================================================
#ifndef UNWINDOWFACTORY_H
#define UNWINDOWFACTORY_H

class pkgPackage;
class unWindowFactory;

// ============================================================================
// unWindowFactory
// ============================================================================
typedef vector<unWindowFactory*> unWindowFactoryArray;


// ============================================================================
// unWindowFactory
// ============================================================================
class unWindowFactory
{
public:
    // Special
    unWindowFactory( wxWindow* parent, void* clientdata, const wxString& name=wxEmptyString )
    :   Parent(parent)
    ,   ClientData(clientdata)
    ,   Name(name)
    {
    }

    virtual ~unWindowFactory()
    {
        for( unWindowFactoryArray::iterator it=Children.begin(); it!=Children.end(); ++it )
        {
            delete *it;
        }
    }

    // Interface
    virtual wxWindow* Create() { return NULL; }

    // Accessors
    virtual wxWindow* GetWindow() const { return NULL; }
    wxString GetName() const { return Name; }

    // Mutators
    void SetParent( wxWindow* p ) { Parent = p; }

    // Tree
    unWindowFactory* AddChildren( unWindowFactory* p ) { Children.push_back(p); return p; }
    unWindowFactoryArray& GetChildren() { return Children; }

    unWindowFactory* FindFactory( const wxString& name )
    {
        for( unWindowFactoryArray::iterator it=Children.begin(); it!=Children.end(); ++it )
        {
            if( (*it)->GetName().IsSameAs(name) )
                return *it;
        }
        return NULL;
    }

protected:
    wxWindow* Parent; 
    void* ClientData;
    wxString Name;
    unWindowFactoryArray Children;
};





#endif UNWINDOWFACTORY_H
// ============================================================================