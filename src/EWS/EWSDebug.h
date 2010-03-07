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

#ifndef __EWSDEBUG_H
#define __EWSDEBUG_H

#include <QDebug>
#include <osg/Vec2>
#include "Barrier.h"
#include "DripSource.h"


namespace ews {
    namespace util {
        namespace debug {
            
            inline QDebug operator<<(QDebug dbg, const osg::Vec2& v) {
                dbg.nospace() << '(' << v.x() << ',' << v.y() << ')';
                return dbg.space();
            }
            
            /**
             * Debug output operator for Barrier.
             */
            inline QDebug operator<<(QDebug dbg, const ews::app::model::Barrier &b) {
                dbg << b.metaObject()->className() << '{';
                dbg << "name=" << b.objectName() << ',';
                dbg << "enabled=" << b.isEnabled() << ',';
                dbg << "numSlits=" << b.getNumSlits() << ',';
                dbg << "slitWidth=" << b.getSlitWidth() << ',';
                dbg << "slitSeparation=" << b.getSlitSeparation() << ',';
                dbg << "start=" << b.getStart() << ',';
                dbg << "end=" << b.getEnd();
                dbg << "}";
                return dbg;
            }
            
            /**
             * Debug output operator for DripSource.
             */
            inline QDebug operator<<(QDebug dbg, const ews::app::model::DripSource &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "enabled=" << ds.isEnabled() << ",";
                dbg << "frequency=" << ds.getFrequency() << ",";
                dbg << "amplitude=" << ds.getAmplitude();
                dbg << "}";
                return dbg;
            }
            
        }
    }
}


#endif