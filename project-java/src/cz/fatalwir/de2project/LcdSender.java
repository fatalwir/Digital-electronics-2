/*
 * Frame.java
 *
 *       Created on:  Dec 05, 2019
 *  Last Updated on:  Dec 05, 2019
 *           Author:  Ondrej Kolar
 *
 * Copyright (c) 2019 Ondrej Kolar
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
package cz.fatalwir.de2project;

import java.text.SimpleDateFormat;
import java.util.Date;

public class LcdSender extends Thread {

    public boolean stop;

    @Override
    public void run() {
        while (!stop) {
            MeasuredSample sample = DataProcessor.getLastSample();
            if (sample != null) {
                double v = sample.getVoltage();
                double c = sample.getCurrent();
                double p = sample.getPower();

                SimpleDateFormat formatterLcd = new SimpleDateFormat("HH:mm:ss");
                SerialCommunicator.sendTextToLCD(0, 0, false,
                        String.format("U=%.1fV  ", v));
                SerialCommunicator.sendTextToLCD(8, 0, false,
                        String.format("I=%.2fA ", c));
                SerialCommunicator.sendTextToLCD(0, 1, false,
                        String.format("P=%.1fW ", p));
                SerialCommunicator.sendTextToLCD(8, 1, false,
                        String.format("%s", formatterLcd.format(new Date())));
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException ex) {
            }
        }
    }

}
