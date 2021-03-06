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
#include <qwt_plot_picker.h>
#include <QtCore>

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
                    double halfspan = std::max(std::max(fabs(retval.top()), fabs(retval.bottom())), 1e-2);
                    retval.setTop(-halfspan);
                    retval.setHeight(halfspan*2.0);
                    return retval;
                }
                
            private:
                const SampleHistory& _history;
            };
            
            /** Plot class contructor. */
            AmplitudePlot::AmplitudePlot(QWidget* parent)
            : QWidget(parent), _ui(new Ui::AmplitudePlotForm) { 
                _ui->setupUi(this);
                QwtPlot* p = _ui->plot;
                p->setCanvasBackground(QColor(Qt::white));
                // We don't need the cache here
                p->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
                p->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
                // Axis 
                p->setAxisTitle(QwtPlot::xBottom, "Ticks");
                
                p->setAxisTitle(QwtPlot::yLeft, "Height");
                
                QwtPlotPicker* picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                          QwtPicker::PointSelection | QwtPicker::DragSelection, 
                                                          QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, 
                                                          p->canvas());
                picker->setRubberBandPen(QColor(Qt::darkGreen));
                picker->setRubberBand(QwtPicker::CrossRubberBand);
                picker->setTrackerPen(QColor(Qt::darkGreen));
            }
            
            AmplitudePlot::~AmplitudePlot() {
                delete _ui;
            }
            
            void AmplitudePlot::addSampleSource(PointSampler* src) {
                
                // Insert new curves
                QwtPlotCurve* curve = new QwtPlotCurve(src->objectName());
                _sources.insert(src, curve);
                curve->setPen(QPen(src->getColor()));
                curve->setRenderHint(QwtPlotItem::RenderAntialiased);
                
                SampleHistoryPlotDataAdapter adapter(src);
                curve->setData(adapter);
                
                curve->attach(_ui->plot);
                
                _ui->plot->setAxisScale(QwtPlot::xBottom, 0, src->getHistory().size());

                connect(src, SIGNAL(sampleHistoryChanged(const PointSampler*)), SLOT(updatePlot()));
                connect(src, SIGNAL(enabledChanged(bool)), SLOT(updateSeriesVisibility(bool)));
            }
            
            void AmplitudePlot::updateSeriesVisibility(bool state) {
                QObject* srcObj = QObject::sender();
                PointSampler* src = qobject_cast<PointSampler*>(srcObj);
                if(src) {
                    QwtPlotCurve* curve = _sources.value(src);
                    if(curve)  {
                        qDebug() << "Setting visibility of" << curve << "to" << state;
                        curve->setVisible(state);
                    }
                }
            }
            
            void AmplitudePlot::removeSampleSource(PointSampler* src) {
                QwtPlotCurve* curve = _sources.take(src);
                curve->detach();
                src->disconnect(this);
                delete curve;
                updatePlot();
            }
            
            void AmplitudePlot::updatePlot() {
                _ui->plot->replot();
            }
            
            void AmplitudePlot::reset() {
                SamplerCurveIterator it;
                for(it = _sources.begin(); it != _sources.end(); ++it) {
                    PointSampler* src = it.key();
                    removeSampleSource(src);
                }
            }
        }
    }
}
