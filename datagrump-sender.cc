#include <stdio.h>
#include <unistd.h>
#include <string>
#include <poll.h>
#include <fstream>
#include <bitset>
#include <math.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "socket.hh"
#include "controller.hh"
#include "bitmap.hh"

using namespace std;
using namespace Network;

int main( int argc, char *argv[] )
{
  /* check arguments */
  bool debug = false;
  if ( argc == 5 && string( argv[ 4 ] ) == "debug" ) {
    debug = true;
  } else if ( argc == 4 ) {
    /* do nothing */
  } else {
    fprintf( stderr, "Usage: %s IP PORT FILE [debug]\n", argv[ 0 ] );
    exit( 1 );
  }

  try {
    /* Fill in destination address from command line arguments */
    Address destination( argv[ 1 ] /* ip */, argv[ 2 ] /* port */ );

    fprintf( stderr, "Sending packets to %s:%d.\n",
	     destination.ip().c_str(), destination.port() );

    /* Create UDP socket for outgoing datagrams. */
    Network::Socket sock;

    string filename = string( argv[ 3 ] );
    ifstream file;
    file.open(filename, ios::in | ios::ate | ios::binary);
    char * file_payload = new char[PAYLOAD_SIZE];
    ifstream::pos_type size;
    bool sentSize = false;
    
    if (file.is_open()){
      size = file.tellg();
    } else {
      throw string("unable to open file");
    }

    Bitmap bitmap(ceil((1.0*size)/PAYLOAD_SIZE));

    /* Initialize packet counters */
    uint64_t sequence_number = 1;
    uint64_t next_ack_expected = 0;

    /* Initialize flow controller */
    Controller controller( debug );

    timeval t1;
    timeval t2;
    timeval t3;
    timeval t4;
    float time_io;
    float total;
    gettimeofday(&t1, NULL);


    /* Loop */
    while ( 1 ) {
      /* Ask controller: what is the window size? */
      unsigned int window_size = controller.window_size();
      //fprintf(stderr, "window size is %u\n", window_size);

      /* fill up window */
      while (sentSize && sequence_number - next_ack_expected < window_size ) {
        int block_num = bitmap.next_block();

        if ( block_num == -1 ) { //transfer is complete
          Packet x( destination, sequence_number++, block_num, "", COMPLETE_MESSAGE);
          sock.send( x );
        } else {
          if ( file.is_open() ) {
            gettimeofday(&t2,NULL);
            //printf("pointer expected at %d\n", block_num*PAYLOAD_SIZE);
            file.seekg ( block_num*PAYLOAD_SIZE, ios::beg);
            //printf("pointer is at %d\n", file.tellg());
            file.read ( file_payload, PAYLOAD_SIZE);
            file.clear();
            gettimeofday(&t3,NULL);
          } else {
            throw string("unable to open file");
          }
          time_io += (t3.tv_sec - t2.tv_sec)*1000 + (t3.tv_usec - t2.tv_usec)/float(1000);
          fprintf(stderr, "%f%%\n",  100.0*bitmap.bits_set()/bitmap.num_blocks());
          Packet x( destination, sequence_number++, block_num, string(file_payload, file.gcount()));
          sock.send( x );
        }
      }

      if ( !sentSize) {
        Packet x( destination, 0, 0, to_string((long long)size));
        sock.send( x );
      }

      /* Wait for acknowledgement or timeout */
      struct pollfd fd = { sock.fd(), POLLIN, 0 };
      int packet_received = poll( &fd, 1, controller.timeout_ms() );

      if ( packet_received < 0 ) { /* error */

        perror( "poll" );

        throw string( "poll returned error." );

      } else if ( packet_received == 0 ) { /* timeout */

        int block_num = bitmap.next_block();

        if ( !sentSize) {
          Packet x( destination, 0, 0, to_string((long long) size));
          sock.send( x );
        } else {
          if ( block_num == -1 ) { //transfer is complete
            Packet x( destination, sequence_number++, block_num, "", COMPLETE_MESSAGE);
            sock.send( x );
          } else {
            if ( file.is_open() ) {
              gettimeofday(&t2,NULL);
              //printf("pointer expected at %d\n", block_num*PAYLOAD_SIZE);
              file.seekg ( block_num*PAYLOAD_SIZE, ios::beg);
              //printf("pointer is at %d\n", file.tellg());
              file.read ( file_payload, PAYLOAD_SIZE);
              file.clear();
              gettimeofday(&t3,NULL);
            } else {
              throw string("unable to open file");
            }
            time_io += (t3.tv_sec - t2.tv_sec)*1000 + (t3.tv_usec - t2.tv_usec)/float(1000);
            fprintf(stderr, "%f%%\n",  100.0*bitmap.bits_set()/bitmap.num_blocks());
            Packet x( destination, sequence_number++, block_num, string(file_payload, file.gcount()));
//            printf("sending block %d\n",  block_num);
            sock.send( x );
          }
        } 

      } else {
        /* we got an acknowledgment */
        Packet ack = sock.recv();
        if ( ack.message_type() == IP_MESSAGE) { /* Update destination IP */
          destination = ack.addr();
        } else {
          if (ack.sequence_number() == 1) {
            sentSize = true;
          } else {
            /* tell the controller */
            if(!bitmap.is_set(ack.block_number())) {
              controller.ack_received( ack.ack_sequence_number(),
                ack.ack_send_timestamp(),
                ack.ack_recv_timestamp(),
                ack.recv_timestamp() );
              bitmap.set_bit(ack.block_number());
            }
          }
          /* update our counter */
          next_ack_expected = max( next_ack_expected,
            ack.ack_sequence_number() + 1 );

          

          if ( ack.message_type() == COMPLETE_MESSAGE) { /* Finished sending */
            gettimeofday(&t4,NULL);

            total += (t4.tv_sec - t1.tv_sec)*1000 + (t4.tv_usec - t1.tv_usec)/float(1000);
            fprintf(stderr, "\n Time to transfer file: %.3f\n", total);
            fprintf(stderr, "\n Time in IO file: %.3f\n", time_io);
            free(file_payload);
            break;
          }
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
