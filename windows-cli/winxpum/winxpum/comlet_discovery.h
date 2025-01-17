/*
 *  Copyright (C) 2021-2022 Intel Corporation
 *  SPDX-License-Identifier: MIT
 *  @file comlet_discovery.h
 */

#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "comlet_base.h"


struct ComletDiscoveryOptions {
    int deviceId = -1;
    int a = 0;
};

class ComletDiscovery : public ComletBase {
public:
    ComletDiscovery();
    virtual ~ComletDiscovery() {}

    virtual void setupOptions() override;
    virtual std::unique_ptr<nlohmann::json> run() override;

    virtual void getTableResult(std::ostream& out) override;

    inline bool isDeviceList() {
        return opts->deviceId < 0;
    }

private:
    std::unique_ptr<ComletDiscoveryOptions> opts;
};
