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
//  uxItem.h
// ============================================================================
#ifndef UXITEM_H
#define UXITEM_H

#include "unXmlDocument.h"
#include "uxLimit.h"
#include "uxCommand.h"
#include "uxContent.h"

class uxParser;
class uxElement;
class uxProperty;



// ============================================================================
// uxItem
// ============================================================================
class uxItem
{
public:
    // Allocator
    STATIC_ALLOCATOR(uxItem);

    // Special
    uxItem( const wxString& name, uxElementCreator creator );
    ~uxItem();
    uxItem* Clone() const { return alloc_copy(*this); }

    // Accessors
    wxString GetName() const { return Name; }
    unXmlProperty* GetLastProperty() const { return LastProperty; }
    const uxContents& GetContents() const { return Contents; }
    const uxCommands& GetCmdPreParse() { return CmdPreParse; }
    const uxCommands& GetCmdPostParse() { return CmdPostParse; }

    // Mutators
    void SetName( const wxString& p ) { Name = p; }
    void SetLastProperty( unXmlProperty* p ) { LastProperty = p; }

    // Contents
    uxItem* LastItem() const;
    void PushItem( uxItem* item, const uxLimit& limit=uxLimits::one );
    void PushSubItem( uxItem* item, const uxLimit& limit=uxLimits::one );
    uxContent* FindItem( const wxString& name );

    // Properties
    uxItem* LastProp() const;
    void PushProp( uxItem* item, const uxLimit& limit=uxLimits::one );
    void PushSubProp( uxItem* item, const uxLimit& limit=uxLimits::one );

    // Commands
    void AddCmdPreParse( uxCommand* cmd ) { CmdPreParse.push_back(cmd); }
    void AddCmdPostParse( uxCommand* cmd ) { CmdPostParse.push_back(cmd); }

    // Parsing
    unXmlNode* ParseRoot( unXmlNode* node, uxParser* parser );
    unXmlNode* ParseChildren( unXmlNode* node, uxParser* parser, uxElement* parent );
    unXmlNode* ParseChoices( unXmlNode* node, uxParser* parser, uxElement* parent );
    unXmlNode* ParseProperties( unXmlNode* node, uxParser* parser, uxElement* parent );

    // Info
    wxString GetInfo();

protected:
    wxString Name;                      // Item name
    uxContents Contents;                // Item contents
    uxContents Properties;              // Item properties
    uxCommands CmdPreParse;             // Commands executed before parsing
    uxCommands CmdPostParse;            // Commands executed after parsing
    uxContentsMap ContentsMap;          // Map of item contents
    uxElementCreator ElementCreator;    // Fabricates uxElement classes
    unXmlProperty* LastProperty;        // Helper for uxProperty
    //uxElementStack Stack;             // Recursive elements stack
};



#endif UXITEM_H
// ============================================================================