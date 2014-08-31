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
//	uxItem.cpp
// ============================================================================
#include "prec.h"
#include "uxItem.h"
#include "uxDebug.h"
#include "uxParser.h"
#include "uxElement.h"
#include "uxCommands.h"


// ============================================================================
// uxItem
// ============================================================================
uxItem::uxItem( const wxString& name, uxElementCreator creator ) 
:   Name(name)
,   ElementCreator(creator)
,   LastProperty(NULL)
{
}

uxItem::~uxItem()
{
    for( uxContents::iterator it=Properties.begin(); it!=Properties.end(); ++it )
        delete (*it);

    for( uxContents::iterator it=Contents.begin(); it!=Contents.end(); ++it )
        delete (*it);

    for( uxCommands::iterator it=CmdPreParse.begin(); it!=CmdPreParse.end(); ++it )
        delete (*it);

    for( uxCommands::iterator it=CmdPostParse.begin(); it!=CmdPostParse.end(); ++it )
        delete (*it);
}


// ============================================================================
// uxItem - Contents
// ============================================================================
uxItem* uxItem::LastItem() const
{
    return Contents.back()->GetItem();
}

void uxItem::PushItem( uxItem* item, const uxLimit& limit )
{
    uxContent* content = new uxContent(item,limit,item->ElementCreator);
    Contents.push_back(content);
    ContentsMap[item->GetName()] = content;
}

void uxItem::PushSubItem( uxItem* item, const uxLimit& limit )
{
    LastItem()->PushItem(item,limit);
}

uxContent* uxItem::FindItem( const wxString& name )
{
    uxContentsMap::iterator it = ContentsMap.find(name);
    if( it != ContentsMap.end() )
        return static_cast<uxContent*>((*it).second);
    return NULL;
}


// ============================================================================
// uxItem - Properties
// ============================================================================
uxItem* uxItem::LastProp() const
{
    return Properties.back()->GetItem();
}

void uxItem::PushProp( uxItem* item, const uxLimit& limit )
{
    uxContent* content = new uxContent(item,limit,item->ElementCreator);
    Properties.push_back(content);
}

void uxItem::PushSubProp( uxItem* item, const uxLimit& limit )
{
    LastProp()->PushProp(item,limit);
}



// ============================================================================
// uxItem - Info
// ============================================================================
wxString uxItem::GetInfo()
{
    return wxString::Format( wxT("<%s>"), GetName().c_str() );
}


// ============================================================================
// uxItem - Parsing
// ============================================================================
unXmlNode* uxItem::ParseChildren( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    if( !Contents.empty() )
    {
        unXmlNode* next = node->GetChildren();
        for( uxContents::const_iterator it=Contents.begin(); it!=Contents.end(); ++it )
        {
            //uxDebugLog( DL_Debug, wxT("[%s] CHILD: %s"), parent->TraceBack().c_str(), xml::TraceBack(node).c_str() );
            uxContent* content = static_cast<uxContent*>(*it);
            auto_ptr<uxElement> element( content->GetElement() );   // get element from pool and take ownership
            //wxLogMessage( wxT("\t\t[%s] %s"), __FUNCTION__, GetName().c_str()/*, element->GetItemInfo()*/ );
            next = element->Parse( next, parser, parent );          // parse element
            content->PutElement( element.release() );               // put element back in pool and dump ownership
        }
    }
    return node;
}

unXmlNode* uxItem::ParseChoices( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    if( !Contents.empty() )
    {
        uxContent* content = FindItem( xml::GetNodeName(node) );
        if( content == NULL )
            throw exString( wxString::Format( wxT("Unknown element: %s"), xml::TraceBack(node).c_str() ) );

        auto_ptr<uxElement> element( content->GetElement() );   // get element from pool and take ownership
        //wxLogMessage( wxT("\t\t[%s] %s"), __FUNCTION__, GetName().c_str()/*, element->GetItemInfo()*/ );
        node = element->Parse( node, parser, parent );          // parse element
        content->PutElement( element.release() );               // put element back in pool and dump ownership
    }
    return node;
}

unXmlNode* uxItem::ParseProperties( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    if( !Properties.empty() )
    {
        LastProperty = node->GetProperties();
        for( uxContents::const_iterator it=Properties.begin(); it!=Properties.end(); ++it )
        {
            //uxDebugLog( DL_Debug, wxT("[%s] CHILD: %s"), parent->TraceBack().c_str(), xml::TraceBack(node).c_str() );
            uxContent* content = static_cast<uxContent*>(*it);
            auto_ptr<uxElement> element( content->GetElement() );   // get element from pool and take ownership
            node = element->Parse( node, parser, parent );          // parse element
            content->PutElement( element.release() );               // put element back in pool and dump ownership
        }
        LastProperty = NULL;
    }
    return node;
}

unXmlNode* uxItem::ParseRoot( unXmlNode* node, uxParser* parser )
{
    //SW_DEF(sw);
    auto_ptr<uxElement> element( new uxElement( *this, uxLimits::one ) );
    //Stack.push(element.get());
    node = element->Parse(node,parser,NULL);
    //Stack.pop();
    //SW_LOGF(sw);
    return node;
}


// ============================================================================