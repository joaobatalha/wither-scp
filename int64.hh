#ifndef INT64_HH
#define INT64_HH

/* Helper class to represent a 64-bit integer that
   will be transmitted in network byte order */

#include <string>

namespace Network {
  class Integer64 {
  private:
    uint64_t contents_;

  public:
    Integer64( void ) : contents_() {}

    Integer64( const uint64_t contents ) : contents_( contents ) {}

    /* Construct integer from network-byte-order string */
    Integer64( const std::string & contents );

    /* Produce network-byte-order representation of integer */
    std::string str( void ) const;

    const uint64_t & int64( void ) const { return contents_; }
  };
}

#endif
