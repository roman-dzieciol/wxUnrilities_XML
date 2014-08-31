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
//  utTypeCastor.h
// ============================================================================
//  TypeCastors are like functors, with function call on type cast.
//  This way TypeCastor can replace objects in a transparent way.
// ============================================================================
#ifndef UTTYPECASTOR_H
#define UTTYPECASTOR_H

#include "utParams.h"


// ============================================================================
// tcFactory :: 
// ============================================================================
template<class TO>
class tcFactory
{
public:
    operator TO*() const   { return new TO; }
    TO* operator()() const { return new TO; }
};


// ============================================================================
// tcCast :: 
// ============================================================================
template<class TC, class TI>
class tcCast
{
public:
    tcCast( TI instance ) : Instance(instance)
    {
    }

    operator TC()   { return static_cast<TC>(Instance); }
    TC operator()() { return static_cast<TC>(Instance); }

protected:
    TI Instance;
};

template<class TC, class TI>
inline tcCast<TC,TI> make_tcCast( TI instance )
{
    return tcCast<TC,TI>( instance );
}


// ============================================================================
// tcCastDouble :: 
// ============================================================================
template<class TT, class TF, class TI>
class tcCastDouble
{
public:
    tcCastDouble( TI instance ) : Instance(instance)
    {
    }

    operator TT()   { return static_cast<TT>(static_cast<TF>(Instance)); }
    TT operator()() { return static_cast<TT>(static_cast<TF>(Instance)); }

protected:
    TI Instance;
};

template<class TC, class TF, class TI>
inline tcCastDouble<TC,TF,TI> make_tcCastDouble( TI instance )
{
    return tcCastDouble<TC,TF,TI>( instance );
}

template<class TC, class TI>
inline tcCastDouble<TC,void*,TI> make_tcCastVoid( TI instance )
{
    return tcCastDouble<TC,void*,TI>( instance );
}


// ============================================================================
// tcNodeName
// ============================================================================
template<class TI>
class tcNodeName
{
public:
    tcNodeName( TI instance ) : Instance(instance) {}
    operator wxString()   { return xml::GetNodeText(static_cast<unXmlNode*>(Instance)); }
    wxString operator()() { return xml::GetNodeText(static_cast<unXmlNode*>(Instance)); }

protected:
    TI Instance;
};

template<class TI>
inline tcNodeName<TI> make_tcNodeName( TI instance )
{
    return tcNodeName<TI>(instance);
}


// ============================================================================
// tcNodeData
// ============================================================================
template<class TI>
class tcNodeData
{
public:
    tcNodeData( TI instance ) : Instance(instance) {}
    operator dword()   { return unStringToDword(xml::GetNodeText(static_cast<unXmlNode*>(Instance))); }
    dword operator()() { return unStringToDword(xml::GetNodeText(static_cast<unXmlNode*>(Instance))); }

protected:
    TI Instance;
};

template<class TI>
inline tcNodeData<TI> make_tcNodeData( TI instance )
{
    return tcNodeData<TI>(instance);
}


// ============================================================================
// tcPropName
// ============================================================================
template<class TI>
class tcPropName
{
public:
    tcPropName( TI instance ) : Instance(instance) {}
    operator wxString()   { return xml::GetPropText(static_cast<unXmlProperty*>(Instance)); }
    wxString operator()() { return xml::GetPropText(static_cast<unXmlProperty*>(Instance)); }

protected:
    TI Instance;
};

template<class TI>
inline tcPropName<TI> make_tcPropName( TI instance )
{
    return tcPropName<TI>(instance);
}


// ============================================================================
// tcPropData
// ============================================================================
template<class TI>
class tcPropData
{
public:
    tcPropData( TI instance ) : Instance(instance) {}
    operator dword()   { return unStringToDword(xml::GetPropText(static_cast<unXmlProperty*>(Instance))); }
    dword operator()() { return unStringToDword(xml::GetPropText(static_cast<unXmlProperty*>(Instance))); }

protected:
    TI Instance;
};

template<class TI>
inline tcPropData<TI> make_tcPropData( TI instance )
{
    return tcPropData<TI>(instance);
}


