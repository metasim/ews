/* Derived from OpenSceneGraph example osgViewerQT by Mustard Seed Software, LLC
* for NVIDIA Foundation.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include "QOSGWidget.h"
    #include <QtCore/QTimer>
    #include <QtGui/QKeyEvent>
#include "ui_QOSGWidget.h"


QOSGWidget::QOSGWidget(QWidget * parent):
    QGLWidget(parent), _ui(new Ui::QOSGWidget)
{
    _ui->setupUi(this);
    _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
    setFocusPolicy(Qt::ClickFocus);
}

QOSGWidget::~QOSGWidget()
{
    delete _ui;
}


void QOSGWidget::resizeGL( int width, int height )
{
    _gw->getEventQueue()->windowResize(0, 0, width, height );
    _gw->resized(0,0,width,height);
}

void QOSGWidget::keyPressEvent( QKeyEvent* event )
{
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
}

void QOSGWidget::keyReleaseEvent( QKeyEvent* event )
{
    _gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
}

void QOSGWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void QOSGWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void QOSGWidget::mouseMoveEvent( QMouseEvent* event )
{
    _gw->getEventQueue()->mouseMotion(event->x(), event->y());
}


void QOSGWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    default:
        break;
    }
}
