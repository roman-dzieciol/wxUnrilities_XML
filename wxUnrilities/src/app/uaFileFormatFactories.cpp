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
//	uaFileFormatFactories.cpp
// ============================================================================
#include "prec.h"
#include "uaFileFormatFactories.h"
#include "uwTreeList.h"
#include "unDataTree.h"
#include "tpDataTree.h"
#include "pkgPackage.h"
#include "pkgPackageFormat.h"
#include "pkgPackageData.h"

#include "unTextPrinter.h"
#include "unTextPrinterPresets.h"

#include "tpList.h"
#include "uwList.h"

#include "unTypeTable.h"

#include "pkgFlags.h"
#include "pkgNativeFunc.h"
#include "pkgType.h"
#include "pkgTypeRegistry.h"

#include "uwAlignList.h"
namespace alw = uwAlignListWidths;
namespace tlc = uwTreeListColumnInfos;





// ============================================================================
// uaFileFormatFlags
// ============================================================================
void uaFileFormatFlags::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Bits"), tlc::FlagBits ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Description"), tlc::Desc ) );
    Tree->SetMainColumn(1);
}

void uaFileFormatFlags::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* group_print = AddPrinter( new unTextPrinterGroup );
    group_print->AddEmpty();
    group_print->Add( new_tpFormat( wxT("%s"), &pkgFlagGroup::GetName ) );
    group_print->AddEmpty();

    unTextPrinterGroup* flag_print = AddPrinter( new unTextPrinterGroup );
    flag_print->Add( new_tpFormat( wxT("0x%.8X"), &pkgFlagData::GetData ) );
    flag_print->Add( new_tpFormat( wxT("%s"), &pkgFlagData::GetName ) );
    flag_print->Add( new_tpFormat( wxT("%s"), &pkgFlagData::GetDesc ) );

    pkgFlags& pkgflags = format.GetFlags();
    TFlagGroupsList& group_array = pkgflags.GetFlagGroupsList();
    for( TFlagGroupsList::iterator it=group_array.begin(); it!=group_array.end(); ++it )
    {
        pkgFlagGroup& group_it = *static_cast<pkgFlagGroup*>(*it);
        unDataTree& group_tree = *DataTree->AddChild( new unDataTree( group_it.GetName(), ID_IGNORE, false, &group_it, group_print ) );	

        TFlagsByData& flag_array = group_it.GetFlagsByData();
        for( TFlagsByData::iterator it=flag_array.begin(); it!=flag_array.end(); ++it )
        {
            pkgFlagData& flag_it = *static_cast<pkgFlagData*>((*it).second);
            unDataTree& flag_tree = *group_tree.AddChild( new unDataTree( flag_it.GetName(), ID_IGNORE, false, &flag_it, flag_print ) );	
        }
    }
}


// ============================================================================
// uaFileFormatFunctions
// ============================================================================
void uaFileFormatFunctions::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Index"), tlc::Hex ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Friendly Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Priority"), tlc::Hex ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Flags"), tlc::Desc ) );
    Tree->SetMainColumn(1);
}

