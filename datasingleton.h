#ifndef DATASINGLETON_H
#define DATASINGLETON_H

#include <QtGui/QColor>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QKeySequence>
#include <QtGui/QFont>

/**
 * @brief Singleton for variables needed for the program.
 *
 */
class DataSingleton
{
public:
    /**
     * @brief Instance of singleton (static)
     *
     * @return DataSingleton Pointer of singleton
     */
    static DataSingleton* Instance();

    inline int getInstrument() { return currentInstrument; }
    inline void setInstrument(const int instrument) { currentInstrument = instrument; }
    inline int getPreviousInstrument() { return previousInstrument; }
    inline void setPreviousInstrument(int instrument) { previousInstrument = instrument; }
    inline int getHistoryDepth() { return historyDepth; }
    inline void setHistoryDepth(const int &historyDepth) { this->historyDepth = historyDepth; }
    inline QString getAppLanguage() { return appLanguage; }
    inline bool getIsRestoreWindowSize() { return isRestoreWindowSize; }
    inline void setIsRestoreWindowSize(const bool &isRestoreWindowSize) { this->isRestoreWindowSize = isRestoreWindowSize; }
    inline void setAppLanguage(const QString &appLanguage) { this->appLanguage = appLanguage; }
    inline QSize getWindowSize() { return windowSize; }
    inline void setWindowSize(const QSize &windowSize) { this->windowSize = windowSize; }
    inline QMap<QString, QKeySequence> getFileShortcuts() { return fileShortcuts; } // TODO: do we want the shortcut editor in the preferences? (ale/20121224)
    inline QKeySequence getFileShortcutByKey(const QString &key) { return fileShortcuts[key]; }
    inline void setFileShortcutByKey(const QString &key, const QKeySequence &value) { fileShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getEditShortcuts() { return editShortcuts; }
    inline QKeySequence getEditShortcutByKey(const QString &key) { return editShortcuts[key]; }
    inline void setEditShortcutByKey(const QString &key, const QKeySequence &value) { editShortcuts[key] = value; }

    void readSetting();
    void writeSettings();
    void readState();
    void writeState();

private:
    DataSingleton();
    DataSingleton(DataSingleton const&){}

    static DataSingleton* instance;
    int currentInstrument, previousInstrument;
    QSize windowSize;
    bool isRestoreWindowSize;
    int historyDepth;
    QString appLanguage;
    QMap<QString, QKeySequence> fileShortcuts, editShortcuts;
};

#endif // DATASINGLETON_H
