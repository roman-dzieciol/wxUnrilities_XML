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
//  uxProperty.h
// ============================================================================
#ifndef UXPROPERTY_H
#define UXPROPERTY_H

#include "unXmlDocument.h"
#include "uxElement.h"


// ============================================================================
// uxProperty
// ============================================================================
class uxProperty : public uxElement
{
public:
    // Allocator
    STATIC_ALLOCATOR(uxProperty);

    // Special
    uxProperty( uxItem& item, const uxLimit& limit );
    virtual ~uxProperty();
    virtual uxProperty* Clone() const { return alloc_copy(*this); }

    // Info
    wxString GetItemName();

    // Parsing
    virtual unXmlNode* Parse( unXmlNode* node, uxParser* parser, uxElement* parent );

    // Creator
    static uxElement* Create( uxItem& item, const uxLimit& limit )
    {
        return new uxProperty(item,limit);
    }
};


// ============================================================================
// uxProperty - Creator
// ============================================================================
inline uxItem* new_uxProperty( const wxString& name )
{
    return new uxItem(name, &uxProperty::Create);
}



#endif UXPROPERTY_H
// ============================================================================