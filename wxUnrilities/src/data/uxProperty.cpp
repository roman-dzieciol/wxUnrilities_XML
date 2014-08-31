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
//	uxProperty.cpp
// ============================================================================
#include "prec.h"
#include "uxProperty.h"
#include "uxDebug.h"
#include "uxParser.h"
#include "uxCommands.h"




// ============================================================================
// uxProperty
// ============================================================================
uxProperty::uxProperty( uxItem& item, const uxLimit& limit ) 
:   uxElement(item,limit)
{
}

uxProperty::~uxProperty()
{
}


// ============================================================================
// uxProperty - Info
// ============================================================================
wxString uxProperty::GetItemName()
{
    return wxString::Format( wxT("@%s"), Item.GetName().c_str() );
}


// ============================================================================
// uxProperty - Parsing
// ============================================================================
unXmlNode* uxProperty::Parse( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    Limit.Reset();
    Parent = parent;
    Object = NULL;
    unXmlProperty* property = parent->GetItem().GetLastProperty();

    // return if no property
    if( property == NULL )
    {
        uxDebugLog( DL_Debug, wxT("\t[%s] NULL"), TraceBack().c_str() );
        CheckLimit();
        return node;
    }

    uxDebugLog( DL_Debug, wxT("[%s] PROPERTY: %s"), TraceBack().c_str(), xml::TraceBack(node,property).c_str() );

    // return if property name doesn't match
    if( !xml::IsSameName(property,Item.GetName()) )
    {
        uxDebugLog( DL_Debug, wxT("\t[%s] EXPECTED: <%s> GOT: %s"), TraceBack().c_str(), GetItemName().c_str(), xml::TraceBack(node,property).c_str() );
        CheckLimit();       
        return node;
    }

    // Command: Parse Begin
    Limit.Increase();
    uxDebugLog( DL_Notice, wxT("[%s] PARSE %s"), TraceBack().c_str(), xml::GetPropText(property).c_str() );
    uxExecCommands( Item.GetCmdPreParse(), node, parser, this, property );

    // Command: Parse End
    uxExecCommands( Item.GetCmdPostParse(), node, parser, this, property );

    // Parse next property
    parent->GetItem().SetLastProperty(property->GetNext());

    CheckLimit();
    return node;
}


#if 0



#include "uxDebug.h"
#include "uxTree.h"
#include "uxTreeItem.h"


// ============================================================================
// uxProperty
// ============================================================================
uxProperty::uxProperty( const wxString& name, bool brequire ) 
:   bRequire(brequire)
,   bFound(false)
,   Parent(NULL)
,   uxItem(name)
{
}

uxProperty::~uxProperty()
{
}


// ============================================================================
// uxProperty - Info
// ============================================================================
wxString uxProperty::GetRequireInfo()
{
    return wxString::Format( wxT("%d/%s"), static_cast<dword>(bFound), bRequire ? wxT("req") : wxT("opt") );
}

wxString uxProperty::GetPropInfo()
{
    return wxString::Format( wxT("%s(%s)"), GetPropName().c_str(), GetRequireInfo() );
}

wxString uxProperty::GetPropName( bool bmarkup )
{
    return bmarkup ? wxString::Format( wxT("@%s"), GetName().c_str() ) : GetName();
}

wxString uxProperty::TraceBack( bool bparentonly )
{
    wxString s;
    if( !bparentonly )
    {
        s = GetPropInfo();
        if( GetParent() != NULL )
            s.Prepend( wxT(' ') );
    }

    for( uxTree* parent=GetParent(); parent!=NULL; parent=parent->GetParent() )
    {
        s.Prepend( parent->GetTreeInfo());
        if( parent->GetParent() != NULL )
            s.Prepend( wxT(' ') );
    }
    return s;
}


// ============================================================================
// uxProperty - Parsing
// ============================================================================
void uxProperty::Parse( unXmlNode* node, unXmlProperty*& pnode )
{
    bFound = false;
    uxDebugLog( DL_Debug, wxT("[%s] PROPERTY: %s"), TraceBack().c_str(), xml::TraceBack(node,pnode).c_str() );

    ParseInternal(node,pnode);

    if( bRequire && !bFound )
    {
        throw exString( wxString::Format( wxT("Property not found: %s."), TraceBack().c_str() ) );
    } 
}   

void uxProperty::ParseInternal( unXmlNode* node, unXmlProperty*& pnode )
{
    // return if no property
    if( pnode == NULL )
    {
        uxDebugLog( DL_Debug, wxT("\t[%s] PR NULL"), TraceBack().c_str() );
        return;
    }

    // return if property name doesn't match
    if( !xml::IsSameName(pnode,Name) )
    {
        uxDebugLog( DL_Debug, wxT("\t[%s] PR EXPECTED: %s GOT: %s"), TraceBack().c_str(), GetPropName().c_str(), xml::TraceBack(node,pnode).c_str() );
        return;
    }

    // parse property
    bFound = true;
    uxDebugLog( DL_Notice, wxT("[%s] PR PARSE"), TraceBack().c_str() );

    if( pnode != NULL )
    {
        pnode = pnode->GetNext();
    }
}   
#endif

// ============================================================================