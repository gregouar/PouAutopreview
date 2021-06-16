#ifndef AUTOPREVIEWER_H
#define AUTOPREVIEWER_H

#include <filesystem>

#include "RenderingWindow.h"


struct Config
{
    Config();

    std::string inputFolder;

    size_t window_width;
    size_t window_height;

    std::vector<CameraAngle> cams;
    std::vector<std::string> cams_filenames;

    std::string defaultPreviewPath;
    std::string previewFormat;

    std::string defaultScene;

    bool exploreSubFolders;
};

class AutoPreviewer
{
    public:
        AutoPreviewer();
        virtual ~AutoPreviewer();

        bool loadConfig(const std::string &configPath);

        void exploreDirectory();

    protected:
        void exploreSubDirectory(const std::string &directoryPath);
        void renderPreviews(const std::filesystem::path &itemDirPath);


    private:
        RenderingWindow m_renderingWindow;
        Config m_config;

        bool m_forceStop;
};

#endif // AUTOPREVIEWER_H
