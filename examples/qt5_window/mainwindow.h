#include "window.h"
#include "troll_engine.h"
#include "program.h"
#include "mesh.h"
#include "scenegraph.h"

class MainWindow : public Engine::Qt5Window {
    public:
        MainWindow();
        ~MainWindow();

        virtual void render() override;

    private:
        Engine::TrollEngine m_engine;
        Engine::Program m_program;
        std::unique_ptr<Engine::Mesh> m_triangle;
        Engine::SceneGraph m_scene;
};
