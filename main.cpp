// --- MAIN ---

#include <QApplication>
#include "photoTweak.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PhotoTweak photoTweak;
    if (argc == 2)
    {
        photoTweak.setFilename(argv[1]);   
    }
	photoTweak.show();

	return app.exec();
}

