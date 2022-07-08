#include <stdio.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

// int main()
// {
//     Py_Initialize();
//     p::object mm, mns, sys_module, env_module;

//     try
//     {
//         mm = p::import("__main__");
//         mns = mm.attr("__dict__");
//         sys_module = p::import("sys");

//         p::str module_dir = (std::string(TEST_DIR) + "/py").c_str();
//         sys_module.attr("path").attr("insert")(1, module_dir);

//         p::exec("from test import ExportClass", mns);

//         env_module = p::eval("ExportClass()", mns);

//         p::object print = env_module.attr("print_msg");
//         p::object setA = env_module.attr("set_a");
//         p::object setB = env_module.attr("set_b");

//         setA(1);
//         print();

//         setB(10);
//         print();
//     }
//     catch (const p::error_already_set &)
//     {
//         PyErr_Print();
//     }
// }
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct int2
{
    int x;
    int y;
    int2() {}
    int2(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

struct uint2
{
    uint x;
    uint y;
    uint2() {}
    uint2(uint x, uint y)
    {
        this->x = x;
        this->y = y;
    }

    uint2(int2 other)
    {
        this->x = other.x;
        this->y = other.y;
    }
};

struct SystemData
{
    // 16 byte alignment
    // int4 rect; // DAR Unused, not implementing a tile renderer.

    int2 resolution;  // The actual rendering resolution. Independent from the launch dimensions for some rendering strategies.
    int2 tileSize;    // Example: make_int2(8, 4) for 8x4 tiles. Must be a power of two to make the division a right-shift.
    int2 tileShift;   // Example: make_int2(3, 2) for the integer division by tile size. That actually makes the tileSize redundant.
    int2 pathLengths; // .x = min path length before Russian Roulette kicks in, .y = maximum path length

    // 4 byte alignment
    int deviceCount; // Number of devices doing the rendering.
    int deviceIndex; // Device index to be able to distinguish the individual devices in a multi-GPU environment.
    // int distribution;  // Indicate if the tile distribution inside the ray generation program should be used. // FIXME Put booleans into bitfield if there are more. Redundant, the deviceCount is what really matters.
    int iterationIndex;
    int samplesSqrt;

    float sceneEpsilon;
    float clockScale; // Only used with USE_TIME_VIEW.

    int lensShader; // Camera type.

    int numCameras;
    int numMaterials;
    int numLights;

    unsigned int envWidth; // The original size of the environment texture.
    unsigned int envHeight;
    float envIntegral;
    float envRotation;

    SystemData() {}
};

struct CompositorData
{
    // 8 byte alignment
    void *outputBuffer;
    void *tileBuffer;

    int2 resolution; // The actual rendering resolution. Independent from the launch dimensions for some rendering strategies.
    int2 tileSize;   // Example: make_int2(8, 4) for 8x4 tiles. Must be a power of two to make the division a right-shift.
    int2 tileShift;  // Example: make_int2(3, 2) for the integer division by tile size. That actually makes the tileSize redundant.

    // 4 byte alignment
    int launchWidth; // The orignal launch width. Needed to calculate the source data index. The compositor launch gridDim.x * blockDim.x might be different!
    int deviceCount; // Number of devices doing the rendering.
    int deviceIndex; // Device index to be able to distinguish the individual devices in a multi-GPU environment.
    CompositorData() {}
};

SystemData sysData;

void compositor(CompositorData *args, uint2 blockDim, uint2 blockIdx, uint2 threadIdx)
{
    const unsigned int xLaunch = blockDim.x * blockIdx.x + threadIdx.x;
    const unsigned int yLaunch = blockDim.y * blockIdx.y + threadIdx.y;

    if (yLaunch < args->resolution.y)
    {
        // First calculate block coordinates of this launch index.
        // That is the launch index divided by the tile dimensions. (No operator>>() on vectors?)
        const unsigned int xBlock = xLaunch >> args->tileShift.x;
        const unsigned int yBlock = yLaunch >> args->tileShift.y;
        // Each device needs to start at a different column and each row should start with a different device.
        const unsigned int xTile = xBlock * args->deviceCount + ((args->deviceIndex + yBlock) % args->deviceCount);

        // The horizontal pixel coordinate is: tile coordinate * tile width + launch index % tile width.
        const unsigned int xPixel = xTile * args->tileSize.x + (xLaunch & (args->tileSize.x - 1)); // tileSize needs to be power-of-two for this modulo operation.

        if (xPixel < args->resolution.x)
        {
            float *dst;
            float *src;
            // The src location needs to be calculated with the original launch width, because gridDim.x * blockDim.x might be different.
            // dst[yLaunch * args->resolution.x + xPixel] = src[yLaunch * args->launchWidth + xLaunch]; // Copy one float4 per launch index.
            // printf("%u ==> %d\n", yLaunch * args->launchWidth + xLaunch, yLaunch * args->resolution.x + xPixel);
            printf("(%u, %u) ==> (%u, %u)\n", xLaunch, yLaunch, xPixel, yLaunch);
        }
    }
}

unsigned int distribute(const uint2 launchIndex)
{
    // First calculate block coordinates of this launch index.
    // That is the launch index divided by the tile dimensions. (No operator>>() on vectors?)
    const unsigned int xBlock = launchIndex.x >> sysData.tileShift.x;
    const unsigned int yBlock = launchIndex.y >> sysData.tileShift.y;

    // Each device needs to start at a different column and each row should start with a different device.
    const unsigned int xTile = xBlock * sysData.deviceCount + ((sysData.deviceIndex + yBlock) % sysData.deviceCount);

    // The horizontal pixel coordinate is: tile coordinate * tile width + launch index % tile width.
    return xTile * sysData.tileSize.x + (launchIndex.x & (sysData.tileSize.x - 1)); // tileSize needs to be power-of-two for this modulo operation.
}

void renderPixels(std::vector<int> pixels, int width)
{
    system("clear");
    // printf("\033[2J");
    for (int i = 0; i < pixels.size(); ++i)
    {
        if (pixels[i] == 0)
        {
            printf("  ");
        }
        else
        {
            switch(pixels[i])
            {
                case 1:
                    printf("\033[91m*\033[0m ");
                    break;
                case 2:
                    printf("\033[92m*\033[0m ");
                    break;
                case 3:
                    printf("\033[93m*\033[0m ");
                    break;
                case 4:
                    printf("\033[94m*\033[0m ");
                    break;
            }
        }

        if ((i + 1) % width == 0)
        {
            printf("\n");
        }
    }
}

int main()
{
    std::vector<int> videoBuffer;

    sysData.tileShift = int2(2, 2);
    sysData.tileSize = int2(4, 4);
    sysData.deviceCount = 3;

    CompositorData compData;
    compData.tileShift = sysData.tileShift;
    compData.tileSize = sysData.tileSize;
    compData.resolution = int2(32, 32);
    compData.deviceCount = sysData.deviceCount;
    auto width = (compData.resolution.x + compData.deviceCount - 1) / compData.deviceCount;
    const int mask = compData.tileSize.x - 1;
    compData.launchWidth = (width + mask) & ~mask;

    sysData.deviceIndex = 0;
    compData.deviceIndex = 0;
    videoBuffer.assign(compData.resolution.x * compData.resolution.y, 0);

    uint sleepTime = 10000;
    while (sysData.deviceIndex < sysData.deviceCount)
    {
        for (int x = 0; x < compData.launchWidth; ++x)
        {
            for (int y = 0; y < compData.resolution.y; ++y)
            {
                auto pos = int2(distribute(uint2(x, y)), y);
                if (pos.x < compData.resolution.x && pos.y < compData.resolution.y)
                {
                    videoBuffer[pos.y * compData.resolution.x + pos.x] = sysData.deviceIndex + 1;
                    renderPixels(videoBuffer, compData.resolution.x);
                    usleep(sleepTime);
                }
            }
        }

        // for (int blockIdX = 0; blockIdX < compData.launchWidth / compData.tileSize.x; ++blockIdX)
        // {
        //     for (int blockIdY = 0; blockIdY < compData.resolution.y / compData.tileSize.y; ++blockIdY)
        //     {
        //         printf("\nblock(%d, %d)\n", blockIdX, blockIdY);
        //         for (int threadIdX = 0; threadIdX < compData.tileSize.x; ++threadIdX)
        //         {
        //             for (int threadIdY = 0; threadIdY < compData.tileSize.y; ++threadIdY)
        //             {
        //                 compositor(&compData, compData.tileSize, uint2(blockIdX, blockIdY), uint2(threadIdX, threadIdY));
        //             }
        //         }
        //     }
        // }

        sysData.deviceIndex += 1;
        compData.deviceIndex += 1;
    }
    return 0;
}
