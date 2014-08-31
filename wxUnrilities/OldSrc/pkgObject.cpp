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
//	pkgObject.cpp
// ============================================================================
#include "prec.h"
#include "pkgObject.h"
#include "pkgSerializer.h"
#include "pkgProperty.h"
#include "pkgProperties.h"
#include "pkgNatives.h"
#include "pkgDataTypes.h"



// ============================================================================
// pkgObject - Static
// ============================================================================
const wxString pkgObject::FunctionName = wxT("Function");


// ============================================================================
// pkgObject
// ============================================================================
pkgObject::pkgObject( pkgSerializer* serializer, dword index ) 
:   Serializer(serializer)
,   ExportIndex(index)
,   ExportItem(Serializer->GetExportItem(index))
,   StateFrame(NULL)
,   Class(NULL)
,   Function(NULL)
{
    Flags = ExportItem.ObjectFlags;
}

pkgObject::~pkgObject()
{
    delete StateFrame;
    delete Class;
    delete Function;
}


// ============================================================================
// pkgObject - Serialization
// ============================================================================
bool pkgObject::InspectObject()
{
    try
    {
        LoadObject();
        return true;
    }
    catch( exception& e )
    {
        wxString oname = Serializer->GetObjectName( ExportIndex+1 );
        wxLogMessage(wxT("!\t[%s] %s"), oname.c_str(), e.what()); 
        //throw exception(s.c_str()); 
    }
    return false;
}

void pkgObject::LoadObject()
{
    dword range = ExportItem.SerialOffset + ExportItem.SerialSize;
    pkgSerializer& A = *Serializer;
    A.Seek( ExportItem.SerialOffset );

    if( A.Tell() >= range )
        return;

    if( Flags.HasFlag( *Serializer->GetFlags()->FindFlag(wxT("Object"),wxT("HasStack")) ) )
    {
        StateFrame = new pkgStateFrame;
        A >> *StateFrame;
    }

    if( A.Tell() >= range )
        return;

    if( ExportItem.Class == 0 )
    {
        Class = new pkgClass;
        A >> *Class;

        if( !Class->HasScript() )
            LoadProperties();
    }
    else
    {
        LoadProperties();

        if( FunctionName.IsSameAs(Serializer->GetClassName(ExportItem.Class)) )
        {
            Function = new pkgFunction;
            A >> *Function;
        }
    }
}

void pkgObject::LoadProperties()
{
    try
    {
        pkgArchive& A = *Serializer;
        dword range = ExportItem.SerialOffset + ExportItem.SerialSize;
        
        do
        {
            if( A.Tell() > range )
                throw exception( wxT("Property read beyond object range.") );  
        } while( Properties.push_existing( GPropertyFactory.Create(*Serializer,A) ) != NULL );
    }
    catch( exception& e )
    {
        wxString oname = Serializer->GetObjectName( ExportIndex+1 );
        if( Properties.empty() )
        {
            wxLogMessage(wxT("!\t[%s] First Property, %s"), oname.c_str(), e.what()); 
        }
        else
        {
            wxLogMessage(wxT("!\t[%s] After Property %s, %s"), oname.c_str(), Properties.back()->GetName().c_str(), e.what()); 
        }

        FixProperties();
    }
}


// ============================================================================
// pkgObject - Fixes
// ============================================================================
void pkgObject::FixProperties()
{
    pkgArchive& A = *Serializer;
    pkgSerializer& S = *Serializer;
    wxString oname = Serializer->GetObjectName( ExportIndex+1 );

    // see if that could be broken array
    for( pkgProperties::reverse_iterator it = Properties.rbegin(); it!=Properties.rend(); ++it )
    {
        pkgProperty* pt = (*it);
        if( pt->GetType() == 0x9 )
        {
            wxLogMessage(wxT("!\tAttemting to read array %s.%s alternatively."), oname.c_str(), pt->GetName().c_str()); 
            A.Seek( pt->GetOffset() );
            FixArrayProperty(pt);
            break;
        }
    }
}

void pkgObject::FixArrayProperty( pkgProperty* p )
{
    pkgArchive& A = *Serializer;
    pkgSerializer& S = *Serializer;
    wxString oname = Serializer->GetObjectName( ExportIndex+1 );

    // delete properties up to broken array
    dword count = 0;
    for( pkgProperties::reverse_iterator it = Properties.rbegin(); it!=Properties.rend(); ++it )
    {
        pkgProperty* pt = (*it);
        ++count;
        if( pt == p )
        {
            delete pt;
            break;
        }
        else
        {
            delete pt;
        }
    }
    Properties.resize(Properties.size()-count);

    // try alternate way
    try
    {
        Properties.push_existing(  new pkgpArrayAlt(S,A,pkgProperty(S,A)) );
    }
    catch( exception& )
    {
        throw exception( wxT("!\tAlternate read method failed.") );
    }

    // load the rest of properties
    LoadProperties();
}


// ============================================================================
// pkgObject - Debug
// ============================================================================
void pkgObject::DumpProperties()
{
    wxString oname = Serializer->GetObjectName( ExportIndex+1 );
    wxString props;
    for( pkgProperties::iterator it = Properties.begin(); it!=Properties.end(); ++it )
    {
        pkgProperty* p = (*it);
        props.Append( p->DumpInfo() );
        if( it+1 != Properties.end() )
            props.Append( wxT(", ") );
    }

    if( Properties.begin() == Properties.end() )
    {
        //wxLogMessage( wxT("[%s] No Properties"), oname );
    }
    else
    {
        wxLogMessage( wxT("[%s] %s"), oname, props );
    }
}


