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

#ifndef __DRAWABLE_ADAPTER_H
#define __DRAWABLE_ADAPTER_H

#include <QtCore>
#include <osg/Node>

/**
 *  Adapter between the Qt and OSG worlds.
 */
class DrawableAdapter : public QObject
{
    Q_OBJECT

public:
    /**
     * Standard ctor.
     */
    DrawableAdapter(QObject* peer, osg::Node);
    virtual ~DrawableAdapter();

private slot:
    void osgSlotDispatch();
    
private:
    Q_DISABLE_COPY(DrawableAdapter)
};

inline QDebug operator<<(QDebug dbg, const DrawableAdapter &ds) {
    dbg << ds.metaObject()->className() << "{";
    dbg << "name=" << ds.objectName() << ",";
    dbg << "}";
    return dbg;
}



#endif // __EWS_DRAWABLE_H


