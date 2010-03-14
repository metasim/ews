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


#include <QString>
#include <QWidget>
#include <QDebug>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Matrix>
#include <sstream>

#include "Barrier.h"
#include "DripSource.h"

namespace ews {
    /**
     * Parent namespace for utility classes independent of physics and UI.
     */
    namespace util {
        /**
         * Contains classes and methods used for gathering debug information.
         */
        namespace debug {
            
            /** Character to use for indentation. */
            const char INDENT_CHAR = ' ';
            /** Number of indentation characters per indentation level. */
            const int INDENT_WIDTH = 4;
            
            /**
             * Create the string representing the given indentation level.
             */
            inline QString makeIndent(int i) {
                return QString(INDENT_WIDTH*i, INDENT_CHAR); 
            }
            
            /**
             * Utility class for tracking program flow for debugging purposes.
             */
            class Tracer {
            public:
                Tracer(const QString& file, int line, const QString& func, const QString& msg) : _file(file), _line(line), _func(func) {
                    // Just get the last file component
                    _file = _file.section('/' , -1);
                    QString indent = makeIndent(count());
                    qDebug() << QString("->%1 %2 started scope at line %3 in %4").arg(indent, _func).arg(_line).arg(_file);
                    if(msg.size() > 0) {
                        qDebug() << QString("  %1 *) %2").arg(indent, msg);
                    }
                    count()++;
                }
                
                ~Tracer() {
                    count()--;
                    QString indent = makeIndent(count());
                    qDebug() << QString("<-%1 Ended scope from line %2 (%3)").arg(indent).arg(_line).arg(_func);
                }
                
            private:
                
                int& count() {
                    static int _count = 0;
                    return _count;
                }
                
                QString _file;
                int _line;
                QString _func;
            };
            
        }
    }
}

/** If debug is on and a QTRACE_LEVEL hasn't been defined, set to zero. */
#if !defined(NO_DEBUG) && !defined(QTRACE_LEVEL)
#define QTRACE_LEVEL 1
#endif

/** Define mapping between QTRACE* macros and supporting class. */
#if defined(QTRACE_LEVEL)
# if defined(__GNUG__)
#  define _QTRACER(s) ews::util::debug::Tracer t ## __LINE__ ( __FILE__, __LINE__, __FUNCTION__, (s) )
# else
#  define _QTRACER(s) ews::util::debug::Tracer t ## __LINE__ ( __FILE__, __LINE__, "??", (s) )
# endif
#else
# define _QTRACER(s) 
#endif

/** Enable or disable basic QTRACE. */
#if defined(QTRACE_LEVEL)
# define QTRACE _QTRACER("")
#else
# define QTRACE
#endif

/** Enable or disable QTRACE1. */
#if QTRACE_LEVEL >= 1
# define QTRACE1 _QTRACER
#else
# define QTRACE1
#endif

/** Enable or disable QTRACE2. */
#if QTRACE_LEVEL >= 2
# define QTRACE2 _QTRACER
#else
# define QTRACE2
#endif



// These QDebug debugging output operators are declared in the global
// scope to simplify usage and detection by compiler.

/** 2-D Vector debug output operator. */
inline QDebug operator<<(QDebug dbg, const osg::Vec2& v) {
    dbg.nospace() << '(' << v.x() << ',' << v.y() << ')';
    return dbg.space();
}

/** 3-D Vector debug output operator. */
inline QDebug operator<<(QDebug dbg, const osg::Vec3& v) {
    dbg.nospace() << '(' << v.x() << ',' << v.y() << ',' << v.z() << ')';
    return dbg.space();
}

/** Quaternion debug output operator. */
inline QDebug operator<<(QDebug dbg, const osg::Quat& q) {
    dbg.nospace() << '{' << q.x() << ',' << q.y() << ',' << q.z() << ',' << q.w() << '}';
    return dbg.space();
}

/** 4x4 Matrix debug output operator. */
inline QDebug operator<<(QDebug dbg, const osg::Matrix& m) {
    osg::Vec3 trans, scale;
    osg::Quat rot, scaleRot;
    m.decompose(trans, rot, scale, scaleRot);
    dbg << " postion:" << trans << '\n';
    dbg << "rotation:" << rot << '\n';
    dbg << "   scale:" << scale << '\n';
    
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

inline QDebug operator<<(QDebug dbg, const QWidget &w) {
    dbg << w.metaObject()->className() << "{";
    dbg << "name=" << w.objectName() << ",";
    dbg << "enabled=" << w.isEnabled();
    dbg << "}";
    return dbg;
}

/** Fallback default QDebug output operator for 
 * types that have an ostream operator. If compiler fails here
 * then either QDebug::operator<< or ostream::operator<< needs
 * to be defined. */
template<typename T> inline
QDebug operator<<(QDebug dbg, const T& t) {
    std::ostringstream oss;
    oss.operator<< (t);
    dbg << oss.str();
    return dbg;
}




#endif // __EWSDEBUG_H