void uaFileFormatFunctions::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* func_print = AddPrinter( new unTextPrinterGroup );
    func_print->Add( new_tpFormat( wxT("0x%.4X"), &pkgNativeFunc::GetIndex ) );
    func_print->Add( new_tpFormat( wxT("%s"), &pkgNativeFunc::GetName ) );
    func_print->Add( new_tpFormat( wxT("%s"), &pkgNativeFunc::GetFriendlyName ) );
    func_print->Add( new_tpFormat( wxT("0x%.2X"), &pkgNativeFunc::GetPriority ) );
    func_print->Add( new_tpFlags( &pkgNativeFunc::GetFlags, format.GetFlags().FindGroup(wxT("Function")) ) );

    unTextPrinterGroup* print_fpkg = AddPrinter( new unTextPrinterGroup );
    print_fpkg->AddEmpty();
    print_fpkg->Add( new_tpFormat( wxT("%s"), &pkgNativeFuncPackage::GetName ) );
    print_fpkg->AddEmpty();
    print_fpkg->AddEmpty();
    print_fpkg->AddEmpty();

    unTextPrinterGroup* fcls_print = AddPrinter( new unTextPrinterGroup );
    fcls_print->AddEmpty();
    fcls_print->Add( new_tpFormat( wxT("%s"), &pkgNativeFuncClass::GetName ) );
    fcls_print->AddEmpty();
    fcls_print->AddEmpty();
    fcls_print->AddEmpty();

    // get functions
    pkgNativeFuncMap& pkg_funcs = format.GetFunctions();

    // get function packages
    pkgNativeFuncPackage::Array& fpkg_array = pkg_funcs.GetPackages();
    for( pkgNativeFuncPackage::Array::iterator it=fpkg_array.begin(); it!=fpkg_array.end(); ++it )
    {
        // add function packages
        pkgNativeFuncPackage& fpkg_it = *static_cast<pkgNativeFuncPackage*>(*it);
        unDataTree* fpkg_tree = DataTree->AddChild( new unDataTree( fpkg_it.GetName(), ID_IGNORE, false, &fpkg_it, print_fpkg ) );	

        // get function classes
        pkgNativeFuncClass::Array& fcls_array = fpkg_it.GetClasses();
        for( pkgNativeFuncClass::Array::iterator it=fcls_array.begin(); it!=fcls_array.end(); ++it )
        {
            // add function classes
            pkgNativeFuncClass& fcls_it = *static_cast<pkgNativeFuncClass*>(*it);
            unDataTree* fcls_tree = fpkg_tree->AddChild( new unDataTree( fcls_it.GetName(), ID_IGNORE, false, &fcls_it, fcls_print ) );	

            // get functions
            pkgNativeFunc::Array& func_array = fcls_it.GetFunctions();
            for( pkgNativeFunc::Array::iterator it=func_array.begin(); it!=func_array.end(); ++it )
            {
                // add functions
                pkgNativeFunc& func_it = *static_cast<pkgNativeFunc*>(*it);
                unDataTree* func_tree = fcls_tree->AddChild( new unDataTree( func_it.GetName(), ID_IGNORE, false, &func_it, func_print ) );	
            }
        }
    }
}

wxWindow* uaFileFormatFunctions::Create()
{
    uwTreeListFactory::Create();
    Tree->ExpandRange(Tree->GetRootItem(),1);
    return Tree;
}


// ============================================================================
// uaFileFormatHeader
// ============================================================================
void uaFileFormatHeader::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::LongName ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Type"), tlc::ShortName ) );
    //Tree->AddColumn( uwTreeListColumnInfo( wxT("Desc"), tlc::Desc ) );
    Tree->SetMainColumn(0);
}

void uaFileFormatHeader::AddData()
{
    // get header types
    pkgType& pkg_thdr = *static_cast<pkgType*>(ClientData);

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* tdt_printer = AddPrinter( new unTextPrinterGroup );
    tdt_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetName ) );
    tdt_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetTypeName ) );
    //tdt_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetDesc ) );

    unTextPrinterGroup* thdr_printer = AddPrinter( new unTextPrinterGroup );
    thdr_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetName ) );
    thdr_printer->AddEmpty();
    //thdr_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetDesc ) );



    // get header types array
    pkgTypeArray& thdr_array = pkg_thdr.GetTypeArray();
    for( pkgTypeArray::iterator it=thdr_array.begin(); it!=thdr_array.end(); ++it )
    {
        // add header types
        pkgType& thdr_it = *static_cast<pkgType*>(*it);
        unDataTree* thdr_tree = DataTree->AddChild( new unDataTree( thdr_it.GetName(), ID_IGNORE, false, &thdr_it, thdr_printer ) );	
        
        //
        if( thdr_it.HasChildren() )
        {
            AddTypeChildren(&thdr_it, thdr_tree, tdt_printer, 64);
        }
    }
}

void uaFileFormatHeader::AddTypeChildren( pkgType* type, unDataTree* tree, unTextPrinterGroup* printer, int limit )
{
    pkgTypeArray& child_array = type->GetTypeArray();
    for( pkgTypeArray::iterator it=child_array.begin(); it!=child_array.end(); ++it )
    {
        pkgType& child_it = *static_cast<pkgType*>(*it);

        // add node
        unDataTree* child_tree = tree->AddChild( new unDataTree( child_it.GetName(), ID_IGNORE, false, &child_it, printer ) );	
        
        // expand automatically
        if( child_tree->GetText(1).IsSameAs(wxT("offset"))
        ||  child_tree->GetText(1).IsSameAs(wxT("seek")) 
        ||  child_tree->GetText(1).IsSameAs(wxT("array")) 
        ||  child_tree->GetText(1).IsSameAs(wxT("if")) 
        //||  child_tree->GetText(1).IsSameAs(wxT("OBJECT")) 
        )
        {
            child_tree->SetAutoExpand(true);
        }

        // inline OBJECT types
        if( child_tree->GetText(1).IsSameAs(wxT("OBJECT")) )
        {
            pkgType& pkg_thdr = *static_cast<pkgType*>(ClientData);
            pkgType* obj_type = pkg_thdr.FindTypeByName(child_tree->GetText(0));
            if( obj_type->HasChildren() && limit != 0 )
            {
                AddTypeChildren(obj_type, child_tree, printer, limit-1);
                continue;
            }
        }

        // add children
        if( child_it.HasChildren() && limit != 0 )
        {
            AddTypeChildren(&child_it, child_tree, printer, limit-1);
        }
    }
}

