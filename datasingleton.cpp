/*
 * This source file is part of photoTweaker.
 * Copyright (c) 2012 Ale Rimoldi <https://github.com/aoloe/photoTweaker>
 *
 * This source file was part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "datasingleton.h"
#include "photo.h"

#include <QtCore/QSettings>

DataSingleton* DataSingleton::instance = 0;

DataSingleton::DataSingleton()
{
    currentInstrument = Photo::NONE_INSTRUMENT;
    previousInstrument = Photo::NONE_INSTRUMENT;
    readSetting();
    readState();
}

DataSingleton* DataSingleton::Instance()
{
    if(!instance)
        instance = new DataSingleton;

    return instance;
}

void DataSingleton::readSetting()
{
    QSettings settings;
    historyDepth = settings.value("/Settings/HistoryDepth", 40).toInt();
    appLanguage = settings.value("/Settings/AppLanguage", "system").toString();
    isRestoreWindowSize = settings.value("/Settings/IsRestoreWindowSize", true).toBool();

    //read shortcuts for file menu
    fileShortcuts.insert("Open", settings.value("/Shortcuts/File/Open", QKeySequence(QKeySequence::Open)).value<QKeySequence>());
    fileShortcuts.insert("Save", settings.value("/Shortcuts/File/Save", QKeySequence(QKeySequence::Save)).value<QKeySequence>());
    fileShortcuts.insert("SaveAs", settings.value("/Shortcuts/File/SaveAs", QKeySequence(QKeySequence::SaveAs)).value<QKeySequence>());
    fileShortcuts.insert("Quit", settings.value("/Shortcuts/File/Quit", QKeySequence(QKeySequence::Quit)).value<QKeySequence>());

    //read shortcuts for edit menu
    editShortcuts.insert("Undo", settings.value("/Shortcuts/Edit/Undo", QKeySequence(QKeySequence::Undo)).value<QKeySequence>());
    editShortcuts.insert("Redo", settings.value("/Shortcuts/Edit/Redo", QKeySequence(QKeySequence::Redo)).value<QKeySequence>());
    editShortcuts.insert("Copy", settings.value("/Shortcuts/Edit/Copy", QKeySequence(QKeySequence::Copy)).value<QKeySequence>());
}

void DataSingleton::writeSettings()
{
    QSettings settings;
    settings.setValue("/Settings/HistoryDepth", historyDepth);
    settings.setValue("/Settings/AppLanguage", appLanguage);
    settings.setValue("/Settings/IsRestoreWindowSize", isRestoreWindowSize);

    //write shortcuts for file menu
    settings.setValue("/Shortcuts/File/Open", fileShortcuts["Open"]);
    settings.setValue("/Shortcuts/File/Save", fileShortcuts["Save"]);
    settings.setValue("/Shortcuts/File/SaveAs", fileShortcuts["SaveAs"]);
    settings.setValue("/Shortcuts/File/Quit", fileShortcuts["Quit"]);

    //write shortcuts for edit menu
    settings.setValue("/Shortcuts/Edit/Undo", editShortcuts["Undo"]);
    settings.setValue("/Shortcuts/Edit/Redo", editShortcuts["Redo"]);
}

void DataSingleton::readState()
{
    QSettings settings;
    windowSize = settings.value("/State/WindowSize", QSize()).toSize();
}

void DataSingleton::writeState()
{
    QSettings settings;
    if (windowSize.isValid()) {
        settings.setValue("/State/WindowSize", windowSize);
    }
}
