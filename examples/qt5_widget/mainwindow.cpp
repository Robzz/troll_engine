#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glWidget(this)
{
    ui->setupUi(this);
    ui->verticalLayout_2->addWidget(&m_glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_redSlider_sliderMoved(int position) {
    updateColor();
}

void MainWindow::on_blueSlider_sliderMoved(int position) {
    updateColor();
}

void MainWindow::on_greenSlider_sliderMoved(int position) {
    updateColor();
}

void MainWindow::updateColor() {
    float r = static_cast<float>(ui->redSlider->value()) / 255,
          g = static_cast<float>(ui->greenSlider->value()) / 255,
          b = static_cast<float>(ui->blueSlider->value()) / 255;
    m_glWidget.setColor(r, g, b);
}
