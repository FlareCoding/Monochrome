#include "FileWatcher.h"

namespace mc::utils {
    FileWatcher::FileWatcher() {
        appendAllowedEvent("fileCreated");
        appendAllowedEvent("fileDeleted");
        appendAllowedEvent("fileModified");
    }

    FileWatcher::~FileWatcher() {
        stopWatching();
    }

    void FileWatcher::watchFolder(const std::string& path) {
        stopWatching();
        d_rootFolderPath = path;

        // Clear out any previously cached paths
        d_paths.clear();

        // Populate the paths map with current folder's filepaths
        for (auto& file : std::filesystem::recursive_directory_iterator(path)) {
            d_paths[file.path().string()] = std::filesystem::last_write_time(file);
        }

        // Start the watching loop
        d_watchingThread = std::thread(&FileWatcher::_startWatchingLoop, this);
    }

    void FileWatcher::watchFile(const std::string& path) {
        stopWatching();

        // Clear out any previously cached paths
        d_paths.clear();

        // Populate the paths map with specified file
        d_paths[path] = std::filesystem::last_write_time(path);

        // Start the watching loop
        d_watchingThread = std::thread(&FileWatcher::_startWatchingLoop, this);
    }

    void FileWatcher::stopWatching() {
        if (d_running) {
            d_running = false;
            d_watchingThread.join();
        }
    }

    void FileWatcher::_startWatchingLoop() {
        // Mark the loop as running
        d_running = true;

        while (d_running) {
            // Save the CPU some sweat
            std::this_thread::sleep_for(std::chrono::milliseconds(d_loopDelay));

            // Checking for any deleted paths
            auto it = d_paths.begin();
            while (it != d_paths.end()) {
                if (!std::filesystem::exists(it->first)) {
                    fireEvent("fileDeleted", { { "path", it->first } });
                    it = d_paths.erase(it);
                } else {
                    it++;
                }
            }

            // Check if a file was created or modified
            for(auto& file : std::filesystem::recursive_directory_iterator(d_rootFolderPath)) {
                auto currentLastWriteTime = std::filesystem::last_write_time(file);

                // File creation
                if(!hasPath(file.path().string())) {
                    d_paths[file.path().string()] = currentLastWriteTime;
                    fireEvent("fileCreated", { { "path", file.path().string() } });
                // File modification
                } else {
                    if(d_paths[file.path().string()] != currentLastWriteTime) {
                        d_paths[file.path().string()] = currentLastWriteTime;
                        fireEvent("fileModified", { { "path", file.path().string() } });
                    }
                }
            }
        }
    }
} // namespace mc::utils
