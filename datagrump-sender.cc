#include <stdio.h>
#include <unistd.h>
#include <string>
#include <poll.h>

#include "socket.hh"
#include "controller.hh"

using namespace std;
using namespace Network;

int main( int argc, char *argv[] )
{
  /* check arguments */
  bool debug = false;
  if ( argc == 4 && string( argv[ 3 ] ) == "debug" ) {
    debug = true;
  } else if ( argc == 3 ) {
    /* do nothing */
  } else {
    fprintf( stderr, "Usage: %s IP PORT [debug]\n", argv[ 0 ] );
    exit( 1 );
  }

  try {
    /* Fill in destination address from command line arguments */
    Address destination( argv[ 1 ] /* ip */, argv[ 2 ] /* port */ );

    fprintf( stderr, "Sending packets to %s:%d.\n",
	     destination.ip().c_str(), destination.port() );

    /* Create UDP socket for outgoing datagrams. */
    Network::Socket sock;

    /* Initialize packet counters */
    uint64_t sequence_number = 0;
    uint64_t next_ack_expected = 0;

    /* Initialize flow controller */
    Controller controller( debug );

    /* Loop */
    while ( 1 ) {
      /* Ask controller: what is the window size? */
      unsigned int window_size = controller.window_size();

      /* fill up window */
      while ( sequence_number - next_ack_expected < window_size ) {
	Packet x( destination, sequence_number++ );
	sock.send( x );
	controller.packet_was_sent( x.sequence_number(),
				    x.send_timestamp() );
      }

      /* Wait for acknowledgement or timeout */
      struct pollfd fd = { sock.fd(), POLLIN, 0 };
      int packet_received = poll( &fd, 1, controller.timeout_ms() );
      if ( packet_received < 0 ) { /* error */
	perror( "poll" );
	throw string( "poll returned error." );
      } else if ( packet_received == 0 ) { /* timeout */
	/* send a packet */
	Packet x( destination, sequence_number++ );
	sock.send( x );
	controller.packet_was_sent( x.sequence_number(),
				    x.send_timestamp() );
      } else {
	/* we got an acknowledgment */
	Packet ack = sock.recv();

	/* update our counter */
	next_ack_expected = max( next_ack_expected,
				 ack.ack_sequence_number() + 1 );

	/* tell the controller */
	controller.ack_received( ack.ack_sequence_number(),
				 ack.ack_send_timestamp(),
				 ack.ack_recv_timestamp(),
				 ack.recv_timestamp() );
      }
    }
  } catch ( const string & exception ) {
    /* We got an exception, so quit. */
    fprintf( stderr, "Exiting on exception: %s\n", exception.c_str() );
    exit( 1 );
  }

  return 0;
}
