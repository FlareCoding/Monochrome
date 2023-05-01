#pragma once
#include <events/EventEmitter.h>
#include <filesystem>
#include <thread>
#include <atomic>

namespace mc::utils {
class FileWatcher : public EventEmitter {
public:
    FileWatcher();
    ~FileWatcher();

    /// @brief Start a loop watching any modifications done to files
    /// @param path Path to the folder to watch
    void watchFolder(const std::string& path);

    /// @brief Start a loop watching any modifications done to the specified file
    /// @param path Path to the file to watch
    void watchFile(const std::string& path);

    /// @brief If a file or folder is currently being watched, the watching loop stops
    void stopWatching();

    /// @brief Specifies how long each watch loop iteration should wait before the next
    /// @param interval Delay between each watch loop iteration
    inline void setWatchInterval(uint64_t interval) { d_loopDelay = interval; }

private:
    std::string d_rootFolderPath;
    std::map<std::string, std::filesystem::file_time_type> d_paths;
    std::atomic_bool d_running = false;
    std::thread d_watchingThread;

    uint64_t d_loopDelay = 750;

    /// @brief Starts the loop that monitors file changes
    void _startWatchingLoop();

    /// @returns Whether or not a path has existed during the last watch loop iteration
    /// @param path Path to check
    inline bool hasPath(const std::string& path) {
        return d_paths.find(path) != d_paths.end();
    }
};
} // namespace mc::utils
