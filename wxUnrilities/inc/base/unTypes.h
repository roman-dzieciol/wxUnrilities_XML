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
//	unTypes.h
// ============================================================================
#ifndef UNTYPES_H
#define UNTYPES_H


// ============================================================================
// POD
// ============================================================================
typedef unsigned __int8		byte;
typedef unsigned __int16	word;
typedef unsigned __int32	dword;
typedef unsigned __int64	qword;

typedef signed __int8		int8;
typedef signed __int16		int16;
typedef signed __int32		int32;
typedef signed __int64		int64;

typedef const char*			cstr;


// ============================================================================
// ptrvector - holds pointers to objects in heap, deletes objects in dtor
// ============================================================================
template <class T>
class ptrvector : public vector<T>
{
public:
    ~ptrvector()
    {
        for( iterator it=begin(); it!=end(); ++it )
            delete static_cast<value_type>(*it);
    }

    value_type push_existing( value_type p )
    {
        if( p != NULL )
            push_back(p);
        return p;
    }
};


// ============================================================================
// namedvector - vector with a name
// ============================================================================
template <class T>
class namedvector : public vector<T>
{
public:
    // Accessors
    wxString GetName() const { return Name; }

    // Mutators
    void SetName( const wxString& d ) { Name = d; }

protected:
    wxString Name;
};


// ============================================================================
// unStringToDword
// ============================================================================
inline dword unStringToDword( const wxString& text )
{
    unsigned long temp;
    if( !text.ToULong(&temp,0) )
        temp = 0;
    return static_cast<dword>(temp);
}


// ============================================================================
// Hash maps
// ============================================================================
WX_DECLARE_STRING_HASH_MAP( wxString, unStringMap );
WX_DECLARE_STRING_HASH_MAP( dword, unUnsignedMap );
WX_DECLARE_STRING_HASH_MAP( int, unSignedMap );



#endif UNTYPES_H
// ============================================================================