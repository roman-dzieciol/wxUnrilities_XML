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
//	pkgType.cpp
// ============================================================================
#include "prec.h"
#include "pkgType.h"
#include "pkgPackage.h"



// ============================================================================
// pkgType - Accessors
// ============================================================================
dword pkgType::GetOffset() const 
{ 
    if( HasChildren() )
    {
        for( dword i=0; i<TypeArray.size(); ++i )
        {
            dword offset = TypeArray.at(i)->GetOffset();
            if( offset != -1 )
                return offset;
        }
    }
    return -1;
};

dword pkgType::GetSize() const 
{ 
    dword size = 0;
    for( dword i=0; i!=TypeArray.size(); ++i )
    {
        size += TypeArray.at(i)->GetSize();
    }
    return size;
};

dword pkgType::GetSerialSize() const 
{ 
    dword size = 0;
    for( dword i=0; i!=TypeArray.size(); ++i )
    {
        size += TypeArray.at(i)->GetSerialSize();
    }
    return size;
};

// ============================================================================
// pkgType - Serialization
// ============================================================================
void pkgType::OnRead( pkgPackage& package, pkgType& parent )
{
    for( pkgTypeArray::iterator it=TypeArray.begin(); it!=TypeArray.end(); ++it )
    {
        pkgType* type = static_cast<pkgType*>(*it);
        type->OnRead(package,parent);
    }
}


// ============================================================================
// pkgType - Population
// ============================================================================
void pkgType::AddChild( pkgType* type, bool bforcemap )
{
    if( type != NULL )
    {
        TypeArray.push_back(type); 
        type->SetParent(this);

        if( type->ShouldMap() || bforcemap )   
        {
            IndexByName[type->GetName()] = TypeArray.size()-1;
            TypeByName[type->GetName()] = type;
        }
    }
}


// ============================================================================
// pkgType - Find: Type
// ============================================================================

//
// By Index
//
pkgType* pkgType::FindTypeByIndex( dword index, bool bcanfail )
{
    if( index < TypeArray.size() )   
        return TypeArray[index];

    if( bcanfail )        
        return NULL;
    else
        throw exTypeReq(wxString::Format( wxT("INDEX=%d"), index ).c_str());
}

//
// By TypePath
//
pkgType* pkgType::FindTypeByTypePath( const pkgTypePath& path, bool bcanfail )
{
    pkgType* type = NULL;

    pkgTypePath::const_iterator it=path.begin();
    type = FindTypeByIndex(*it,true);

    if( type != NULL )
    {
        while( ++it != path.end() )
        {
            type = type->FindTypeByIndex(*it,true);
            if( type == NULL )
                break;
        }
    }

    if( type != NULL || bcanfail )
        return type;
    else
        throw exTypeReq(wxString::Format( wxT("TYPEPATH=%s"), pkgTypePathToString(path).c_str() ).c_str());
}

//
// By Name
//
pkgType* pkgType::FindTypeByName( const wxString& name, bool bcanfail )
{
    pkgTypeHashMap::iterator it = TypeByName.find(name);
    if( it != TypeByName.end() )
        return (*it).second;

    if( bcanfail )
        return NULL;
    else
        throw exTypeReq(wxString::Format( wxT("NAME=%s"), name.c_str() ).c_str());
}

//
// By NamePath
//
pkgType* pkgType::FindTypeByNamePath( const wxString& name, bool bcanfail )
{
    wxStringTokenizer parser( name, wxT("/"), wxTOKEN_DEFAULT );
    pkgType* type = NULL;

    if( parser.HasMoreTokens() )
    {
        wxString item = parser.GetNextToken();
        type = FindTypeByName(item,true);

        if( type != NULL )
        {
            while( parser.HasMoreTokens() )
            {
                item = parser.GetNextToken();
                type = type->FindTypeByName(item,true);
                if( type == NULL )
                    break;
            } 
        }
    }

    if( type != NULL || bcanfail )
        return type;
    else
        throw exTypeReq(wxString::Format( wxT("NAMEPATH=%s"), name.c_str() ).c_str());
}

//
// By SuperName
//
pkgType* pkgType::FindTypeBySuperName( const wxString& name, bool bcanfail )
{
    pkgType* type = FindTypeByName(name,true);

    if( type == NULL && Parent != NULL )
        type = Parent->FindTypeBySuperName(name,true);

    if( type != NULL || bcanfail )
        return type;
    else
        throw exTypeReq(wxString::Format( wxT("SUPERNAME=%s"), name.c_str() ).c_str());
}


// ============================================================================
// pkgType - Find: Index
// ============================================================================

//
// By Name
//
dword pkgType::FindIndexByName( const wxString& name, bool bcanfail )
{
    pkgIndexHashMap::iterator it = IndexByName.find(name);
    if( it != IndexByName.end() )
        return static_cast<dword>((*it).second);

    if( bcanfail )
        return -1;
    else
        throw exTypeReq(wxString::Format( wxT("NAME=%s"), name.c_str() ).c_str());
}


// ============================================================================
// pkgType - Find: TypePath
// ============================================================================

//
// By NamePath
//
pkgTypePath pkgType::FindTypePathByNamePath( const wxString& name, bool bcanfail )
{
    pkgTypePath path;

    wxStringTokenizer parser( name, wxT("/"), wxTOKEN_DEFAULT );

    if( parser.HasMoreTokens() )
    {
        wxString item = parser.GetNextToken();
        dword index = FindIndexByName(item,true);

        if( index != -1 )
        {
            path.push_back(index);

            while( parser.HasMoreTokens() )
            {
                item = parser.GetNextToken();
                pkgType* type = FindTypeByIndex(index,true);
                if( type != NULL )
                {
                    index = type->FindIndexByName(item,true);
                    if( index != -1 )
                    {
                        path.push_back(index);
                        continue;
                    }
                }

                // failed
                path = pkgTypePath();
                break;
            } 
        }
    }

    if( !path.empty() || bcanfail )
        return path;
    else
        throw exTypeReq(wxString::Format( wxT("NAMEPATH=%s"), name.c_str() ).c_str());
}


// ============================================================================
// pkgType
// ============================================================================
void pkgType::SetPrototype( pkgType* p )
{
    Prototype = p;
    DataPrinter = p->GetDataPrinter();
    TextPrinter = p->GetTextPrinter();
}


// ============================================================================