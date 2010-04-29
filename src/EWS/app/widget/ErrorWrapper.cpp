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

#include "ErrorWrapper.h"

#include <QTimer>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <iostream>
#include <sstream>
#include <cstring>

namespace ews {
    namespace app {
        namespace widget {
            static ErrorWrapper singletonErrWrapper;

            void errHandler(QtMsgType type, const char *msg) {
                singletonErrWrapper.handle(type, msg);
            }
            
            void saveHandler(QtMsgHandler handler) {
                singletonErrWrapper.setHandler(handler);
            }

            inline void logMessage(const char* typeStr, const char* msg) {
                std::ostringstream oss;
                oss << typeStr << ": " << msg << std::endl;
                std::cerr << oss.str();
#if defined(__WINLOG)
                OutputDebugStringA(oss.str().c_str());
#endif
            }
            
            void ErrorWrapper::handle(QtMsgType type, const char *msg) {
                strncpy(_msg, msg, MAX_ERR_BUFF - 1);
                _msg[MAX_ERR_BUFF - 1] = '\0'; // Just in case msg size is > MAX_ERR_BUFF
                _type = type;
                switch (type) {
                    case QtDebugMsg:
                        logMessage("Debug", msg);
                        break;
                    case QtWarningMsg:
                        logMessage("Warning", msg);
                        break;
                    case QtCriticalMsg:
                        logMessage("Critical", msg);
                        break;
                    case QtFatalMsg:
                        logMessage("Fatal", msg);
                        break;
                }
                
                if (type != QtDebugMsg && _handler != NULL) {
                    // Like Java EventQueue's invokeLater
                    QTimer::singleShot(0, this, SLOT(showMessage()));
                }
            }
            
            void ErrorWrapper::showMessage() {
                QWidget* win = QApplication::activeWindow();
                QMainWindow* main = qobject_cast<QMainWindow*>(win);
                if (main != NULL) {
                    main->lower();
                }
                _handler(_type, _msg);
            }
        }
    }
}
