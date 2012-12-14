#include <stdio.h>
#include <math.h>
#include <algorithm>

#include "controller.hh"
#include "timestamp.hh"

using namespace Network;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), the_estimated_rtt(60), the_throughput(0.0), update_time(0),
    update_interval(20), last_seq_num(0)
{
}

/* Get current window size, in packets */
unsigned int Controller::window_size( void )
{
  /* Default: fixed window size of one outstanding packet */

  if ( debug_ ) {
    fprintf( stderr, "At time %lu, return window_size = %i.\n",
	     timestamp(), (int) (the_throughput * the_estimated_rtt));
  }

  return the_throughput * the_estimated_rtt;
}


/* A packet was sent */
void Controller::packet_was_sent( const uint64_t sequence_number,
				  /* of the sent packet */
				  const uint64_t send_timestamp )
                                  /* in milliseconds */
{
  /* Default: take no action */
  if ( debug_ ) {
    fprintf( stderr, "At time %lu, sent packet %lu.\n",
	     send_timestamp, sequence_number );
  }

}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged packet was sent */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged packet was received */
			       const uint64_t timestamp_ack_received )
                   /* when the ack was received (by sender) */
{
  uint64_t rtt = recv_timestamp_acked - send_timestamp_acked;
  uint64_t timediff = timestamp_ack_received - update_time;

  if(timediff >= update_interval){
      int numpkts = sequence_number_acked - last_seq_num;

      the_throughput = std::max(1.0/rtt, 
              .70 * the_throughput + .35 * numpkts/timediff);
 
      update_time = timestamp_ack_received;
      last_seq_num = sequence_number_acked;

      fprintf(stderr, "The throughput is %f \n", 
         the_throughput);
  } 

  /* Default: take no action */

  if ( debug_ ) {
    fprintf( stderr, "At time %lu, received ACK for packet %lu",
	     timestamp_ack_received, sequence_number_acked );

    fprintf( stderr, " (sent %lu, received %lu by receiver's clock).\n",
	     send_timestamp_acked, recv_timestamp_acked );

  }
}


/* How long to wait if there are no acks before sending one more packet */
unsigned int Controller::timeout_ms( void )
{
  return 100; /* timeout of one second */
}
