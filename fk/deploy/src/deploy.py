# -*- coding: utf-8 -*-
import sys
import os
from singleton_instance import SingletonInstance
import argparse
from loghelper import LogError
from loghelper import LogInfo
from gcm import Gcm


def CheckDeployRootEXist(dir_path, deploy_name):
    files = os.listdir(dir_path)
    for file in files:
        if file == deploy_name:
            return True

    return False


if __name__ == '__main__':
    singleton = SingletonInstance()
    if singleton.already_running():
        LogError('Error: A Deploy Program is already running, please try it later')
        sys.exit(-1)

    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument("cmd", help='push_agent|start_agent|start|stop|restart|update|check|push|reload|clean')
    arg_parser.add_argument('pattern', help='*.*.*.* or *.*')
    arg_parser.add_argument('-u', '--user', help='target machine username')
    arg_parser.add_argument('-p', '--password', help='target machine password')
    arg_parser.add_argument('-c', '--conf', nargs='+', help='config path')
    args = arg_parser.parse_args()

    pattern_splice = args.pattern.split('.')
    if len(pattern_splice) not in [2, 4]:
        LogError('Use instance pattern like *.*.*.* or *.*')
        sys.exit(-1)

    if len(pattern_splice) == 2:
        args.pattern = "%s.*.%s.*" % (pattern_splice[0], pattern_splice[1])

    if not args.conf:
        LogError("conf parameter can't be empty")
        sys.exit(-1)
    else:
        for conf in args.conf:
            if not os.path.isdir(conf):
                LogError('%s is not dir' % conf)
                sys.exit(-1)

    # 查找出DEPLOY_ROOT文件目录
    exec_path = os.path.split(os.path.realpath(__file__))[0]
    exe_dir = os.path.dirname(exec_path)
    deploy_root = None
    while True:
        if CheckDeployRootEXist(exe_dir, 'DEPLOY_ROOT'):
            deploy_root = exe_dir
            break
        new_exe_dir = os.path.dirname(exe_dir)
        if new_exe_dir == exe_dir:
            break
        exe_dir = new_exe_dir

    if not deploy_root:
        LogError("can't find the DEPLOY_ROOT file")
        sys.exit(-1)

    LogInfo('DEPLOY_ROOT file in: %s' % deploy_root)

    try:
        gcm = Gcm(args.user, args.password, args.conf, deploy_root)
        if args.cmd == 'push_agent':
            gcm.push_agent(args.pattern)
        elif args.cmd == 'start_agent':
            gcm.start_agent(args.pattern)
        elif args.cmd == 'start':
            gcm.start(args.pattern)
        elif args.cmd == 'stop':
            gcm.stop(args.pattern)
        elif args.cmd == 'restart':
            gcm.restart(args.pattern)
        elif args.cmd == 'update':
            gcm.update(args.pattern)
        elif args.cmd == 'check':
            gcm.check(args.pattern)
        elif args.cmd == 'push':
            gcm.push(args.pattern)
        elif args.cmd == 'reload':
            gcm.reload(args.pattern)
        elif args.cmd == 'clean':
            gcm.clean(args.pattern)
        else:
            LogError('illegal cmd: %s' % args.cmd)

    #except BaseException as err:
        #LogError(err)
    finally:
        pass


