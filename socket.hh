#ifndef SOCKET_HH
#define SOCKET_HH

#include "address.hh"
#include "packet.hh"

namespace Network {
/* Socket class. Wraps socket() */
  class Socket {
  private:
    int sock_; /* the socket file descriptor */

  public:
    /* Default constructor */
    Socket();

    /* Bind to port (typically used by server) */
    void bind( const Address & addr ) const;

    /* Connect to ip/port (typically used by client) */
    void connect( const Address & addr ) const;

    /* Send packet */
    void send( Packet & packet ) const;

    /* Receive a packet */
    Packet recv( void ) const;

    /* Getter for underlying socket fd */
    int fd( void ) const { return sock_; }
  };
}

#endif
