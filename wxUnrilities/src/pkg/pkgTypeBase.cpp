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
//	pkgTypeBase.cpp
// ============================================================================
#include "prec.h"
#include "pkgTypeBase.h"
#include "pkgPackage.h"
#include "pkgPackageData.h"
#include "pkgPackageFormat.h"
#include "pkgType.h"
#include "unTextPrinterPresets.h"
#include "unTextPrinter.h"

const wxString tbINT8::StaticTypeName           = wxT("INT8");
const wxString tbINT16::StaticTypeName          = wxT("INT16");
const wxString tbINT32::StaticTypeName          = wxT("INT32");
const wxString tbFLOAT::StaticTypeName          = wxT("FLOAT");
const wxString tbBYTE::StaticTypeName           = wxT("BYTE");
const wxString tbWORD::StaticTypeName           = wxT("WORD");
const wxString tbDWORD::StaticTypeName          = wxT("DWORD");
const wxString tbQWORD::StaticTypeName          = wxT("QWORD");
const wxString tbGUID::StaticTypeName           = wxT("GUID");
const wxString tbINDEX::StaticTypeName          = wxT("INDEX");
const wxString tbASCIIZ::StaticTypeName         = wxT("ASCIIZ");
const wxString tbLASCIIZ::StaticTypeName        = wxT("LASCIIZ");
const wxString tbLASCII::StaticTypeName         = wxT("LASCII");
const wxString tbOBJECT::StaticTypeName         = wxT("OBJECT");
const wxString tbOBJIND::StaticTypeName         = wxT("OBJIND");
const wxString tbOBJVAR::StaticTypeName         = wxT("OBJVAR");
const wxString tbPROP::StaticTypeName           = wxT("PROP");
const wxString tbINTERNAL::StaticTypeName       = wxT("");

const wxString tbINT8::StaticDesc               = wxT("Signed number, 1 bytes.");
const wxString tbINT16::StaticDesc              = wxT("Signed number, 2 bytes.");
const wxString tbINT32::StaticDesc              = wxT("Signed number, 4 bytes.");
const wxString tbBYTE::StaticDesc               = wxT("Unsigned number, 1 byte.");
const wxString tbWORD::StaticDesc               = wxT("Unsigned number, 2 Bytes.");
const wxString tbDWORD::StaticDesc              = wxT("Unsigned number, 4 bytes.");
const wxString tbQWORD::StaticDesc              = wxT("Unsigned number, 8 bytes.");
const wxString tbFLOAT::StaticDesc              = wxT("Floating point number, 4 bytes.");
const wxString tbGUID::StaticDesc               = wxT("Unique identifier, 16 bytes.");
const wxString tbINDEX::StaticDesc              = wxT("Unsigned compressed number, 1-5 bytes.");
const wxString tbASCIIZ::StaticDesc             = wxT("String, zero-terminated.");
const wxString tbLASCIIZ::StaticDesc            = wxT("String, zero-terminated, starts with length serialized as index.");
const wxString tbLASCII::StaticDesc             = wxT("String, starts with length serialized as index.");
const wxString tbOBJECT::StaticDesc             = wxT("Composite object, name holds object type.");
const wxString tbOBJIND::StaticDesc             = wxT("Composite object, name holds object type, lazy loading.");
const wxString tbOBJVAR::StaticDesc             = wxT("Composite object, name holds name of variable holding object type.");
const wxString tbPROP::StaticDesc               = wxT("Composite object, creates property tree.");
const wxString tbINTERNAL::StaticDesc           = wxT("For internal use only.");



// ============================================================================
// PropertyInfo
// ============================================================================
#define PR_MASK_TYPE    0x0f
#define PR_MASK_SIZE    0x70
#define PR_MASK_ARRAY   0x80


