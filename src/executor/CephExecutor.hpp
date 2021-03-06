/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CEPHMESOS_EXECUTOR_CEPHEXECUTOR_HPP_
#define _CEPHMESOS_EXECUTOR_CEPHEXECUTOR_HPP_

#include <mesos/executor.hpp>

using std::string;
using namespace mesos;

class CephExecutor : public Executor
{
public:
  CephExecutor(){}

  ~CephExecutor(){}

  void registered(
      ExecutorDriver* driver,
      const ExecutorInfo& executorInfo,
      const FrameworkInfo& frameworkInfo,
      const SlaveInfo& slaveInfo);

  void reregistered(ExecutorDriver* driver, const SlaveInfo& slaveInfo);

  void disconnected(ExecutorDriver* driver);

  void launchTask(ExecutorDriver* driver, const TaskInfo& task);

  void killTask(ExecutorDriver* driver, const TaskID& taskId);

  void frameworkMessage(ExecutorDriver* driver, const string& data);

  void shutdown(ExecutorDriver* driver);

  void error(ExecutorDriver* driver, const string& message);

private:
  string runShellCommand(string cmd);

  void deleteConfigDir(string localSharedConfigDir);

  bool existsConfigFiles(string localSharedConfigDir);

  bool createLocalSharedConfigDir(string localSharedConfigDir);

  bool copyWaitingNICEntryPoint(string localSharedConfigDir);

  bool copySharedConfigFiles(string localSharedConfigDir);

  string getContainerName(string taskId);

  string constructMonCommand(
      string localMountDir,
      string _containerName);

  string registerOSD();

  string constructOSDCommand(
      string localMountDir,
      string osdId,
      string _containerName);

  string constructRADOSGWCommand(
      string localMountDir,
      string _containerName);

  bool block_until_started(string _containerName, string timeout);

  bool createNICs(string _containerName, string physicalNIC, string containerNIC);

  void startLongRunning(string binaryName, string cmd);

  bool downloadDockerImage(string imageName);

  bool parseHostConfig(TaskInfo taskinfo);

  bool prepareDisks();

  bool partitionDisk(string diskname, int partitionCount);

  bool mkfsDisk(string diskname, string type, string flags);

  bool mountDisk(string diskname, string dir, string flags);

  string containerName;

  TaskID myTaskId;

  string myHostname;

  pid_t myPID;

  string localSharedConfigDirRoot;
  string localConfigDirName = "ceph_config_root";
  string sandboxAbsolutePath;
  string localMountOSDDir = "";
  //host hardware info
  string mgmtNIC;
  string dataNIC;
  string osddisk;
  string jnldisk;
  string fsType = "xfs";
  string mkfsFLAGS = " -f -i size=2048 -n size=64k";
  string mountFLAGS = " -o inode64,noatime,logbsize=256k";
};

#endif
