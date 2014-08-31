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
//	cfgFormatFlags.cpp
// ============================================================================
#include "prec.h"
#include "cfgFormatFlags.h"
#include "pkgFlags.h"
#include "unTextPrinterPresets.h"

#define UXP_CLASS cfgFormatFlags

// REGEX
// ^[\s]+[aA-zZ]+_([aA-zZ]+)[\s]+= (0x[0-9A-F]+),[\s/]*([\s\S]*)
// \t\t<flag><id>\2</id><name>\1</name><desc>\3</desc></flag>


// ============================================================================
// cfgFormatFlags
// ============================================================================
cfgFormatFlags::cfgFormatFlags( wxFileName filename, pkgFlags* pkgflags )
{
    // --------------------------------------------------------------------
    // create items
    // --------------------------------------------------------------------

    UXR_ITEM(root,              wxT("fileformat") );

    UXR_ITEM(root_version,      wxT("version") );
    UXR_ITEM(version_name,      wxT("name") );

    UXR_ITEM(root_flags,        wxT("flags") );

    UXR_ITEM(flags_group,       wxT("group") );
    UXR_ITEM(group_name,        wxT("name") );
    UXR_ITEM(group_desc,        wxT("desc") );

    UXR_ITEM(group_flag,        wxT("flag") );
    UXR_ITEM(flag_data,         wxT("id") );
    UXR_ITEM(flag_name,         wxT("name") );
    UXR_ITEM(flag_desc,         wxT("desc") );


    // --------------------------------------------------------------------
    // group - pre
    // --------------------------------------------------------------------

    // create group
    flags_group->AddCmdPreParse( UXS_SET(CurrentGroup, tcFactory<pkgFlagGroup>()) );

    // set group name
    // add preset
    // set group desc
    group_name->AddCmdPreParse( UXS_PROC_1(CurrentGroup, &pkgFlagGroup::SetName, UXS_NODENAME) );
    group_name->AddCmdPreParse( UXC_PROC_1(&GTextPrinterPresets, &unTextPrinterPresets::AddFlag, UXS_NODENAME) );
    group_desc->AddCmdPreParse( UXS_PROC_1(CurrentGroup, &pkgFlagGroup::SetDesc, UXS_NODENAME) );
    


    // --------------------------------------------------------------------
    // group - post
    // --------------------------------------------------------------------

    // add unknown flags
    flags_group->AddCmdPostParse( UXS_PROC_1(CurrentGroup, &pkgFlagGroup::AddUnknownFlags, 32) );

    // add group to flags
    flags_group->AddCmdPostParse( UXC_PROC_1(pkgflags, &pkgFlags::AddFlagGroup, UXS_GET(CurrentGroup)) );


    // --------------------------------------------------------------------
    // flag - pre
    // --------------------------------------------------------------------

    // create flag
    group_flag->AddCmdPreParse( UXS_SET(CurrentFlag, tcFactory<pkgFlagData>()) );

    // set flag id
    // set flag name
    // set flag desc
    flag_data->AddCmdPreParse( UXS_PROC_1(CurrentFlag, &pkgFlagData::SetData, UXS_NODEDATA) );
    flag_name->AddCmdPreParse( UXS_PROC_1(CurrentFlag, &pkgFlagData::SetName, UXS_NODENAME) );
    flag_desc->AddCmdPreParse( UXS_PROC_1(CurrentFlag, &pkgFlagData::SetDesc, UXS_NODENAME) );


    // --------------------------------------------------------------------
    // flag - post
    // --------------------------------------------------------------------

    // add flag to group
    group_flag->AddCmdPostParse( UXS_PROC_1(CurrentGroup, &pkgFlagGroup::AddFlagData, UXS_GET(CurrentFlag)) );


    // --------------------------------------------------------------------
    // create tree
    // --------------------------------------------------------------------

    group_flag->PushItem( flag_data );
    group_flag->PushItem( flag_name );
    group_flag->PushItem( flag_desc );

    flags_group->PushItem( group_name );
    flags_group->PushItem( group_desc );
    flags_group->PushItem( group_flag, uxLimits::minone );

    root_flags->PushItem( flags_group, uxLimits::minone );

    root_version->PushItem( version_name );

    root->PushItem( root_version );
    root->PushItem( root_flags );

    Root = root;


    // --------------------------------------------------------------------
    // parse file
    // --------------------------------------------------------------------

    ParseFile(filename);
}


// ============================================================================