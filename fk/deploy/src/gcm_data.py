# -*- coding: utf-8 -*-

# 需要安装protobuf
import gen_py_from_pb
import sys
import os

gen_py_from_pb.gen_py_from_pb('../proto/')
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(__file__)), "./proto/"))

from google.protobuf import text_format
from gcm_pb2 import *
from loghelper import LogError

class GcmData:
    def __init__(self):
        self.host_map = {}
        self.agent_artifact = None
        self.template_artifact_map = {}
        self.artifact_map = {}
        self.artifact_group_map = {}
        self.deploy_info = DeployInfo()
        self.variable_map = {}
        self.wolrd_map = {}
        self.artifact_instances = []

    def init(self, conf_paths):
        for path in conf_paths:
            self._load_conf(path)

        # check syntax
        self._check_syntax()

    def _load_conf(self, conf_path):
        # parse hosts.conf
        host_cfg = HostCfg()
        host_path = os.path.join(conf_path, 'hosts.conf')
        self._load_proto(host_path, host_cfg)
        for host in host_cfg.hosts:
            self.host_map[host.name] = host

        # parse agent_artifacts.conf
        agent_artifact = AgentArtifact()
        agent_artifact_path = os.path.join(conf_path, 'agent_artifacts.conf')
        self._load_proto(agent_artifact_path, agent_artifact)
        self.agent_artifact = agent_artifact

        # parse artifact_templates.conf
        template_artifact = TemplateArtifact()
        artifact_templates_path = os.path.join(conf_path, 'artifact_templates.conf')
        self._load_proto(artifact_templates_path, template_artifact)
        for item in template_artifact.template_artifacts:
            self.template_artifact_map[item.template_name] = item

        # parse artifacts.conf
        normal_artifact = NormalArtifact()
        artifact_path = os.path.join(conf_path, 'artifacts.conf')
        self._load_proto(artifact_path, normal_artifact)
        for item in normal_artifact.artifacts:
            self.artifact_map[item.name] = item

        # parse artifact_groups.conf
        artifact_group = ArtifactGroup()
        artifact_groups_path = os.path.join(conf_path, 'artifact_groups.conf')
        self._load_proto(artifact_groups_path, artifact_group)
        for item in artifact_group.artifact_groups:
            self.artifact_group_map[item.name] = item

        # parse deploy.conf
        deploy_path = os.path.join(conf_path, 'deploy.conf')
        deploy = Deploy()
        self._load_proto(deploy_path, deploy)
        for var in deploy.variables:
            self.variable_map[var.key] = var.value

        # parse global const variable
        self.deploy_info.src_root_path = deploy.src_root_path
        self.deploy_info.tmp_root_path = deploy.tmp_root_path
        self.deploy_info.dst_root_path = deploy.dst_root_path

        for word in deploy.worlds:
            if word.name in self.wolrd_map:
                LogError('world name: %s is duplicated' % word.name)
                raise BaseException()
            else:
                self.wolrd_map[word.name] = word

    @staticmethod
    def _load_proto(file_path, proto):
        if not os.path.exists(file_path):
            return
        with open(file_path) as f:
            text_format.Parse(f.read(), proto)

    def _check_syntax(self):
        world_id_set = set()
        for world_name in self.wolrd_map:
            world = self.wolrd_map[world_name]
            if world.id in world_id_set:
                LogError('world id: %d is duplicated' % world.id)
                raise BaseException()
            world_id_set.add(world.id)
            zone_name_set = set()
            zone_id_set = set()
            for zone in world.zones:
                if zone.name in zone_name_set:
                    LogError('zone name: %s is duplicated in world name: %s' % (zone.name, world.name))
                    raise BaseException()
                if zone.id in zone_id_set:
                    LogError('zone id: %d is duplicated in world name: %s' % (zone.id, world.name))
                    raise BaseException()
                zone_name_set.add(zone.name)
                zone_id_set.add(zone.id)

                groups_set = []
                for groups in zone.instance_groups:
                    if [groups.artifact_group_name, groups.instance_id] in groups_set:
                        LogError('group_name: %s, instance_id: %d is duplicated in zone name: %s in world name: %s'
                                 % (groups.artifact_group_name, groups.instance_id, zone.name, world.name))
                        raise BaseException()
                    groups_set.append([groups.artifact_group_name, groups.instance_id])
