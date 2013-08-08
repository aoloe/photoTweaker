# Documentation

photoTweaker is a

- Fast
- Simple
- Portable

image editor which can be embedded in a scriptable workflow.

You should use it when you don't want to spend more than a few seconds to edit a picture. For more complex work, please use a "real" image editor like Gimp.

The project's main goal is to provide a tool that allows you to be really fast when interactively applying a set of frequent used transformations to individual pictures.

Some facts:

- Actions are performed without showing a dialog.
- Actions can be personalized in the preferences.
- One level undo.

In the `scripts/` directory you can find two examples of bash scripts showing how to:
- Take a screenshot, tweak it and upload it to to an image sharing service.
- Copy the last image from your digital camera, tweak it and upload to an image sharing service.

photoTweaker is first released in it's version 1.0 and not labelled as alpha or beta software, because we don't know of a better time to set it to 1.0 then time when it's publicly available to be used.

But it has not been used in production yet, you have to compile it by yoursself and it's still a very young project.

Don't hesitate to write feature request and bug reporting in the GitHub tracker, to fork it and make pull request, talk about it on the Create mailing list.
- https://github.com/aoloe/photoTweaker/issues
- https://github.com/aoloe/photoTweaker/fork
- http://lists.freedesktop.org/mailman/listinfo/create

## Features

Currently, the following features have been implemented:

- Fast loading of the application, only picture at a time.
- Intuitive cropping of the image by selecting the cropping area.
- Resize the image to sizes predefined in the preferences
- You can undo (and redo) the last effect.
- The effects are applied on the full image. The selection is only for cropping.
- Rotation in 90° steps.

For the near future, the following features are planned:
- More filters:
  - auto white balance
  - vintage
  - remove red eyes
- expand the selection wotj keys

- scripting mode (by parameter):
  - "save" and "save as" also quit the program.
  - on quit the filename is returned for further processing.
  - a filename must be passed as parameter and "open" is not available.

## License

photoTweake is free software. It's not clear yet, if it will be GPL or MIT/BSD licensed.

