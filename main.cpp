/*#include "UDRMS.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
*/
#include <QApplication>
#include "UDRMS.h"
#include "setupdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MainWindow window;
    window.show();

    return app.exec();
}