void pkgObject::Decompile()
{
}

void pkgObject::GetNativeFunctionIndex()
{
    try
    {
        // only functions
        if( !FunctionName.IsSameAs(Serializer->GetClassName(ExportItem.Class)) )
            return;

        dword range = ExportItem.SerialOffset + ExportItem.SerialSize;
        pkgSerializer& A = *Serializer;
        A.Seek( ExportItem.SerialOffset );

        if( A.Tell() >= range )
            return;

        if( Flags.HasFlag( *Serializer->GetFlags()->FindFlag(wxT("Object"),wxT("HasStack")) ) )
        {
            StateFrame = new pkgStateFrame;
            A >> *StateFrame;
        }

        if( A.Tell() >= range )
            return;

        LoadProperties();

        dtIndex superfield(A);
        dtIndex next(A);
        dtIndex scripttext(A);
        dtIndex children(A);
        dtIndex friendlyname(A); 

        A.Seek( ExportItem.SerialOffset+ExportItem.SerialSize-7 );

        dtWord inative(A);
        dtByte precedence(A);
        unFlag flags;  
        A >> flags;
   
        if( inative.GetData() == 0 )
            return;


        unFlagGroup* group = Serializer->GetFlags()->FindGroup(wxT("Function"));

        if(!(flags.HasFlag( *group->FindFlag(wxT("Native")) )
        ||   flags.HasFlag( *group->FindFlag(wxT("Operator")) )
        ||   flags.HasFlag( *group->FindFlag(wxT("PreOperator")) )
        ))
            return;

        wxFileName xpath( wxT("dump.xml") );
        xpath.Normalize();

        wxTextFile xdoc( xpath.GetFullPath() );
        if( !xdoc.Exists() )
        {
            xdoc.Create();
        }
        else
        {
            xdoc.Open();
        }

        wxString fname = Serializer->GetName( ExportItem.ObjectName );
        wxString line;

        line.Append( wxT("<func>") );

        line.Append( wxString::Format(wxT("<pkg>%s</pkg>"), Serializer->GetObjectName( ExportItem.Package ).c_str() ));
        line.Append( wxString::Format(wxT("<id>0x%.4X</id>"), inative.GetData() ));
        line.Append( wxString::Format(wxT("<op>0x%.2X</op>"), precedence.GetData() ));
        line.Append( wxString::Format(wxT("<flags>0x%.8X</flags>"), flags ));

        line.Append( wxString::Format(wxT("<name>%s</name>"), Serializer->GetName( ExportItem.ObjectName ).c_str() ));
        if( ExportItem.ObjectName != friendlyname.GetData() )
            line.Append( wxString::Format(wxT("<fname>%s</fname>"), Serializer->GetName( friendlyname.GetData() ).c_str() ));

        //line.Append( wxT('\n') );

        /*vector<dtToken*>& stream = Function->ScriptStream;
        for( vector<dtToken*>::iterator it = stream.begin(); it!= stream.end(); ++it )
        {
            dtToken* t = static_cast<dtToken*>(*it);
            line.Append( wxString::Format(wxT("<t>0x%.2X</t>\n"), t->GetData() ));
        }*/
        line.Append( wxT("</func>") );

        xdoc.AddLine( line );
        xdoc.Write(wxTextFileType_None);
        xdoc.Close();

    }
    catch( exception& e )
    {
        wxLogMessage(wxT("%s"), e.what()); 
    }
}

void pkgObject::GetNativeFunctionIndex2()
{
    wxLogMessage("GetNativeFunctionIndex");

    // only functions
    if( !FunctionName.IsSameAs(Serializer->GetClassName(ExportItem.Class)) )
    {
        return;
    }

    // library file must exist
    wxDynamicLibrary lib;
    wxFileName libpath( Serializer->GetFileName() );
    libpath.ClearExt();
    libpath = lib.CanonicalizeName(libpath.GetFullPath());
    if( !libpath.FileExists() )
    {
        wxLogMessage( "File not found: %s", libpath.GetFullPath().c_str() );
        return;
    }

    // load library
    if( !lib.Load( libpath.GetFullPath(), wxDL_DEFAULT | wxDL_VERBATIM ) )
    {
        return;
    }

    wxString oname = Serializer->GetObjectName( ExportItem.Package );
    wxString fname = Serializer->GetName( ExportItem.ObjectName );
    wxString symbol_u = wxString::Format( wxT("intU%sexec%s"), oname.c_str(), fname.c_str() );
    wxString symbol_a = wxString::Format( wxT("intA%sexec%s"), oname.c_str(), fname.c_str() );
    wxString symbolname;


    void* symbol = NULL;
    bool exists = false;

    if( lib.HasSymbol(symbol_u) ) 
    {
        symbolname = symbol_u;
        symbol = lib.GetSymbol(symbolname,&exists);
    }
    else if( lib.HasSymbol(symbol_a) ) 
    {
        symbolname = symbol_a;
        symbol = lib.GetSymbol(symbolname,&exists);
    }

    if( !exists )
        return;


    dword idx = *static_cast<int*>(symbol);

    wxLogMessage( "%d %s", idx, symbolname.c_str() );
}


// ============================================================================