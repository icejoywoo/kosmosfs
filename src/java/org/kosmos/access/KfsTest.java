/**
 * $Id: KfsTest.java $
 *
 * Created 2007/08/25
 *
 * Copyright (C) 2007 Kosmix Corp.
 *
 * This file is part of Kosmix File System (KFS).
 *
 * KFS is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation under version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * \brief A Java unit test program to access KFSAccess APIs.
 */


package org.kosmos.access;

import java.io.*;
import java.net.*;
import java.util.Random;
import java.nio.ByteBuffer;

public class KfsTest
{
    public static void main(String args[]) {
        if (args.length < 1) {
            System.out.println("Usage: KfsTest <meta server> <port>");
            System.exit(1);
        }
        try {
            int port = Integer.parseInt(args[1].trim());
            KfsAccess kfsAccess = new KfsAccess(args[0], port);
            
            String basedir = new String("jtest");

            if (!kfsAccess.kfs_exists(basedir)) {
                if (kfsAccess.kfs_mkdirs(basedir) != 0) {
                    System.out.println("Unable to mkdir");
                    System.exit(1);
                }
            }

            if (!kfsAccess.kfs_isDirectory(basedir)) {
                System.out.println("KFS doesn't think " + basedir + " is a dir!");
                System.exit(1);

            }
            String path = new String(basedir + "/foo.1");
            KfsOutputChannel outputChannel;
            if ((outputChannel = kfsAccess.kfs_create(path)) == null) {
                System.out.println("Unable to call create");
                System.exit(1);
            }
            
            String [] entries;
            if ((entries = kfsAccess.kfs_readdir(basedir)) == null) {
                System.out.println("Readdir failed");
                System.exit(1);
            }

            System.out.println("Readdir returned: ");
            for (int i = 0; i < entries.length; i++) {
                System.out.println(entries[i]);
            }

            // write something
            int numBytes = 2048;
            char [] dataBuf = new char[numBytes];
            
            generateData(dataBuf, numBytes);
            
            String s = new String(dataBuf);
            byte[] buf = s.getBytes();

            ByteBuffer b = ByteBuffer.wrap(buf, 0, buf.length);
            int res = outputChannel.write(b);
            if (res != buf.length) {
                System.out.println("Was able to write only: " + res);
            }

            // flush out the changes
            outputChannel.sync();

            outputChannel.close();

            System.out.println("Trying to lookup blocks for file: " + path);

            String [][] locs;
            if ((locs = kfsAccess.kfs_getDataLocation(path, 10, 512)) == null) {
                System.out.println("Get locs failed");
                System.exit(1);
            }
            
            System.out.println("Block Locations:");
            for (int i = 0; i < locs.length; i++) {
                System.out.print("chunk " + i + " : ");
                for (int j = 0; j < locs[i].length; j++) {
                    System.out.print(locs[i][j] + " ");
                }
                System.out.println();
            }

            long sz = kfsAccess.kfs_filesize(path);

            if (sz != buf.length) {
                System.out.println("System thinks the file's size is: " + sz);
            }

            // rename the file
            String npath = new String(basedir + "/foo.2");
            kfsAccess.kfs_rename(path, npath);

            if (kfsAccess.kfs_exists(path)) {
                System.out.println(path + " still exists after rename!");
                System.exit(1);
            }

            KfsOutputChannel outputChannel1 = kfsAccess.kfs_create(path);

            if (outputChannel1 != null) {
                outputChannel1.close();
            }

            if (!kfsAccess.kfs_exists(path)) {
                System.out.println(path + " doesn't exist");
                System.exit(1);
            }

            // try to rename and don't allow overwrite
            if (kfsAccess.kfs_rename(npath, path, false) == 0) {
                System.out.println("Rename with overwrite disabled succeeded!");
                System.exit(1);
            }

            kfsAccess.kfs_remove(path);

            if (!kfsAccess.kfs_isFile(npath)) {
                System.out.println(npath + " is not a normal file!");
                System.exit(1);
            }

            KfsInputChannel inputChannel = kfsAccess.kfs_open(npath);
            if (inputChannel == null) {
                System.out.println("open on " + npath + "failed!");
                System.exit(1);
            }
            
            // read some bytes
            buf = new byte[128];
            res = inputChannel.read(ByteBuffer.wrap(buf, 0, 128));

            s = new String(buf);
            for (int i = 0; i < 128; i++) {
                if (dataBuf[i] != s.charAt(i)) {
                    System.out.println("Data mismatch at char: " + i);
                }
            }
            
            // seek to offset 40
            inputChannel.seek(40);

            sz = inputChannel.tell();
            if (sz != 40) {
                System.out.println("After seek, we are at: " + sz);
            }

            inputChannel.close();

            // remove the file
            kfsAccess.kfs_remove(npath);

            // remove the dir
            if (kfsAccess.kfs_rmdir(basedir) < 0) {
                System.out.println("unable to remove: " + basedir);
                System.exit(1);
            }
            System.out.println("All done...Test passed!");

        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Unable to setup KfsAccess");
            System.exit(1);
        }
    }

    private static Random randGen = new Random(100);

    private static void generateData(char buf[], int numBytes)
    {
        int i;
        // String nameBuf = new String("sriram");
            
        for (i = 0; i < numBytes; i++) {
            buf[i] = (char) ('a' + (randGen.nextInt(26)));
            // buf[i] = nameBuf.charAt((i + 6) % 6);
        }
    }

}