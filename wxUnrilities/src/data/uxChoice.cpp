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
//	uxChoice.cpp
// ============================================================================
#include "prec.h"
#include "uxChoice.h"
#include "uxDebug.h"
#include "uxParser.h"
#include "uxCommands.h"




// ============================================================================
// uxChoice
// ============================================================================
uxChoice::uxChoice( uxItem& item, const uxLimit& limit ) 
:   uxElement(item,limit)
{
    //wxLogMessage("%s",__FUNCSIG__);
}

uxChoice::~uxChoice()
{
}


// ============================================================================
// uxChoice - Info
// ============================================================================
wxString uxChoice::GetItemName()
{
    return wxString::Format( wxT("%s"), Item.GetName().c_str() );
}


// ============================================================================
// uxChoice - Parsing
// ============================================================================
unXmlNode* uxChoice::Parse( unXmlNode* node, uxParser* parser, uxElement* parent )
{
    //SW_DEF(sw);
    Limit.Reset();
    Parent = parent;
    Object = NULL;

    // parse node
    while( true )
    {
        uxDebugLog( DL_Debug, wxT("[%s] CHOICE: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );

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

        // Command: Parse Begin
        Limit.Increase();
        uxDebugLog( DL_Notice, wxT("[%s] PARSE %s"), TraceBack().c_str(), xml::GetNodeText(node).c_str() );
        uxExecCommands( Item.GetCmdPreParse(), node, parser, this, NULL );

        // Parse: children
        //sw.Pause();
        node = Item.ParseChoices(node,parser,this);
        //sw.Resume();

        // Command: Parse End
        uxExecCommands( Item.GetCmdPostParse(), node, parser, this, NULL );

        // return if limit invalid
        if( !Limit.IsValid() )
        {
            break;
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