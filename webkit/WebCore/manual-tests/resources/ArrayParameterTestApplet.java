/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;


public class ArrayParameterTestApplet
    extends Applet
{
    public void init()
    {
        setLayout(new BorderLayout());
    }


    public void start()
    {
    }


    public void stop()
    {
    }


    public void destroy()
    {
    }


    public void arrayFunction(String [] array) {
        System.out.println("arrayFunction called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
    
    public void booleanFunction(boolean[] array) {
        System.out.println("booleanArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }

    public void byteFunction(byte[] array) {
        System.out.println("byteArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
    
    public void charFunction(char[] array) {
        System.out.println("charArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void shortFunction(short[] array) {
        System.out.println("shortArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void intFunction(int[] array) {
        System.out.println("intArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void longFunction(long[] array) {
        System.out.println("longArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void floatFunction(float[] array) {
        System.out.println("floatArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void doubleFunction(double[] array) {
        System.out.println("doubleArray called");
        for (int i = 0; i < array.length; i++)
            System.out.println(array[i]);
        }
        
    public void objectFunction(Applet[] array) {
        System.out.println("objectArray called");
        for (int i = 0; i < array.length; i++) 
            System.out.println(array[i]);
        }
}
