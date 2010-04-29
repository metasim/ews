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

#ifndef __ERROR_WRAPPER_H
#define __ERROR_WRAPPER_H

#include <QObject>
#include <QtGlobal> // Has QtMsgHandler and QtMsgType

namespace ews {
    namespace app {
        namespace widget {
            /** Maximum size of displayed error messages. */
            const int MAX_ERR_BUFF = 256;
            /** Wrapper around the singleton ErrorWrapper's handle */
            void errHandler(QtMsgType type, const char *msg);
            /** Wrapper around the singleton ErrorWrapper's setHandler */
            void saveHandler(QtMsgHandler handler);
            /** Message handler for Qt logging system. */
            class ErrorWrapper : public QObject {
                Q_OBJECT
            public:
                /**
                 * Default constructor
                 */
                ErrorWrapper():_handler(NULL), _msg(new char[MAX_ERR_BUFF]), _type(QtDebugMsg) { /* do nothing */ }
                virtual ~ErrorWrapper() { delete[] _msg; }
                /**
                 * Sets the handle to pass off exceptions more significant than debug messages.
                 */
                void setHandler(QtMsgHandler handler) { _handler = handler; }
                /**
                 * Handles all messages, including debug messages, although more significant messages
                 * are passed on to an additional handler.
                 */
                void handle(QtMsgType type, const char *msg);
            private slots:
                /**
                 * Slot for actually invoking additional handler, but after other events have been
                 * processed.
                 */
                void showMessage();
            private:
                QtMsgHandler _handler;
                char* _msg;
                QtMsgType _type;
            };
        }
    }
}

#endif // __ERROR_WRAPPER_H
