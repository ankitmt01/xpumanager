/* 
 *  Copyright (C) 2021-2022 Intel Corporation
 *  SPDX-License-Identifier: MIT
 *  @file comlet_statistics.cpp
 */

#include "comlet_statistics.h"

#include <map>
#include <nlohmann/json.hpp>

#include "cli_table.h"
#include "core_stub.h"

namespace xpum::cli {

static CharTableConfig ComletConfigDeviceStatistics(R"({
    "showTitleRow": false,
    "columns": [{
        "title": "none",
        "size": 26
    }, {
        "title": "none"
    }],
    "rows": [{
        "instance": "",
        "cells": [
            { "rowTitle": "Device ID" },
            "device_id"
        ]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Start Time" },
            { "rowTitle": "End Time" },
            { "rowTitle": "Elapsed Time (Second) " },
            { "rowTitle": "Energy Consumed (J) " },
            { "rowTitle": "GPU Utilization (%) " },
            { "rowTitle": "EU Array Active (%) " },
            { "rowTitle": "EU Array Stall (%) " },
            { "rowTitle": "EU Array Idle (%) " }
        ], [
            { "value": "begin" },
            { "value": "end" },
            { "value": "elapsed_time" },
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_ENERGY].value", "scale": 1 }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_GPU_UTILIZATION].avg", "fixer": "round" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_EU_ACTIVE].avg", "scale": 1 }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_EU_STALL].avg", "scale": 1 }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_EU_IDLE].avg", "scale": 1 }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Reset" },
            { "rowTitle": "Programming Errors" },
            { "rowTitle": "Driver Errors" },
            { "rowTitle": "Cache Errors Correctable" },
            { "rowTitle": "Cache Errors Uncorrectable" },
            { "rowTitle": "Mem Errors Correctable" },
            { "rowTitle": "Mem Errors Uncorrectable" }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_RESET].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_RESET].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_PROGRAMMING_ERRORS].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_PROGRAMMING_ERRORS].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_DRIVER_ERRORS].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_DRIVER_ERRORS].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_CACHE_ERRORS_CORRECTABLE].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_CACHE_ERRORS_CORRECTABLE].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_CACHE_ERRORS_UNCORRECTABLE].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_CACHE_ERRORS_UNCORRECTABLE].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_NON_COMPUTE_ERRORS_CORRECTABLE].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_NON_COMPUTE_ERRORS_CORRECTABLE].total" }
            ]},
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subs": [
                { "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_NON_COMPUTE_ERRORS_UNCORRECTABLE].value" },
                { "label": "total", "value": "data_list[metrics_type==XPUM_STATS_RAS_ERROR_CAT_NON_COMPUTE_ERRORS_UNCORRECTABLE].total" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Power (W) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_POWER].avg", "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_POWER].min", "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_POWER].max", "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_POWER].value", "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Frequency (MHz) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_GPU_FREQUENCY].avg" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_GPU_FREQUENCY].min" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_GPU_FREQUENCY].max" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_GPU_FREQUENCY].value" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Core Temperature\n(Celsius Degree) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_GPU_CORE_TEMPERATURE].avg", "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_GPU_CORE_TEMPERATURE].min", "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_GPU_CORE_TEMPERATURE].max", "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_GPU_CORE_TEMPERATURE].value", "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Memory Temperature\n(Celsius Degree) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_TEMPERATURE].avg", "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_TEMPERATURE].min", "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_TEMPERATURE].max", "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_TEMPERATURE].value", "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Memory Read (kB/s) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_READ_THROUGHPUT].avg", "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_READ_THROUGHPUT].min", "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_READ_THROUGHPUT].max", "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_READ_THROUGHPUT].value", "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Memory Write (kB/s) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_WRITE_THROUGHPUT].avg", "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_WRITE_THROUGHPUT].min", "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_WRITE_THROUGHPUT].max", "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_WRITE_THROUGHPUT].value", "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Memory Bandwidth (%) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_BANDWIDTH].avg" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_BANDWIDTH].min" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_BANDWIDTH].max" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_BANDWIDTH].value" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "GPU Memory Used (MiB) " }
        ], [
            { "label": "Tile ", "label_tag": "tile_id", "value": "tile_level[]", "subrow": true, "subs": [
                { "label": "avg", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_USED].avg", "scale": 1, "fixer": "round" },
                { "label": "min", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_USED].min", "scale": 1, "fixer": "round" },
                { "label": "max", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_USED].max", "scale": 1, "fixer": "round" },
                { "label": "current", "value": "data_list[metrics_type==XPUM_STATS_MEMORY_USED].value", "scale": 1, "fixer": "round" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "PCIe Read (kB/s) " }
        ], [
            { "value": "", "subs": [
                { "label": "avg", "value": "device_level[metrics_type==XPUM_STATS_PCIE_READ_THROUGHPUT].avg" },
                { "label": "min", "value": "device_level[metrics_type==XPUM_STATS_PCIE_READ_THROUGHPUT].min" },
                { "label": "max", "value": "device_level[metrics_type==XPUM_STATS_PCIE_READ_THROUGHPUT].max" },
                { "label": "current", "value": "device_level[metrics_type==XPUM_STATS_PCIE_READ_THROUGHPUT].value" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "PCIe Write (kB/s) " }
        ], [
            { "value": "", "subs": [
                { "label": "avg", "value": "device_level[metrics_type==XPUM_STATS_PCIE_WRITE_THROUGHPUT].avg" },
                { "label": "min", "value": "device_level[metrics_type==XPUM_STATS_PCIE_WRITE_THROUGHPUT].min" },
                { "label": "max", "value": "device_level[metrics_type==XPUM_STATS_PCIE_WRITE_THROUGHPUT].max" },
                { "label": "current", "value": "device_level[metrics_type==XPUM_STATS_PCIE_WRITE_THROUGHPUT].value" }
            ]}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Compute Engine Util (%) " }
        ], [
            { "value": "compute_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Render Engine Util (%) " }
        ], [
            { "value": "render_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Decoder Engine Util (%) " }
        ], [
            { "value": "decoder_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Encoder Engine Util (%) " }
        ], [
            { "value": "encoder_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Copy Engine Util (%) " }
        ], [
            { "value": "copy_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Media EM Engine Util (%) " }
        ], [
            { "value": "media_em_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "3D Engine Util (%) " }
        ], [
            { "value": "3d_engine_util"}
        ]]
    }, {
        "instance": "",
        "cells": [[
            { "rowTitle": "Xe Link Throughput (kB/s) " }
        ], [
            { "value": "fabric_throughput"}
        ]]
    }]
})"_json);