Lot of code is inspired by EasyPaint (https://github.com/Gr1N/EasyPaint) 
- the undo
- the filters
- the selection tool


# Developers documentation

## The general structure of photoTweaker

- The entry point is main.cpp which builds the PhotoTweaker Qt application in `photoTweaker.cpp`, forwards the command line options and runs it
- PhotoTweaker:
  - Initializes the effects.
    - For now the effects list and its loading is hard coded in PhotoTweaker, but a refactoring towards real plugins is planned.
  - Initializes and coordinates the GUI.

## Code conventions

- Camel case method names.
- All conditionals and loop are correctly "braced" and indented.
- Indenting by 4 spaces.
- `{` and `}` are on their own line.
- Avoid inline comments, except when your doing something in an unusual way, then you have to comment it!
- If you leave something unfinished, adda  `TODO:` comment with the details on what there is todo, your name and the date (`TODO: add the other effects (ale/2013087)`)
- Use `foreach (const ItemType item, list)` instead of `foreach (i = 0; i < list.count; i++)` wherever it's possible. It's not faster but it's more readable.

## photoTweaker for learning C++ and Qt

This project aims at teaching some best practices when programming with C++ and Qt.

photoTweaker is a real application, aimed at being the best program for its specific use case. It is not a demo or toy application created for teaching purposes. But during the whole development process, we have tried to keep its code and structure as small and clean as possible.

We also always had an educative goal throughout the project: we were ourselves learning the best practices of Qt while programming it and we wanted people who are learning programming Qt to learn from its code and from contributing to its code!

While we are trying to keep the documentation in the code itself as short as possible -- by only documenting the classes and methods content at their beginning and avoiding to put comments in the code itself, except where we thing that we have put "unusual hacks" in there. On the other side, we are providing a more detailed documentation in this document that will explain you "every" step of the program.

By reading the source code of photoTweaker you can learn:

- creating a simple Qt application.
- compiling a Qt project on several platforms.
- editing a .pro project file.
- managing the menu bar.
- managing the toolbar with normal and with parametrized buttons.
- managing the status bar.
- opening and saving files.
- defining the Gui with Qt designer .ui files.
- embedding .ui files into each other.
- reading and writing settings.
- writing modular code where each functionality is as isolated from the rest of the code as possible.
- using `qDebug()` to output debugging information.

We plan to created a detailed documentation showing where you can find the relevant snippets in the source code and explaining them.

Of course, we are aware that not all the code is already as it should be and we are really interested in feedback to further improve the quality of the code.

Some further goals are:

- defining and loading plugins.
- when to use pointers and heap.

Tasks:
- find a good (and short) introduction to C++ programming that matches the type of programming used in photoTweaker.
- explain how to find help (Qt inline help, stackoverflow, ...)

### Managing the menu bar

see `PhotoTweaker::initializeMenu().

- create a menu bar, attach shortcuts and actions to its items.
- attach multiple shortcuts to one item (cf. file > quit).
- enable and disable items (cf. edit > undo).

### Adding a dialog

This applies for adding a dialog or a widget.

- Create the basic dialog as an .ui file in Qt designer.
- Add a .h file extending the .ui file.
- Add a .cpp file implementing the .h file
- Add the .ui, .h and .cpp files in the .pro file.
- run qmake
- use the .h
- run make
- call the class from your code

### Creating an effect

In this example we will create the fictive "improve" plugin.

- create the `effect/improve.h` and `effect/improve.cpp` files defining the `EffectImprove`, extending `AbstractEffect`. It's the class where you will put effect's main code (creating the buttons for the toolbar, the `apply()` that applies the modifications on the current image).
- in `EffectImprove` you will -- at least -- have to define the following methods:
  - `void addToToolBar(QToolBar &toolbar)`
  - `void apply(const QString &value)`
  if your effect has some settings, you will also need:
  - `void writeSettings()`
  - void readSettings()`
  - `QWidget* getPreferencesWidget()`
  - `void acceptPreferencesWidget(bool enabled, QList<int> size)`
- implement the `addToToolbar()` method that adds the effects button(s) to the toolbar. You can find a simple example in the Rotate effect, and a more complex one -- with multiple buttons created by a single effect -- in the Select effect.
- if the effect does not have settings, `AbstractEffect::getPreferencesWidget()` will provide a default widget with a checkbox to enable/disable the effect.
- if the effect has some settings you have to implement `EffectImprove::writeSettings()`, `EffectImprove::readSettings()`  that will use `QSettings` to store the values.
- add all the created files in the right header, source and forms sections of the `photoTweaker.pro` file

### Writint working code

When working on your project, you don't have to try to get the perfect code from the beginning. Try to get working code that somehow does what you want to achieve. Further on, like when you're adding new tiles to a jigsaw puzzle, don't be scared to move things around: refactor your code.

### Writing comments

Don't write inline comments (add a sample with inline comments). There is a much higher risk to write trivial comments and when reading the code the flow is broken.

Instead, describe at the beginning of the method (and of the class) what it is doing. This way it's much more likely that you only write relevant things, that you keep it short and it will be much easier for the reader to understand what your method is doing and how. On top of it, it will help you keeping the methods short and doing one specific thing.

### Debugging

Using QDebug
- where is the output in Qt Creator?
- defining custom `qDebug()` methos for your specific data types (cf. `QDebug operator<< (... effectStruct)` in `PhotoTweaker`)

Add "CONFIG += debug" at the beginning of the .pro file if you want to debug your project (by using gdb) without using Qt Creator. (You  when run `qmake CONFIG += debug`). You may have to `make clean` for the change to take effect.

How do I set the command line parameter (argv) of a programm which runs in the debugger ? In Qt Creator the command line arguments are set under Projects, Run Settings.

#Todo

## For version 1.0 we have to do:

- everything implemented works correctly.
- refactor the relationship between the effect and its preferences
  - get the effects settings from each effect (and not the dialog).
  - save and read the settings from each effect.
- add the toolbar buttons from each effect.
- reload the toolbar after saving the settings.
- rotate settings (left or right? should the button change?)
- save on quit option (script mode; remove the file > save entry)
- help / about menu entry
- add screenshots and screencasts.
- choose a license: gpl or bsd?
- use the github repository as the webseite (clean up the README file)

## After 1.0

- announce on the create mailing list.
- provide packages for windows and os x.
- create a website, based on github files in this repository (ideale.ch/photoTweaker)
- create a logo / icon for the program.

## For version 1.1.

- get it to build with jenkins
- scale should not be immediately be applied, but scheduled for the next save (and the defined scale should apply to the current selection). should it be shown as a pressed button, which you can unpress?
- on open without a file, show the file open dialog. quit on cancel. maybe remove file > open. (except in file preferences definition mode? then no open dialog nor file > open)
- create a doxygen code documentation.
- refactor the selection / selection tool
  - selection should be a property of photo
  - add a selection effect creating a (as big as possible) selection constrained to predefined ratios
  - add a constrained selection resizing
- enable an effect without having to restart the program.
- clear the selection after rotating

## Further taks:

- implement the effects and instruments as plugin?
- make it testable and implement tests

## Notes

- after having saved, should we show the original picture or the cropped part? at least document it well...
- how to put dependencies in the qmake file?
  `->>> LIBS += -lpodofo`
- add the name of the current file in the title bar
- add an asterisk in the title bar if the file has been changed (or a selection is active)
- reload the image with a new crop after saving with a selection? should it be an option?
- on quit, ask [ok, cancel] if image isEdited (if not in script modus)
- implement modifying the selection with the keyboard
- eventually, put a semi transparent mask on the part not selected
- add a filter to auto white balance
- show the image size and the selection size in a status bar (or in a tooltip?)
- in the code rename "instrument" to "tool" or "plugin"
  http://stackoverflow.com/questions/2802960/what-is-a-qt-plugin
- ESC to stop selection changes and to release current selection (or is undo enough?)
- create a user documentation
- create a developer documentation
- translate
- release for windows and mac os
- save and load profiles for the settings
- create a script that opens each file in a directory and let's edit a copy
- white balance
- allow drag and drop of images into the application to open them.
- check the windows software from lucern and get inspiration from it.
- avoid passing PhotoTweaker to the effects to give them access to the current photo.
- find matching icons for the toolbar, show them also in the preferences

## Open "architecture" questions

- How to correctly give the effects access to the Photo?
- When to pass a pointer, a reference to a struct or a struct?

#Goal

A simple photo "tweaker" allowing to fast crop and resize an image and apply a bunch of predefined filter. PhotoTweaker is thought as an interactive step inside of a workflow.

This is not -- and never will be -- a full fledged image editor.

Applying effects should not trigger any dialog. Settings for the effects can be defined in the preferences and effects may have several button, one for each defined set of values.

## Possible future features
- offer a way to open directly the image in gimp (or any other image editor).
- eventually read the image from standard IO (not the filename) and output it to standard IO; but: does it make sense?
- a way to trigger ImageMagick filters.

## Features that are unlikely to be 
- Zoom: the only way to zoom is to change the window size. If you need a real zoom tool, you should use a real image editor.

Inspired by
- https://github.com/Gr1N/EasyPaint
- MagickLoader.zip from http://www.qtcentre.org/threads/6553-Qt-ImageMagick
- Scrible: http://doc.qt.digia.com/qt/widgets-scribble.html

Possible inspirations:
- http://cropper.codeplex.com/
- http://code.google.com/p/imageclipper/

#Thanks to

ntome, cbreak and the other friendly guys on #qt


#Notes

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

# The status bar



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