wxWindow* uaFileFormatHeader::Create()
{
    uwTreeListFactory::Create();
    //Tree->ExpandRange(Tree->GetRootItem(),1);
    return Tree;
}



// ============================================================================
// uaFileFormatBaseTypes
// ============================================================================
void uaFileFormatBaseTypes::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Description"), tlc::Desc ) );
    Tree->SetMainColumn(0);
}

void uaFileFormatBaseTypes::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* type_print = AddPrinter( new unTextPrinterGroup );
    type_print->Add( new_tpFormat( wxT("%s"), &pkgType::GetTypeName ) );
    type_print->Add( new_tpFormat( wxT("%s"), &pkgType::GetDesc ) );

    pkgType& type_group = format.GetBaseTypes();
    pkgTypeArray& type_array = type_group.GetTypeArray();

    for( pkgTypeArray::iterator it=type_array.begin(); it!=type_array.end(); ++it )
    {
        pkgType& type_it = *static_cast<pkgType*>(*it);
        unDataTree& type_tree = *DataTree->AddChild( new unDataTree( type_it.GetName(), ID_IGNORE, false, &type_it, type_print ) );	

    }
}


// ============================================================================
// uaFileFormatFlowTypes
// ============================================================================
void uaFileFormatFlowTypes::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Description"), tlc::Desc ) );
    Tree->SetMainColumn(0);
}

void uaFileFormatFlowTypes::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* type_print = AddPrinter( new unTextPrinterGroup );
    type_print->Add( new_tpFormat( wxT("%s"), &pkgType::GetTypeName ) );
    type_print->Add( new_tpFormat( wxT("%s"), &pkgType::GetDesc ) );

    pkgType& type_group = format.GetFlowTypes();
    pkgTypeArray& type_array = type_group.GetTypeArray();

    for( pkgTypeArray::iterator it=type_array.begin(); it!=type_array.end(); ++it )
    {
        pkgType& type_it = *static_cast<pkgType*>(*it);
        unDataTree& type_tree = *DataTree->AddChild( new unDataTree( type_it.GetName(), ID_IGNORE, false, &type_it, type_print ) );	
    }
}


// ============================================================================
// uaFileFormatHeaderData
// ============================================================================
void uaFileFormatHeaderData::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Offset"), tlc::Hex ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::LongName ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Type"), tlc::ShortName ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Data Text"), tlc::Hex ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Full Text"), tlc::Desc ) );
    Tree->SetMainColumn(1);
}

void uaFileFormatHeaderData::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();
    pkgPackageData& pkgdata = package.GetData();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    tdt_printer = AddPrinter( new unTextPrinterGroup );
    tdt_printer->Add( new_tpFormat( wxT("0x%.8x"), &pkgType::GetOffset ) );
    tdt_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetName ) );
    tdt_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetTypeName ) );
    tdt_printer->AddEmpty();
    tdt_printer->AddEmpty();

    thdr_printer = AddPrinter( new unTextPrinterGroup );
    thdr_printer->Add( new_tpFormat( wxT("0x%.8x"), &pkgType::GetOffset ) );
    thdr_printer->Add( new_tpFormat( wxT("%s"), &pkgType::GetName ) );
    thdr_printer->AddEmpty();
    thdr_printer->AddEmpty();
    thdr_printer->AddEmpty();

    array_printer = AddPrinter( new unTextPrinterGroup );
    array_printer->Add( new tpDataTreeTypeOffset() );
    array_printer->Add( new_tpAccessor( &unDataTree::GetCaption ) );
    array_printer->AddEmpty();
    array_printer->AddEmpty();
    array_printer->AddEmpty();


    pkgType& thdr = *Root->FindTypeByNamePath(PathName);
    unDataTree* thdr_tree = DataTree->AddChild( new unDataTree( thdr.GetName(), ID_IGNORE, false, &thdr, thdr_printer ) );
    thdr_tree->SetAutoExpand(true);	

    if( thdr.HasChildren() )
    {
        AddTypeChildren(&thdr, thdr_tree, 64);
    }

}


