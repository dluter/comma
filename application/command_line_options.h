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

#ifndef COMMA_APPLICATION_COMMANDLINEOPTIONS_H_
#define COMMA_APPLICATION_COMMANDLINEOPTIONS_H_

#include <map>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <comma/base/exception.h>
#include <comma/string/string.h>

namespace comma {

/// a simple command line options class
class command_line_options
{
    public:
        /// constructor
        command_line_options( int argc, char ** argv );
        
        /// constructor
        command_line_options( const std::vector< std::string >& argv );
        
        /// constructor
        command_line_options( const command_line_options& rhs );
        
        /// return argv
        const std::vector< std::string >& argv() const;
        
        /// return true, if option exists (list, e.g.: "--binary,-b" is allowed)
        bool exists( const std::string& name ) const;
        
        /// return option value; throw, if it does not exist (list, e.g.: "--binary,-b" is allowed)
        template < typename T >
        T value( const std::string& name ) const;
        
        /// return optional option value (convenience method)
        template < typename T >
        boost::optional< T > optional( const std::string& name ) const;
        
        /// return option value; default, if option not specified (list, e.g.: "--binary,-b" is allowed)
        template < typename T >
        T value( const std::string& name, T default_value ) const;
        
        /// return option values
        template < typename T >
        std::vector< T > values( const std::string& name ) const;
        
        /// return option values; one default value, if option not specified 
        template < typename T >
        std::vector< T > values( const std::string& name, T default_value ) const;

        /// return option names
        std::vector< std::string > names() const;
        
        /// return free-standing values; arguments are comma-separated lists of options
        /// @todo make the interface semantics prettier
        std::vector< std::string > unnamed( const std::string& valueless_options = "", const std::string& options_with_values = "" ) const;
        
        /// throw, if more than one of given options exists (freaking ugly name)
        void assert_mutually_exclusive( const std::string& names ) const;
    
    private:
        typedef std::map< std::string, std::vector< std::string > > map_type_;

        void fill_map_( const std::vector< std::string >& v );
        template < typename T >
        static T lexical_cast_( const std::string& s );
        
        std::vector< std::string > argv_;
        map_type_ map_;
        std::vector< std::string > names_;
};

template < typename T >
inline T command_line_options::lexical_cast_( const std::string& s )
{
    return boost::lexical_cast< T >( s );
}

template <>
inline bool command_line_options::lexical_cast_< bool >( const std::string& s )
{
    if( s == "true" ) { return true; }
    if( s == "false" ) { return false; }
    return boost::lexical_cast< bool >( s );
}

template < typename T >
inline std::vector< T > command_line_options::values( const std::string& name ) const
{
    std::vector< T > v;
    std::vector< std::string > names = comma::split( name, ',' );
    for( std::size_t i = 0; i < names.size(); ++i )
    {
        typename map_type_::const_iterator it = map_.find( names[i] );
        if( it == map_.end() ) { continue; }
        for( std::size_t j = 0; j < it->second.size(); ++j ) { v.push_back( lexical_cast_< T >( it->second[j] ) ); }
    }
    return v;    
}

template < typename T >
inline boost::optional< T > command_line_options::optional( const std::string& name ) const
{
    std::vector< T > v = values< T >( name );
    return v.empty() ? boost::optional< T >() : boost::optional< T >( v[0] );
}

template < typename T >
inline T command_line_options::value( const std::string& name ) const
{
    std::vector< T > v = values< T >( name );
    if( v.empty() ) { COMMA_THROW( comma::exception, "option \"" << name << "\" not specified" ); }
    return v[0];
}

template < typename T >
inline T command_line_options::value( const std::string& name, T default_value ) const
{
    std::vector< T > v = values< T >( name, default_value );
    return v[0];
}

template < typename T >
inline std::vector< T > command_line_options::values( const std::string& name, T default_value ) const
{
    std::vector< T > v = values< T >( name );
    if( v.empty() ) { v.push_back( default_value ); }
    return v;
}

} // namespace comma {

#endif // COMMA_APPLICATION_COMMANDLINEOPTIONS_H_
