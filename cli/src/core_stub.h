/* 
 *  Copyright (C) 2021-2022 Intel Corporation
 *  SPDX-License-Identifier: MIT
 *  @file core_stub.h
 */

#pragma once

#include <grpc++/channel.h>

#include <chrono>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

#include "core.grpc.pb.h"
#include "xpum_structs.h"

namespace xpum::cli {

class CoreStub {
   public:
    CoreStub(bool priv);

    bool isChannelReady();

    std::unique_ptr<nlohmann::json> getVersion();

    std::unique_ptr<nlohmann::json> getDeviceList();

    std::unique_ptr<nlohmann::json> getDeviceProperties(int deviceId);

    std::unique_ptr<nlohmann::json> getAMCFirmwareVersions();

    std::unique_ptr<nlohmann::json> getTopology(int deviceId);

    std::unique_ptr<nlohmann::json> groupCreate(std::string groupName);
    std::unique_ptr<nlohmann::json> groupDelete(int groupId);
    std::unique_ptr<nlohmann::json> groupListAll();
    std::unique_ptr<nlohmann::json> groupList(int groupId);
    std::unique_ptr<nlohmann::json> groupAddDevice(int groupId, int deviceId);
    std::unique_ptr<nlohmann::json> groupRemoveDevice(int groupId, int deviceId);

    std::unique_ptr<nlohmann::json> runDiagnostics(int deviceId, int level, bool rawComponentTypeStr);
    std::unique_ptr<nlohmann::json> getDiagnosticsResult(int deviceId, bool rawComponentTypeStr);
    std::unique_ptr<nlohmann::json> runDiagnosticsByGroup(uint32_t groupId, int level, bool rawComponentTypeStr);
    std::unique_ptr<nlohmann::json> getDiagnosticsResultByGroup(uint32_t groupId, bool rawComponentTypeStr);
    std::string diagnosticResultEnumToString(DiagnosticsTaskResult result);
    std::string diagnosticTypeEnumToString(DiagnosticsComponentInfo_Type type, bool rawComponentTypeStr);

    std::unique_ptr<nlohmann::json> getAllHealth();
    std::unique_ptr<nlohmann::json> getHealth(int deviceId, int componentType);
    std::unique_ptr<nlohmann::json> getHealth(int deviceId, HealthType type);
    std::unique_ptr<nlohmann::json> getHealthByGroup(uint32_t groupId, int componentType);
    std::unique_ptr<nlohmann::json> getHealthByGroup(uint32_t groupId, HealthType type);
    std::unique_ptr<nlohmann::json> setHealthConfig(int deviceId, HealthConfigType cfgtype, int threshold);
    std::unique_ptr<nlohmann::json> setHealthConfigByGroup(uint32_t groupId, HealthConfigType cfgtype, int threshold);
    int getHealthConfig(int deviceId, HealthConfigType cfgtype);
    std::string healthStatusEnumToString(HealthStatusType status);
    std::string healthTypeEnumToString(HealthType type);
    nlohmann::json appendHealthThreshold(int deviceId, nlohmann::json, HealthType type, uint64_t throttleValue, uint64_t shutdownValue);

