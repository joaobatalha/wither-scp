#ifndef PACKET_HH
#define PACKET_HH

#include <string>

#include "address.hh"
#include "int64.hh"

/* Packet class */
namespace Network {
  class Packet {
  private:
    /* Dest address on outgoing packets,
       source address on incoming packets */
    Address addr_;

    /* These timestamps are included in every packet. */
    Integer64 sequence_number_;
    Integer64 send_timestamp_;

    Integer64 ack_sequence_number_;
    Integer64 ack_send_timestamp_;
    Integer64 ack_recv_timestamp_;

    static const unsigned int HEADER_SIZE = sizeof( Integer64 ) * 5;
    static const unsigned int DATA_PACKET_SIZE = 1472;

    /* This field is filled in on receipt, and is not
       transmitted over the wire */
    Integer64 recv_timestamp_;

    /* Length of the payload when the packet is transmitted.
       In Datagrump the payload is just filled with garbage data. */
    unsigned int payload_len_;

  public:
    /* Make outgoing data packet */
    Packet( const Address & addr, const uint64_t sequence_number );

    /* Make ACK */
    Packet( const Address & addr, const uint64_t sequence_number,
	    const Packet & other );

    /* Make incoming packet from wire */
    Packet( const Address & addr, const std::string & str,
	    const struct timespec & receive_ts );

    /* Prepare to send */
    void set_send_timestamp( void );

    /* Make wire representation of packet */
    std::string str( void ) const;

    /* Getters */
    const Address & addr( void ) const { return addr_; }
    uint64_t sequence_number( void ) const { return sequence_number_.int64(); }
    uint64_t send_timestamp( void ) const { return send_timestamp_.int64(); }
    uint64_t recv_timestamp( void ) const { return recv_timestamp_.int64(); }

    uint64_t ack_sequence_number( void ) const
    { return ack_sequence_number_.int64(); }
    uint64_t ack_send_timestamp( void ) const
    { return ack_send_timestamp_.int64(); }
    uint64_t ack_recv_timestamp( void ) const
    { return ack_recv_timestamp_.int64(); }

    unsigned int payload_len( void ) const { return payload_len_; }

    /* An ACK has an ack_sequence_number less than 2^64 - 1. */
    bool is_ack( void ) const;
  };
}

#endif
