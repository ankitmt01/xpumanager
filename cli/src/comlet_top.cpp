/* 
 *  Copyright (C) 2022 Intel Corporation
 *  SPDX-License-Identifier: MIT
 *  @file comlet_top.cpp
 */

#include "comlet_top.h"

#include <map>
#include <nlohmann/json.hpp>

#include "core_stub.h"
#include "cli_table.h"

namespace xpum::cli {

ComletTop::ComletTop() : ComletBase("top", 
    "List GPU engine utilization per process.") {
}

void ComletTop::setupOptions() {
    this->opts = std::unique_ptr<ComletTopOptions>(new ComletTopOptions());
    addOption("-d,--device", this->opts->deviceId, 
            "To specify a device ID to query.");
}

std::unique_ptr<nlohmann::json> ComletTop::run() {
    std::unique_ptr<nlohmann::json> json;
    if (this->opts->deviceId == -1) {
        json = this->coreStub->getAllDeviceUtilizationByProcess(200 * 1000);
    } else {
        json = this->coreStub->getDeviceUtilizationByProcess(
            this->opts->deviceId, 200 * 1000);
    }
    return json;
}

void ComletTop::getTableResult(std::ostream &out) {
    auto res = run();
    if (res->contains("error")) {
        out << "Error: " << (*res)["error"].get<std::string>() << std::endl;
        return;
    }
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    *json = *res;
    std::cout 
        << std::left << std::setfill(' ') 
        << std::setw(10) << "PID" 
        << std::setw(19) << "Command"
        << std::setw(11) << "DeviceID"
        << std::setw(7) << "%REN"
        << std::setw(7) << "%COM"
        << std::setw(7) << "%CPY"
        << std::setw(7) << "%MED"
        << std::setw(7) << "%MEE"
        << std::setw(10) << "SHR"
        << std::setw(10) << "MEM"
        << std::endl;
    for (auto iter = (*json)["device_util_by_proc_list"].begin(); 
            iter != (*json)["device_util_by_proc_list"].end(); iter++) {
        std::cout 
            << std::left << std::setfill(' ') 
            << std::setw(10) << (*iter)["process_id"].get<uint32_t>()
            << std::setw(19) << (*iter)["process_name"].get<std::string>()
            << std::setw(11) << (*iter)["device_id"].get<uint32_t>()
            << std::setprecision(4)
            << std::setw(7) << rnd_2(
                    (*iter)["rendering_engine_util"].get<double>())
            << std::setw(7) << rnd_2(
                    (*iter)["compute_engine_util"].get<double>())
            << std::setw(7) << rnd_2(
                    (*iter)["copy_engine_util"].get<double>()) 
            << std::setw(7) << rnd_2(
                    (*iter)["media_engine_util"].get<double>()) 
            << std::setw(7) << rnd_2(
                    (*iter)["media_enhancement_util"].get<double>()) 
            << std::setw(10) << (*iter)["shared_mem_size"].get<uint64_t>() 
            << std::setw(10) << (*iter)["mem_size"].get<uint64_t>() 
            << std::endl;
    } 
}

} // end namespace xpum::cli
