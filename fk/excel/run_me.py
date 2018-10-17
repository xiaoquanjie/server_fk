#! /usr/bin/env python
# coding=utf-8

import os
import platform

def generator_build_file():
    file_str = "cc_library(\n"
    file_str += "\tname = 'excelconf',\n"
    file_str += "\tsrcs = [\n"

    files = os.listdir('./src/')
    for f in files:
        if not f.endswith('.cc'):
            continue
        file_str += "\t\t'" + "src/" + f + "',\n"

    file_str += "\t],\n"
    file_str += "\tdeps = [\n"
    file_str += "\t\t'//google:protobuf',\n"
    file_str += "\t],\n"
    file_str += ")"

    f = open('BUILD', mode='w')
    f.write(file_str)

if __name__ == '__main__':
    dir_path = './excel/'
    files = os.listdir(dir_path)
    for f in files:
        cmd = 'python xls_translator.py Data ' + dir_path + f
        os.system(cmd)

    if not os.path.exists('./src'):
        os.makedirs('./src/')
    files = os.listdir('./proto/')
    for f in files:
        if not f.endswith('.proto'):
            continue

        command = None
        if platform.system() == 'Windows':
            command = "..\\bin\\protoc.exe --proto_path=./proto --cpp_out=./src/ " + f
        else:
            command = "cd proto; protoc --cpp_out=../src/ " + f
        os.system(command)

    generator_build_file()

