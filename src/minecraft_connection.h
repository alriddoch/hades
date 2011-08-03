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

#ifndef HADES_MINECRAFT_CONNECTION_H
#define HADES_MINECRAFT_CONNECTION_H

#include "base_connection.h"

namespace minecraft {

class connection : public base_connection
{
  public:
    explicit connection(boost::asio::io_service & s);

    virtual void start();

  private:
    void handle_read(const boost::system::error_code& e,
                     std::size_t bytes_transferred);


    boost::array<char, 8192> m_buffer;
};

} // namespace minecraft

#endif // HADES_MINECRAFT_CONNECTION_H
