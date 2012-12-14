#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

#include "timestamp.hh"

using namespace std;
using namespace Network;

/* nanoseconds per millisecond */
static const uint64_t MILLION = 1000000;

/* nanoseconds per second */
static const uint64_t BILLION = 1000 * MILLION;

/* 6.829 epoch */
static const uint64_t EPOCH = 1362700000000;

/* Current time in milliseconds since the epoch */
uint64_t Network::timestamp( void )
{
  struct timespec ts;

  if ( clock_gettime( CLOCK_REALTIME, &ts ) < 0 ) {
    perror( "clock_gettime" );
    throw string( "clock_gettime error" );
  }

  return timestamp( ts );
}

uint64_t Network::timestamp( const struct timespec & ts )
{
  const uint64_t nanos = ts.tv_sec * BILLION + ts.tv_nsec;
  return nanos / MILLION - EPOCH;
}