// ============================================================================
// tcTypeCall
// ============================================================================
template<class TI, class TF, class TP>
class tcTypeCall
{
public:
    tcTypeCall( TI instance, TF function, TP params ) : Instance(instance), Function(function), Params(params)
    {
    }

    typedef typename TP::TR TResult;

    operator TResult()   { return TypeCall(Params); }
    TResult operator()() { return TypeCall(Params); }

    template<class R>
    R TypeCall( utParams_0<R>& p ) { return ((*Instance).*Function)(); }

    template<class R, class A>
    R TypeCall( utParams_1<R,A>& p ) { return ((*Instance).*Function)(p.A); }

    template<class R, class A, class B>
    R TypeCall( utParams_2<R,A,B>& p ) { return ((*Instance).*Function)(p.A,p.B); }

    template<class R, class A, class B, class C>
    R TypeCall( utParams_3<R,A,B,C>& p ) { return ((*Instance).*Function)(p.A,p.B,p.C); }

protected:
    TI Instance;
    TF Function;
    TP Params;
};


// Creators - heap
template<class TI, class FR, class FI> inline tcTypeCall< TI,FR(FI::*)(),utParams_0<FR> >* new_tcTypeCall_0( TI instance, FR(FI::*function)() )
{   return new tcTypeCall< TI,FR(FI::*)(),utParams_0<FR> >(instance, function, utParams_0<FR>() ); }

template<class TI, class TA, class FR, class FI, class A> inline tcTypeCall< TI,FR(FI::*)(A),utParams_1<FR,TA> >* new_tcTypeCall_1( TI instance, FR(FI::*function)(A), TA a )
{   return new tcTypeCall< TI,FR(FI::*)(A),utParams_1<FR,TA> > (instance, function, utParams_1<FR,TA>(a) ); }

template<class TI, class TA, class TB, class FR, class FI, class A, class B> inline tcTypeCall< TI,FR(FI::*)(A,B),utParams_2<FR,TA,TB> >* new_tcTypeCall_2( TI instance, FR(FI::*function)(A,B), TA a, TB b )
{   return new tcTypeCall< TI,FR(FI::*)(A,B),utParams_2<FR,TA,TB> > (instance, function, utParams_2<FR,TA,TB>(a,b) ); }

template<class TI, class TA, class TB, class TC, class FR, class FI, class A, class B, class C> inline tcTypeCall< TI,FR(FI::*)(A,B,C),utParams_3<FR,TA,TB,TC> >* new_tcTypeCall_3( TI instance, FR(FI::*function)(A,B,C), TA a, TB b, TC c )
{   return new tcTypeCall< TI,FR(FI::*)(A,B,C),utParams_3<FR,TA,TB,TC> > (instance, function, utParams_3<FR,TA,TB,TC>(a,b,c) ); }


// Creators - heap, const
template<class TI, class FR, class FI> inline tcTypeCall< TI,FR(FI::*)()const,utParams_0<FR> >* new_tcTypeCall_0( TI instance, FR(FI::*function)()const )
{   return new tcTypeCall< TI,FR(FI::*)()const,utParams_0<FR> >(instance, function, utParams_0<FR>() ); }

template<class TI, class TA, class FR, class FI, class A> inline tcTypeCall< TI,FR(FI::*)(A)const,utParams_1<FR,TA> >* new_tcTypeCall_1( TI instance, FR(FI::*function)(A)const, TA a )
{   return new tcTypeCall< TI,FR(FI::*)(A)const,utParams_1<FR,TA> > (instance, function, utParams_1<FR,TA>(a) ); }

template<class TI, class TA, class TB, class FR, class FI, class A, class B> inline tcTypeCall< TI,FR(FI::*)(A,B)const,utParams_2<FR,TA,TB> >* new_tcTypeCall_2( TI instance, FR(FI::*function)(A,B)const, TA a, TB b )
{   return new tcTypeCall< TI,FR(FI::*)(A,B)const,utParams_2<FR,TA,TB> > (instance, function, utParams_2<FR,TA,TB>(a,b) ); }