    std::unique_ptr<nlohmann::json> getStatistics(int deviceId, bool enableFilter = false, bool enableScale = false);
    std::unique_ptr<nlohmann::json> getStatisticsByGroup(uint32_t groupId, bool enableFilter = false, bool enableScale = false);
    std::shared_ptr<nlohmann::json> getEngineStatistics(int deviceId);
    std::shared_ptr<std::map<int, std::map<int, int>>> getEngineCount(int deviceId);
    std::shared_ptr<nlohmann::json> getFabricStatistics(int deviceId);
    //config related interface
    std::unique_ptr<nlohmann::json> getDeviceConfig(int deviceId, int tileId);
    std::unique_ptr<nlohmann::json> setDeviceSchedulerMode(int deviceId, int tileId, XpumSchedulerMode mode, int val1, int val2);
    std::unique_ptr<nlohmann::json> setDevicePowerlimit(int deviceId, int tileId, int power, int interval);
    std::unique_ptr<nlohmann::json> setDeviceStandby(int deviceId, int tileId, XpumStandbyMode mode);
    std::unique_ptr<nlohmann::json> setDeviceFrequencyRange(int deviceId, int tileId, int minFreq, int maxFreq);
    std::unique_ptr<nlohmann::json> getDeviceProcessState(int deviceId);
    std::unique_ptr<nlohmann::json> getDeviceUtilizationByProcess(int deviceId, int utilizationInterval);
    std::unique_ptr<nlohmann::json> getAllDeviceUtilizationByProcess(int utilizationInterval);
    std::unique_ptr<nlohmann::json> getPerformanceFactor(int deviceId, int tileId);
    std::unique_ptr<nlohmann::json> setPerformanceFactor(int deviceId, int tileId, xpum_engine_type_flags_t engine, double factor);
    std::unique_ptr<nlohmann::json> setFabricPortEnabled(int deviceId, int tileId, uint32_t port, uint32_t enabled);
    std::unique_ptr<nlohmann::json> setFabricPortBeaconing(int deviceId, int tileId, uint32_t port, uint32_t beaconing);
    std::unique_ptr<nlohmann::json> setMemoryEccState(int deviceId, bool enabled);
    std::unique_ptr<nlohmann::json> resetDevice(int deviceId, bool force);
    std::string schedulerModeEnumToString(XpumSchedulerMode mode);
    std::string standbyModeEnumToString(XpumStandbyMode mode);

    std::string policyTypeEnumToString(XpumPolicyType type);
    std::string policyConditionTypeEnumToString(XpumPolicyConditionType type);
    std::string policyActionTypeEnumToString(XpumPolicyActionType type);
    std::unique_ptr<nlohmann::json> getAllPolicyType();
    std::unique_ptr<nlohmann::json> getAllPolicyConditionType();
    std::unique_ptr<nlohmann::json> getAllPolicyActionType();
    std::unique_ptr<nlohmann::json> getAllPolicy();
    std::unique_ptr<nlohmann::json> getPolicyById(bool isDevice, uint32_t id);
    std::unique_ptr<nlohmann::json> getPolicy(bool isDevcie, uint32_t id);
    std::unique_ptr<nlohmann::json> setPolicy(bool isDevcie, uint32_t id, XpumPolicyData& policy);
    bool isCliSupportedPolicyType(XpumPolicyType type);

    std::unique_ptr<nlohmann::json> runFirmwareFlash(int deviceId, unsigned int type, const std::string& filePath);
    std::unique_ptr<nlohmann::json> getFirmwareFlashResult(int deviceId, unsigned int type);

    std::unique_ptr<nlohmann::json> startDumpRawDataTask(uint32_t deviceId, int tileId, std::vector<xpum_dump_type_t> metricsTypeList);
    std::unique_ptr<nlohmann::json> stopDumpRawDataTask(int taskId);
    std::unique_ptr<nlohmann::json> listDumpRawDataTasks();

    static std::string isotimestamp(uint64_t t);

    static std::string metricsTypeToString(xpum_stats_type_t metricsType);

    std::unique_ptr<nlohmann::json> setAgentConfig(std::string key, void* pValue);

    std::unique_ptr<nlohmann::json> getAgentConfig();

    std::string getTopoXMLBuffer();

    std::unique_ptr<nlohmann::json> getXelinkTopology();

    std::shared_ptr<nlohmann::json> getFabricCount(int deviceId);

   private:
    std::unique_ptr<XpumCoreService::Stub> stub;

    std::shared_ptr<grpc::Channel> channel;

    std::string getCardUUID(const std::string& rawUUID);
};
} // end namespace xpum::cli
