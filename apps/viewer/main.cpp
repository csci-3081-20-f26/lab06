#include <iostream>

// Include glad and glfw for graphics and windowing
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "sys/stat.h"

#include "WebService.h"
#include "Image.h"
#include "Glyph.h"

#include <cmath>

using namespace gis;
using namespace csci3081;

double longitudeToMercator(double longitude) {
  return (180.0+longitude)/360.0;
}

double latitudeToMercator(double latitude) {
  return (1.0/(2.0*M_PI)*(M_PI-std::log(std::tan(M_PI/4 + latitude*M_PI/180.0/2.0))));
}

int longitudeToTile(double longitude, int zoom) {
  return (int)std::floor(std::pow(2, zoom)*longitudeToMercator(longitude));
}

int latitudeToTile(double latitude, int zoom) {
  return (int)std::floor(std::pow(2, zoom)*latitudeToMercator(latitude));
}

float mercatorToLongitude(float mercator) {
  return 360.0*mercator-180; 
}

float mercatorToLatitude(float mercator) {
  float x = 2.0*(std::atan(std::exp(-2.0*M_PI*mercator + M_PI))-M_PI/4.0);
  return x*180/M_PI;
}

float tileToLongitude(int tile, int zoom) {
  float mercator = 1.0*tile/std::pow(2, zoom);
  return mercatorToLongitude(mercator);
}

float tileToLatitude(int tile, int zoom) {
  float mercator = 1.0*tile/std::pow(2, zoom);
  return mercatorToLatitude(mercator);
}

inline bool checkFileExists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

Image* getTile(int tileX, int tileY, int zoom) {
    std::string cacheFile = "apps/viewer/data/" + std::string("") + std::to_string(zoom) + "-" + std::to_string(tileY) + "-" + std::to_string(tileX) + ".png";
    if (checkFileExists(cacheFile)) {
        return new Image(cacheFile);
    }

    WebService ws("https://server.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer/tile/");
    std::cout << std::string("") + std::to_string(zoom) + "/" + std::to_string(tileY) + "/" + std::to_string(tileX) << std::endl;
    std::string output = ws.get(std::string("") + std::to_string(zoom) + "/" + std::to_string(tileY) + "/" + std::to_string(tileX));
    Image* img = new Image((unsigned char*)(output.c_str()), output.size());
    img->saveAs(cacheFile);
    return img;
}

Image* createSatelliteImage(float minLon, float minLat, float maxLon, float maxLat) {
    WebService ws("https://server.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer/tile/");

    double minX = longitudeToMercator(minLon);
    double maxX = longitudeToMercator(maxLon);
    double minY = latitudeToMercator(minLat);
    double maxY = latitudeToMercator(maxLat);

    std::cout << mercatorToLatitude(maxY) << std::endl;

    int xZoom = std::log(4.0/std::abs(maxX-minX))/std::log(2.0);
    std::cout << xZoom << std::endl;

    int yZoom = std::log(4.0/std::abs(maxY-minY))/std::log(2.0);

    int zoom = std::min(xZoom, yZoom);

    int minXTile = longitudeToTile(minLon,zoom);
    int maxXTile = longitudeToTile(maxLon,zoom);
    int minYTile = latitudeToTile(minLat,zoom);
    int maxYTile = latitudeToTile(maxLat,zoom);

    int width = (std::abs(maxXTile-minXTile)+1)*256;
    int height = (std::abs(maxYTile-minYTile)+1)*256;

    std::cout << width << "x" << height << std::endl;

    Image combined(width, height);
    for (int tileX = minXTile; tileX <= maxXTile; tileX++) {
        for (int tileY = maxYTile; tileY <= minYTile; tileY++) {
        std::cout << std::string("") + std::to_string(zoom) + "/" + std::to_string(tileY) + "/" + std::to_string(tileX) << std::endl;
        std::string output = ws.get(std::string("") + std::to_string(zoom) + "/" + std::to_string(tileY) + "/" + std::to_string(tileX));
        Image img((unsigned char*)(output.c_str()), output.size());
        //img.saveAs(std::string("tile-") + std::to_string(zoom) + "-" + std::to_string(tileY) + "-" + std::to_string(tileX) + ".png");

        int xStart = (tileX-minXTile)*256;
        int yStart = (tileY-maxYTile)*256;

        for (int x = 0; x < 256; x++) {
            for (int y = 0; y < 256; y++) {
                combined.setPixel(xStart + x, yStart + y, img.getPixel(x, y));
            }
        }

        }
    }
    //combined.saveAs("combined.png");

    float imageMinLon = tileToLongitude(minXTile, zoom);
    float imageMaxLon = tileToLongitude(maxXTile+1, zoom);
    float imageMinLat = tileToLatitude(minYTile+1, zoom);
    float imageMaxLat = tileToLatitude(maxYTile, zoom);
    int newXStart = width*((minLon-imageMinLon)/(imageMaxLon-imageMinLon));
    int newXEnd = width*((maxLon-imageMinLon)/(imageMaxLon-imageMinLon));
    int newYStart = height*(1-(maxLat-imageMinLat)/(imageMaxLat-imageMinLat));
    int newYEnd = height*(1-(minLat-imageMinLat)/(imageMaxLat-imageMinLat));
    std::cout << imageMinLat << " " << imageMaxLat << " " << newYStart << " " << newYEnd << std::endl;
    std::cout << imageMinLon << " " << imageMaxLon << " " << newXStart << " " << newXEnd << std::endl;

    int finalWidth = newXEnd-newXStart;
    int finalHeight = newYEnd-newYStart;

    Image* final = new Image(finalWidth, finalHeight);
    for (int x = 0; x < finalWidth; x++) {
        for (int y = 0; y < finalHeight; y++) {
            final->setPixel(x, y, combined.getPixel(x+newXStart, y+newYStart));
        }
    }

    return final;
}

