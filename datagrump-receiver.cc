#include <stdio.h>
#include <poll.h>
#include <fstream>
#include <iostream>

#include "socket.hh"

#define TIMEOUT 100

using namespace Network;
using namespace std;

int main( int argc, char *argv[] )
{
  /* check arguments */
  if ( argc != 2 ) {
    fprintf( stderr, "Usage: %s PORT\n", argv[ 0 ] );
    exit( 1 );
  }

  try {
    /* Create UDP socket for incoming datagrams. */
    Network::Socket sock;

    /* Listen on UDP port. */
    sock.bind( Address( "0" /* all IP addresses */,
			argv[ 1 ] /* port */ ) );

    fprintf( stderr, "Listening on port %s...\n", argv[ 1 ] );

    /* Loop */
    uint64_t sequence_number = 1;

    /*file written */
    string filename = string("output.txt");
    ofstream file;
    file.open(filename, ios::out | ios::binary | ios::trunc);
    
    while ( 1 ) {

      /* Wait for acknowledgement or timeout */
      struct pollfd fd = { sock.fd(), POLLIN, 0 };
      int packet_received = poll( &fd, 1, TIMEOUT);

      if ( packet_received < 0 ) { /* error */

        perror( "poll" );
        throw string( "poll returned error." );

      } else if ( packet_received == 0 ) { /* timeout */
        /* send a IP_MESSAGE*/
//        Packet ack(received_packet.addr(), 1, received_packet, IP_MESSAGE);
//        sock.send( ack );

      } else {

        Packet received_packet = sock.recv();
        
        if (received_packet.sequence_number() == 0) {
          //payload is filesize
          int filesize = atoi(received_packet.payload().c_str());
          //printf("filesize %d\n", filesize);
          file.seekp(filesize-1); 
          file.write("\0", 1);

          Packet ack( received_packet.addr(), 1, received_packet );
          sock.send( ack );
        } else {
          //cout << "payload received: " + received_packet.payload();
          if (received_packet.message_type() == COMPLETE_MESSAGE){
            Packet ack( received_packet.addr(), 1, received_packet, COMPLETE_MESSAGE );
            sock.send( ack );
            file.close();
            break;
          }

          if(!file.is_open()){
            printf("not open file");
          }
          file.seekp(received_packet.block_number() * PAYLOAD_SIZE, ios::beg); 
          file.write(received_packet.payload().c_str(), received_packet.payload().size());

          /* Send back acknowledgment */
          Packet ack( received_packet.addr(), sequence_number++, received_packet );
          sock.send( ack );
        }
      }

    }
  } catch ( const string & exception ) {
    /* We got an exception, so quit. */
    fprintf( stderr, "Exiting on exception: %s\n", exception.c_str() );
    exit( 1 );
  }

  return 0;
}
