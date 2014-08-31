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
//  uxCommands.h
// ============================================================================
#ifndef UXCOMMANDS_H
#define UXCOMMANDS_H

#include "uxCommand.h"
#include "utParams.h"


// ============================================================================
// xcBreak
// ============================================================================
class xcBreak : public uxCommand
{
public:
    xcBreak()
    {
    }

    virtual void Execute()
    {
        wxTrap();
    }

protected:
};

// Creator
inline xcBreak* new_xcBreak()
{
    return new xcBreak();
}


// ============================================================================
// xcDebug
// ============================================================================
class xcDebug : public uxCommand
{
public:
    xcDebug( const wxString& text ) : Text(text)
    {
    }

    virtual void Execute()
    {
        wxLogMessage( wxT("xcDebug: %s"), Text.c_str() );
    }

protected:
    wxString Text;
};

// Creator
inline xcDebug* new_xcDebug( const wxString& text )
{
    return new xcDebug(text);
}


// ============================================================================
// xcDebugObj
// ============================================================================
template<class TI>
class xcDebugObj : public uxCommand
{
public:
    xcDebugObj( const wxString& text, TI instance ) : Text(text), Instance(instance)
    {
    }

    virtual void Execute()
    {
        wxLogMessage( wxT("xcDebug: %s [%s]"), Text.c_str(), GetObjName().c_str() );
    }

    wxString GetObjName()
    {
        return static_cast<wxString>(Instance);
    }

protected:
    wxString Text;
    TI Instance;
};

// Creator
template<class TI>
inline xcDebugObj<TI>* new_xcDebugObj( const wxString& text, TI instance )
{
    return new xcDebugObj<TI>(text,instance);
}


// ============================================================================
// xcMutator
// ============================================================================
template<class TI, class TF, class TP>
class xcMutator : public uxCommand
{
public:
    xcMutator( TI instance, TF function, TP param ) : Instance(instance), Function(function), Param(param)
    {
    }

    virtual void Execute()
    {
        Mutate(Param);
    }

    template<class R>
    void Mutate( utParams_0<R>& p ) { ((*Instance).*Function)(); }

    template<class R, class A>
    void Mutate( utParams_1<R,A>& p ) { ((*Instance).*Function)(p.A); }

    template<class R, class A, class B>
    void Mutate( utParams_2<R,A,B>& p ) { ((*Instance).*Function)(p.A,p.B); }

    template<class R, class A, class B, class C>
    void Mutate( utParams_3<R,A,B,C>& p ) { ((*Instance).*Function)(p.A,p.B,p.C); }

protected:
    TI Instance;
    TF Function;
    TP Param;
};

// Creators
template<class TI, class FR, class FI>
inline uxCommand* new_xcMutator_0( TI instance, FR(FI::*function)() )
{
    return new xcMutator< TI,FR(FI::*)(),utParams_0<FR> >
        (instance, function, utParams_0<FR>() );
}

template<class TI, class TA, class FR, class FI, class A>
inline uxCommand* new_xcMutator_1( TI instance, FR(FI::*function)(A), TA a )
{
    return new xcMutator< TI,FR(FI::*)(A),utParams_1<FR,TA> >
        (instance, function, utParams_1<FR,TA>(a) );
}

template<class TI, class TA, class TB, class FR, class FI, class A, class B>
inline uxCommand* new_xcMutator_2( TI instance, FR(FI::*function)(A,B), TA a, TB b )
{
    return new xcMutator< TI,FR(FI::*)(A,B),utParams_2<FR,TA,TB> >
        (instance, function, utParams_2<FR,TA,TB>(a,b) );
}

template<class TI, class TA, class TB, class TC, class FR, class FI, class A, class B, class C>
inline uxCommand* new_xcMutator_3( TI instance, FR(FI::*function)(A,B,C), TA a, TB b, TC c )
{
    return new xcMutator< TI,FR(FI::*)(A,B,C),utParams_3<FR,TA,TB,TC> >
        (instance, function, utParams_3<FR,TA,TB,TC>(a,b,c) );
}



#endif UXCOMMANDS_H
// ============================================================================