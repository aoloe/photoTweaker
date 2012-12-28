// --- MAIN ---

#include <QApplication>
#include "photoTweaker.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PhotoTweaker photoTweaker;
    if (argc == 2)
    {
        photoTweaker.setFilePath(argv[1]);   
    }
	photoTweaker.run();

	return app.exec();
}

