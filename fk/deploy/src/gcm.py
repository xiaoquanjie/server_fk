# -*- coding: utf-8 -*-

from loghelper import LogInfo
from gcm_data import GcmData
import re

class Gcm:
    def __init__(self, user, password, conf_paths, temp_path):
        self.user = user
        self.password = password
        self.conf_paths = conf_paths
        self.temp_path = temp_path
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
        for instance in instance_list:
            LogInfo(instance.instance_name)

    @staticmethod
    def _regex_pattern(pattern):
        return "^" + "\\.".join([s if s != "*" else ".+" for s in pattern.split(".")]) + "$"

    def _get_instance(self, pattern):
        def Priority(instance):
            LogInfo('priotiry: %d' % instance.start_priority)
            return instance.start_priority

        regex_pattern = self._regex_pattern(pattern)
        instance_list = []
        for instance in self.gcm_data.artifact_instances:
            if not re.match(regex_pattern, instance.instance_name):
                continue
            instance_list.append(instance)

        instance_list.sort(key=Priority, reverse=True)
        return instance_list
