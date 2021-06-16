#include "AutoPreviewer.h"

#include "utils.h"


Config::Config() :
    window_width(800),
    window_height(600)
{
    inputFolder = "ToPreview";
    previewFormat = "jpg";
    exploreSubFolders = 1;
}

AutoPreviewer::AutoPreviewer()
{
    //ctor
}

AutoPreviewer::~AutoPreviewer()
{
    //dtor
}



bool AutoPreviewer::loadConfig(const std::string &configPath)
{
    std::cout<<"Load Config from file: "<<configPath<<std::endl;

    std::ifstream dataFile(configPath);

    if(!dataFile)
    {
        std::cout<<"ERROR: Could not open config file..."<<std::endl;
        return (false);
    }

    std::string fieldStr, line;
    while(dataFile >> fieldStr)
    {
        if(fieldStr == "InputFolder:") {
            std::getline(dataFile, line);
            m_config.inputFolder = removeWhite(line);

        } else if(fieldStr == "Width:") {
            dataFile >> fieldStr;
            m_config.window_width = std::stoi(fieldStr);
        }
        else if(fieldStr == "Height:") {
            dataFile >> fieldStr;
            m_config.window_height = std::stoi(fieldStr);
        } else if(fieldStr == "Cam:") {
            m_config.cams.push_back({});

            dataFile >> fieldStr;
            m_config.cams.back().distance = std::stof(fieldStr);

            dataFile >> fieldStr;
            m_config.cams.back().angleXY = std::stof(fieldStr);

            dataFile >> fieldStr;
            m_config.cams.back().angleZ = std::stof(fieldStr);

            m_config.cams.back().perspective = readBool(dataFile);

            m_config.cams_filenames.push_back(std::string());
            m_config.cams_scenes.push_back(std::string());

            while(dataFile.peek() == ' ')
                dataFile.ignore();
            if(dataFile.peek() != '\n')
                dataFile >> m_config.cams_filenames.back();


            while(dataFile.peek() == ' ')
                dataFile.ignore();
            if(dataFile.peek() != '\n')
                dataFile >> m_config.cams_scenes.back();



        } else if(fieldStr == "AutoPreviewSuffix:") {
            dataFile >> m_config.defaultPreviewPath;
        }
        else if(fieldStr == "PreviewFormat:") {
            dataFile >> m_config.previewFormat;
        }
        else if(fieldStr == "DefaultScene:") {
            dataFile >> m_config.defaultScene;
        }
        else if(fieldStr == "Scene:") {
            std::string sceneName, scenePath;
            dataFile>>sceneName;
            dataFile>>scenePath;

            m_config.scenesPerName.insert({sceneName, scenePath});
        }
        else if(fieldStr == "ExploreSubFolders:") {
             m_config.exploreSubFolders = readBool(dataFile);
        }
    }

    int i = 0;
    for(auto &filename : m_config.cams_filenames)
        if(filename.empty()) filename = /*m_config.defaultPreviewPath+*/std::to_string(i++);

    return (true);
}


void AutoPreviewer::exploreDirectory()
{
    m_forceStop = false;

    std::cout<<"Initializing rendering..."<<std::endl;

    m_renderingWindow.create(m_config.window_width, m_config.window_height, "Poupou's AutoPreviewer");

    m_renderingWindow.loadDefaultScene(m_config.defaultScene);
    for(auto scene : m_config.scenesPerName)
        m_renderingWindow.addAndLoadScene(scene.first, scene.second);

    m_renderingWindow.initRendering();

   this->exploreSubDirectory(m_config.inputFolder);

    m_renderingWindow.destroy();
}


void AutoPreviewer::exploreSubDirectory(const std::string &directoryPath)
{
    std::cout<<"Exploring directory: "<<directoryPath<<std::endl;

    for (const auto & entry :  std::filesystem::directory_iterator(directoryPath))
    {
        auto &filepath = entry.path();
        std::cout<<filepath<<std::endl;

        auto strExt = filepath.extension().string();
        if(strExt == ".stl" || strExt == ".STL"
        || strExt == ".obj" || strExt == ".OBJ"
        || strExt == ".fbx" || strExt == ".FBX"
        || strExt == ".ply" || strExt == ".PLY")
            this->renderPreviews(filepath);

        if(m_forceStop)
            break;

        if(m_config.exploreSubFolders && strExt == "")
            this->exploreSubDirectory(filepath.string());
    }
}



void AutoPreviewer::renderPreviews(const std::filesystem::path &modelPath)
{
    std::cout<<"Previewing: "<<modelPath<<std::endl;

    m_renderingWindow.setupModel(modelPath.string());
    m_renderingWindow.setupScene("");

    int i = 0;
    for(auto cam : m_config.cams)
    {
        m_renderingWindow.setupScene(m_config.cams_scenes[i]);

        m_renderingWindow.setupCamera(cam);
        if(!m_renderingWindow.update())
            m_forceStop = true;

        std::string previewFilePath =
                modelPath.parent_path().string()+"\\"+modelPath.stem().string()+"_"+m_config.defaultPreviewPath+"_"+m_config.cams_filenames[i]; //std::to_string(i);

        std::cout<<"Writing: "<<previewFilePath<<std::endl;
        m_renderingWindow.printscreen(previewFilePath, m_config.previewFormat);

        i++;
    }
}

