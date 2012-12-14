#ifndef ADDRESS_HH
#define ADDRESS_HH

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>

namespace Network {
  /* Datagram address class. Wraps sockaddr_in */
  class Address {
  private:
    struct sockaddr_in sockaddr_;

  public:
    /* Construct from IP address and port number */
    Address( const std::string ip, const std::string port );

    /* Construct from sockaddr_in */
    Address( const struct sockaddr_in s_sockaddr ) : sockaddr_( s_sockaddr ) {}

    /* Get underlying sockaddr_in */
    const struct sockaddr_in & sockaddr( void ) const { return sockaddr_; }

    /* Render address to string */
    const std::string str( void ) const;

    /* Get underlying IP address */
    std::string ip( void ) const;

    /* Get underlying port number */
    uint16_t port( void ) const;

    /* Compare two addresses */
    bool operator==( const Address & other ) const
    {
      return (0 ==
	      memcmp( &sockaddr_, &other.sockaddr_, sizeof( sockaddr_ )));
    }
  };
}

#endif
