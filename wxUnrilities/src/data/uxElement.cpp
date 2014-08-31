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
//	uxElement.cpp
// ============================================================================
#include "prec.h"
#include "uxElement.h"
#include "uxDebug.h"
#include "uxParser.h"
#include "uxCommands.h"






// ============================================================================
// uxElement - Info
// ============================================================================
wxString uxElement::TraceBack( bool bparentonly )
{
    wxString s;
    if( !bparentonly )
    {
        s = GetItemInfo();
        if( GetParent() != NULL )
            s.Prepend( wxT(' ') );
    }

    for( uxElement* parent=GetParent(); parent!=NULL; parent=parent->GetParent() )
    {
        s.Prepend( parent->GetItemInfo() );
        if( parent->GetParent() != NULL )
            s.Prepend( wxT(' ') );
    }
    return s;
}

wxString uxElement::GetLimitInfo()
{
    return wxString::Format( wxT("%d/%s"), Limit.GetCount(), Limit.GetName().c_str() );
}

wxString uxElement::GetItemInfo()
{
    return wxString::Format( wxT("%s(%s)"), GetItemName().c_str(), GetLimitInfo().c_str() );
}

wxString uxElement::GetItemName()
{
    return wxString::Format( wxT("<%s>"), Item.GetName().c_str() );
}


// ============================================================================
// uxElement - Parsing
// ============================================================================
unXmlNode* uxElement::Parse( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    //SW_DEF(sw);
    Limit.Reset();
    Parent = parent;
    Object = NULL;

    // parse node
    while( true )
    {
        uxDebugLog( DL_Debug, wxT("[%s] ELEMENT: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );

        // skip to first valid node type
        while( node != NULL && node->GetType() != wxXML_ELEMENT_NODE )
        {
            uxDebugLog( DL_Debug, wxT("\t[%s] SKIP: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );
            node = node->GetNext();
        }

        // return if no node
        if( node == NULL )
        {
            uxDebugLog( DL_Debug, wxT("\t[%s] NULL"), TraceBack().c_str() );
            break;
        }

        // return if node name doesn't match
        if( !xml::IsSameName(node,Item.GetName()) )
        {
            uxDebugLog( DL_Debug, wxT("\t[%s] EXPECTED: <%s> GOT: %s"), TraceBack().c_str(), Item.GetName().c_str(), xml::TraceBack(node).c_str() );
            break;
        }

        // Command: Parse Begin
        Limit.Increase();
        uxDebugLog( DL_Notice, wxT("[%s] PARSE %s"), TraceBack().c_str(), xml::GetNodeText(node).c_str() );
        uxExecCommands( Item.GetCmdPreParse(), node, parser, this, Item.GetLastProperty() );

        // parse properties
        //unXmlProperty* pnode = node->GetProperties();
        //for( uxPropArray::iterator it=master.Properties.begin(); it!=master.Properties.end(); ++it )
        //{
        //    uxProperty* prop = static_cast<uxProperty*>(*it);
        //    prop->SetParent(this);
        //    prop->Parse(node, pnode);
        //}

        // Parse: properties
        //sw.Pause();
        node = Item.ParseProperties(node,parser,this);
        //sw.Resume();

        // Parse: children
        //sw.Pause();
        node = Item.ParseChildren(node,parser,this);
        //sw.Resume();

        // Command: Parse End
        uxExecCommands( Item.GetCmdPostParse(), node, parser, this, Item.GetLastProperty() );

        // return if limit invalid
        if( !Limit.IsValid() )
        {
            break;
        } 

        // get next node
        if( node != NULL )
        {
            node = node->GetNext();
        }

        // continue if limit valid
        if( !Limit.IsNextValid() )
        {
            break;
        }

        uxDebugLog( DL_Debug, wxT("[%s] CONTINUE"), TraceBack().c_str() );
    }

    CheckLimit();
    //sw.Pause(); 
    //wxLogMessage(wxT("%s %s %s"), __FUNCTION__, TimeStr(sw), Item.GetInfo().c_str());
    return node;
}




// ============================================================================