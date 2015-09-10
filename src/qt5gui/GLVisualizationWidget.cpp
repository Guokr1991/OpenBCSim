/*
Copyright (c) 2015, Sigurd Storve
All rights reserved.

Licensed under the BSD license.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GLScattererWidget.hpp"
#include "GLVisualizationWidget.hpp"
#include "ScattererModel.hpp"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

GLVisualizationWidget::GLVisualizationWidget(QWidget* parent)
    : QWidget(parent)
{
    glWidget = new GLScattererWidget;
    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();
    camera_slider = new QSlider;
    camera_slider->setRange(-0.5*256, 0);
    camera_slider->setSingleStep(1);
    camera_slider->setTickPosition(QSlider::TicksRight);

    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));
    connect(camera_slider, SIGNAL(valueChanged(int)), glWidget,SLOT(setCameraZ(int)));
    connect(glWidget, SIGNAL(cameraZChanged(int)), camera_slider, SLOT(setValue(int)));

    QHBoxLayout* container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(xSlider);
    container->addWidget(ySlider);
    container->addWidget(zSlider);
    container->addWidget(camera_slider);
    setLayout(container);
    
    xSlider->setValue(15*16);
    ySlider->setValue(345*16);
    zSlider->setValue(0*16);
}

QSlider* GLVisualizationWidget::createSlider() {
    QSlider* slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360*16);
    slider->setSingleStep(16);
    slider->setPageStep(15*16);
    slider->setTickInterval(15*16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void GLVisualizationWidget::updateTimestamp(float new_timestamp) {
    // This is a hack. It would be better to have some sort of adapter
    // registered to the actual scatterer object in the main application.
    auto temp = m_scatterer_model.dynamicCast<SplineScattererModel>();
    if (temp) {
        temp->setTimestamp(new_timestamp);
    } else {
        qDebug() << "Cast to SplineScattererModel failed!";
    }
    
    glWidget->updateFromModel();
    update();
}

void GLVisualizationWidget::setScattererSplines(const std::vector<SplineCurve<float, bcsim::vector3> >& splines) {
    auto temp = new SplineScattererModel;
    temp->setSplines(splines);

    m_scatterer_model = QSharedPointer<IScattererModel>(temp);
    glWidget->setScatterers(m_scatterer_model);
    glWidget->updateFromModel();
    update();
}

void GLVisualizationWidget::setFixedScatterers(const std::vector<bcsim::vector3>& scatterers) {
    auto temp = new FixedScattererModel;
    temp->setPoints(scatterers);

    m_scatterer_model = QSharedPointer<IScattererModel>(temp);
    glWidget->setScatterers(m_scatterer_model);
    glWidget->updateFromModel();
    update();
}


void GLVisualizationWidget::setScanSequence(bcsim::ScanSequence::s_ptr scan_seq) {
    glWidget->setScanSequence(scan_seq);
}