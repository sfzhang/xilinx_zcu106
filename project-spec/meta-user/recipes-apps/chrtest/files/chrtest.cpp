/*
* Placeholder PetaLinux user application.
*
* Replace this with your application code

* Copyright (C) 2013 - 2016  Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in this
* Software without prior written authorization from Xilinx.
*
*/

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Hello, PetaLinux World!\n";

    int fd = open("/dev/chrdev", O_RDWR);
    if (fd < 0) {
        cout << "open() failed: errno " << errno << endl;
        return -1;
    }

    cout << "open /dev/chrdev success" << endl;

    char buffer[32] = {0};
    size_t count = sizeof(buffer) - 1;
    auto ret = read(fd, buffer, count);
    if (ret < 0) {
        cout << "read() failed: errno " << errno << endl;
        return -1;
    }

    cout << "read: " << buffer << endl;

    const char *str = "*** Hello, char device ***";
    ret = write(fd, str, strlen(str));
    if (ret < 0) {
        cout << "write() failed: errno " << errno << endl;
        return -1;
    }

    cout << "write: " << str << endl;

    memset(buffer, 0, sizeof(buffer));
    ret = read(fd, buffer, count);
    if (ret < 0) {
        cout << "read() failed: errno " << errno << endl;
        return -1;
    }

    cout << "read: " << buffer << endl;

    ret = close(fd);
    if (ret != 0) {
        cout << "close() failed: errno " << errno << endl;
        return -1;
    }

    cout << "close() success!" << endl;

	return 0;
}


