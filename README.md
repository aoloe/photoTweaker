#Todo

- after having saved, should we show the original picture or the cropped part? at least document it well...
- how to put dependencies in the qmake file?
  ->>> LIBS += -lpodofo
- add the name of the current file in the title bar
- add an asterisk in the title bar if the file has been changed (or a selection is active)
- on quit, ask [ok, cancel] if image isEdited (if not in script modus)
- implement the undo
- implement modifying the selection with the mouse
- implement modifying the selection with the keyboard
- add a sample effect
- add some preferences
- add an about / help menu
- eventually, put a semi transparent mask on the part not selected
- add a filter to auto white balance
- show the image size and the selection size in a status bar (or in a tooltip?)

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
- eventually read the image from standard IO (not the filename) and output it to standard IO; but: does it make sense?

No:
- zoom: always fills (proportions kept) the window,

Inspired by
- https://github.com/Gr1N/EasyPaint
- MagickLoader.zip from http://www.qtcentre.org/threads/6553-Qt-ImageMagick
- Scrible: http://doc.qt.digia.com/qt/widgets-scribble.html

Possible inspirations:
- http://cropper.codeplex.com/
- http://code.google.com/p/imageclipper/
- http://code.google.com/p/imageclipper/

#Thanks to

ntome, cbreak and the other friendly guys on #qt


#Notes

- EasyPaint implements crop by resizing the image "window"


