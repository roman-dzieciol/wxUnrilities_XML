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
//  pkgType.h
// ============================================================================
#ifndef PKGTYPE_H
#define PKGTYPE_H

class pkgType;
class pkgPackage;
class exToken;
class unTextPrinter;


// ============================================================================
// pkgType - Exceptions
// ============================================================================
#define exTypeReq(STRING) \
    exString( wxString::Format( wxT("[%s][%s] Unknown type requested: %s [%s]"), TypeName.c_str(), Name.c_str(), STRING, unFuncSig ) )


// ============================================================================
// pkgType - Debug
// ============================================================================
#define PKG_LOG_TYPEINFO wxLogMessage( wxT("[%p %s][%s] - [%s] - [%s]"), this, GetTypeName().c_str(), GetName().c_str(), GetInfo().c_str(), __FUNCSIG__ )
#define PKG_LOG_TYPE wxLogMessage( wxT("[%p %s][%s] - [%s]"), this, GetTypeName().c_str(), GetName().c_str(), __FUNCSIG__ )

#define pkgLogTypeInfo      PKG_LOG_TYPEINFO
#define pkgLogType          PKG_LOG_TYPE
#define pkgLogTypeRead      //pkgLogTypeInfo
#define pkgLogTypeOnRead    //pkgLogTypeInfo
#define pkgLogTypeCopy      //pkgLogTypeInfo

#define unFuncSig           __FUNCSIG__


// ============================================================================
// pkgType - Containers
// ============================================================================
WX_DECLARE_STRING_HASH_MAP( pkgType*, pkgTypeHashMap );     
WX_DECLARE_STRING_HASH_MAP( dword, pkgIndexHashMap );     
typedef vector<pkgType*> pkgTypeArray;                     
typedef vector<dword> pkgTypePath;      

inline wxString pkgTypePathToString( const pkgTypePath& path )
{
    wxString text = wxT('[');
    for( pkgTypePath::const_iterator it=path.begin(); it!=path.end(); ++it )
    {
        text.append( wxString::Format(" %d",(*it)) );
    }
    text.append( wxT(" ]") );
    return text;
}



// ============================================================================
// pkgType
// ============================================================================
class pkgType
{
public:
    // Special
    pkgType( const wxString& tname=wxEmptyString, const wxString& name=wxEmptyString, const wxString& desc=wxEmptyString )
    :   TypeName(tname)
    ,   Name(name)
    ,   Desc(desc)
    ,   Parent(NULL)
    ,   Prototype(NULL)
    {
        Prototype = this;
    }

    // clone prototype
    pkgType( const pkgType& c )
    :   TypeName(c.TypeName)
    ,   Name(c.Name)
    ,   Desc(c.Desc)
    ,   Parent(NULL)
    ,   DataPrinter(c.DataPrinter)
    ,   TextPrinter(c.TextPrinter)
    ,   Prototype(NULL)
    {
        pkgLogTypeCopy;
        Prototype = this;
        for( pkgTypeArray::const_iterator it=c.TypeArray.begin(); it!=c.TypeArray.end(); ++it )
        {
            pkgType* type = static_cast<pkgType*>(*it);
            AddChild(type->Clone());
        }
    }

    virtual ~pkgType() 
    {
        for( pkgTypeArray::iterator it=TypeArray.begin(); it!=TypeArray.end(); ++it )
        {
            delete (*it);
        }
    }

    // Virtual ctors
    virtual pkgType* Create() { exNO_IMPL; }
    virtual pkgType* Clone() { exNO_IMPL; }
    virtual pkgType* Construct() { exNO_IMPL; }

    // Accessors
    virtual wxString GetTypeName() const { return TypeName; };
    virtual wxString GetName() const { return Name; };
    virtual wxString GetDesc() const { return Desc; };
    virtual pkgType* GetParent() const { return Parent; };
    virtual exToken* GetExpression() const { exNO_IMPL; };
    virtual wxString GetDataPrinter() const { return DataPrinter; };
    virtual wxString GetTextPrinter() const { return TextPrinter; };
    virtual pkgType* GetPrototype() const { return Prototype; };
    virtual dword GetOffset() const;

    virtual dword GetSize() const;
    virtual dword GetSerialSize() const;

    // Accessors - Tree
    pkgTypeArray& GetTypeArray() { return TypeArray; }
    pkgTypeHashMap& GetTypeByName() { return TypeByName; }
    pkgIndexHashMap& GetIndexByName() { return IndexByName; }
    virtual bool HasChildren() const { return !TypeArray.empty(); };
    virtual bool ShouldMap() const { return !Name.empty(); };


    // Mutators
    virtual void SetTypeName( const wxString& p ) { TypeName = p; }
    virtual void SetName( const wxString& p ) { Name = p; }
    virtual void SetDesc( const wxString& p ) { Desc = p; }
    virtual void SetParent( pkgType* p ) { Parent = p; }
    virtual void SetExpression( exToken* p ) { exNO_IMPL; }
    virtual void SetDataPrinter( const wxString& p ) { DataPrinter = p; }
    virtual void SetTextPrinter( const wxString& p ) { TextPrinter = p; }
    virtual void SetPrototype( pkgType* p );

    virtual void SetOffset( dword p ) { exNO_IMPL; }
    virtual void SetSize( dword p ) { exNO_IMPL; }
    virtual void SetSerialSize( dword p ) { exNO_IMPL; }

    // Serialization
    virtual void OnRead( pkgPackage& package, pkgType& parent );
    virtual void ReadChildren( pkgPackage& package, pkgType& parent ) { OnRead(package,parent); }


    // Debug info
    virtual wxString GetInfo() { return Name; }


    //
    // Data / Flow
    //

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

    // Ident
    virtual bool IsArray() { return false; }
    virtual bool IsFlow() { return false; }
    virtual bool IsData() { return false; }


    //
    // Tree
    //

    // Population
    virtual void AddChild( pkgType* type, bool bforcemap=false );

    // Find: Type
    pkgType* FindTypeByIndex( dword index, bool bcanfail=false );
    pkgType* FindTypeByTypePath( const pkgTypePath& path, bool bcanfail=false );
    pkgType* FindTypeByName( const wxString& name, bool bcanfail=false );
    pkgType* FindTypeByNamePath( const wxString& name, bool bcanfail=false );
    pkgType* FindTypeBySuperName( const wxString& name, bool bcanfail=false );

    // Find: Index
    dword FindIndexByName( const wxString& name, bool bcanfail=false );

    // Find: TypePath
    pkgTypePath FindTypePathByNamePath( const wxString& name, bool bcanfail=false );


protected:
    wxString        TypeName;
    wxString        Name;
    wxString        Desc;
    pkgType*        Parent;

    pkgTypeArray    TypeArray;
    pkgTypeHashMap  TypeByName;
    pkgIndexHashMap IndexByName;

    wxString  DataPrinter;
    wxString  TextPrinter;
    pkgType* Prototype;
};




// ============================================================================
// pkgType - Macros
// ============================================================================
#define DECLARE_NAMEPATH_CACHED_ACCESSOR(TYPEPATH,PATHNAME) \
protected: \
    pkgTypePath TYPEPATH; \
public: \
    pkgType* Get##TYPEPATH( pkgType* type ) \
    { \
        if( TYPEPATH.empty() ) \
            TYPEPATH = type->FindTypePathByNamePath(PATHNAME); \
        return type->FindTypeByTypePath(TYPEPATH); \
    }



#endif PKGTYPE_H
// ============================================================================