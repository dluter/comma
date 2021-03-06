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


#include <gtest/gtest.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/filesystem/operations.hpp>
#include <comma/io/select.h>
#include <comma/io/stream.h>
#include <boost/concept_check.hpp>

#include <cstdio>
#include <fstream>

TEST( io, file_stream )
{
    {
        boost::filesystem::remove( "./blah" );
        boost::filesystem::remove( "./testfile" );
        comma::io::ostream ostream( "./testfile" );
        comma::io::istream istream( "./testfile" );
        std::string line;
        *ostream << "hello, world" << std::endl;
        ostream->flush();
        std::getline( *istream(), line );
        EXPECT_EQ( line, "hello, world" );
        ostream.close();
        istream.close();
        boost::filesystem::remove( "./testfile" );
    }
    // todo: more testing?
    system( "mkfifo blah" );
    comma::io::ostream os( "./blah" );
    while( true )
    {
        if( os() == NULL ) { std::cerr << "---> still NULL" << std::endl; }
        else
        {
            *os << "blah" << std::endl;
            std::cerr << "---> " << ( os->good() ? "good" : "bad" ) << std::endl;
        }
#ifndef WIN32
        sleep( 1 );
#else
        Sleep( 1000 );
#endif
    }
    system( "rm ./blah" );
}

TEST( io, std_stream )
{
    comma::io::istream istream( "-" );
    comma::io::ostream ostream( "-" );
    istream.close();
    ostream.close();
    // todo: more testing
}

TEST( io, tcp_stream )
{
//     boost::asio::io_service service;
//     boost::asio::ip::tcp::resolver resolver( service );
//     //boost::asio::ip::tcp::resolver::query query( "localhost" );
//     boost::asio::ip::tcp::resolver::query query( "localhost", "80" );
//     boost::asio::ip::tcp::resolver::iterator it = resolver.resolve( query );
//     std::cerr << "---> address=" << it->endpoint().address() << std::endl;
//     std::cerr << "---> port=" << it->endpoint().port() << std::endl;
//     //boost::asio::ip::tcp::endpoint endpoint( it, 12345 );
//     boost::asio::ip::tcp::socket socket( service );
//     //socket.connect( endpoint );
//     std::cerr << "---> socket.is_open()=" << socket.is_open() << std::endl;

//     // asio tcp server sample code
//     boost::asio::io_service service;
//     boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), 12345 );
//     boost::asio::ip::tcp::acceptor acceptor( service, endpoint );
//     boost::asio::ip::tcp::iostream stream;
//     std::cerr << "testTcpstream(): accepting..." << std::endl;
//     acceptor.accept( *stream.rdbuf() );
//     std::cerr << "testTcpstream(): accepted" << std::endl;
//     std::string line;
//     
//     int fd = stream.rdbuf()->native();
//     std::cerr << "testTcpstream(): fd = " << fd << std::endl;
//     comma::io::select select;
//     select.read().add( fd );
//     while( !stream.eof() )
//     {
//         std::cerr << "testTcpstream(): selecting..." << std::endl;
//         select.wait();
//         std::cerr << "testTcpstream(): select returned" << std::endl;
//         if( !select.read().ready( fd ) ) { break; }
//         std::getline( stream, line );
//         std::cerr << "testTcpstream(): just read \"" << line << "\"" << std::endl;
//     }
}

TEST( io, local_stream )
{
    #ifndef WIN32
    {
        boost::filesystem::remove( "./test.localsocket" );
        boost::asio::local::stream_protocol::endpoint endpoint( "test.localsocket" );
        EXPECT_TRUE( !boost::asio::local::stream_protocol::iostream( endpoint ) );
        boost::asio::io_service service;
        boost::asio::local::stream_protocol::acceptor acceptor( service, endpoint );
        EXPECT_TRUE( boost::asio::local::stream_protocol::iostream( endpoint ) );
        comma::io::istream istream( "./test.localsocket" );
        comma::io::ostream ostream( "./test.localsocket" );
        istream.close();
        ostream.close();
        acceptor.close();
        EXPECT_TRUE( !boost::asio::local::stream_protocol::iostream( endpoint ) );
        EXPECT_TRUE( !boost::filesystem::is_regular_file( "./test.localsocket" ) );
        boost::filesystem::remove( "./test.localsocket" );
    }
    {
        boost::filesystem::remove( "./test.file" );
        comma::io::ostream ostream( "./test.file" );
        ostream.close();
        boost::asio::io_service service;
        boost::asio::local::stream_protocol::endpoint endpoint( "test.file" );
        try { boost::asio::local::stream_protocol::acceptor acceptor( service, endpoint ); EXPECT_TRUE( false ); } catch( ... ) {}
        boost::filesystem::remove( "./test.file" );
    }
    #endif
}

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}