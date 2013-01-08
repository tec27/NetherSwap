#include "netherswap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("tec27");
    QCoreApplication::setOrganizationDomain("tec27.com");
    QCoreApplication::setApplicationName("NetherSwap");

    NetherSwap w;
    if(!w.is_second_instance()) {
      w.show();
    }

    return a.exec();
}
