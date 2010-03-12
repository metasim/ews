/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 */


/* This file modified version of osgGA::TerrainManipulator. Not enough of the
 * implementation was virtual to get desired behavior from subclassing. */
#ifndef __CAMERA_CONTROLLER_H
#define __CAMERA_CONTROLLER_H

#include <osgGA/MatrixManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/GUIEventAdapter>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgGA;
            
            /**
             * Hack of osgGA::TerrainManipulator for
             * setting up some defaults and limiting movement to some degree.
             */
            class CameraController : public osgGA::TerrainManipulator {
            public:
                CameraController();
                virtual void computeHomePosition();
                
 //               virtual const char* className() const { return "CameraController"; }
//                
//                
//                /** set the position of the matrix manipulator using a 4x4 Matrix.*/
//                virtual void setByMatrix(const Matrixd& matrix);
//                
//                /** set the position of the matrix manipulator using a 4x4 Matrix.*/
//                virtual void setByInverseMatrix(const Matrixd& matrix) { setByMatrix(Matrixd::inverse(matrix)); }
//                
//                /** get the position of the manipulator as 4x4 Matrix.*/
//                virtual Matrixd getMatrix() const;
//                
//                /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
//                virtual Matrixd getInverseMatrix() const;
//                
//                /** Get the FusionDistanceMode. Used by SceneView for setting up stereo convergence.*/
//                virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }
//                
//                /** Get the FusionDistanceValue. Used by SceneView for setting up stereo convergence.*/
//                virtual float getFusionDistanceValue() const { return _distance; }
//                
//                /** Attach a node to the manipulator. 
//                 Automatically detaches previously attached node.
//                 setNode(NULL) detaches previously nodes.
//                 Is ignored by manipulators which do not require a reference model.*/
//                virtual void setNode(Node*);
//                
//                /** Return node if attached.*/
//                virtual const Node* getNode() const;
//                
//                /** Return node if attached.*/
//                virtual Node* getNode();
//                
//                /** Move the camera to the default position. 
//                 May be ignored by manipulators if home functionality is not appropriate.*/
//                virtual void home(const GUIEventAdapter& ea,GUIActionAdapter& us);
//                
//                /** Start/restart the manipulator.*/
//                virtual void init(const GUIEventAdapter& ea,GUIActionAdapter& us);
//                
//                /** handle events, return true if handled, false otherwise.*/
//                virtual bool handle(const GUIEventAdapter& ea,GUIActionAdapter& us);
//                
//                /** Get the keyboard and mouse usage of this manipulator.*/
//                virtual void getUsage(ApplicationUsage& usage) const;
                
            protected:
                
                virtual ~CameraController();
                
//                bool intersect(const Vec3d& start, const Vec3d& end, Vec3d& intersection) const;
//                
//                /** Apply rotation based on mouse change in x and y. */
//                virtual void doRotate(double dx, double dy);
//                /** Apply "zoom" based on mouse change in x and y. */
//                virtual void doZoom(double dx, double dy);
//                /** Apply "pan" based on mouse change in x and y. */
//                virtual void doPan(double dx, double dy);
//                
//                /** Reset the internal GUIEvent stack.*/
//                void flushMouseEventStack();
//                /** Add the current mouse GUIEvent to internal stack.*/
//                void addMouseEvent(const GUIEventAdapter& ea);
//                
//                void computePosition(const Vec3d& eye,const Vec3d& lv,const Vec3d& up);
//                
//                /** For the give mouse movement calculate the movement of the camera.
//                 Return true is camera has moved and a redraw is required.*/
//                virtual bool calcMovement();
//                
//                void trackball(Vec3& axis,double& angle, double p1x, double p1y, double p2x, double p2y);
//                double tb_project_to_sphere(double r, double x, double y);
//                
//                virtual void clampOrientation();
//                virtual void clampPosition();
//                
//                
//                // Internal event stack comprising last two mouse events.
//                ref_ptr<const GUIEventAdapter> _ga_t1;
//                ref_ptr<const GUIEventAdapter> _ga_t0;
//                
//                ref_ptr<Node> _node;
//                Vec3d _center;
//                Quat _rotation;
//                double _distance;
//                Vec3d _previousUp;

            };
            
        }
    }
}


#endif //__CAMERA_CONTROLLER_H

