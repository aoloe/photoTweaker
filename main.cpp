// --- MAIN ---

#include <QApplication>
#include <QDebug>
#include "photoTweaker.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PhotoTweaker photoTweaker;
    if (argc == 2)
    {
        qDebug() << "arg 0" << argv[0];
        qDebug() << "arg 1" << argv[1];
        photoTweaker.setFilePath(argv[1]);   
    }
	photoTweaker.run();

	return app.exec();
}

