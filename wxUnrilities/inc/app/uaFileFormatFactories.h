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
//  uaFileFormatFactories.h
// ============================================================================
#ifndef UAFILEFORMATFACTORIES_H
#define UAFILEFORMATFACTORIES_H

#include "unWindowFactory.h"
#include "uwTreeListFactory.h"
#include "uwListFactory.h"

class pkgTypeGroup;
class unDataTree;
class unTextPrinter;
class unTextPrinterGroup;
class pkgType;
class pkgPackage;
class unTypeTable;


// ============================================================================
// uaFileFormatFlags
// ============================================================================
class uaFileFormatFlags : public uwTreeListFactory
{
public:
    uaFileFormatFlags( wxWindow* parent, void* clientdata, const wxString& name=wxT("Flags") )
    :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual void AddColumns();
    virtual void AddData();
};


// ============================================================================
// uaFileFormatFunctions
// ============================================================================
class uaFileFormatFunctions : public uwTreeListFactory
{
public:
    uaFileFormatFunctions( wxWindow* parent, void* clientdata, const wxString& name=wxT("Functions") )
    :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual wxWindow* Create();
    virtual void AddColumns();
    virtual void AddData();
};


// ============================================================================
// uaFileFormatHeader
// ============================================================================
class uaFileFormatHeader : public uwTreeListFactory
{
public:
    uaFileFormatHeader( wxWindow* parent, void* clientdata, const wxString& name=wxT("Composites") )
    :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual wxWindow* Create();
    virtual void AddColumns();
    virtual void AddData();
    void AddTypeChildren( pkgType* type, unDataTree* tree, unTextPrinterGroup* printer, int limit );
};


// ============================================================================
// uaFileFormatBaseTypes
// ============================================================================
class uaFileFormatBaseTypes : public uwTreeListFactory
{
public:
    uaFileFormatBaseTypes( wxWindow* parent, void* clientdata, const wxString& name=wxT("Primitives") )
        :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual void AddColumns();
    virtual void AddData();
};


// ============================================================================
// uaFileFormatFlowTypes
// ============================================================================
class uaFileFormatFlowTypes : public uwTreeListFactory
{
public:
    uaFileFormatFlowTypes( wxWindow* parent, void* clientdata, const wxString& name=wxT("Flow") )
    :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual void AddColumns();
    virtual void AddData();
};


// ============================================================================
// uaFileFormatHeaderData
// ============================================================================
class uaFileFormatHeaderData : public uwTreeListFactory
{
public:
    uaFileFormatHeaderData( wxWindow* parent, void* clientdata, const wxString& name=wxEmptyString, const wxString& pathname=wxEmptyString
        , pkgType* root=NULL )
    :   uwTreeListFactory(parent,clientdata,name)
    ,   PathName(pathname)
    ,   Root(root)
    {
    }

protected:
    WX_DECLARE_STRING_HASH_MAP( unTextPrinterGroup*, unTextPrinterGroupMap );

protected:
    virtual void AddColumns();
    virtual void AddData();
    
    void AddTypeChildren( pkgType* parent, unDataTree* tree, int limit );
    void AddInlineArrayItem( pkgType* parent, unDataTree* tree, int limit, int counter );
    unTextPrinterGroup* GetPrinterGroup( pkgType& type, bool binline=false );

protected:
    unTextPrinterGroup* thdr_printer;
    unTextPrinterGroup* tdt_printer;
    unTextPrinterGroup* array_printer;
    wxString PathName;
    unTextPrinterGroupMap Map;
    pkgType* Root;
};


// ============================================================================
// uaFileFormatPackages
// ============================================================================
class uaFileFormatPackages : public uwTreeListFactory
{
public:
    uaFileFormatPackages( wxWindow* parent, void* clientdata, const wxString& name=wxT("Info") )
    :   uwTreeListFactory(parent,clientdata,name)
    {
    }

    virtual void AddColumns();
    virtual void AddData();

};



// ============================================================================
// uaImportTreeFactory
// ============================================================================
class uaImportTreeFactory : public unWindowFactory
{
public:
    uaImportTreeFactory( wxWindow* parent, pkgPackage* clientdata, const wxString& name, unTypeTable* table );
    virtual ~uaImportTreeFactory();

    virtual wxWindow* Create();
    virtual void AddColumns();
    virtual void AddData();

    unTextPrinterGroup* AddPrinter( unTextPrinterGroup* p )
    {
        Printers.push_back(p);
        return p;
    }

protected:
    pkgPackage* Package;
    unTypeTable* Table;
    uwTreeList* Tree; 
    unDataTree* DataTree;
    vector<unTextPrinterGroup*> Printers;
};



#endif UAFILEFORMATFACTORIES_H
// ============================================================================