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
//	uaPackageMenu.cpp
// ============================================================================
#include "prec.h"
#include "uaPackageMenu.h"
#include "uwList.h"
#include "unTextPrinter.h"
#include "pkgPackage.h"
#include "pkgPackageData.h"
#include "pkgPackageFormat.h"
#include "pkgType.h"
#include "asvNamedPath.h"
#include "unWindowFactory.h"
#include "uwTreeBook.h"
#include "uwTreeList.h"
#include "unDataTree.h"
#include "uaFileFormatFactories.h"
#include "uwHexTreeFactory.h"
#include "unTextPrinterPresets.h"


enum
{
    Menu_FindInExportTree = wxID_HIGHEST + 1
,   Menu_InspectObject
};


// ============================================================================
// uaMenuExportsList
// ============================================================================
BEGIN_EVENT_TABLE		( uaMenuExportsList, uwContextMenu )
    EVT_MENU			( Menu_InspectObject,       uaMenuExportsList::OnMenuInspectObject )
    EVT_MENU			( Menu_FindInExportTree,    uaMenuExportsList::OnMenuFindInExportTree )
END_EVENT_TABLE()


// ============================================================================
// uaMenuExportsList
// ============================================================================
uaMenuExportsList::uaMenuExportsList( pkgPackage* package, uwTreeBook* treebook )
:   Package(package)
,   Treebook(treebook)
{
    Append(Menu_FindInExportTree, wxT("&Find in Export Tree"));
    Append(Menu_InspectObject, wxT("&View serialized data"));
}

void uaMenuExportsList::PopupMenu( wxWindow* parent, wxListEvent& event )
{
    uwList* listview = static_cast<uwList*>(parent);

    Group = listview->GetPrinterGroup();
    ListIndex = event.GetIndex();
    ItemIndex = Group->TranslateIndex(ListIndex);

    pkgType* exportitem = Package->GetData().GetRoot().FindTypeByNamePath( GNamedPath.Get(wxT("G.ExportTable.Array")) )->FindTypeByIndex(ItemIndex);
    pkgType* offset = exportitem->FindTypeByNamePath( GNamedPath.Get(wxT("I.ExportItem.Offset")), true );

    wxString pkgcaption = Group->PrintText(ListIndex,3);
    if( !pkgcaption.empty() )
        pkgcaption.append(wxT('.'));

    pkgcaption.append( Group->PrintText(ListIndex,5) );
    if( pkgcaption.empty() )
        pkgcaption = wxT(" ");

    SetTitle( pkgcaption );
    Enable( Menu_InspectObject, offset != NULL );

    listview->PopupMenu(this);
}

void uaMenuExportsList::OnMenuFindInExportTree( wxCommandEvent& event )
{
    guard;
    unguard;
}