template<class TI, class TA, class TB, class TC, class FR, class FI, class A, class B, class C> inline tcTypeCall< TI,FR(FI::*)(A,B,C)const,utParams_3<FR,TA,TB,TC> >* new_tcTypeCall_3( TI instance, FR(FI::*function)(A,B,C)const, TA a, TB b, TC c )
{   return new tcTypeCall< TI,FR(FI::*)(A,B,C)const,utParams_3<FR,TA,TB,TC> > (instance, function, utParams_3<FR,TA,TB,TC>(a,b,c) ); }


// Creators - stack
template<class TI, class FR, class FI> inline tcTypeCall< TI,FR(FI::*)(),utParams_0<FR> > make_tcTypeCall_0( TI instance, FR(FI::*function)() )
{   return tcTypeCall< TI,FR(FI::*)(),utParams_0<FR> >(instance, function, utParams_0<FR>() ); }

template<class TI, class TA, class FR, class FI, class A> inline tcTypeCall< TI,FR(FI::*)(A),utParams_1<FR,TA> > make_tcTypeCall_1( TI instance, FR(FI::*function)(A), TA a )
{   return tcTypeCall< TI,FR(FI::*)(A),utParams_1<FR,TA> > (instance, function, utParams_1<FR,TA>(a) ); }

template<class TI, class TA, class TB, class FR, class FI, class A, class B> inline tcTypeCall< TI,FR(FI::*)(A,B),utParams_2<FR,TA,TB> > make_tcTypeCall_2( TI instance, FR(FI::*function)(A,B), TA a, TB b )
{   return tcTypeCall< TI,FR(FI::*)(A,B),utParams_2<FR,TA,TB> > (instance, function, utParams_2<FR,TA,TB>(a,b) ); }

template<class TI, class TA, class TB, class TC, class FR, class FI, class A, class B, class C> inline tcTypeCall< TI,FR(FI::*)(A,B,C),utParams_3<FR,TA,TB,TC> > make_tcTypeCall_3( TI instance, FR(FI::*function)(A,B,C), TA a, TB b, TC c )
{   return tcTypeCall< TI,FR(FI::*)(A,B,C),utParams_3<FR,TA,TB,TC> > (instance, function, utParams_3<FR,TA,TB,TC>(a,b,c) ); }
    

// Creators - stack, const
template<class TI, class FR, class FI> inline tcTypeCall< TI,FR(FI::*)()const,utParams_0<FR> > make_tcTypeCall_0( TI instance, FR(FI::*function)()const )
{   return tcTypeCall< TI,FR(FI::*)()const,utParams_0<FR> >(instance, function, utParams_0<FR>() ); }

template<class TI, class TA, class FR, class FI, class A> inline tcTypeCall< TI,FR(FI::*)(A)const,utParams_1<FR,TA> > make_tcTypeCall_1( TI instance, FR(FI::*function)(A)const, TA a )
{   return tcTypeCall< TI,FR(FI::*)(A)const,utParams_1<FR,TA> > (instance, function, utParams_1<FR,TA>(a) ); }

template<class TI, class TA, class TB, class FR, class FI, class A, class B> inline tcTypeCall< TI,FR(FI::*)(A,B)const,utParams_2<FR,TA,TB> > make_tcTypeCall_2( TI instance, FR(FI::*function)(A,B)const, TA a, TB b )
{   return tcTypeCall< TI,FR(FI::*)(A,B)const,utParams_2<FR,TA,TB> > (instance, function, utParams_2<FR,TA,TB>(a,b) ); }

template<class TI, class TA, class TB, class TC, class FR, class FI, class A, class B, class C> inline tcTypeCall< TI,FR(FI::*)(A,B,C)const,utParams_3<FR,TA,TB,TC> > make_tcTypeCall_3( TI instance, FR(FI::*function)(A,B,C)const, TA a, TB b, TC c )
{   return tcTypeCall< TI,FR(FI::*)(A,B,C)const,utParams_3<FR,TA,TB,TC> > (instance, function, utParams_3<FR,TA,TB,TC>(a,b,c) ); }


#endif UTTYPECASTOR_H
// ============================================================================