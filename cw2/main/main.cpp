/*
 * main.cpp
 *
 */

#include <fstream>
#include <iostream>

#define RAPIDJSON_HAS_STDSTRING 1

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "math/geometry.h"
#include "parsers/PPMWriter.h"

#include "core/Camera.h"
#include "core/RayTracer.h"
#include "core/Scene.h"
#include "parser.h"

using namespace rt;
using namespace rapidjson;

int main(int argc, char* argv[]) {
    // parse commandline arguments
    char* inputFile = argv[1];  // first command line argument holds the path to
                                // the json input file
    char* outputFile = argv[2];  // second command line argument holds the path
                                 // to the output image file

    std::printf("Input file: %s\n", inputFile);

    // parse json input file using rapidjson
    std::ifstream ifs(inputFile);
    IStreamWrapper is(ifs);
    Document d;  // d holds the complete json object
    d.ParseStream<kParseCommentsFlag | kParseTrailingCommasFlag>(is);

    // generate a camera according to the input file
    Camera* camera = nullptr;
    Parser::Parse(camera, d["camera"]);

    // print camera data (based on the input file provided)
    camera->printCamera();

    // generate the scene according to the input file
    Scene* scene = nullptr;
    Parser::Parse(scene, d["scene"]);

    //
    // Main function, render scene
    //
    Vec3f* pixelbuffer =
        RayTracer::render(camera, scene, d["nbounces"].GetInt());

    // convert linear RGB pixel values [0-1] to range 0-255
    RayTracer::tonemap(camera->getWidth() * camera->getHeight(), pixelbuffer);

    std::printf("Output file: %s\n", outputFile);

    // write rendered scene to file (pixels RGB values must be in range 0255)
    PPMWriter::PPMWriter(camera->getWidth(), camera->getHeight(), pixelbuffer,
                         outputFile);

    // free resources when rendering is finished
    delete camera;
    delete scene;
    delete pixelbuffer;
}
