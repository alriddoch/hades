
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

#include "http_connection.h"

#include <boost/asio/placeholders.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>

#include <iostream>

using boost::bind;

namespace http {

connection::connection(boost::asio::io_service & s) :
    m_socket(s)
{
}

void connection::start()
{
    std::cout << "New http connection" << std::endl << std::flush;
    boost::asio::async_read_until(m_socket, m_rdata, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));
/*
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
 */
}

void connection::handle_header_read(const boost::system::error_code & e)
{
    if (e) {
        // stop
        return;
    }

    
    std::streamsize c = m_rdata.in_avail();
    while (c != 0) {
        char * buf = new char[c + 1];
        std::istream is(&m_rdata);
        is.getline(buf, c);

        std::string header(buf);
        delete [] buf;

        while (header.size() > 0 && header[header.size() - 1] == '\r') {
            header.resize(header.size() - 1);
        }

        std::cout << "HEDDA " << header.size() << " " << std::endl << "BB: " << header << std::endl << std::flush;

        if (header.size() == 0) {
            std::ostream out(&m_wdata);

            std::cout << "HEDDAS ONE " << std::endl << std::flush;

            out << "HTTP/1.1 200 OK" << std::endl;
            out << "Content-Type: text/html" << std::endl << std::endl;
            out << "<html><head></head><body>Text</body></html>" << std::endl;
    
            boost::asio::async_write(m_socket, m_wdata,
                bind(&connection::handle_write, this,
                     boost::asio::placeholders::error,
                     boost::asio::placeholders::bytes_transferred));
            return;
        }

        c = m_rdata.in_avail();
    }

    boost::asio::async_read_until(m_socket, m_rdata, "\n",
        bind(&connection::handle_header_read, this,
             boost::asio::placeholders::error));

}

void connection::handle_read(const boost::system::error_code & e,
                             std::size_t bytes)
{
    std::cout << "New http data " << bytes << " " << e << std::endl << std::flush;
    if (e) {
        // stop
        return;
    }
#if 0
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));
#endif
}

void connection::handle_write(const boost::system::error_code & e,
                             std::size_t bytes)
{
    std::cout << "Written http data " << bytes << " " << e << std::endl << std::flush;
    if (e) {
        // stop
        return;
    }
}

} // namespace http
