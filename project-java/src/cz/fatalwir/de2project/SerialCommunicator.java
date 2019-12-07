/*
 * SerialCommunicator.java
 *
 *       Created on:  Dec 01, 2019
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

import com.fazecast.jSerialComm.*;
import java.nio.charset.Charset;

public class SerialCommunicator {

    // Serial port object variable
    public static SerialPort port;
    private static int byteCounter = 0;
    private static int vBuffer = 0;
    private static int cBuffer = 0;
    
    private static LcdSender lcdSender;

    // Method for connecting to the serial port
    public static boolean connectPort(String name, int baud) {
        port = SerialPort.getCommPort(name);
        port.setBaudRate(baud);
        if (port.openPort()) {
            port.addDataListener(new SerialPortDataListener() {
                @Override
                public int getListeningEvents() {
                    return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
                }

                @Override
                public void serialEvent(SerialPortEvent event) {
                    if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                        return;
                    }
                    byte[] newData = new byte[port.bytesAvailable()];
                    int numRead = port.readBytes(newData, newData.length);
                    System.out.println("Read " + numRead + " byte(s).");
                    for (byte b : newData) {
                        int n = b & 0xFF;
                        System.out.print(String.format("%03d 0x%02x 0b", n, n, n));
                        System.out.println(String.format("%8s", Integer.toBinaryString(n)).replace(' ', '0'));
                        processIncomingByte(n);
                    }
                }
            });
            lcdSender = new LcdSender();
            lcdSender.start();
        } else {
            return false;
        }

        return true;
    }

    // Method for disconnecting the serial port
    public static boolean disconnectPort() {
        lcdSender.stop = true;
        lcdSender = null;
        return port.closePort();
    }

    // Method for sending text to LCD
    public static boolean sendTextToLCD(int x, int y, boolean clr, String s) {
        if (port.isOpen()) {
            byte buffer[] = new byte[s.length() + 2];
            // Control byte (0b1cyyxxxx)
            buffer[0] = (byte) (128 + ((clr ? 1 : 0) * 64) + 16 * y + x);
            System.arraycopy(s.getBytes(Charset.forName("US-ASCII")), 0, buffer, 1, s.length());
            buffer[s.length() + 1] = (byte) 0;
            //port.writeBytes(buffer, buffer.length);
            for (byte b : buffer) {
                port.writeBytes(new byte[]{b}, 1);
                try {
                    Thread.sleep(1);
                } catch (Exception e) {
                }
            }
            return true;
        } else {
            return false;
        }
    }

    private static void processIncomingByte(int val) {
        if(val > 127) {
            // If the byte is first one then decode it
            byteCounter = 1;
            cBuffer = val%8;
            vBuffer = (val/8)%8;
        } else {
            if(byteCounter == 1) {
                // If second byte received
                vBuffer = (vBuffer*128)+val;
                byteCounter = 2;
            } else if(byteCounter == 2) {
                // If third byte is received
                cBuffer = (cBuffer*128)+val;
                byteCounter = 0;
                DataProcessor.processValues(vBuffer, cBuffer);
            }
        }
    }
}
