// --- MAIN ---

#include <QApplication>
#include "photoTweak.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PhotoTweak photoTweak;
    if (argc == 2)
    {
        photoTweak.setFilePath(argv[1]);   
    }
	photoTweak.run();

	return app.exec();
}