// ============================================================================
// pkgTypeBaseRegistry
// ============================================================================
void pkgTypeBaseRegistry::OnReload()
{
    AddChild( new tbINT8, true );
    AddChild( new tbINT16, true );
    AddChild( new tbINT32, true );
    AddChild( new tbBYTE, true );
    AddChild( new tbWORD, true );
    AddChild( new tbDWORD, true );
    AddChild( new tbQWORD, true );
    AddChild( new tbGUID, true );
    AddChild( new tbINDEX, true );
    AddChild( new tbASCIIZ, true );
    AddChild( new tbLASCIIZ, true );
    AddChild( new tbLASCII, true );
    AddChild( new tbOBJECT, true );
    AddChild( new tbOBJVAR, true );
    AddChild( new tbOBJIND, true );
    AddChild( new tbPROP, true );
}


// ============================================================================
// pkgTypeBase
// ============================================================================
void pkgTypeBase::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;

    // create data type
    pkgTypeBase* datatype = Construct();
    datatype->SetPrototype(this);
    datatype->SetParent(&parent);

    // add data type
    parent.AddChild(datatype);

    // store offset
    datatype->SetOffset( static_cast<dword>(package.GetFile().Tell()) );

    // read data
    datatype->Read(package);

}


// ============================================================================
// tbBYTE
// ============================================================================
void tbBYTE::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbWORD
// ============================================================================
void tbWORD::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbDWORD
// ============================================================================
void tbDWORD::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbQWORD
// ============================================================================
void tbQWORD::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}


// ============================================================================
// tbINT8
// ============================================================================
void tbINT8::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbINT16
// ============================================================================
void tbINT16::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbINT32
// ============================================================================
void tbINT32::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbFLOAT
// ============================================================================
void tbFLOAT::Read( pkgPackage& package )
{
    package >> Data;
    pkgLogTypeRead;
}

// ============================================================================
// tbGUID
// ============================================================================
void tbGUID::Read( pkgPackage& package )
{
    package >> A;
    package >> B;
    package >> C;
    package >> D;
    pkgLogTypeRead;
}


// ============================================================================
// tbINDEX
// ============================================================================
void tbINDEX::Read( pkgPackage& package )
{
    Data = 0;
    SerialSize = 0;

    byte B0;
    package >> B0;
    ++SerialSize;
    if(B0 & 0x40){
        byte B1;
        package >> B1;
        ++SerialSize;
        if(B1 & 0x80){
            byte B2;
            package >> B2;
            ++SerialSize;
            if(B2 & 0x80){
                byte B3;
                package >> B3;
                ++SerialSize;
                if(B3 & 0x80){
                    byte B4;
                    package >> B4;
                    ++SerialSize;
                    Data = B4;
                }Data = (Data << 7) | (B3 & 0x7f);
            }Data = (Data << 7) | (B2 & 0x7f);
        }Data = (Data << 7) | (B1 & 0x7f);
    }Data = (Data << 6) | (B0 & 0x3f);

    if(B0 & 0x80) 
        Data = -Data;

    pkgLogTypeRead;
}


// ============================================================================
// tbASCIIZ
// ============================================================================
void tbASCIIZ::Read( pkgPackage& package )
{
    Data.Clear();
    dword len = 0;
    wxChar c;

    // ASSERT:: extra 0 isnt added
    do		{ package >> c; Data.Append(c); ++len; } 
    while	( c != 0 ); 

    wxASSERT( len < tbASCIIZ::MaxLength );
    pkgLogTypeRead;
}

// ============================================================================
// tbLASCIIZ
// ============================================================================
void tbLASCIIZ::Read( pkgPackage& package )
{
    Length.Read(package);
    dword len = Length.GetSigned();
    wxASSERT( len < tbLASCIIZ::MaxLength );

    Data.Clear();
    package.Read( Data.GetWriteBuf(len), len );
    Data.UngetWriteBuf(--len); // dont put 0 into string buffer

    pkgLogTypeRead;
}

// ============================================================================
// tbLASCII
// ============================================================================
void tbLASCII::Read( pkgPackage& package )
{
    Length.Read(package);
    dword len = Length.GetSigned();
    wxASSERT( len < tbLASCII::MaxLength );

    Data.Clear();
    package.Read( Data.GetWriteBuf(len), len );
    Data.UngetWriteBuf(len);

    pkgLogTypeRead;
}


// ============================================================================
// tbOBJECT
// ============================================================================
void tbOBJECT::Read( pkgPackage& package )
{
    pkgLogTypeRead;
}

