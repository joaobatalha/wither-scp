#include <cassert>

#include "address.hh"
#include "packet.hh"
#include "timestamp.hh"

using namespace std;
using namespace Network;

/* Make outgoing data packet */
Packet::Packet( const Address & addr, const uint64_t sequence_number, const uint64_t block_number, const string & s_payload, const uint64_t message_type )
  : addr_( addr ),
    sequence_number_( sequence_number ),
    send_timestamp_(),
    ack_sequence_number_( -1 ),
    ack_send_timestamp_(),
    ack_recv_timestamp_(),
    block_number_(block_number),
    recv_timestamp_(),
    payload_len_(s_payload.size()),
    message_type_(message_type),
    payload_( s_payload )
{
  assert( !is_ack() );
}

/* Make ACK */
Packet::Packet( const Address & addr, const uint64_t sequence_number,
		const Packet & other, const uint64_t message_type)
  : addr_( addr ),
    sequence_number_( sequence_number ),
    send_timestamp_(),
    ack_sequence_number_( other.sequence_number() ),
    ack_send_timestamp_( other.send_timestamp() ),
    ack_recv_timestamp_( other.recv_timestamp() ),
    block_number_( other.block_number() ), 
    recv_timestamp_(),
    payload_len_(),
    message_type_(message_type),
    payload_()
{
  assert( is_ack() );
}

/* Make incoming packet from wire */
Packet::Packet( const Address & addr, const string & str,
		const struct timespec & receive_ts )
  : addr_( addr ),
    sequence_number_(), send_timestamp_(),
    ack_sequence_number_(), ack_send_timestamp_(),
    ack_recv_timestamp_(),
    block_number_(),
    recv_timestamp_( timestamp( receive_ts ) ),
    payload_len_(),
    message_type_(),
    payload_()
{
  if ( str.size() < HEADER_SIZE ) {
    throw string( "Incoming datagram not long enough to decode." );
  }

  sequence_number_ = str.substr( 0*sizeof( uint64_t ), sizeof( uint64_t ) );
  send_timestamp_ = str.substr( 1*sizeof( uint64_t ), sizeof( uint64_t ) );
  ack_sequence_number_ = str.substr( 2*sizeof(uint64_t), sizeof(uint64_t) );
  ack_send_timestamp_ = str.substr( 3*sizeof(uint64_t), sizeof(uint64_t) );
  ack_recv_timestamp_ = str.substr( 4*sizeof(uint64_t), sizeof(uint64_t) );
  block_number_ = str.substr( 5*sizeof(uint64_t), sizeof(uint64_t) );
  message_type_ = str.substr(6*sizeof(uint64_t), sizeof(uint64_t) ); 
  payload_len_ = str.size() - HEADER_SIZE;
  payload_ = str.substr(7*sizeof(uint64_t), payload_len_); 
}

/* Prepare to send */
void Packet::set_send_timestamp( void )
{
  /* Fill in send timestamp */
  send_timestamp_ = Integer64( timestamp() );  
}

/* Make wire representation of packet */
string Packet::str( void ) const
{
  string ret = sequence_number_.str()
    + send_timestamp_.str()
    + ack_sequence_number_.str()
    + ack_send_timestamp_.str()
    + ack_recv_timestamp_.str()
    + block_number_.str()
    + message_type_.str()
    + payload_;

  assert( ret.size() <= DATA_PACKET_SIZE );

  return ret;
}

/* An ACK has an ack_sequence_number less than 2^64 - 1. */
bool Packet::is_ack( void ) const
{
  if ( ack_sequence_number() == uint64_t( -1 ) ) {
    return false;
  }

  /* It's an ACK. */
  assert( payload_len_ == 0 );
  return true;
}
