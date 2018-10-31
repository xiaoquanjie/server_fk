# -*- coding: utf-8 -*-

import paramiko
from loghelper import LogInfo
from loghelper import LogError
import sys

paramiko.util.log_to_file('./ssh.log', paramiko.common.ERROR)


class ShowProcess():
    # i = 0  # 当前的处理进度
    # max_steps = 0  # 总共需要处理的次数
    # max_arrow = 50  # 进度条的长度
    # infoDone = 'done'

    def __init__(self, max_steps, infoDone = 'Done'):
        self.max_arrow = 50
        self.max_steps = max_steps
        self.i = 0
        self.infoDone = infoDone

    def show_process(self, i=None):
        if i is not None:
            self.i = i
        else:
            self.i += 1
        num_arrow = int(self.i * self.max_arrow / self.max_steps)  # 计算显示多少个'>'
        num_line = self.max_arrow - num_arrow  # 计算显示多少个'-'
        percent = self.i * 100.0 / self.max_steps  # 计算完成进度，格式为xx.xx%
        process_bar = '[' + '>' * num_arrow + '-' * num_line + ']'\
                      + '%.2f' % percent + '%' + '\r'  # 带输出的字符串，'\r'表示不换行回到最左边
        sys.stdout.write(process_bar)  # 这两句打印字符到终端
        sys.stdout.flush()
        if self.i >= self.max_steps:
            self.close()

    def close(self):
        print('')
        print(self.infoDone)
        self.i = 0


def ssh(ip, user, passwd, cmd):
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(ip, 22, user, passwd, timeout=10)
    stdin, stdout, stderr = client.exec_command(cmd)
    LogInfo(cmd)
    LogInfo(stdout.read())
    LogError(stderr.read())
    client.close()


def upload_proccess(pro):
    def proccess(transferred, total_bytes):
        pro.max_steps = total_bytes
        pro.i = transferred
        pro.show_process()
    return proccess


def ssh_upload(ip, user, passwd, src_file, dst_file):
    try:
        proccess = ShowProcess(100)
        transport = paramiko.Transport((ip, 22))
        transport.connect(username=user, password=passwd)
        sftp = paramiko.SFTPClient.from_transport(transport)
        sftp.put(src_file, dst_file, callback=upload_proccess(proccess))
        sftp.close()
        LogInfo('success to upload %s to %s@%s:%s' % (src_file, user, ip, dst_file))
    except BaseException as err:
        LogError('fail to upload %s to %s@%s:%s' % (src_file, user, ip, dst_file))
        raise err





