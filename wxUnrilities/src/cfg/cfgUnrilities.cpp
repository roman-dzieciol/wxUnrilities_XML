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
//	cfgUnrilities.cpp
// ============================================================================
#include "prec.h"
#include "cfgUnrilities.h"

#include "asvNamedSize.h"
#include "asvNamedFormat.h"
#include "asvNamedPath.h"
#include "unTextPrinterPresets.h"


#define UXP_CLASS cfgUnrilities

// ============================================================================
// cfgUnrilities
// ============================================================================
cfgUnrilities::cfgUnrilities( const wxFileName& filename )
{

    // --------------------------------------------------------------------
    // create items
    // --------------------------------------------------------------------

    UXR_ITEM(root,                  wxT("unrilities") );

    UXR_ITEM(root_version,          wxT("version") );
    UXR_ITEM(version_name,          wxT("name") );

    UXR_ITEM(root_sizes,            wxT("sizes") );
    UXR_ITEM(sizes_size,            wxT("size") );
    UXR_PROP(size_width,            wxT("width") );
    UXR_PROP(size_name,             wxT("name") );

    UXR_ITEM(root_formats,          wxT("formats") );
    UXR_ITEM(formats_format,        wxT("format") );
    UXR_PROP(format_text,           wxT("text") );
    UXR_PROP(format_name,           wxT("name") );

    UXR_ITEM(root_paths,            wxT("paths") );
    UXR_ITEM(paths_path,            wxT("path") );
    UXR_PROP(path_name,             wxT("name") );
    UXR_PROP(path_text,             wxT("text") );

    UXR_ITEM(root_printers,         wxT("printers") );
    UXR_ITEM(printers_preset,       wxT("preset") );
    UXR_ITEM(preset_name,           wxT("name") );
    UXR_ITEM(preset_printer,        wxT("printer") );
    UXR_PROP(printer_name,          wxT("id") );
    UXR_PROP(printer_param,         wxT("param") );


    // --------------------------------------------------------------------
    // sizes - pre
    // --------------------------------------------------------------------

    // get size width
    size_width->AddCmdPreParse( UXS_SET(SizeWidth, UXS_PROPDATA) );

    // get size name
    // add named size
    size_name->AddCmdPreParse( UXC_PROC_2(&GNamedSize, &asvNamedSize::Add, UXS_PROPNAME, UXS_GET(SizeWidth)) );


    // --------------------------------------------------------------------
    // sizes - pre
    // --------------------------------------------------------------------

    // get format string
    format_text->AddCmdPreParse( UXS_SET(FormatString, UXS_PROPNAME) );

    // get format name
    // add named format
    format_name->AddCmdPreParse( UXC_PROC_2(&GNamedFormat, &asvNamedFormat::Add, UXS_PROPNAME, UXS_GET(FormatString)) );


    // --------------------------------------------------------------------
    // paths - pre
    // --------------------------------------------------------------------

    // get path name
    path_name->AddCmdPreParse( UXS_SET(PathName, UXS_PROPNAME) );

    // get path text
    // add named path
    path_text->AddCmdPreParse( UXC_PROC_2(&GNamedPath, &asvNamedPath::Add, UXS_GET(PathName), UXS_PROPNAME) );


    // --------------------------------------------------------------------
    // printers
    // --------------------------------------------------------------------

    // create preset
    printers_preset->AddCmdPreParse( UXS_SET(PrinterPreset, tcFactory<unTextPrinterPreset>()) );

    // set preset name
    preset_name->AddCmdPreParse( UXS_PROC_1(PrinterPreset, &unTextPrinterPreset::SetName, UXS_NODENAME) );

    // get printer name
    printer_name->AddCmdPreParse( UXS_SET(PrinterName, UXS_PROPNAME) );
    printer_param->AddCmdPreParse( UXS_SET(PrinterParam, UXS_PROPNAME) );

    // add printer
    // reset optional param
    preset_printer->AddCmdPostParse( UXS_PROC_2(PrinterPreset, &unTextPrinterPreset::Add, UXS_GET(PrinterName), UXS_GET(PrinterParam)) );
    preset_printer->AddCmdPostParse( UXS_SET(PrinterParam, wxEmptyString) );

    // add preset
    printers_preset->AddCmdPostParse( UXC_PROC_1(&GTextPrinterPresets, &unTextPrinterPresets::Add, UXS_GET(PrinterPreset)) );


    // --------------------------------------------------------------------
    // create tree
    // --------------------------------------------------------------------

    root_printers->PushItem( printers_preset, uxLimits::minone );
    printers_preset->PushItem( preset_name );
    printers_preset->PushItem( preset_printer, uxLimits::minone );
    preset_printer->PushProp( printer_name );
    preset_printer->PushProp( printer_param, uxLimits::maxone );

    root_paths->PushItem( paths_path, uxLimits::minone );
    paths_path->PushProp( path_name );
    paths_path->PushProp( path_text );

    root_formats->PushItem( formats_format, uxLimits::minone );
    formats_format->PushProp( format_text );
    formats_format->PushProp( format_name );

    root_sizes->PushItem( sizes_size, uxLimits::minone );
    sizes_size->PushProp( size_width );
    sizes_size->PushProp( size_name );

    root_version->PushItem( version_name );

    root->PushItem( root_version );
    root->PushItem( root_sizes );
    root->PushItem( root_formats );
    root->PushItem( root_paths );
    root->PushItem( root_printers );

    Root = root;


    // --------------------------------------------------------------------
    // parse file
    // --------------------------------------------------------------------

    ParseFile(filename);
}




// ============================================================================