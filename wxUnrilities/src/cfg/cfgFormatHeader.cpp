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
//	cfgFormatHeader.cpp
// ============================================================================
#include "prec.h"
#include "cfgFormatHeader.h"

#include "pkgType.h"
#include "pkgObject.h"
#include "pkgPackageFormat.h"
#include "pkgExpression.h"
#include "pkgTypeRegistry.h"
#include "unTextPrinterPresets.h"


#define UXP_CLASS cfgFormatHeader


// ============================================================================
// cfgFormatHeader
// ============================================================================
cfgFormatHeader::cfgFormatHeader( wxFileName filename, pkgPackageFormat* packageformat, pkgType* registry )
{
    pkgTypeRegistry* basetypes      = &packageformat->GetBaseTypes();
    pkgTypeRegistry* flowtypes      = &packageformat->GetFlowTypes();
    exParser expressions;

    pkgType* type_assert    = flowtypes->FindTypeByName(wxT("assert"));
    pkgType* type_array     = flowtypes->FindTypeByName(wxT("array"));
    pkgType* type_if        = flowtypes->FindTypeByName(wxT("if"));
    pkgType* type_offset    = flowtypes->FindTypeByName(wxT("offset"));
    pkgType* type_seek      = flowtypes->FindTypeByName(wxT("seek"));
    pkgType* type_break     = flowtypes->FindTypeByName(wxT("break"));


    // --------------------------------------------------------------------
    // create items
    // --------------------------------------------------------------------

    UXR_ITEM(root,              wxT("package") );

    UXR_ITEM(root_version,      wxT("version") );
    UXR_ITEM(version_name,      wxT("name") );

    UXR_ITEM(root_object,       wxT("object") );
    UXR_ITEM(object_name,       wxT("name") );
    UXR_ITEM(object_format,     wxT("data") );

    UXR_ITEM(format_var,        wxT("var") );
    UXR_PROP(var_type,          wxT("id") );
    UXR_PROP(var_name,          wxT("name") );
    UXR_PROP(var_data,          wxT("data") );
    UXR_PROP(var_text,          wxT("text") );

    UXR_ITEM(format_break,      wxT("break") );

    UXR_ITEM(format_if,         wxT("if") );
    UXR_PROP(if_test,           wxT("test") );

    UXR_ITEM(format_assert,     wxT("assert") );
    UXR_PROP(assert_test,       wxT("test") );

    UXR_ITEM(format_array,      wxT("array") );
    UXR_PROP(array_count,       wxT("count") );
    UXR_ITEM(array_name,        wxT("name") );

    UXR_ITEM(format_offset,     wxT("offset") );
    UXR_PROP(offset_loc,        wxT("loc") );

    UXR_ITEM(format_seek,       wxT("seek") );
    UXR_PROP(seek_delta,        wxT("delta") );

    UXR_CHOICE(format_choice,   wxEmptyString );


    // --------------------------------------------------------------------
    // object - pre
    // --------------------------------------------------------------------

    // create object type
    root_object->AddCmdPreParse( UXS_SET(CurrentType, tcFactory<pkgObject>()) );

    // make object current container
    root_object->AddCmdPreParse( UXS_SET(CurrentContainer, UXS_GET(CurrentType)) );

    // set object name
    object_name->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_NODENAME) );

    // add object to type templates
    object_name->AddCmdPreParse( UXC_PROC_2(registry, &pkgType::AddChild, UXS_GET(CurrentType), false) );

    // --------------------------------------------------------------------
    // break - pre
    // --------------------------------------------------------------------

    // create break type
    format_break->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_break, &pkgType::Clone)) );

    // add break to current container
    format_break->AddCmdPreParse( UXS_PROC_2(CurrentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // var - pre
    // --------------------------------------------------------------------

    // find by type name
    // clone
    var_type->AddCmdPreParse( UXS_SET(FoundType, UXC_FUNC_2(basetypes, &pkgTypeRegistry::FindTypeByName, UXS_PROPNAME, false)) );
    var_type->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(UXS_GET(FoundType), &pkgType::Clone)) );

    // set name
    // set data text
    // set full text
    var_name->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_PROPNAME) );
    var_data->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetDataPrinter, UXS_PROPNAME) );
    var_text->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetTextPrinter, UXS_PROPNAME) );

    // add base to current container
    var_name->AddCmdPreParse( UXS_PROC_2(CurrentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // assert - pre
    // --------------------------------------------------------------------

    // create assert type
    format_assert->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_assert, &pkgType::Clone)) );

    // add assert to current container
    format_assert->AddCmdPreParse( UXS_PROC_2(CurrentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );

    // set assert name
    // set assert expression
    assert_test->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_PROPNAME) );
    assert_test->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetExpression, UXC_FUNC_1(&expressions, &exParser::GetTokens, UXS_PROPNAME)) );


    // --------------------------------------------------------------------
    // array - pre
    // --------------------------------------------------------------------

    // create array type
    format_array->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_array, &pkgType::Clone)) );

    // store current container as parent container
    // make array current container
    format_array->AddCmdPreParse( UXS_SET(ParentContainer, UXS_GET(CurrentContainer)) );
    format_array->AddCmdPreParse( UXS_SET(CurrentContainer, UXS_GET(CurrentType)) );

    // set array expression
    // set array name
    array_count->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetExpression, UXC_FUNC_1(&expressions, &exParser::GetTokens, UXS_PROPNAME)) );
    array_name->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_NODENAME) );

    // add array to parent container
    array_name->AddCmdPreParse( UXS_PROC_2(ParentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // array - post
    // --------------------------------------------------------------------

    // get parent container
    // make parent container current container
    format_array->AddCmdPostParse( UXS_SET(ParentContainer, UXC_FUNC_0(UXS_GET(CurrentContainer), &pkgType::GetParent)) );
    format_array->AddCmdPostParse( UXS_SET(CurrentContainer, UXS_GET(ParentContainer)) );


    // --------------------------------------------------------------------
    // if - pre
    // --------------------------------------------------------------------

    // create if type
    format_if->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_if, &pkgType::Clone)) );

    // store current container as parent container
    // store current container as parent container
    format_if->AddCmdPreParse( UXS_SET(ParentContainer, UXS_GET(CurrentContainer)) );
    format_if->AddCmdPreParse( UXS_SET(CurrentContainer, UXS_GET(CurrentType)) );

    // set if name
    // set if expression
    if_test->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_PROPNAME) );
    if_test->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetExpression, UXC_FUNC_1(&expressions, &exParser::GetTokens, UXS_PROPNAME)) );

    // add if to parent container
    if_test->AddCmdPreParse( UXS_PROC_2(ParentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // if - post
    // --------------------------------------------------------------------

    // get parent container
    // make parent container current container
    format_if->AddCmdPostParse( UXS_SET(ParentContainer, UXC_FUNC_0(UXS_GET(CurrentContainer), &pkgType::GetParent)) );
    format_if->AddCmdPostParse( UXS_SET(CurrentContainer, UXS_GET(ParentContainer)) );


    // --------------------------------------------------------------------
    // offset - pre
    // --------------------------------------------------------------------

    // create offset type
    format_offset->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_offset, &pkgType::Clone)) );

    // store current container as parent container
    // make offset current container
    format_offset->AddCmdPreParse( UXS_SET(ParentContainer, UXS_GET(CurrentContainer)) );
    format_offset->AddCmdPreParse( UXS_SET(CurrentContainer, UXS_GET(CurrentType)) );

    // set offset name
    // set offset expression
    offset_loc->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_PROPNAME) );
    offset_loc->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetExpression, UXC_FUNC_1(&expressions, &exParser::GetTokens, UXS_PROPNAME)) );

    // add offset to parent container
    offset_loc->AddCmdPreParse( UXS_PROC_2(ParentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // offset - post
    // --------------------------------------------------------------------

    // get parent container
    // make parent container current container
    format_offset->AddCmdPostParse( UXS_SET(ParentContainer, UXC_FUNC_0(UXS_GET(CurrentContainer), &pkgType::GetParent)) );
    format_offset->AddCmdPostParse( UXS_SET(CurrentContainer, UXS_GET(ParentContainer)) );


    // --------------------------------------------------------------------
    // seek - pre
    // --------------------------------------------------------------------

    // create seek type
    format_seek->AddCmdPreParse( UXS_SET(CurrentType, UXC_FUNC_0(type_seek, &pkgType::Clone)) );

    // store current container as parent container
    // make seek current container
    format_seek->AddCmdPreParse( UXS_SET(ParentContainer, UXS_GET(CurrentContainer)) );
    format_seek->AddCmdPreParse( UXS_SET(CurrentContainer, UXS_GET(CurrentType)) );

    // set seek name
    // set seek expression
    seek_delta->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetName, UXS_PROPNAME) );
    seek_delta->AddCmdPreParse( UXS_PROC_1(CurrentType, &pkgType::SetExpression, UXC_FUNC_1(&expressions, &exParser::GetTokens, UXS_PROPNAME)) );

    // add seek to parent container
    seek_delta->AddCmdPreParse( UXS_PROC_2(ParentContainer, &pkgType::AddChild, UXS_GET(CurrentType), false) );


    // --------------------------------------------------------------------
    // seek - post
    // --------------------------------------------------------------------

    // get parent container
    // make parent container current container
    format_seek->AddCmdPostParse( UXS_SET(ParentContainer, UXC_FUNC_0(UXS_GET(CurrentContainer), &pkgType::GetParent)) );
    format_seek->AddCmdPostParse( UXS_SET(CurrentContainer, UXS_GET(ParentContainer)) );


    // --------------------------------------------------------------------
    // create tree
    // --------------------------------------------------------------------

    format_choice->PushItem( format_break );
    format_choice->PushItem( format_var );
    format_choice->PushItem( format_if );
    format_choice->PushItem( format_assert );
    format_choice->PushItem( format_array );
    format_choice->PushItem( format_offset );
    format_choice->PushItem( format_seek );

    format_var->PushProp( var_type );
    format_var->PushProp( var_name );
    format_var->PushProp( var_data );
    format_var->PushProp( var_text );

    format_if->PushProp( if_test );
    format_if->PushItem( format_choice, uxLimits::minone );

    format_assert->PushProp( assert_test );

    format_array->PushProp( array_count );
    format_array->PushItem( array_name );
    format_array->PushItem( format_choice, uxLimits::minone );

    format_offset->PushProp( offset_loc );
    format_offset->PushItem( format_choice, uxLimits::minone );

    format_seek->PushProp( seek_delta );
    format_seek->PushItem( format_choice, uxLimits::minone );

    object_format->PushItem( format_choice, uxLimits::minone );

    root_object->PushItem( object_name );
    root_object->PushItem( object_format );

    root_version->PushItem( version_name );

    root->PushItem( root_version );
    root->PushItem( root_object, uxLimits::any );

    Root = root;


    // --------------------------------------------------------------------
    // parse file
    // --------------------------------------------------------------------

    ParseFile(filename);
}

// ============================================================================