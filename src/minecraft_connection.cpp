
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

#include "minecraft_connection.h"

#include <boost/bind.hpp>

#include <iostream>

using boost::bind;

namespace minecraft {

connection::connection(boost::asio::io_service & s) :
    m_socket(s)
{
}

void connection::start()
{
    std::cout << "New connection" << std::endl << std::flush;
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        bind(&connection::handle_read, this,
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred));

}

void connection::handle_read(const boost::system::error_code & e,
                             std::size_t bytes)
{
    std::cout << "New data" << std::endl << std::flush;
}

} // namespace minecraft
