// This file is part of comma, a generic and flexible library
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this software
//    must display the following acknowledgement:
//    This product includes software developed by the The University of Sydney.
// 4. Neither the name of the The University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


/// @author vsevolod vlaskine

#ifndef COMMA_CSV_ASCII_HEADER_GUARD_
#define COMMA_CSV_ASCII_HEADER_GUARD_

//#include <comma/base/exception.h>
#include <comma/csv/names.h>
#include <comma/csv/options.h>
#include <comma/csv/impl/ascii_visitor.h>
#include <comma/csv/impl/from_ascii.h>
#include <comma/csv/impl/to_ascii.h>
#include <comma/string/string.h>

namespace comma { namespace csv {

template < typename S >
class ascii
{
    public:
        /// constructor
        ascii( const std::string& column_names, char d = ',', bool full_path_as_name = true, const S& sample = S() );

        /// constructor from options
        ascii( const options& o, const S& sample = S() );

        /// constructor from default options
        ascii( const S& sample = S() );

        /// get value (returns reference pointing to the parameter)
        const S& get( S& s, const std::vector< std::string >& v ) const;

        /// get value (convenience function)
        const S& get( S& s, const std::string& line ) const { return get( s, split( line, delimiter_ ) ); }

        /// get value (unfilled fields have the same value as in default constructor; convenience function)
        S get( const std::vector< std::string >& v ) const { S s; get( s, v ); return s; }

        /// get value (unfilled fields have the same value as in default constructor; convenience function)
        S get( const std::string& line ) const { S s; get( s, line ); return s; }

        /// put value at the right place in the vector
        const std::vector< std::string >& put( const S& s, std::vector< std::string >& v ) const;

        /// put value at the right place in the line (convenience function)
        const std::string& put( const S& s, std::string& line ) const;

        /// return delimiter
        char delimiter() const { return delimiter_; }

        /// set precision
        void precision( unsigned int p ) { precision_ = p; }

    private:
        char delimiter_;
        boost::optional< unsigned int > precision_;
        impl::asciiVisitor ascii_;
};

template < typename S >
inline ascii< S >::ascii( const std::string& column_names, char d, bool full_path_as_name, const S& sample )
    : delimiter_( d )
    , precision_( 12 )
    , ascii_( join( csv::names( column_names, full_path_as_name, sample ), ',' ), full_path_as_name )
{
    visiting::apply( ascii_, sample );
    //if( ascii_.size() == 0 ) { COMMA_THROW( comma::exception, "expected at least one field of \"" << comma::join( csv::names< S >( full_path_as_name ), ',' ) << "\"; got \"" << column_names << "\"" ); }
}

template < typename S >
inline ascii< S >::ascii( const options& o, const S& sample )
    : delimiter_( o.delimiter )
    , precision_( o.precision )
    , ascii_( join( csv::names( o.fields, o.full_xpath, sample ), ',' ), o.full_xpath )
{
    visiting::apply( ascii_, sample );
    //if( ascii_.size() == 0 ) { COMMA_THROW( comma::exception, "expected at least one field of \"" << comma::join( csv::names< S >( o.full_xpath ), ',' ) << "\"; got \"" << o.fields << "\"" ); }
}

template < typename S >
inline ascii< S >::ascii( const S& sample )
    : delimiter_( options().delimiter )
    , precision_( options().precision )
    , ascii_( join( csv::names( options().fields, true, sample ), ',' ), true ) //, ascii_( join( csv::names( options().fields, options().full_xpath, sample ), ',' ), options().full_xpath )
{
    visiting::apply( ascii_, sample );
}

template < typename S >
inline const S& ascii< S >::get( S& s, const std::vector< std::string >& v ) const
{
    impl::from_ascii_ f( ascii_.indices(), ascii_.optional(), v );
    visiting::apply( f, s );
    return s;
}

template < typename S >
inline const std::vector< std::string >& ascii< S >::put( const S& s, std::vector< std::string >& v ) const
{
    if( v.empty() ) { v.resize( ascii_.size() ); }
    impl::to_ascii f( ascii_.indices(), v );
    if( precision_ ) { f.precision( *precision_ ); }
    visiting::apply( f, s );
    return v;
}

template < typename S >
inline const std::string& ascii< S >::put( const S& s, std::string& line ) const
{
    std::vector< std::string > v;
    if( !line.empty() ) { v = split( line, delimiter_ ); }
    line = join( put( s, v ), delimiter_ );
    return line;
}

} } // namespace comma { namespace csv {

#endif // #ifndef COMMA_CSV_ASCII_HEADER_GUARD_