void tbOBJECT::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;

    // find and read object template
    pkgType* object = package.GetFormat().GetHeaderTypes().FindTypeByName(Name);
    object->OnRead(package,parent);
}


// ============================================================================
// tbOBJVAR
// ============================================================================
void tbOBJVAR::Read( pkgPackage& package )
{
    pkgLogTypeRead;
}

void tbOBJVAR::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;

    // find and read object template
    pkgType* type = parent.FindTypeBySuperName(Name,true);
    if( type == NULL )
        return;
    //unTextPrinter* printer = GTextPrinterPresets.Get( type->GetPrototype()->GetTextPrinter(), &package );


    wxString objtype = GTextPrinterPresets( type->GetTextPrinter(), &package )->GetString( type );
    int i = objtype.rfind(wxT('.'));
    if( i != -1 )
        objtype = objtype.Mid(i+1);

    if( objtype.empty() )
        objtype = wxT("Class");

    wxLogMessage("tbOBJVAR %s",objtype);

    pkgType* object = package.GetFormat().GetHeaderTypes().FindTypeByName(objtype,true);
    if( object == NULL )
        return;

    object->OnRead(package,parent);
}


// ============================================================================
// tbOBJIND
// ============================================================================
void tbOBJIND::Read( pkgPackage& package )
{
    pkgLogTypeRead;
}

void tbOBJIND::OnRead( pkgPackage& package, pkgType& parent )
{
    // find and read object template
    if( (package.GetLoadFlags() & PLF_Force) == PLF_Force )
    {
        pkgLogTypeOnRead;
        pkgType* object = package.GetFormat().GetHeaderTypes().FindTypeByName(Name);
        object->OnRead(package,parent);
    }
}


// ============================================================================
// tbPROP
// ============================================================================
void tbPROP::Read( pkgPackage& package )
{
    pkgLogTypeRead;
}

void tbPROP::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;


    // create data type
    pkgTypeBase* datatype = Construct();
    datatype->SetPrototype(this);
    datatype->SetParent(&parent);
    parent.AddChild(datatype);
    datatype->Read(package);

	ReadProperty(package,parent,datatype);
}
    
