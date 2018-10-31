# -*- coding: utf-8 -*-

import tarfile
import os


def tar(tarfilename, directory):
    with tarfile.open(tarfilename, "w:gz") as tarObj:
        tarObj.add(directory, arcname=os.path.basename(directory))