void uaMenuExportsList::OnMenuInspectObject( wxCommandEvent& event )
{
    guard;

    // Get data
    pkgType* t_export = Package->GetData().GetRoot().FindTypeByNamePath( GNamedPath(wxT("G.ExportTable.Array")) )->FindTypeByIndex(ItemIndex);
    pkgType* t_off = t_export->FindTypeByNamePath( GNamedPath(wxT("I.ExportItem.Offset")), false );
    pkgType* t_len = t_export->FindTypeByNamePath( GNamedPath(wxT("I.ExportItem.SerialSize")), false );
    pkgType* t_cls = t_export->FindTypeByNamePath( GNamedPath(wxT("I.ExportItem.Class")), false );
    pkgType* t_pkg = t_export->FindTypeByNamePath( GNamedPath(wxT("I.ExportItem.Package")), false );
    pkgType* t_obj = t_export->FindTypeByNamePath( GNamedPath(wxT("I.ExportItem.ObjectName")), false );

    wxString s_obj = GTextPrinterPresets(t_obj->GetTextPrinter(),Package)->GetString(t_obj);
    wxString s_pkg = GTextPrinterPresets(t_pkg->GetTextPrinter(),Package)->GetString(t_pkg);
    wxString s_cls = GTextPrinterPresets(t_cls->GetTextPrinter(),Package)->GetString(t_cls);

    const dword d_offset = t_off->GetUnsigned();
    const dword d_size = t_len->GetUnsigned();

    uwTreeList* treelist = Treebook->GetTree();
    unDataTree* datatree = treelist->GetDataTree();

    // Fix Class name
    if( s_cls.empty() )
        s_cls = wxT("Class");

    // Remove package from class name
    int pos = s_cls.rfind(wxT('.'));
    if( pos != -1 )
        s_cls = s_cls.Mid(pos+1);
    
    // Make caption
    wxString s_caption = s_obj;
    if( !s_pkg.empty() )
    {
        s_caption.append(wxT(" - "));
        s_caption.append(s_pkg);
    }


/*  
    s_caption = s_pkg;
    if( !s_caption.empty() )
    {
        if( s_caption.length() > 16 )
            s_caption = wxString::Format(wxT("(%s..)"),s_caption.Left(12));
        s_caption.append(wxT('.'));
    }

    s_caption.append( s_obj );
    if( s_caption.empty() )
        s_caption = wxT(" ");
*/

    // Navigate to "Preview/Data" in navtree
    unDataTree* dt_preview = GetDataTreeNode( wxT("Preview"), datatree, treelist );
    unDataTree* dt_data = GetDataTreeNode( wxT("Data"), dt_preview, treelist );

    // Create preview node
    unDataTree* dt_object = dt_data->Find( &unDataTree::GetCaption, s_caption );
    if( dt_object == NULL )
    {
        wxLogMessage("ViewSerialized CAPTION=%s CLASS=%s OFFSET=%X, SIZE=%d"
        ,   s_caption.c_str()
        ,   s_cls.c_str()
        ,   d_offset
        ,   d_size
        );

        // Get existing preview data if possible
        pkgType* exporttype = Package->GetData().GetRoot().FindTypeByNamePath( GNamedPath.Get(wxT("G.ExportTable.Array")) )->FindTypeByIndex(ItemIndex)->FindTypeByName(wxT("ExportItem"));
        pkgType* serialtype = Package->GetFormat().GetHeaderTypes().FindTypeByName( wxT("SerialObject") ); 
        pkgType* serialdatatype = exporttype->FindTypeByName(s_caption,true);

        // Jump to offset in file
        Package->GetFile().Seek(d_offset);
        Package->SetReadRange( d_offset, d_size );

        // Create data types
        if( serialdatatype == NULL )
        {
            pkgType* clonedtype = serialtype->Clone(); 
            clonedtype->SetName( s_caption );
            clonedtype->OnRead(*Package,*exporttype);
            serialdatatype = exporttype->FindTypeByName( s_caption, false );
            serialdatatype->SetPrototype(serialtype);
            delete clonedtype;
        }

        // Reset read range
        Package->ResetReadRange();

        unWindowFactory* ftree = new uaFileFormatHeaderData(Treebook,Package,wxT("Tree"),s_caption,exporttype);
        unWindowFactory* fhex = new uwHexTreeFactory(Treebook,ftree,wxT("HexTree"),Package,d_offset,d_size); // LEAK :: fhex
        
        unDataTree* newtree = dt_data->AddChild( new unDataTree(s_caption, ID_IGNORE, false, fhex, NULL) );
        newtree->SetTreeID( treelist->AppendItem( dt_data->GetTreeID(), newtree->GetCaption(), -1, -1, newtree->NewTreeData() ) );
        newtree->SetExpanded(true);
        treelist->SetItemHasChildren( newtree->GetTreeID(), false );
        treelist->ExpandTree( *newtree );
        dt_object = newtree;
    }



    //pkgType* tclass = exportitem->FindTypeByNamePath( GNamedPath.Get(wxT("I.ExportItem.Class")), false );


    treelist->Refresh();

    //wxTreeItemId found = treelist->FindItem(treelist->GetRootItem(),wxT("Preview"),wxTL_MODE_NAV_LEVEL);

    //if( !found.IsOk() )
   //     treelist->AddChildren( new unDataTree(wxT("Preview")) );
//AppendItem( parent.GetTreeID(), tree.GetCaption(), -1, -1, tree.NewTreeData() )
 

    /*unWindowFactory* fserial = Factory->FindFactory( wxT("Serialized Data") );
    if( fserial == NULL )
        fserial = Factory->AddChildren( new unWindowFactory(Parent,Package,wxT("Serialized Data")) );

    unWindowFactory* fobj = fserial->FindFactory( pkgcaption );
    if( fobj == NULL )
        fobj = fserial->AddChildren( new unWindowFactory(Parent,Package,pkgcaption) );*/

    unguard;
}

unDataTree* uaMenuExportsList::GetDataTreeNode( const wxString& name, unDataTree* datatree, uwTreeList* treelist )
{
    unDataTree* tree = datatree->Find( &unDataTree::GetCaption, name );
    if( tree == NULL )
    {
        tree = datatree->AddChild( new unDataTree(name, ID_IGNORE, false, NULL, NULL) );
        tree->SetTreeID( treelist->AppendItem( datatree->GetTreeID(), tree->GetCaption(), -1, -1, tree->NewTreeData() ) );
        tree->SetExpanded(true);
        treelist->SetItemHasChildren( tree->GetTreeID(), true );
        treelist->ExpandTree( *tree );
    }
    return tree;
}


// ============================================================================