void tbPROP::ReadProperty( pkgPackage& package, pkgType& parent, pkgType* datatype )
{
    // create property type
    tbOBJECT* t_prop = new tbOBJECT;
    t_prop->SetName(wxT("Property"));
    t_prop->SetPrototype(datatype);
    t_prop->SetParent(datatype);
    datatype->AddChild(t_prop);
    //t_prop->SetDataPrinter( wxT("TSigned") );
    t_prop->SetTextPrinter( wxT("TPropertyData") );
	//t_prop->SetSigned(1);

    // read property name
    tbINDEX* i_index = CreateInstance<tbINDEX>(wxT("Name"),package,t_prop);
    i_index->SetTypeName(wxT(""));
    i_index->SetDataPrinter( wxT("TSigned") );
    i_index->SetTextPrinter( wxT("TObjName") );

    // update property type 
    unTextPrinter* pr_name = GTextPrinterPresets.Get( wxT("TObjName"), &package );
    wxString str_name = pr_name->GetString(i_index);
    t_prop->SetName(str_name);

    if( wxString(wxT("None")).IsSameAs(str_name) )
        return;

    // property info byte
    tbINTERNAL* i_pinfo = CreateInstance<tbINTERNAL>(wxT("Info Byte"),package,t_prop);
    //i_pinfo->SetDataPrinter( wxT("TSigned") );
    //i_pinfo->SetTextPrinter( wxT("TString") );
    i_pinfo->SetSerialSize(1);

    // property type bits
    tbINTERNAL* i_ptype = CreateInstance<tbINTERNAL>(wxT("Property Type"),package,i_pinfo);
    i_ptype->SetDataPrinter( wxT("TSigned") );
    i_ptype->SetTextPrinter( wxT("TString") );
    i_ptype->SetSerialSize(1);

    // property size bits
    tbINTERNAL* i_psizet = CreateInstance<tbINTERNAL>(wxT("Size Type"),package,i_pinfo);
    i_psizet->SetDataPrinter( wxT("THex2") );
    i_psizet->SetTextPrinter( wxT("TString") );
    i_psizet->SetSerialSize(1);
	
    // property size
	tbINTERNAL* i_psize = CreateInstance<tbINTERNAL>(wxT("Size"),package,i_pinfo);
	i_psize->SetDataPrinter( wxT("TSigned") );
	i_psize->SetTextPrinter( wxT("TString") );
    i_psize->SetSerialSize(1);

    // property array bit
    tbINTERNAL* i_parr = CreateInstance<tbINTERNAL>(wxT("IsArray"),package,i_pinfo);
    i_parr->SetDataPrinter( wxT("TSigned") );
    i_parr->SetTextPrinter( wxT("TString") );
    i_parr->SetSerialSize(1);


    // read info byte
    byte info = 0;
    package >> info;
    byte itype = info & PR_MASK_TYPE;
    dword isizetype = info & PR_MASK_SIZE;
    byte iarr = (info & PR_MASK_ARRAY) == PR_MASK_ARRAY;
	wxString idata;

	dword isersize = GetPropertySerialSize(package,isizetype);

    // get property type name
    wxString itypename = GetPropertyName(itype);

    // get struct name
	if( itype == 10 )
	{
		// read property name
		tbINDEX* i_pstruct = CreateInstance<tbINDEX>(wxT("Struct Type"),package,t_prop);
		i_pstruct->SetTypeName(wxT(""));
		i_pstruct->SetDataPrinter( wxT("TSigned") );
		i_pstruct->SetTextPrinter( wxT("TObjName") );
	}

	// read size
    dword isize = GetPropertySize(package,isizetype);

	// update property size
	i_psize->SetUnsigned(isize);

    // property extended size
    if( isersize != 0 )
    {
		tbINTERNAL* i_pexsize = CreateInstance<tbINTERNAL>(wxT("Extended Size"),package,t_prop);
		i_pexsize->SetDataPrinter( wxT("TSigned") );
		i_pexsize->SetTextPrinter( wxT("TString") );
        i_pexsize->SetSerialSize(isersize);
        i_pexsize->SetOffset(static_cast<dword>(package.GetFile().Tell())-isersize);
		i_pexsize->SetUnsigned(isize);
    }
	else
	{
	}
	

	// data
	switch(itype)
    {
		case 1: // wxT("ByteProperty");
		{
			tbBYTE* i_pdata = CreateInstance<tbBYTE>(wxT("Data"),package,t_prop);
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TSigned") );
			break;
		}
		case 2: // wxT("IntegerProperty");
		{
			tbINT32* i_pdata = CreateInstance<tbINT32>(wxT("Data"),package,t_prop);
			i_pdata->SetTypeName(wxT(""));
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TSigned") );
			break;
		}
		case 3: // wxT("BooleanProperty");
		{
			tbINTERNAL* i_pdata = CreateInstance<tbINTERNAL>(wxT("Data"),package,t_prop);
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TBool") );
			i_pdata->SetSigned(iarr);
			i_pdata->SetSerialSize(isize);
			i_pdata->SetOffset(i_pinfo->GetOffset());
			break;
		}
		case 4: // wxT("FloatProperty");
		{
			tbFLOAT* i_pdata = CreateInstance<tbFLOAT>(wxT("Data"),package,t_prop);
			i_pdata->SetTypeName(wxT(""));
			i_pdata->SetDataPrinter( wxT("TString") );
			i_pdata->SetTextPrinter( wxT("TString") );
			break;
		}
		case 5: // wxT("ObjectProperty");
		{
			tbINDEX* i_pdata = CreateInstance<tbINDEX>(wxT("Data"),package,t_prop);
			i_pdata->SetTypeName(wxT(""));
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TObjRef") );
			break;
		}
		case 6: // wxT("NameProperty");
		{
			tbINDEX* i_pdata = CreateInstance<tbINDEX>(wxT("Data"),package,t_prop);
			i_pdata->SetTypeName(wxT(""));
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TObjName") );
			break;
		}
    //case 7:  wxT("StringProperty");
    //case 8:  wxT("ClassProperty");
    //case 9:  wxT("ArrayProperty");
    //case 10:  wxT("StructProperty");
    //case 11:  wxT("VectorProperty");
    //case 12:  wxT("RotatorProperty");
		case 13: // wxT("StrProperty");
		{
			tbLASCIIZ* i_pdata = CreateInstance<tbLASCIIZ>(wxT("Data"),package,t_prop);
			i_pdata->SetTypeName(wxT(""));
			i_pdata->SetDataPrinter( wxT("TSigned") );
			i_pdata->SetTextPrinter( wxT("TString") );
			break;
		}
    //case 14:  wxT("MapProperty");
    //case 15:  wxT("FixedArrayProperty");
		default:
		{
			if( isize > 0 )
			{
				tbINTERNAL* i_pdata = CreateInstance<tbINTERNAL>(wxT("Data"),package,t_prop);
				i_pdata->SetDataPrinter( wxT("TString") );
				i_pdata->SetTextPrinter( wxT("TString") );
				i_pdata->SetSerialSize(isize);
				package.Read( idata.GetWriteBuf(isize), isize );
				idata.UngetWriteBuf(isize-1); // dont put 0 into string buffer
				i_pdata->SetString(idata);
			}
			break;
		}
    }



    // get property array index



    // update internal types
    i_ptype->SetSigned(itype);
    i_ptype->SetString(itypename);
    i_psizet->SetSigned(isizetype);
    i_parr->SetSigned(iarr);

    // bool fixup
    //if( itype == 3 )
    //    i_parr->SetName(wxT("Data"));
		//i_parr->SetTypeName(wxT("Data"));

	ReadProperty(package,parent,datatype);
}