void uaFileFormatHeaderData::AddTypeChildren( pkgType* parent, unDataTree* tree, int limit )
{
    pkgTypeArray& child_array = parent->GetTypeArray();

    dword len = child_array.size();
    const dword len_group = 50;

    // group when child count too big
    if( len > len_group )
    {
        dword counter = 0;
        unDataTree* group_tree = NULL;

        for( pkgTypeArray::iterator it=child_array.begin(); it!=child_array.end(); ++it )
        {
            pkgType& child_it = *static_cast<pkgType*>(*it);

            if( counter++ % len_group == 0 )
            {
                dword cmin = (counter-(counter%len_group));
                dword cmax = cmin+len_group-1;
                wxString caption = wxString::Format(wxT("%s[%d-%d]"),parent->GetName().c_str(),cmin,cmax);
                group_tree = tree->AddChild( new unDataTree( caption, ID_IGNORE, false, NULL, array_printer ) );
                group_tree->SetClientData(group_tree);
            }

            if( child_it.GetTypeArray().size() != 1 || (child_it.HasChildren() && child_it.GetTypeArray().at(0)->HasChildren()) )
            {
                unTextPrinterGroup* printer = GetPrinterGroup( child_it );

                // add node
                unDataTree* child_tree = group_tree->AddChild( new unDataTree( child_it.GetName(), ID_IGNORE, false, &child_it, printer ) );	

                // expand OBJECT types
                if( child_it.GetTypeName().IsEmpty() )
                {
                    child_tree->SetAutoExpand(true);
                }

                // add children
                if( child_it.HasChildren() && limit != 0 )
                {
                    AddTypeChildren(&child_it, child_tree, limit-1);
                }
            }
            else
            {
                // inline array item with single child
                if( limit != 0 )
                {
                    AddInlineArrayItem(&child_it, group_tree, limit-1, counter);
                }
            }

        }
    }
    else
    {

        for( pkgTypeArray::iterator it=child_array.begin(); it!=child_array.end(); ++it )
        {
            pkgType& child_it = *static_cast<pkgType*>(*it);

            unTextPrinterGroup* printer = GetPrinterGroup( child_it );

            // add node
            unDataTree* child_tree = tree->AddChild( new unDataTree( child_it.GetName(), ID_IGNORE, false, &child_it, printer ) );	

            // expand OBJECT types
            if( child_it.GetTypeName().IsEmpty() )
            {
                child_tree->SetAutoExpand(true);
            }

            // add children
            if( child_it.HasChildren() && limit != 0 )
            {
                AddTypeChildren(&child_it, child_tree, limit-1);
            }
        }
    }
}


void uaFileFormatHeaderData::AddInlineArrayItem( pkgType* parent, unDataTree* tree, int limit, int counter )
{
    pkgTypeArray& child_array = parent->GetTypeArray();
    dword len = child_array.size(); // ASSERT :: 1
    pkgType& child_it = *child_array.at(0);

    unTextPrinterGroup* printer = GetPrinterGroup( child_it, true );


    // add node
    unDataTree* child_tree = tree->AddChild( new unDataTree( child_it.GetName(), ID_IGNORE, false, &child_it, printer ) );	

    // expand OBJECT types
    if( child_it.GetTypeName().IsEmpty() )
    {
        child_tree->SetAutoExpand(true);
    }

    // add children
    if( child_it.HasChildren() && limit != 0 )
    {
        AddTypeChildren(&child_it, child_tree, limit-1);
    }
}

unTextPrinterGroup* uaFileFormatHeaderData::GetPrinterGroup( pkgType& type, bool binline )
{
    wxString dname = type.GetDataPrinter();
    wxString fname = type.GetTextPrinter();
    //wxString dname = type.GetPrototype()->GetDataPrinter();
    //wxString fname = type.GetPrototype()->GetTextPrinter();
    wxString name = dname + fname + wxString::Format("%d",static_cast<int>(binline));

    //wxLogMessage("%s",name);
    if( name.empty() )
        return tdt_printer;

    unTextPrinterGroupMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return (*it).second;

    //wxLogMessage("Mapping printer preset: %s",name);
    pkgPackage* package = static_cast<pkgPackage*>(ClientData);

    unTextPrinter* pdata;
    if( dname.empty() )
        pdata = new unTextPrinter;
    else
        pdata = GTextPrinterPresets.Get(dname,package);

    unTextPrinter* ptext;
    if( fname.empty() )
        ptext = new unTextPrinter;
    else
        ptext = GTextPrinterPresets.Get(fname,package);


    // TODO :: find a better way to reuse shared printers, unlink printer, group and group stuff
    unTextPrinterGroup* group =  AddPrinter( new unTextPrinterGroup );
    group->Add( new_tpFormat( wxT("0x%.8x"), &pkgType::GetOffset ) );
    if( !binline )
    {
        group->Add( new_tpFormat( wxT("%s"), &pkgType::GetName ) );
    }
    else
    {
        group->Add( new_tpTypeParent( new_tpFormat( wxT("%s"), &pkgType::GetName ) ) );
    }
    group->Add( new_tpFormat( wxT("%s"), &pkgType::GetTypeName ) );
    group->Add( pdata );
    group->Add( ptext );
    Map[name] = group;
    return group;
}




