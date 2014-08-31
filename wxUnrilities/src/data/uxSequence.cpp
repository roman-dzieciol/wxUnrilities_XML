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
//	uxSequence.cpp
// ============================================================================
#include "prec.h"
#include "uxSequence.h"
#include "uxDebug.h"





// ============================================================================
// uxSequence
// ============================================================================
uxSequence::uxSequence( const wxString& name, const uxLimit& limit ) 
:   uxTree(name,limit)
{
}

uxSequence::~uxSequence()
{
}


// ============================================================================
// uxSequence - Info
// ============================================================================
wxString uxSequence::GetTreeName( bool bmarkup ) const
{
    if( !Name.IsEmpty() )
        return Name;

    wxString s = wxT("SEQ(");
    for( uxTreeArray::const_iterator it=Content.begin(); it!=Content.end(); ++it )
    {
        if( it != Content.begin() )
            s.Append(wxT(','));
        s.Append(static_cast<uxTree*>(*it)->GetTreeName(false));
    }
    s.Append(wxT(')'));
    return s;
}


// ============================================================================
// uxSequence - Parsing
// ============================================================================
void uxSequence::Parse( unXmlNode*& node, uxParser* parser )
{
    Limit.Reset();

    uxDebugLog( DL_Debug, wxT("[%s] ST CURRENT: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );

    ParseInternal(node,parser);


    if( !Limit.IsValid() )
    {
        throw exString( wxString::Format( wxT("Invalid element count: %s found [%d] times, expected [%s] times!.")
            , TraceBack().c_str(), Limit.GetCount(), Limit.GetName().c_str() ) );
    } 

}

void uxSequence::ParseInternal( unXmlNode*& node, uxParser* parser )
{
    uxDebugLog( DL_Debug, wxT("[%s] SP CURRENT: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );

    // skip to first valid node type
    while( node != NULL && node->GetType() != wxXML_ELEMENT_NODE )
    {
        uxDebugLog( DL_Notice, wxT("\t[%s] SP SKIP: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );
        node = node->GetNext();
    }

    // return if no node
    if( node == NULL )
    {
        uxDebugLog( DL_Debug, wxT("\t[%s] SP NULL"), TraceBack().c_str() );
        return;
    }


    // get master
    uxSequence& master = *static_cast<uxSequence*>(Master);

    // parse node
    Limit.Increase();
    uxDebugLog( DL_Notice, wxT("[%s] SP PARSE"), TraceBack().c_str() );

    // parse content
    //unXmlNode* next = node->GetChildren();
    for( uxTreeArray::iterator it=master.Content.begin(); it!=master.Content.end(); ++it )
    {
        uxDebugLog( DL_Debug, wxT("[%s] SP CONTENT: %s"), TraceBack().c_str(), xml::TraceBack(node).c_str() );
        uxTree* tree = static_cast<uxTree*>(*it);
        tree->ParseInstance(this,node,parser);
    }

    if( !Limit.IsValid() )
    {
        return;
    } 

    if( node != NULL )
    {
        node = node->GetNext();
    }

    if( Limit.IsNextValid() )
    {
        uxDebugLog( DL_Debug, wxT("[%s] SP CONTINUE"), TraceBack().c_str() );
        ParseInternal(node,parser);
    }
}

// ============================================================================