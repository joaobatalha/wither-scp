#include <string>
#include <stdlib.h>
#include <string.h>

#include "util.hh"

using namespace std;

/* Convert string to integer with error-checking */
long int myatoi( const char *str )
{
  char *end;

  errno = 0;
  long int ret = strtol( str, &end, 10 );

  if ( ( errno != 0 )
       || ( end != str + strlen( str ) ) ) {
    throw string( "Bad integer." );
  }

  return ret;
}
