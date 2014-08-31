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
//	uxParser.cpp
// ============================================================================
#include "prec.h"
#include "uxParser.h"
#include "uxItem.h"


// ============================================================================
// uxParser
// ============================================================================
uxParser::uxParser() 
:   Root(NULL)
{
}

uxParser::~uxParser()
{
    for( uxItemArray::iterator it=Items.begin(); it!=Items.end(); ++it )
    {
        delete (*it);
    }
}

void uxParser::ParseFile( wxFileName filename )
{
    if( !filename.FileExists() )
        throw exception( wxString::Format( wxT("Can't open file %s"), filename.GetFullPath().c_str() ).c_str() );

    unXmlDocument doc;
    if( !doc.Load( filename.GetFullPath() ) )
        throw exception( wxString::Format( wxT("File %s contains invalid xml data."), filename.GetFullPath().c_str() ).c_str() );

    unXmlNode* node = doc.GetRoot();
    if( Root && node )
        Root->ParseRoot( node, this );
}




// ============================================================================