#include <qapplication.h>
#include "distributor.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    Distributor w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