// ============================================================================
// uaFileFormatPackages
// ============================================================================
void uaFileFormatPackages::AddColumns()
{
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Name"), tlc::Name ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Signature"), tlc::Hex ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Version"), tlc::LDecimal ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("License"), tlc::LDecimal ) );
    Tree->AddColumn( uwTreeListColumnInfo( wxT("Flags"), tlc::Desc ) );
    Tree->SetMainColumn(0);
    bVerticalAlign = false;
}

void uaFileFormatPackages::AddData()
{
    pkgPackage& package = *static_cast<pkgPackage*>(ClientData);
    pkgPackageFormat& format = package.GetFormat();
    pkgPackageData& pkgdata = package.GetData();

    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );

    unTextPrinterGroup* thdr_printer = AddPrinter( new unTextPrinterGroup );
    thdr_printer->Add( new_tpFormat( wxT("%s"), &pkgPackage::GetName ) );
    thdr_printer->Add( new_tpPkgType( package, wxT("Header/Signature"), new_tpFormat( wxT("0x%.8x"), &pkgType::GetUnsigned ) ) );
    thdr_printer->Add( new_tpPkgType( package, wxT("Header/Version"), new_tpFormat( wxT("%u"), &pkgType::GetUnsigned ) ) );
    thdr_printer->Add( new_tpPkgType( package, wxT("Header/License"), new_tpFormat( wxT("%u"), &pkgType::GetUnsigned ) ) );
    thdr_printer->Add( new_tpPkgType( package, wxT("Header/Flags"), new_tpFlags( &pkgType::GetUnsigned, format.GetFlags().FindGroup(wxT("Package"))) ) );

    pkgType& thdr = *pkgdata.GetRoot().FindTypeByName(wxT("Header"));
    unDataTree* thdr_tree = DataTree->AddChild( new unDataTree( thdr.GetName(), ID_IGNORE, false, ClientData, thdr_printer ) );	
}



// ============================================================================
// uaImportTreeFactory
// ============================================================================
uaImportTreeFactory::uaImportTreeFactory( wxWindow* parent, pkgPackage* clientdata, const wxString& name, unTypeTable* table )
:   unWindowFactory(parent,clientdata,name)
,   Package(clientdata)
,   Table(table)
,   Tree(NULL)
,   DataTree(NULL)
{
}

uaImportTreeFactory::~uaImportTreeFactory()
{
    //delete Table;
    delete DataTree;
    for( vector<unTextPrinterGroup*>::iterator it=Printers.begin(); it!=Printers.end(); ++it )
        delete (*it);
}

wxWindow* uaImportTreeFactory::Create()
{
    if( Tree != NULL )
        return Tree;

    Tree = new uwTreeList(Parent);

    AddColumns();
    AddData();

    Tree->SetVerticalAlign( false );
    Tree->SetDataTree( *DataTree );
    Tree->ExpandRoot();
    return Tree;
}

void uaImportTreeFactory::AddColumns()
{
    if( Tree->GetColumnCount() == 0 )
    {
        vector<unTextColumn*>& columns = Table->GetColumns();
        for( vector<unTextColumn*>::iterator it=columns.begin(); it!=columns.end(); ++it )
        { 
            unTextColumn* column = (*it);
            Tree->AddColumn( column->GetName(), column->GetWidth(), column->GetAlign() );
        }
    }
    Tree->SetMainColumn(0);
}

void uaImportTreeFactory::AddData()
{
    DataTree = new unDataTree( wxT("root"), ID_IGNORE, false );
    unDataTree* child_tree = DataTree->AddChild( new unDataTree( "test", ID_IGNORE, false, reinterpret_cast<void*>(1), Table ) );	
    //Tree->SetPrinters( Table, Table->GetItemCount(Package) );
}


// ============================================================================