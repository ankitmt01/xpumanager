/* 
 *  Copyright (C) 2021-2022 Intel Corporation
 *  SPDX-License-Identifier: MIT
 *  @file utility.h
 */

#pragma once

#include <string>

#include "../include/xpum_structs.h"
#include "const.h"
#include "device/device.h"
#include "device_capability.h"
#include "measurement_type.h"

namespace xpum {

class Utility {
   public:
    static long long getCurrentMillisecond();

    static Timestamp_t getCurrentTime();

    static std::string getCurrentTimeString();

    static std::string getCurrentUTCTimeString();

    static std::string getTimeString(long long milliseconds);

    static std::string getUTCTimeString(uint64_t milliseconds);

    static MeasurementType measurementTypeFromCapability(DeviceCapability& capability);

    static DeviceCapability capabilityFromMeasurementType(const MeasurementType& measurementType);

    static xpum_stats_type_t xpumStatsTypeFromMeasurementType(MeasurementType& MeasurementType);

    static MeasurementType measurementTypeFromXpumStatsType(xpum_stats_type_t& xpum_stats_type);

    static bool isMetric(MeasurementType type);

    static bool isCounterMetric(MeasurementType type);

    static void getMetricsTypes(std::vector<MeasurementType>& metrics);

    static std::string getXpumStatsTypeString(MeasurementType type);

    static xpum_engine_type_t toXPUMEngineType(zes_engine_group_t type);

    static zes_engine_group_t toZESEngineType(xpum_engine_type_t type);

    static xpum_fabric_throughput_type_t toXPUMFabricThroughputType(FabricThroughputType type);

    static bool isATSPlatform(std::string device_name);

    static bool isATSM1(std::string device_name);

    static bool isATSM3(std::string device_name);
};

} // end namespace xpum
