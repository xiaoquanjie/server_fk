# -*- coding: utf-8 -*-
import os
import sys
import time
from singleton_instance import SingletonInstance

if __name__ == '__main__':
    singleton = SingletonInstance()
    if singleton.already_running():
        print('Error: A Deploy Program is already running, please try it later')
        sys.exit(-1)

    print('begin proccess')
    time.sleep(100)
