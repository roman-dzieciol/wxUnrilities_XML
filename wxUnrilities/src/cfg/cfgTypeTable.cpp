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
//	cfgTypeTable.cpp
// ============================================================================
#include "prec.h"
#include "cfgTypeTable.h"
#include "asvNamedSize.h"
#include "asvNamedAlign.h"
#include "asvNamedFormat.h"
#include "unTextPrinter.h"
#include "dtComparerFactory.h"
#include "unTextPrinterFactory.h"
#include "unTypeTable.h"


#define UXP_CLASS cfgTypeTable


// ============================================================================
// cfgTypeTable
// ============================================================================
cfgTypeTable::cfgTypeTable( wxFileName filename, pkgPackage* package, vector<unTypeTable*>& tables )
{

    // --------------------------------------------------------------------
    // create items
    // --------------------------------------------------------------------

    UXR_ITEM(root,              wxT("tables") );

    UXR_ITEM(root_version,      wxT("version") );
    UXR_ITEM(version_name,      wxT("name") );

    UXR_ITEM(root_table,        wxT("table") );
    UXR_ITEM(table_name,        wxT("name") );
    UXR_ITEM(table_path,        wxT("path") );
    UXR_ITEM(table_column,      wxT("column") );

    UXR_PROP(column_name,       wxT("name") );
    UXR_PROP(column_align,      wxT("align") );
    UXR_PROP(column_size,       wxT("size") );
    UXR_PROP(column_sort,       wxT("sort") );
    UXR_ITEM(column_printer,    wxT("printer") );

    UXR_PROP(printer_id,        wxT("id") );
    UXR_PROP(printer_param,     wxT("param") );


    // --------------------------------------------------------------------
    // table - pre
    // --------------------------------------------------------------------

    // create table
    root_table->AddCmdPreParse( UXS_SET(CurrentTable, tcFactory<unTypeTable>()) );

    // set table name
    table_name->AddCmdPreParse( UXS_PROC_1(CurrentTable, &unTypeTable::SetName, UXS_NODENAME) );

    // set table path
    table_path->AddCmdPreParse( UXS_PROC_1(CurrentTable, &unTypeTable::SetPath, UXS_NODENAME) );
    table_path->AddCmdPreParse( UXS_SET(TablePath, UXS_NODENAME) );


    // --------------------------------------------------------------------
    // table - post
    // --------------------------------------------------------------------

    // add table to type templates
    root_table->AddCmdPostParse( UXC_PROC_1(&tables, &vector<unTypeTable*>::push_back, UXS_GET(CurrentTable)) );


    // --------------------------------------------------------------------
    // column - pre
    // --------------------------------------------------------------------

    // create column
    table_column->AddCmdPreParse( UXS_SET(CurrentColumn, tcFactory<unTextColumn>()) );

    // set column name
    column_name->AddCmdPreParse( UXS_PROC_1(CurrentColumn, &unTextColumn::SetName, UXS_PROPNAME) );

    // get column align
    // set column align
    column_align->AddCmdPreParse( UXS_SET(ColumnAlign, UXC_FUNC_1(&GNamedAlign, &asvNamedAlign::Get, UXS_PROPNAME)) );
    column_align->AddCmdPreParse( UXS_PROC_1(CurrentColumn, &unTextColumn::SetAlign, UXS_GET(ColumnAlign)) );

    // get column size
    // set column size
    column_size->AddCmdPreParse( UXS_SET(ColumnWidth, UXC_FUNC_1(&GNamedSize, &asvNamedSize::Get, UXS_PROPNAME)) );
    column_size->AddCmdPreParse( UXS_PROC_1(CurrentColumn, &unTextColumn::SetWidth, UXS_GET(ColumnWidth)) );

    // add column to table
    column_size->AddCmdPreParse( UXS_PROC_1(CurrentTable, &unTypeTable::AddColumn, UXS_GET(CurrentColumn)) );

    // create column sorter    
    // add sorter to table   
    column_sort->AddCmdPreParse( UXS_SET(CurrentSorter, UXC_FUNC_1(&GComparerFactory, &dtComparerFactory::Create, UXS_PROPNAME)) );
    column_sort->AddCmdPreParse( UXS_PROC_1(CurrentTable, &unTypeTable::AddComparer, UXS_GET(CurrentSorter)) );

    // add base printer
    column_sort->AddCmdPreParse( UXS_SET(BasePrinter, UXC_FUNC_3(&GTextPrinterFactory, &unTextPrinterFactory::Create, wxT("TArray"), UXS_GET(TablePath), package)) );
    column_sort->AddCmdPreParse( UXS_SET(CurrentPrinter, UXS_GET(BasePrinter)) );


    // --------------------------------------------------------------------
    // column - post
    // --------------------------------------------------------------------

    // add column printers
    table_column->AddCmdPostParse( UXS_PROC_1(CurrentTable, &unTypeTable::AddPrinter, UXS_GET(BasePrinter)) );


    // --------------------------------------------------------------------
    // printer - pre
    // --------------------------------------------------------------------

    // printer name
    printer_id->AddCmdPreParse( UXS_SET(PrinterName, UXS_PROPNAME) );

    // printer param
    printer_param->AddCmdPreParse( UXS_SET(PrinterParams, UXS_PROPNAME) );

    // --------------------------------------------------------------------
    // printer - post
    // --------------------------------------------------------------------

    // create printer
    // add printer to table
    // update pointer to current printer
    column_printer->AddCmdPostParse( UXS_SET(NextPrinter, UXC_FUNC_3(&GTextPrinterFactory, &unTextPrinterFactory::Create, UXS_GET(PrinterName), UXS_GET(PrinterParams), package)) );
    column_printer->AddCmdPostParse( UXS_PROC_1(CurrentPrinter, &unTextPrinter::SetNextPrinter, UXS_GET(NextPrinter)) );
    column_printer->AddCmdPostParse( UXS_SET(CurrentPrinter, UXS_GET(NextPrinter)) );

    // reset param
    column_printer->AddCmdPostParse( UXS_SET(PrinterParams, wxEmptyString) );


    // --------------------------------------------------------------------
    // create tree
    // --------------------------------------------------------------------

    column_printer->PushProp( printer_id );
    column_printer->PushProp( printer_param, uxLimits::maxone );

    table_column->PushProp( column_name );
    table_column->PushProp( column_align );
    table_column->PushProp( column_size );
    table_column->PushProp( column_sort );
    table_column->PushItem( column_printer, uxLimits::minone );

    root_table->PushItem( table_name );
    root_table->PushItem( table_path );
    root_table->PushItem( table_column, uxLimits::minone );

    root_version->PushItem( version_name );

    root->PushItem( root_version );
    root->PushItem( root_table, uxLimits::minone );

    Root = root;


    // --------------------------------------------------------------------
    // parse file
    // --------------------------------------------------------------------

    ParseFile(filename);
}



// ============================================================================