class Tile {
    
};

int main(int argc, char *argv[]) {

    /*if (argc < 6) {
        std::cout << "Usage: ./build/bin/terrain <output file prefix> <minLon> <minLat> <maxLon> <maxLat>" << std::endl;
        std::cout << "Example: ./build/bin/terrain data/gis/umn -93.25000 44.96610 -93.22080 44.97887" << std::endl;
        std::cout << "Example: ./build/bin/terrain data/gis/iterlaken 7.80939 46.65674 7.94415 46.70779" << std::endl;
        std::cout << "Example: ./build/bin/terrain data/gis/upheaval-dome -109.9697 38.42492 -109.91306 38.45406" << std::endl;
        std::cout << "Example: ./build/bin/terrain data/gis/yosemite-valley -119.63047 37.71221 -119.51717 37.77105" << std::endl;
        
        return 1;
    }

    std::string pathPrefix = argv[1];
    double minLon = std::stod(argv[2]);
    double minLat = std::stod(argv[3]);
    double maxLon = std::stod(argv[4]);
    double maxLat = std::stod(argv[5]);

    Image* sat = createSatelliteImage(minLon, minLat, maxLon, maxLat);
    if (sat) {
        sat->saveAs(pathPrefix + "-sat.png");
        delete sat;
    }*/

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // -------------------------------------
    // Create window of height and width.  Initialize input callbacks
    // -------------------------------------
    GLFWmonitor* monitor =  glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    bool mainloopActive = true;

    //Set up an alternate thread for event polling
    std::thread refresh_thread;
    int refresh = 1000 * (1.0 / 60.0);

    refresh_thread = std::thread(
        [refresh, &mainloopActive]() {
            std::chrono::milliseconds time(refresh);
            while (mainloopActive) {
                std::this_thread::sleep_for(time);
                glfwPostEmptyEvent();
            }
        }
    );

    // Application GL Context scope
    {
        int a = 0;

        std::vector<Glyph*> tiles;

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                Image* img = getTile(2+x,2+y,3);
                tiles.push_back(new Glyph(1.0*x/4.0 ,1.0*y/4.0, 1.0/4.0, 1.0/4.0, *img));
                delete img;
            }
        }

        

        while (!glfwWindowShouldClose(window)) {
            glfwWaitEvents();
            //glfwPollEvents();
            
            glClearColor(1.0,0,0,1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            a++;
            std::cout << a << std::endl;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }


            for (int i = 0; i < tiles.size(); i++) {
                tiles[i]->draw();
            }

            glfwSwapBuffers(window);
        }

        mainloopActive = false;

        //glfwSetWindowUserPointer(window, this);
        refresh_thread.join();
    }  

    glfwTerminate();

    return 0;
}