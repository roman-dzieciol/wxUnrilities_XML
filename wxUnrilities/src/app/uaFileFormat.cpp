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
//	uaFileFormat.cpp
// ============================================================================
#include "prec.h"
#include "uaFileFormat.h"
#include "uwNotebook.h"
#include "uwTreeBookFactory.h"
#include "uaFileFormatFactories.h"
#include "uwTreeList.h"
#include "uwNotebook.h"
#include "uwTreeBookFactory.h"
#include "uaFileFormatFactories.h"
#include "uwTreeList.h"
#include "cfgTypeTable.h"
#include "uwListFactory.h"
#include "unTextPrinter.h"
#include "uwList.h"
#include "unTypeTable.h"
#include "pkgPackage.h"
#include "pkgPackageFormat.h"
#include "pkgPackageData.h"
#include "uaPackageMenu.h"
#include "asvNamedSize.h"


// ============================================================================
// uaTypeTableFactory
// ============================================================================
class uaTypeTableFactory : public uwListFactory
{
public:
    uaTypeTableFactory( wxWindow* parent, pkgPackage* clientdata, const wxString& name, unTypeTable* table, uwContextMenu* menu )
    :   uwListFactory(parent,clientdata,name,menu)
    ,   Package(clientdata)
    ,   Table(table)
    {
    }

    ~uaTypeTableFactory()
    {
        delete Table;
    }

    virtual void AddColumns()
    {
    }

    virtual void AddData()
    {
        Window->SetPrinterGroup( Table );
        Window->SetItemCount( Table->GetItemCount(Package) );
    }

protected:
    pkgPackage* Package;
    unTypeTable* Table;
};


// ============================================================================
// uaFileFormat - Event Table
// ============================================================================
BEGIN_EVENT_TABLE(uaFileFormat, uwTreeBook)
    EVT_COMMAND (wxID_ANY, unEVT_NOTEPAGE_CHANGED, uaFileFormat::OnNotePageChanged)
    EVT_COMMAND (wxID_ANY, unEVT_NOTEPAGE_ADD, uaFileFormat::OnNotePageAdd)
    EVT_COMMAND (wxID_ANY, unEVT_TREEPAGE_CHANGED, uaFileFormat::OnTreePageChanged)
END_EVENT_TABLE()


// ============================================================================
// uaFileFormat - Special
// ============================================================================
uaFileFormat::uaFileFormat
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	uwTreeBook(parent,id,pos,size,style,name)
{
}


// ============================================================================
// uaFileFormat - Events
// ============================================================================
void uaFileFormat::OnNotePageChanged( wxCommandEvent& event )
{
    //wxLogMessage( "OnNotePageChanged: %s", event.GetString().c_str() );
}

void uaFileFormat::OnTreePageChanged( wxCommandEvent& event )
{
    guard;
    //wxLogMessage( "OnTreePageChanged: %s", event.GetString().c_str() );
    if( event.GetClientData() == NULL )
        return;

    unWindowFactory* factory = static_cast<unWindowFactory*>(event.GetClientData());
    wxWindow* window = factory->Create();

    if( window == NULL || window == GetWindow2() )
        return;
    
    if( IsSplit() )
        Unsplit( GetWindow2() );

    SplitVertically( window, GNamedSize(wxT("NavTree")) );
    unguard;
}

