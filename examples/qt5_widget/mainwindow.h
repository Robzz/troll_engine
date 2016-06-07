#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_redSlider_sliderMoved(int position);

        void on_greenSlider_sliderMoved(int position);

        void on_blueSlider_sliderMoved(int position);

    private:
        void updateColor();
        Ui::MainWindow *ui;
        GLWidget m_glWidget;
};

#endif // MAINWINDOW_H
