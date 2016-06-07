/**
  * \file troll_engine.h
  * \brief Defines the TrollEngine class
  * \author R.Chavignat
  */
#ifndef TROLL_ENGINE_H
#define TROLL_ENGINE_H

#include <FreeImage.h>

namespace Engine {

class RenderSurface;

/**
  * \class TrollEngine
  * \brief The TrollEngine class represents an instance of the Engine.
  */
class TrollEngine {
    public:
        TrollEngine();
        ~TrollEngine();

        RenderSurface* currentRenderSurface() const;

        TrollEngine(TrollEngine const& other) = delete;
        TrollEngine(TrollEngine&& other) = delete;
        TrollEngine& operator=(TrollEngine const& other) = delete;
        TrollEngine& operator=(TrollEngine&& other) = delete;

    private:
        static unsigned int s_instanceCount;
        static void freeimage_error_callback(FREE_IMAGE_FORMAT, const char* message);
};

}

#endif // TROLL_ENGINE_H
