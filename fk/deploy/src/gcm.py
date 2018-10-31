# -*- coding: utf-8 -*-

from loghelper import LogInfo
from loghelper import LogError
from gcm_data import GcmData
import re
import os
import shutil
import tar
import tarfile

class Gcm:
    def __init__(self, user, password, conf_paths, src_path):
        self.user = user
        self.password = password
        self.conf_paths = conf_paths
        self.src_path = src_path
        self.gcm_data = GcmData()
        self.gcm_data.init(self.conf_paths)

    def push_agent(self, pattern):
        LogInfo('\n\n==================================== push_agent %s ====================================' % pattern)
        self.do_cmd('push_agent', pattern)

    def start_agent(self, pattern):
        LogInfo('\n\n==================================== start_agent %s ====================================' % pattern)
        self.do_cmd('start_agent', pattern)

    def start(self, pattern):
        LogInfo('\n\n==================================== start %s ====================================' % pattern)
        self.do_cmd('start', pattern)

    def stop(self, pattern):
        LogInfo('\n\n==================================== stop %s ====================================' % pattern)
        self.do_cmd('stop', pattern)

    def restart(self, pattern):
        self.stop(pattern)
        self.start(pattern)

    def update(self, pattern):
        self.stop(pattern)
        self.push(pattern)
        self.start(pattern)

    def check(self, pattern):
        LogInfo('\n\n==================================== check %s ====================================' % pattern)
        self.do_cmd("check", pattern)

    def push(self, pattern):
        LogInfo('\n\n==================================== push %s ====================================' % pattern)
        self.do_cmd("push", pattern)

    def reload(self, pattern):
        LogInfo('\n\n==================================== reload %s ====================================' % pattern)
        self.do_cmd("reload", pattern)

    def clean(self, pattern):
        LogInfo('\n\n==================================== clean %s ====================================' % pattern)
        self.do_cmd('clean', pattern)

    def do_cmd(self, cmd, pattern):
        instance_list = self._get_instance(pattern)
        if not instance_list:
            LogError('find no instance in pattern: %s' % pattern)
            return

        host_set = set()
        for instance in instance_list:
            host_set.add(instance.deploy_ip)

        # pack file
        self._pack_files(instance_list)

    @staticmethod
    def _regex_pattern(pattern):
        return "^" + "\\.".join([s if s != "*" else ".+" for s in pattern.split(".")]) + "$"

    def _get_instance(self, pattern):
        def Priority(instance):
            return instance.start_priority

        regex_pattern = self._regex_pattern(pattern)
        instance_list = []
        for instance in self.gcm_data.artifact_instances:
            if not re.match(regex_pattern, instance.instance_name):
                continue
            instance_list.append(instance)

        instance_list.sort(key=Priority, reverse=True)
        return instance_list

    def _pack_files(self, instance_list):
        LogInfo('\n\n==================================== collecting file or directory ====================================')
        tmp_path = self.gcm_data.deploy_info.tmp_root_path
        repo_path = os.path.join(tmp_path, 'repo.tar.gz')
        if not os.path.exists(self.src_path):
            LogError('src_path: %s is not exist' % self.src_path)
            raise BaseException()
        if not os.path.exists(tmp_path):
            os.mkdir(tmp_path)

        tar_file = tarfile.open(repo_path, "w:gz")
        cwd = os.getcwd()
        os.chdir(self.src_path)
        for instance in instance_list:
            artifacts = self.gcm_data.artifact_map[instance.artifact_name]
            for f in artifacts.files:
                LogInfo(f.src)
                tar_file.add(f.src)

        tar_file.close()
        os.chdir(cwd)
        LogInfo('tar.gz file: %s' % repo_path)
        return repo_path