void uaFileFormat::OnNotePageAdd( wxCommandEvent& event )
{
    guard;
    //wxLogMessage( "OnNotePageAdd: %s", event.GetString().c_str() );

    // Get package
    pkgPackage* package = static_cast<pkgPackage*>(event.GetClientData());


    // ------------------------------------------------------------------------
    // File Format 
    // ------------------------------------------------------------------------
    unWindowFactory* fileformat = AddFactory(new unWindowFactory(this,package,wxT("File Format")));
    fileformat->AddChildren(new uaFileFormatBaseTypes(this,package));
    fileformat->AddChildren(new uaFileFormatFlowTypes(this,package));
    fileformat->AddChildren(new uaFileFormatFlags(this,package));
    fileformat->AddChildren(new uaFileFormatFunctions(this,package));
    fileformat->AddChildren(new uaFileFormatHeader(this,&package->GetFormat().GetHeaderTypes(),wxT("Composites")));
    //fileformat->AddChildren(new uaFileFormatHeader(this,&package->GetFormat().GetHeaderTypes(),wxT("Object Composites")));


    // ------------------------------------------------------------------------
    // Package
    // ------------------------------------------------------------------------
    unWindowFactory* pkg = AddFactory(new unWindowFactory(this,package,wxT("Package")));

    // ------------------------------------------------------------------------
    // Package Header
    // ------------------------------------------------------------------------
    unWindowFactory* pkgdata = pkg->AddChildren(new unWindowFactory(this,package,wxT("Header")));
    pkgdata->AddChildren(new uaFileFormatPackages(this,package));
    pkgdata->AddChildren(new uaFileFormatHeaderData(this,package,wxT("Tree"),wxT("Header"),&package->GetData().GetRoot()));



    // ------------------------------------------------------------------------
    // Package Tables
    // ------------------------------------------------------------------------
    unWindowFactory* pkgtables = pkg->AddChildren(new unWindowFactory(this,package,wxT("Tables")));

    // Load tables
    vector<unTypeTable*> tables;
    cfgTypeTable parser( wxFileName(wxT(".\\Config\\UT2004\\Unrilities\\TypeTables.xml")), package, tables );

    unTypeTable* imports = NULL;
    unTypeTable* exports = NULL;

    // Add factories
    for( vector<unTypeTable*>::iterator it=tables.begin(); it!=tables.end(); ++it )
    {
        unTypeTable* table = static_cast<unTypeTable*>(*it);

        if( wxString(wxT("Imports")).IsSameAs(table->GetName()) )   
        {
            imports = table;
            pkgtables->AddChildren( new uaTypeTableFactory(this,package,table->GetName(),table,new uaMenuExportsList(package,this)) );
        }
        else if( wxString(wxT("Exports")).IsSameAs(table->GetName()) )
        {
            exports = table;
            pkgtables->AddChildren( new uaTypeTableFactory(this,package,table->GetName(),table,new uaMenuExportsList(package,this)) );
        }
        else
        {
            pkgtables->AddChildren( new uaTypeTableFactory(this,package,table->GetName(),table,NULL) );
        }

    }

    // ------------------------------------------------------------------------
    // Package Trees
    // ------------------------------------------------------------------------
    //unWindowFactory* pkgtrees = pkg->AddChildren(new unWindowFactory(this,package,wxT("Trees")));

    //if( exports != NULL )
    //{
    //    unWindowFactory* pkgext = pkgtrees->AddChildren(new unWindowFactory(this,package,wxT("Exports")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Package")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Package (Data)")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Package (Scripts)")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Class")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Class (Data)")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Class (Scripts)")));
    //    pkgext->AddChildren(new unWindowFactory(this,package,wxT("Inheritance (Scripts)")));
    //}

    //if( imports != NULL )
    //{
    //    unWindowFactory* pkgimt = pkgtrees->AddChildren(new unWindowFactory(this,package,wxT("Imports")));
    //    pkgimt->AddChildren(new uaImportTreeFactory(this,package,wxT("Package"),imports));
    //    pkgimt->AddChildren(new unWindowFactory(this,package,wxT("Class")));
    //}


    // ------------------------------------------------------------------------
    // Create nav tree
    // ------------------------------------------------------------------------
    TreeFactory = new uwTreeBookFactory(this,Factories);
    Tree = static_cast<uwTreeList*>(TreeFactory->Create());
    Tree->ExpandRange(Tree->GetRootItem(),3);

    // ------------------------------------------------------------------------
    // Init
    // ------------------------------------------------------------------------
    Initialize(Tree);
    SplitVertically( Blank, GNamedSize(wxT("NavTree")) );
    unguard;
}




// ============================================================================