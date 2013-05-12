#ifndef PACKET_HH
#define PACKET_HH

#include <string>

#include "address.hh"
#include "int64.hh"

#define HEADER_SIZE (sizeof( Integer64 ) * 7)
#define DATA_PACKET_SIZE 1472
//#define PAYLOAD_SIZE ((DATA_PACKET_SIZE) - (HEADER_SIZE))
#define PAYLOAD_SIZE 10

#define PAYLOAD_MESSAGE 0
#define IP_MESSAGE 1
#define COMPLETE_MESSAGE 2

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

    Integer64 block_number_;

    /* This field is filled in on receipt, and is not
       transmitted over the wire */
    Integer64 recv_timestamp_;

    /* Length of the payload when the packet is transmitted.
       In Datagrump the payload is just filled with garbage data. */
    unsigned int payload_len_;

    Integer64 message_type_;
 
    std::string payload_;

public:
    /* Make outgoing data packet */
    Packet( const Address & addr, const uint64_t sequence_number, uint64_t block_number, const std::string & payload, const uint64_t message_type=PAYLOAD_MESSAGE);

    /* Make ACK */
    Packet( const Address & addr, const uint64_t sequence_number,
	    const Packet & other, const uint64_t message_type=PAYLOAD_MESSAGE);

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
    uint64_t block_number( void ) const
    { return block_number_.int64(); }

    unsigned int payload_len( void ) const { return payload_len_; }
    std::string payload( void ) const
    { return payload_; }

    uint64_t message_type( void ) const
    { return message_type_.int64(); }

    /* An ACK has an ack_sequence_number less than 2^64 - 1. */
    bool is_ack( void ) const;
  };
}

#endif
