#ifndef QOSGWIDGET_H
#define QOSGWIDGET_H

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

#include <QtGui/QWidget>
#include <QtOpenGL/QGLWidget>
#include <QtCore/QTimer>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

class QOSGWidget : public QGLWidget, public osgViewer::Viewer {
    Q_OBJECT
public:
    QOSGWidget( QWidget* parent = 0);
    virtual ~QOSGWidget();

    osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
    const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }
    virtual void paintGL();
    
protected:
    void changeEvent(QEvent *e);
//
//    void init();
//
    virtual void resizeGL( int width, int height );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void closeEvent( QCloseEvent * event );
    virtual void destroyEvent( bool destroyWindow = true, bool destroySubWindows = true);
    

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;

private:
    QTimer _timer;
    
};

#endif // QOSGWIDGET_H