wxString tbPROP::GetPropertyName( byte b )
{
    switch(b)
    {
    case 1: return wxT("ByteProperty");
    case 2: return wxT("IntegerProperty");
    case 3: return wxT("BooleanProperty");
    case 4: return wxT("FloatProperty");
    case 5: return wxT("ObjectProperty");
    case 6: return wxT("NameProperty");
    case 7: return wxT("StringProperty");
    case 8: return wxT("ClassProperty");
    case 9: return wxT("ArrayProperty");
    case 10: return wxT("StructProperty");
    case 11: return wxT("VectorProperty");
    case 12: return wxT("RotatorProperty");
    case 13: return wxT("StrProperty");
    case 14: return wxT("MapProperty");
    case 15: return wxT("FixedArrayProperty");
    default: return wxT("UnknownProperty");
    }
}

dword tbPROP::GetPropertySize( pkgPackage& package, dword sizetype )
{
    switch( sizetype ) 
    {
    case	0x00:	return 1;	
    case	0x10:	return 2;											
    case	0x20:	return 4;											
    case	0x30:	return 12;										
    case	0x40:	return 16;	
    case	0x50:	byte  psizeb; package >> psizeb; return psizeb;       
    case	0x60:	word  psizew; package >> psizew; return psizew;	      
    case	0x70:	dword psized; package >> psized; return psized;
    default:		
        throw exFName( wxString::Format( wxT("Unknown size type: %d"), sizetype ).c_str() );
    }
}

dword tbPROP::GetPropertySerialSize( pkgPackage& package, dword sizetype )
{
    switch( sizetype ) 
    {
    case	0x00:	
    case	0x10:										
    case	0x20:									
    case	0x30:									
    case	0x40:	return 0;	
    case	0x50:	return 1;	 
    case	0x60:	return 2;	      
    case	0x70:	return 4;	 
    default:		
        throw exFName( wxString::Format( wxT("Unknown size type: %d"), sizetype ).c_str() );
    }
}


// ============================================================================
// tbINTERNAL
// ============================================================================
void tbINTERNAL::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;

    // create data type
    pkgTypeBase* instance = Construct();
    instance->SetPrototype(this);
    instance->SetParent(&parent);

    // add data type
    parent.AddChild(instance);

    // store offset
    instance->SetOffset( static_cast<dword>(package.GetFile().Tell()) );
}

void tbINTERNAL::Read( pkgPackage& package )
{
    pkgLogTypeRead;
}


// ============================================================================