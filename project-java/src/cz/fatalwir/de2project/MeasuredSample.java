/*
 * MeasuredSample.java
 *
 *       Created on:  Dec 04, 2019
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

public class MeasuredSample {
    
    private double voltage;
    private double current;
    private int averagedCount;
    private final double refConstant;

    public MeasuredSample(double refConst) {
        this.voltage = 0;
        this.current = 0;
        this.averagedCount = 0;
        this.refConstant = refConst;
    }
    
    public MeasuredSample(double refConst, int v, int c) {
        this.refConstant = refConst;
        this.voltage = v/refConstant;
        this.current = c/refConstant;
        this.averagedCount = 1;
    }

    public void addToAverage(int v, int c) {
        voltage+= v/refConstant;
        current+= c/refConstant;
        averagedCount++;
    }
    
    public double getVoltage() {
        return voltage/averagedCount;
    }
    
    public double getCurrent() {
        return current/averagedCount;
    }
    
    public double getPower() {
        return voltage*current/averagedCount;
    }
    
    public int getAveragedCount() {
        return averagedCount;
    }
    
}