void ComletStatistics::setupOptions() {
    this->opts = std::unique_ptr<ComletStatisticsOptions>(new ComletStatisticsOptions());
    addOption("-d,--device", this->opts->deviceId, "The device ID to query");
    addOption("-g,--group", this->opts->groupId, "The group ID to query");
}

std::unique_ptr<nlohmann::json> ComletStatistics::run() {
    if (isDeviceOp()) {
        auto json = this->coreStub->getStatistics(this->opts->deviceId, true, true);
        return json;
    } else if (isGroupOp()) {
        auto json = this->coreStub->getStatisticsByGroup(this->opts->groupId, true, true);
        return json;
    }
    auto json = std::unique_ptr<nlohmann::json>(new nlohmann::json());
    (*json)["error"] = "Unknow operation";
    return json;
}

std::string engineUtilFormater(nlohmann::json json, bool intent = false) {
    std::string intent_str = intent ? std::string(2, ' ') : "";
    std::string tmp;
    std::string res;
    int i = 0;
    auto func = [](nlohmann::json obj1, nlohmann::json obj2) {
        return obj1["engine_id"] <= obj2["engine_id"];
    };
    std::sort(json.begin(), json.end(), func);
    for (auto obj : json) {
        if (tmp.empty()) tmp = intent_str;
        tmp += "Engine " + std::to_string(obj["engine_id"].get<int>()) + ": " + std::to_string(obj["avg"].get<int>());
        i++;
        if (i % 4 == 0) {
            res += tmp + "\n";
            tmp.clear();
        } else {
            tmp += ", ";
        }
    }
    if (!tmp.empty()) {
        res += tmp;
        res.pop_back();
        res.pop_back();
    } else if (i > 0) {
        res.pop_back();
    }
    return res;
}

