#Todo

- how to put dependencies in the qmake file?
  ->>> LIBS += -lpodofo
- how to create a widget in the main window that is always full screen?
- find the right way to call the show in photoTweak from the photo->show()
  -> when photo triggers the "update()" signal, photoTweak should run its show() method

#Goal

A simple photo "tweaker" allowing to fast crop and resize an image and (eventually) apply a bunch of predefined filter.

This is not an image editor.

- crop
- canvas resize
- apply predefined filter
  - auto white
  - vintage
- one picture open per session
- undo
- effects are always on the full image (except red eyes?)
- expand the selection wotj keys
- rotate in 90° steps

- scripting mode (by parameter):
  - "save" and "save as" also quit the program.
  - on quit the filename is returned for further processing.
  - if a filename is passed as parameter, "open" is not available.

Maybe:
- offer a way to open directly the image in gimp (or any other image editor)

No:
- zoom: always fills (proportions kept) the window,

Inspired by
- https://github.com/Gr1N/EasyPaint
- MagickLoader.zip from http://www.qtcentre.org/threads/6553-Qt-ImageMagick

Possible inspirations:
- http://cropper.codeplex.com/
- http://code.google.com/p/imageclipper/
- http://code.google.com/p/imageclipper/


#Notes

- EasyPaint implements crop by resizing the image "window"

#History

I am creating a simple photo tweaking applications. It can open and save one image at a time. It has singe window, where the image is shown in a way that it fills it (proportions are kept, overflowing part is shown as gray).

## Creating the main .ui file

With Qt Designer i've created a simple QMainWindow with the entries "Open", "Save", "Save as" and "Quit" in the menues.

## The main.cpp file

The main.cpp file, creates a photoTweak application object, parses the parameters and passes them to the Application and finally runs photoTweak.

## The photoTweak.cpp file

The photoTweak is an application extending the photoTweak.ui file.

The constructor sets up the user interface, creates the photo object and connects the signals and slots relative to the application.

photoTweak.cpp manages the dialogs (open, save, save as and quit) and passes the commands and values to the ptwPhoto object.

## The .pro file
