
/* Copyright (C) 2011 Alistair Riddoch <alriddoch@googlemail.com>
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "atlas_connection.h"

#include <Atlas/Net/Stream.h>

#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>

#include <iostream>

using boost::bind;

namespace atlas {

connection::connection(boost::asio::io_service & s) :
    base_connection(s), m_ios(&m_data), m_negotiate(0), m_codec(0)
{
    m_negotiate = new Atlas::Net::StreamAccept("hades foo", m_ios);
}

void connection::start()
{
    std::cout << "New atlas connection" << std::endl << std::flush;
    m_negotiate->poll(false);
    // Check if there is something to write? I kinda know there is....
    boost::asio::async_write(this->socket(), m_data,
        bind(&connection::negotiate_write, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
/*
    boost::asio::async_read_until(this->socket(), m_data, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));
 */
}

void connection::handle_header_read(const boost::system::error_code & e)
{
    if (e) {
        // stop
        return;
    }

    
    std::streamsize c = m_data.in_avail();
    while (c != 0) {
        char * buf = new char[c + 1];
        std::istream is(&m_data);
        is.getline(buf, c);

        std::string header(buf);
        delete [] buf;

        while (header.size() > 0 && header[header.size() - 1] == '\r') {
            header.resize(header.size() - 1);
        }

        std::cout << "HEDDA " << header.size() << " " << std::endl << "BB: " << header << std::endl << std::flush;

        if (header.size() == 0) {
            std::ostream out(&m_data);

            std::cout << "HEDDAS ONE " << std::endl << std::flush;

            out << "HTTP/1.1 200 OK" << std::endl;
            out << "Content-Type: text/html" << std::endl << std::endl;
            out << "<html><head></head><body>" << std::endl;
            for (int i = 0; i < 100000; ++i) {
                out << "Textewfihwofwpaefhaqpwoiefhwpoefhpowqifehwpoif" << std::endl;
            }
            out << "</body></html>" << std::endl;
    
            return;
        }

        c = m_data.in_avail();
    }

    boost::asio::async_read_until(this->socket(), m_data, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));

}

void connection::negotiate_read(const boost::system::error_code & e,
                                std::size_t bytes)
{
    std::cout << "New neg data " << bytes << " " << e << std::endl << std::flush;
    if (e) {
        // stop
        return;
    }

    m_negotiate->poll(true);

    // There may be nothing to write
    boost::asio::async_write(this->socket(), m_data,
        bind(&connection::negotiate_write, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
#if 0
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
#endif
}

void connection::negotiate_write(const boost::system::error_code & e,
                                 std::size_t bytes)
{
    std::cout << "Written neg data " << bytes << " " << e << std::endl << std::flush;
    if (e) {
        // stop
        return;
    }

    if (m_negotiate->getState() != Atlas::Negotiate::IN_PROGRESS) {
        std::cout << "Neg done" << std::endl;
    }

    boost::asio::async_read_until(this->socket(), m_data, "\n",
        bind(&connection::negotiate_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
}

} // namespace atlas
