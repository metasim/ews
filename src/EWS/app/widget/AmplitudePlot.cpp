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


#include "AmplitudePlot.h"
#include "ui_AmplitudePlot.h"
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_math.h>
#include <qwt_data.h>
#include "SampleHistory.h"

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::SampleHistory;
            
            class SampleHistoryPlotDataAdapter : public QwtData {
            public:
                SampleHistoryPlotDataAdapter(PointSampler* src) : _history(src->getHistory()) {}
                SampleHistoryPlotDataAdapter(const SampleHistory& hist) : _history(hist) {}
                virtual ~SampleHistoryPlotDataAdapter() {}
                virtual QwtData* copy() const {
                    return new SampleHistoryPlotDataAdapter(_history);
                }
                virtual size_t size() const { 
                    return _history.size(); 
                }
                virtual double x(size_t i) const {
                    return i;
                }
                virtual double y(size_t i) const {
                    double val = _history.valueAt(i);
                    return val;
                }
                virtual QwtDoubleRect boundingRect() const {
                    QwtDoubleRect retval = QwtData::boundingRect();
                    double halfspan = std::max(fabs(retval.top()), fabs(retval.bottom()));
                    retval.setTop(-halfspan);
                    retval.setHeight(halfspan*2.0);
                    return retval;
                }
                
            private:
                const SampleHistory& _history;
            };
            
            AmplitudePlot::AmplitudePlot(QWidget * parent)
            : QWidget(parent), _ui(new Ui::AmplitudePlotForm) { 
                
                _ui->setupUi(this);
                
                // Disable polygon clipping
//                QwtPainter::setDeviceClipping(false);
                
                QwtPlot* p = _ui->plot;
                p->setCanvasBackground(QColor(Qt::white));
                // We don't need the cache here
                p->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
                p->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
                // Assign a title
                // p->setTitle("Amplitude vs Time");
                p->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
                
                // Axis 
                p->setAxisTitle(QwtPlot::xBottom, "Ticks");

//                QwtPlotGrid *grid = new QwtPlotGrid;
//                grid->enableXMin(true);
//                grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
//                grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
//                grid->attach(p);
            }
            
            AmplitudePlot::~AmplitudePlot() {
                delete _ui;
            }
            
            void AmplitudePlot::addSampleSource(PointSampler* src) {
                
                // Insert new curves
                QwtPlotCurve* curve = new QwtPlotCurve(src->objectName());
                _sources.insert(src, curve);
                curve->setPen(QPen(Qt::red));
                SampleHistoryPlotDataAdapter adapter(src);
                curve->setData(adapter);
                
                curve->attach(_ui->plot);
                
                _ui->plot->setAxisScale(QwtPlot::xBottom, 0, src->getHistory().size());
//                _ui->plot->setAxisScale(QwtPlot::yLeft, -1, 1);

                connect(src, SIGNAL(sampleHistoryChanged()), SLOT(updatePlot()));
            }
            
            void AmplitudePlot::removeSampleSource(PointSampler* src) {
                QwtPlotCurve* curve = _sources.take(src);
                curve->detach();
                src->disconnect(this);
                delete curve;
            }
            
            void AmplitudePlot::updatePlot() {
                _ui->plot->replot();
            }
        }
    }
}
