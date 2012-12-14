#ifndef TIMESTAMP_HH
#define TIMESTAMP_HH

namespace Network {
  /* Current time in milliseconds since the epoch */
  uint64_t timestamp( void );

  uint64_t timestamp( const struct timespec & ts );
}

#endif
