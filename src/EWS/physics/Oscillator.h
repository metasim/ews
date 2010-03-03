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

#ifndef __OSCILLATOR_H
#define __OSCILLATOR_H

#include "WaveModel.h"

namespace ews {
    namespace physics {
        const double DEFAULT_PERIOD = 2.0;
        const double DEFAULT_AMPLITUDE = 1.0;
        const double MAX_AMPLITUDE = 2.0;
        const double DEFAULT_RADIUS = 2.0;
        const unsigned int DEFAULT_X = 8;
        /**
         * @ingroup Physics
         * Oscillator
         */        
        class Oscillator {
        public:
            /**
             * Constructor for default location
             * @param waveModel Model this oscillator is attached to
             */
            Oscillator(WaveModel& waveModel);
            /**
             * Constructor for location x, y
             * @param waveModel Model this oscillator is attached to
             * @param x X location of oscillator
             * @param y Y location of oscillator
             */
            Oscillator(WaveModel& waveModel, unsigned int x, unsigned int y);
            /**
             * Updates the time value of the oscillator and oscillates the attached wave model.
             * @param time Current time
             */
            void updateTimeAndOscillator(double time);
            /**
             * Reconfigures the oscillator with a new pulse
             */
            void firePulse();
            /**
             * Gets the current value associated with the oscillator.
             */
            double getValue() const;
            
            /**
             * Gets the velocity of the oscillator (CRUFT?)
             */
            double getVelocity() const;
            
            /**
             * Gets the radius the oscillator acts upon.
             * @return Oscillator radius
             */
            double getRadius() const { return _radius; }
            /**
             * Sets the radius the oscillator acts upon.
             * @param r Oscillator radius
             */
			void setRadius(double r) { _radius = r > 0 ? r : 0; }
            /**
             * Gets the period of the oscillator
             * @return Oscillator period
             */
            double getPeriod() const { return _period; }
            /**
             * Sets the period of the oscillator
             * @param p Oscillator period
             */
            void setPeriod(double p) { _period = p; }
            /**
             * Sets the location of the oscillator.
             * @param x X location of oscillator
             * @param y Y location of oscillator
             */
            void setLocation(unsigned int x, unsigned int y) { _x = x; _y = y; }
            /**
             * Gets the x location of the oscillator.
             * @return X location of oscillator
             */
            unsigned int x() const { return _x; }
            /**
             * Gets the y location of the oscillator.
             * @return Y location of oscillator
             */
            unsigned int y() const { return _y; }
            /**
             * Computes the frequency of the oscillator.
             * @return Oscillator frequency.
             */
            double computeFrequency() const { return 1 / _period; }
            /**
             * Gets the amplitude of the oscillator.
             * @return Oscillator amplitude.
             */
            double getAmplitude() const { return _amplitude; }
            /**
             * Gets the amplitude of the oscillator.
             * @param a Oscillator amplitude.
             */
            void setAmplitude(double a) { _amplitude = a; }
            /**
             * Gets the osillator's time state
             * @return time per the oscillator's perspective
             */
            double getTime() const { return _time; }
            /**
             * Sets whether the oscillator should oscillate.
             */
            void setOscillateStatus(bool shouldOscillate);
            /**
             * Gets whether or not this oscillator is turned on.
             * @return oscillating status
             */
            bool getOscillateStatus() const { return _oscillating; }
            
        private:
            void resetPhase();
            double getCosArg() const;
            WaveModel& _waveModel;
            unsigned int _x;
            unsigned int _y;
            double _radius;
            double _amplitude;
            double _period;
            double _time;
            double _phase;
            bool _oscillating;
            bool _inPulse;
        };
    }
}

#endif // __OSCILLATOR_H
