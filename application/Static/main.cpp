#include <QApplication>
#include "Interactive3DWidget.h" 

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create an instance of the widget
    Interactive3DWidget widget;
    widget.resize(800, 600); 
    widget.show(); 

    return app.exec(); 
}
