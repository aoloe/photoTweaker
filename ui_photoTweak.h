/********************************************************************************
** Form generated from reading UI file 'photoTweak.ui'
**
** Created: Thu Dec 20 18:20:38 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHOTOTWEAK_H
#define UI_PHOTOTWEAK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhotoTweak
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuOpen_Media;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PhotoTweak)
    {
        if (PhotoTweak->objectName().isEmpty())
            PhotoTweak->setObjectName(QString::fromUtf8("PhotoTweak"));
        PhotoTweak->resize(800, 600);
        actionOpen = new QAction(PhotoTweak);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpen->setShortcutContext(Qt::WindowShortcut);
        actionQuit = new QAction(PhotoTweak);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(PhotoTweak);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        vboxLayout = new QVBoxLayout(centralwidget);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        vboxLayout->addWidget(label);

        PhotoTweak->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PhotoTweak);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 19));
        menuOpen_Media = new QMenu(menubar);
        menuOpen_Media->setObjectName(QString::fromUtf8("menuOpen_Media"));
        PhotoTweak->setMenuBar(menubar);
        statusbar = new QStatusBar(PhotoTweak);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PhotoTweak->setStatusBar(statusbar);

        menubar->addAction(menuOpen_Media->menuAction());
        menuOpen_Media->addAction(actionOpen);
        menuOpen_Media->addAction(actionQuit);

        retranslateUi(PhotoTweak);

        QMetaObject::connectSlotsByName(PhotoTweak);
    } // setupUi

    void retranslateUi(QMainWindow *PhotoTweak)
    {
        PhotoTweak->setWindowTitle(QApplication::translate("PhotoTweak", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("PhotoTweak", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("PhotoTweak", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("PhotoTweak", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("PhotoTweak", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        menuOpen_Media->setTitle(QApplication::translate("PhotoTweak", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PhotoTweak: public Ui_PhotoTweak {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHOTOTWEAK_H
