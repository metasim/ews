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
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>

#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/Channel>
#include <osgAnimation/UpdateCallback>

#include "CameraController.h"


namespace ews {
    namespace app {
        namespace widget {
            QOSGWidget::QOSGWidget(QWidget * parent):
            QGLWidget(parent), osgViewer::Viewer(), _gw(0), _timer()
            {
                osg::setNotifyLevel(osg::INFO);
                _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
                setFocusPolicy(Qt::ClickFocus);
                
                if(true) {
                    QGLFormat fmt; 
                    QGLFormat::setDefaultFormat(fmt); 
                    fmt.setSamples(4); 
                    fmt.setSampleBuffers(true); 
                    setFormat(fmt);                    
                }
                
                osg::Camera* c = getCamera();
                c->setViewport(new osg::Viewport(0,0,width(),height()));
                c->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
                c->setGraphicsContext(getGraphicsWindow());
                c->setClearColor(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
                
                setThreadingModel(osgViewer::Viewer::SingleThreaded);
                
                osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
                
//                keyswitchManipulator->addMatrixManipulator( '1', "Default", new ews::app::drawable::CameraController);
//                keyswitchManipulator->addMatrixManipulator( '2', "Terrain", new osgGA::TerrainManipulator() );
//                keyswitchManipulator->addMatrixManipulator( '3', "Trackball", new osgGA::TrackballManipulator() );
//                keyswitchManipulator->addMatrixManipulator( '3', "Flight", new osgGA::FlightManipulator() );
//                keyswitchManipulator->addMatrixManipulator( '4', "Drive", new osgGA::DriveManipulator() );
                
//                setCameraManipulator(keyswitchManipulator.get());
                
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
            
            QOSGWidget::~QOSGWidget()
            {
                _timer.stop();
                // Don't delete _gw, smart pointer takes care of it.
            }
            
            void QOSGWidget::destroyEvent(bool destroyWindow, bool destroySubWindows)
            {   
                _gw->getEventQueue()->closeWindow();
            }
            
            
            void QOSGWidget::closeEvent( QCloseEvent * event )
            {
                event->accept();
                _gw->getEventQueue()->closeWindow();
            }
            
            void QOSGWidget::paintGL()
            {
                frame();
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
        }
    }
}
