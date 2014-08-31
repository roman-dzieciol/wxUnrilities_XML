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
//	cfgNativeFunctions.cpp
// ============================================================================
#include "prec.h"
#include "cfgNativeFunctions.h"
#include "pkgNativeFunc.h"

#define UXP_CLASS cfgNativeFunctions


// ============================================================================
// cfgNativeFunctions
// ============================================================================
cfgNativeFunctions::cfgNativeFunctions( wxFileName filename, pkgNativeFuncStore* store )
{

    // --------------------------------------------------------------------
    // create items
    // --------------------------------------------------------------------

    UXR_ITEM(root,                  wxT("functions") );

    UXR_ITEM(root_version,          wxT("version") );
    UXR_ITEM(version_name,          wxT("name") );

    UXR_ITEM(root_extended,         wxT("extended") );
    UXR_ITEM(root_first,            wxT("first") );

    UXR_ITEM(root_package,          wxT("package") );
    UXR_ITEM(package_name,          wxT("name") );

    UXR_ITEM(package_class,         wxT("class") );
    UXR_ITEM(class_name,            wxT("name") );

    UXR_ITEM(class_func,            wxT("func") );
    UXR_ITEM(func_id,               wxT("id") );
    UXR_ITEM(func_op,               wxT("op") );
    UXR_ITEM(func_flags,            wxT("flags") );
    UXR_ITEM(func_name,             wxT("name") );
    UXR_ITEM(func_fname,            wxT("fname") );

 
    // --------------------------------------------------------------------
    // package - pre
    // --------------------------------------------------------------------

    // create package
    // set package name
    root_package->AddCmdPreParse( UXS_SET(CurrentPackage, tcFactory<pkgNativeFuncPackage>()) );
    package_name->AddCmdPreParse( UXS_PROC_1(CurrentPackage, &pkgNativeFuncPackage::SetName, UXS_NODENAME) );

    // --------------------------------------------------------------------
    // package - post
    // --------------------------------------------------------------------

    // add package
    root_package->AddCmdPostParse( UXC_PROC_1(store, &pkgNativeFuncStore::AddPackage, UXS_GET(CurrentPackage)) );


    // --------------------------------------------------------------------
    // class - pre
    // --------------------------------------------------------------------

    package_class->AddCmdPreParse( UXS_SET(CurrentClass, tcFactory<pkgNativeFuncClass>()) );
    class_name->AddCmdPreParse( UXS_PROC_1(CurrentClass, &pkgNativeFuncClass::SetName, UXS_NODENAME) );

    // --------------------------------------------------------------------
    // class - post
    // --------------------------------------------------------------------

    package_class->AddCmdPostParse( UXS_PROC_1(CurrentPackage, &pkgNativeFuncPackage::AddClass, UXS_GET(CurrentClass)) );


    // --------------------------------------------------------------------
    // func - pre
    // --------------------------------------------------------------------

    class_func->AddCmdPreParse( UXS_SET(CurrentFunc, tcFactory<pkgNativeFunc>() ) );
    class_func->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetPackage, UXS_GET(CurrentPackage)) );
    class_func->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetClass, UXS_GET(CurrentClass)) );
    func_id->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetIndex, UXS_NODEDATA) );
    func_op->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetPriority, UXS_NODEDATA) );
    func_flags->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetFlags, UXS_NODEDATA) );
    func_name->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetName, UXS_NODENAME) );
    func_fname->AddCmdPreParse( UXS_PROC_1(CurrentFunc, &pkgNativeFunc::SetFriendlyName, UXS_NODENAME) );

    // --------------------------------------------------------------------
    // func - post
    // --------------------------------------------------------------------

    class_func->AddCmdPostParse( UXS_PROC_1(CurrentClass, &pkgNativeFuncClass::AddFunction, UXS_GET(CurrentFunc)) );


    // --------------------------------------------------------------------
    // create tree
    // --------------------------------------------------------------------

    class_func->PushItem( func_id );
    class_func->PushItem( func_op );
    class_func->PushItem( func_flags );
    class_func->PushItem( func_name );
    class_func->PushItem( func_fname, uxLimits::maxone );

    package_class->PushItem( class_name );
    package_class->PushItem( class_func, uxLimits::minone );

    root_package->PushItem( package_name );
    root_package->PushItem( package_class, uxLimits::minone );

    root_version->PushItem( version_name );

    root->PushItem( root_version );
    root->PushItem( root_extended );
    root->PushItem( root_first );
    root->PushItem( root_package, uxLimits::minone );

    Root = root;
}


// ============================================================================