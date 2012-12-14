#include <stdio.h>
#include <arpa/inet.h>

#include "address.hh"
#include "util.hh"

using namespace std;
using namespace Network;

/* Construct address from IP address and port number */
Address::Address( const string ip, const string port )
  : sockaddr_()
{
  sockaddr_.sin_family = AF_INET;

  /* Convert IP address */
  if ( inet_aton( ip.c_str(), &sockaddr_.sin_addr ) == 0 ) {
    fprintf( stderr, "Invalid IP address (%s)\n", ip.c_str() );
    throw string( "Invalid IP address." );
  }

  /* Convert port */
  try {
    long int the_port = myatoi( port.c_str() );
    if ( (the_port < 0) || (the_port > 65535) ) {
      throw string( "Port out of valid range." );
    }
    sockaddr_.sin_port = htons( the_port );
  } catch ( const string & exception ) {
    fprintf( stderr, "Bad port number %s (%s)\n", port.c_str(), exception.c_str() );
    throw;
  }
}

/* Get underlying IP address */
std::string Address::ip( void ) const
{
  char tmp[ 64 ];
  snprintf( tmp, 64, "%s", inet_ntoa( sockaddr_.sin_addr ) );
  return string( tmp );
}

/* Get underlying port number */
uint16_t Address::port( void ) const
{
  return ntohs( sockaddr_.sin_port );
}

/* Render address to string */
const string Address::str( void ) const
{
  char tmp[ 64 ];
  snprintf( tmp, 64, "%s:%d", inet_ntoa( sockaddr_.sin_addr ),
	    ntohs( sockaddr_.sin_port ) );
  return string( tmp );
}

