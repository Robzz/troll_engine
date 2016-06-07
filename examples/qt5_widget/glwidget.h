#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "troll_engine.h"
#include "window.h"
#include "program.h"
#include "scenegraph.h"

class GLWidget : public Engine::Qt5Surface
{
    public:
        GLWidget(QWidget *parent = nullptr);

        virtual void initializeGL();
        virtual void paintGL();

        void setColor(float r, float g, float b);

    private:
        Engine::TrollEngine m_engine;
        Engine::Program m_program;
        std::unique_ptr<Engine::Mesh> m_triangle;
        Engine::SceneGraph m_scene;
};

#endif // GLWIDGET_H
