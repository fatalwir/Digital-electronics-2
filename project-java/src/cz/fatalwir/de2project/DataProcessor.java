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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import static cz.fatalwir.de2project.Main.f;
import java.text.SimpleDateFormat;
import java.util.Date;

public class DataProcessor {
    
    private static MeasuredSample sample;
    private static MeasuredSample lastSample;
    private static int avgCount = 0;
    private static double refConst = 1;
    
    private static File logFile;
    private static PrintWriter logFileWriter;
    
    public static void processValues(int v, int c) {
        if(sample == null) {
            sample = new MeasuredSample(refConst, v, c);
            if(avgCount >= sample.getAveragedCount()) {
                propagateValues();
                lastSample = sample;
                sample = null;
            }
        } else {
            sample.addToAverage(v, c);
            if(avgCount >= sample.getAveragedCount()) {
                propagateValues();
                lastSample = sample;
                sample = null;
            }
        }
    }
    
    private static void propagateValues() {
        double v = sample.getVoltage();
        double c = sample.getCurrent();
        double p = sample.getPower();
        
        f.setVoltage(v);
        f.setCurrent(c);
        f.setPower(p);

        SimpleDateFormat formatterPc = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss.SSS");
        Date date = new Date();
        String d = formatterPc.format(date);
        f.setTimeLabelText(d);
    }

    public static void setAvgCount(int n) {
        avgCount = n;
    }
    
    public static void setRefConst(double ref) {
        refConst = ref;
    }

    public static boolean openFile() {
        logFile = new File("log.csv");
        try {
            if (logFile.createNewFile()) {
                // New file created so have to make CSV header
                try {
                    logFileWriter = new PrintWriter(logFile);
                    logFileWriter.append("\"Voltage [V]\";\"Current [A]\";\"Power [W]\";\"DateTimeStamp\"\r\n");
                } catch (FileNotFoundException ex) {
                    logFile = null;
                    return false;
                }
            } else {
                try {
                    logFileWriter = new PrintWriter(logFile);
                } catch (FileNotFoundException ex) {
                    logFile = null;
                    return false;
                }
            }
        } catch (IOException e) {
            logFile = null;
            return false;
        }
        return true;
    }
    
    public static void closeFile() {
        logFileWriter.close();
        logFileWriter = null;
        logFile = null;
    }
    
    public static MeasuredSample getLastSample() {
        return lastSample;
    }
    
}
