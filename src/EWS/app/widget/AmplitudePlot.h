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


#ifndef __AMPLITUDE_PLOT_H
#define __AMPLITUDE_PLOT_H

#include <QtGui>
#include <QMap>
#include "PointSampler.h"

/** Forward declaration of UI implementation class. */
/** @cond */
namespace Ui {
    class AmplitudePlotForm;
}

// Forward declaration for map.
class QwtPlotCurve;

/** @endcond */

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::PointSampler;
            
            class AmplitudePlot : public QWidget {
                Q_OBJECT
                
            public:
                AmplitudePlot(QWidget * parent = 0);
                virtual ~AmplitudePlot();
                
                void addSampleSource(PointSampler* src);
                void removeSampleSource(PointSampler* src);
                
            public slots:
                void updatePlot();
                
            private:
                Ui::AmplitudePlotForm* _ui;
                QMap<PointSampler*, QwtPlotCurve*> _sources;
            };
        }
    }
}

#endif // __AMPLITUDE_PLOT_H
