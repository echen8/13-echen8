#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int main() {

  int READ, WRITE;
  READ = 0;
  WRITE = 1;

  srand( time( NULL ) );

  int p; // p is true only for the 'p'arent

  // Pipe from P to C
  int fds1[2];
  pipe( fds1 );

  // Pipe from C to P
  int fds2[2];
  pipe( fds2 );

  p = fork();
  
  // PARENT: Send a random number to child.
  if ( p ) {
    close( fds1[ READ ] );
    close( fds2[ WRITE ] );

    printf( "P| Generating random number for operation within range [0,999]...\n" );
    unsigned int n = (unsigned int)rand() % 1000;

    printf( "P| Sending '%d' to child.  Awaiting response...\n", n );
    write( fds1[ WRITE ], &n, sizeof( n ) );
  }

  // CHILD: Performs operation, sends new value back to parent.
  if ( !p ) {
    close( fds2[ READ ] );
    close( fds1[ WRITE ] );

    unsigned int m;

    read( fds1[ READ ], &m, sizeof( m ) );

    printf( "C| Received '%d' from parent.  Dividing by 2...\n", m );
    m /= 2;

    printf( "C| New value is '%d'.  Sending it back to parent...\n", m );
    write( fds2[ WRITE ], &m, sizeof( m ) );
  }

  // PARENT: Prints out received value.
  if ( p ) {
    close( fds1[ READ ] );
    close( fds2[ WRITE ] );

    unsigned int l;

    read( fds2[ READ ], &l, sizeof( l ) );

    printf( "P| Received '%d' from child!\n", l );
  }

  return 0;
}
