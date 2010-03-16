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
#include <QtGui/QKeyEvent>
#include <osgGA/StateSetManipulator>
#include <osgGA/MatrixManipulator>
#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>

#include "CameraController.h"


namespace ews {
    namespace app {
        namespace widget {
            QOSGWidget::QOSGWidget(QWidget* parent)
            : QGLWidget(parent), osgViewer::Viewer(), _gw(0), _timer() {
#if defined(QT_DEBUG)                
                osg::setNotifyLevel(osg::WARN);
#endif                
                _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
                setFocusPolicy(Qt::StrongFocus);
                
#if defined(GL_MULTISAMPLE_ARB)
                QGLFormat fmt; 
                QGLFormat::setDefaultFormat(fmt); 
                fmt.setSamples(4); 
                fmt.setSampleBuffers(true); 
                setFormat(fmt);                    
#endif
                
                osg::Camera* c = getCamera();
                c->setViewport(new osg::Viewport(0,0,width(),height()));
                c->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
                c->setGraphicsContext(getGraphicsWindow());
                c->setClearColor(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
                
                setThreadingModel(osgViewer::Viewer::SingleThreaded);
                
                setCameraManipulator(new ews::app::drawable::CameraController);
                addEventHandler(new osgViewer::StatsHandler);
                
                // add the help handler
                addEventHandler(new osgViewer::HelpHandler());
                
                // Allows us to toggle lighting and other basic state items with keyboard
                addEventHandler( new osgGA::StateSetManipulator(getCamera()->getOrCreateStateSet()));
                
                // add the thread model handler
                addEventHandler(new osgViewer::ThreadingHandler);
                
                // add the window size toggle handler
                addEventHandler(new osgViewer::WindowSizeHandler);               
                                
                
                connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
                _timer.start(30);
            }
            
            QOSGWidget::~QOSGWidget() {
                _timer.stop();
                // Don't delete _gw, smart pointer takes care of it.
            }
            
            void QOSGWidget::setSceneData(osg::Node* node) {
                osgViewer::Viewer::setSceneData(node);
                getCameraManipulator()->setNode(node);
                computeHomePosition();
            }
            
            void QOSGWidget::computeHomePosition() {
                using ews::app::drawable::CameraController;
                
                osgGA::MatrixManipulator* mat = getCameraManipulator();
                CameraController* ctrl;
                if(ctrl = dynamic_cast<CameraController*> (mat)) {
                    ctrl->computeHomePosition();
                }
            }
            
            void QOSGWidget::homePosition() {
                // HACK: Happen to know that the CameraController responds
                // to the space key for a camera reset, which seems to need to be 
                // called inside the event loop.
                _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Space);
            }

            
            void QOSGWidget::destroyEvent(bool destroyWindow, bool destroySubWindows) {   
                _gw->getEventQueue()->closeWindow();
            }
            
            
            void QOSGWidget::closeEvent( QCloseEvent * event ) {
                event->accept();
                _gw->getEventQueue()->closeWindow();
            }
            
            void QOSGWidget::paintGL() {
                frame();
            }
            
            
            void QOSGWidget::resizeGL( int width, int height ) {
                _gw->getEventQueue()->windowResize(0, 0, width, height );
                _gw->resized(0,0,width,height);
            }
            
            void QOSGWidget::keyPressEvent( QKeyEvent* event ) {
                _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
            }
            
            void QOSGWidget::keyReleaseEvent( QKeyEvent* event ) {
                _gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
            }
            
            void QOSGWidget::mousePressEvent( QMouseEvent* event ) {
                int button = 0;
                switch(event->button()) {
                    case(Qt::LeftButton): button = 1; break;
                    case(Qt::MidButton): button = 2; break;
                    case(Qt::RightButton): button = 3; break;
                    case(Qt::NoButton): button = 0; break;
                    default: button = 0; break;
                }
                _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
            }
            
            void QOSGWidget::mouseReleaseEvent( QMouseEvent* event ) {
                int button = 0;
                switch(event->button()) {
                    case(Qt::LeftButton): button = 1; break;
                    case(Qt::MidButton): button = 2; break;
                    case(Qt::RightButton): button = 3; break;
                    case(Qt::NoButton): button = 0; break;
                    default: button = 0; break;
                }
                _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
            }
            
            void QOSGWidget::mouseMoveEvent( QMouseEvent* event ) {
                _gw->getEventQueue()->mouseMotion(event->x(), event->y());
            }
            
        }
    }
}
