#!/bin/python3
import sys
import os

if len(sys.argv) == 3:
    print(os.path.getsize(sys.argv[1]) / os.path.getsize(sys.argv[2]))
else:
    print("Usage:\n"
          "\tcompare-size [file1] [file2]\n"
          "\treturn sizeof(file1)/sizeof(file2)")