std::string engineUtilByType(std::shared_ptr<nlohmann::json> jsonPtr, std::string key) {
    std::string res;

    // device level
    if (jsonPtr->contains("engine_util") && (*jsonPtr)["engine_util"].contains(key)) {
        auto jsonObj = (*jsonPtr)["engine_util"][key];
        res += engineUtilFormater(jsonObj) + "\n";
    }
    // tile level
    if (jsonPtr->contains("tile_level")) {
        for (auto tileJson : (*jsonPtr)["tile_level"]) {
            if (tileJson.contains("engine_util") && tileJson["engine_util"].contains(key)) {
                auto jsonObj = tileJson["engine_util"][key];
                auto engineStr = engineUtilFormater(jsonObj, true);
                if (!engineStr.empty()) {
                    res += "Tile " + std::to_string(tileJson["tile_id"].get<int>()) + ":\n";
                    res += engineUtilFormater(jsonObj, true) + "\n";
                }
            }
        }
    }
    if (!res.empty()) res.pop_back();
    return res;
}

std::string getXelinkThroughput(std::shared_ptr<nlohmann::json> jsonPtr) {
    std::string res;
    if (!jsonPtr->contains("fabric_throughput"))
        return res;
    std::stringstream ss;
    for (auto& obj : (*jsonPtr)["fabric_throughput"]) {
        ss.str("");
        auto key = obj["name"].get<std::string>();
        int i = key.find("->");
        key.insert(i + 2, " ");
        key.insert(i, " ");
        ss << key << ": ";
        ss << "avg: " << obj["avg"] << ", ";
        ss << "min: " << obj["min"] << ", ";
        ss << "max: " << obj["max"] << ", ";
        ss << "current: " << obj["value"];
        res += ss.str() + "\n";
    }
    if (!res.empty()) res.pop_back();
    return res;
}

static void showDeviceStatistics(std::ostream& out, std::shared_ptr<nlohmann::json> jsonPtr, const bool cont = false) {
    nlohmann::json& json = *jsonPtr;
    bool noTile = true;
    auto tileJson = json.find("tile_level");
    if (tileJson != json.end()) {
        if (tileJson->is_array() && tileJson->size() >= 2) {
            noTile = false;
        }
    }

    if (noTile) {
        auto deviceJson = json.find("device_level");
        if (deviceJson != json.end() && deviceJson->is_array()) {
            if (tileJson != json.end()) {
                json.erase(tileJson);
            }
            json["tile_level"] = nlohmann::json::array();
            tileJson = json.find("tile_level");
            auto tile0 = nlohmann::json::object();
            tile0["tile_id"] = 0;
            tile0["data_list"] = (*deviceJson);
            tileJson->push_back(tile0);
        }
    }

    json["compute_engine_util"] = engineUtilByType(jsonPtr, "compute");
    json["render_engine_util"] = engineUtilByType(jsonPtr, "render");
    json["decoder_engine_util"] = engineUtilByType(jsonPtr, "decoder");
    json["encoder_engine_util"] = engineUtilByType(jsonPtr, "encoder");
    json["copy_engine_util"] = engineUtilByType(jsonPtr, "copy");
    json["media_em_engine_util"] = engineUtilByType(jsonPtr, "media_enhancement");
    json["3d_engine_util"] = engineUtilByType(jsonPtr, "3d");

    json["fabric_throughput"] = getXelinkThroughput(jsonPtr);

    CharTable table(ComletConfigDeviceStatistics, json, cont);
    table.show(out);
}

void ComletStatistics::getTableResult(std::ostream& out) {
    auto res = run();
    if (res->contains("error")) {
        out << "Error: " << (*res)["error"].get<std::string>() << std::endl;
        return;
    }
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    *json = *res;
    if (this->opts->groupId != 0) {
        auto devices = (*json)["datas"].get<std::vector<nlohmann::json>>();
        bool cont = false;
        for (auto device : devices) {
            showDeviceStatistics(out, std::make_shared<nlohmann::json>(device), cont);
            cont = true;
        }
    } else {
        showDeviceStatistics(out, json);
    }
}

} // end namespace xpum::cli
