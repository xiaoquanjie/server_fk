#! /usr/bin/env python
# coding=utf-8

import os

if __name__ == '__main__':
    dir_path = './excel/'
    files = os.listdir(dir_path)
    for f in files:
        cmd = 'python xls_translator.py Data ' + dir_path + f
        os.system(cmd)