Lot of code comes from EasyPaint (https://github.com/Gr1N/EasyPaint) 
- the undo
- the filters
- the selection tool

... it's still in the starting blocks and i'm not the best at c++ / qt... but i try to improve
but i'm open to comments and hints!

The goal is to create an application that allows me to crop and scale one image in a very fast way....

Tt should work on linux, without KDE or Gnome and i should be able to call it from a script. If possible, it should be multiplatform.

I've been looking for alternatives but didn't find any matching my needs:
- while Gimp does does crop and scale in a more then satisfactory way, it takes me between one and two minutes to achieve my result...  I've been using for years for this task. And the Gimp (GUI) guys have been very clear that they're not interested in supporting this type of tasks. They're targetting professional graphic designers spending hours on a single image.
gimp is scriptable. I could have written a bunch of scripts to help me in my workflow, but i'm sure that at the end it would have not been as good and it would have been less fun!
- imagemagick?  i've tried imagemagick but it's not what i'm looking for.

I should be able to crop the image in about 2 seconds...

One usecase is getting the image as a screenshot or getting it from the camera connected as usb, crop and scale it and upload it to a server in max 5 seconds. photoTweaker will only take care of the part scale/crop/filter, and be callable from scripts that take the screenshot, upload the image to a server and puts the URL in the clipboard.


the problem is that i have to crop / scale (and white balance) each picture i upload... but very few image need more care than a few automatic clicks.

i'd like to create a gui which is focused on such a fast workflow.

The toolbar will onyl have scale buttons... and one button for each available filter (like the white balance)
The image is automatically cropped to the current selection when saving:  since you can only apply filters to the whole image, the only goal for selecting will be to crop.
And if you start the application in "scripter mode" it will automatically save on quit, without asking anything.

#History

I am creating a simple photo tweaking applications. It can open and save one image at a time. It has singe window, where the image is shown in a way that it fills it (proportions are kept, overflowing part is shown as gray).

## Creating the main .ui file

With Qt Designer i've created a simple QMainWindow with the entries "Open", "Save", "Save as" and "Quit" in the menues.

## The main.cpp file

The main.cpp file, creates a photoTweaker application object, parses the parameters and passes them to the Application and finally runs photoTweaker.

## The photoTweaker.cpp file

The photoTweaker is an application extending the photoTweaker.ui file.

The constructor sets up the user interface, creates the photo object and connects the signals and slots relative to the application.

photoTweaker.cpp manages the dialogs (open, save, save as and quit) and passes the commands and values to the ptwPhoto object.

## The .pro file


# Random links

- http://qt-apps.org/content/show.php/EasyPaint?content=140877
- http://qt-apps.org/content/show.php/Painter?content=134450
- http://www.qtcentre.org/threads/43787-WYSIWYG-editor-with-good-image-handling-Not-web-based
- http://code.google.com/p/picworks/
- http://maemo.gitorious.org/meego-image-editor/qt-image-editor-demo
- http://www.qtforum.org/article/3942/image-editor-in-qt.html?s=cf09d808fccc94de43348bf6b035ff40f69b65af#post16960
- http://www.qtcentre.org/threads/6553-Qt-ImageMagick
- http://www.imagemagick.org/api/Magick++/classMagick_1_1ColorGray.html
- http://www.imagemagick.org/Magick++/Image.html
- http://code.google.com/p/imageclipper/
- http://www.gnu.org/software/gpaint/
- https://bbs.archlinux.org/viewtopic.php?id=121545
- http://softwaretopic.informer.com/crop-or-resize-fotos-linux/
- http://linuxundich.de/de/freiesmagazin/mirage-ein-schneller-bildbetrachter/
- http://www.imagemagick.org/Usage/crop/
- http://www.imagemagick.org/script/command-line-options.php
- http://stackoverflow.com/questions/7854830/how-can-i-trim-just-the-left-and-right-side-of-an-image-using-imagemagick-in-php
- http://ps-scripts.com/bb/viewtopic.php?p=16749
- http://ps-scripts.com/bb/viewtopic.php?p=16815
- http://media.unpythonic.net/emergent-files/01235516977/cropgui.py
- http://emergent.unpythonic.net/01248401946
- http://tips.webdesign10.com/general/imagemagick
- http://imgur.com/apps
- http://argandgahandapandpa.wordpress.com/2010/11/08/command-line-script-to-upload-images-to-imagebin/
- https://help.gnome.org/users/zenity/stable/forms.html.en
- http://linux.byexamples.com/archives/265/a-complete-zenity-dialog-examples-2/
- http://ubuntuforums.org/showthread.php?t=274567
 

- http://stackoverflow.com/questions/7010611/how-can-i-crop-an-image-in-qt
- http://stackoverflow.com/questions/11887425/how-to-crop-a-qimage-to-its-opaque-area
- http://www.qtcentre.org/threads/9965-QImage-crop
- http://linuxappfinder.com/blog/create_photo_mosaics_with_metapixel


- http://unix.stackexchange.com/questions/3728/how-to-do-a-print-screen-or-capture-an-area-of-the-screen
- http://stackoverflow.com/questions/847924/how-can-i-find-out-when-a-pyqt-application-is-idle
- http://pastebin.com/7YvzE9Yh  How can I find out when a PyQt-application is idle
- http://stackoverflow.com/questions/6468981/qthread-signal-logic
- http://qt-project.org/doc/qt-4.8/QThread.html
- http://en.wikibooks.org/wiki/Python_Programming/PyQt4
- http://stackoverflow.com/questions/4938723/what-is-the-correct-way-to-make-my-pyqt-application-quit-when-killed-from-the-co
- http://www.qtcentre.org/threads/34947-How-can-i-draw-on-desktop%EF%BC%9F
- http://qt-project.org/forums/viewthread/16216
- http://stackoverflow.com/questions/1909092/qt4-transparent-window-with-rounded-corners
- http://zetcode.com/gui/qt4/painting/
- http://www.qtforum.org/article/3851/how-to-mouse-select-as-you-do-on-desktop.html
- http://stackoverflow.com/questions/5565206/how-to-draw-rectangle-using-qt-to-xlib-window


- http://www.streamzoo.com/home
- http://pixlr.com/o-matic/
- https://chrome.google.com/webstore/detail/pixlr-o-matic/ehcibdjmpjlekgjhepbfmenfppliikcj?hl=en-US Pixlr-o-matic
- http://imm.io/
- http://imgh.us/old_test.jpg
- http://linux.about.com/od/softgraphics/Linux_Software_Graphics_Image.htm
- http://www.doc-diy.net/photo/image_processing_linux
- http://www.imagemagick.org/script/index.php
- http://www.imagemagick.org/discourse-server/viewtopic.php?f=1&t=13393&start=0
- http://softwaretopic.informer.com/image-magick-vintage-filter/
- http://rubydoc.info/gems/filtr/0.0.3/frames

