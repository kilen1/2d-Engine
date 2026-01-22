#ifndef AUTO_UPDATER_H
#define AUTO_UPDATER_H

class AutoUpdater {
public:
    static void Initialize();
    static bool CheckForUpdates();
    static bool DownloadUpdate();
    static bool InstallUpdate();
    static void SetUpdateUrl(const char* url);
    static bool IsUpdateAvailable();
};

#endif // AUTO_UPDATER_H