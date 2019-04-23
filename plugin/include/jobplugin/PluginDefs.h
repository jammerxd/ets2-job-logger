/****************************************************************************
 * Copyright (c) 2019 Tomi Lähteenmäki <lihis@lihis.net>                    *
 *                                                                          *
 * This program is free software; you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,               *
 * MA 02110-1301, USA.                                                      *
 ****************************************************************************/

/**
 * @file PluginDefs.h
 * @brief Shared header for plugin and application
 * @details Definition of data transported via WebSocket.
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#ifndef ETS2_JOB_LOGGER_PLUGINDEFS_H
#define ETS2_JOB_LOGGER_PLUGINDEFS_H

#include "jobplugin/Version.h"
#include <string>
#include <msgpack.hpp>
#ifndef PLUGIN_INTERNAL
#include <json/value.h>
#endif

#define WEBSOCK_PORT 20210

namespace JobPlugin {
    enum PacketType {
        Unknown = 0,
        Version = 1,
        Job = 2,
        JobPartial = 3,
        Truck = 4
    };
} // namespace Logger
MSGPACK_ADD_ENUM(JobPlugin::PacketType)

struct version_t {
    version_t(uint8_t major = 0, uint8_t minor = 0, uint8_t patch = 0) {
        this->major = 0;
        this->minor = 0;
        this->patch = 0;
    }

    uint8_t major;
    uint8_t minor;
    uint8_t patch;

    MSGPACK_DEFINE(major, minor, patch);
};

struct truck_t {
    truck_t() {
        odometer = 0.f;
        fuel = 0.f;
    }

    float odometer;
    float fuel;

    MSGPACK_DEFINE(odometer, fuel);
};

struct job_partial_t {
    explicit job_partial_t(float drivenKm = 0.f,
                           float fuelConsumed = 0.f,
                           float trailerDamage = 0.f) {
        this->drivenKm = drivenKm;
        this->fuelConsumed = fuelConsumed;
        this->trailerDamage = trailerDamage;
    }

    float drivenKm;
    float fuelConsumed;
    float trailerDamage;

    MSGPACK_DEFINE(drivenKm, fuelConsumed, trailerDamage);
};

struct job_t {
    job_t() {
        onJob = false;
        delivered = false;
        drivenKm = 0.f;
        fuelConsumed = 0.f;
        income = 0;
        trailer = {};
        cargo = {};
        source = {};
        destination = {};
    }

    bool onJob;
    bool delivered;

    float drivenKm;
    float fuelConsumed;

    uint64_t income;

    struct trailer_t {
        trailer_t() {
            connected = false;
            damage = 0.f;
        }

        bool connected;
        float damage;
        MSGPACK_DEFINE(connected, damage)
    } trailer;

    struct cargo_t {
        cargo_t() {
            name = "-";
            mass = 0.f;
        }

        std::string name;
        float mass;
        MSGPACK_DEFINE(name, mass)
    } cargo;

    struct source_t {
        source_t() {
            city = "-";
            company = "-";
        }

        std::string city;
        std::string company;
        MSGPACK_DEFINE(city, company);
    } source;

    struct destination_t {
        destination_t() {
            city = "-";
            company = "-";
        }
        std::string city;
        std::string company;
        MSGPACK_DEFINE(city, company);
    } destination;

    MSGPACK_DEFINE(onJob, delivered, drivenKm, fuelConsumed, income, trailer, cargo, source, destination);

#ifndef PLUGIN_INTERNAL
    void Serialize(Json::Value &root) const {
        root["onJob"] = onJob;
        root["delivered"] = delivered;
        root["distanceDriven"] = drivenKm;
        root["fuelConsumed"] = fuelConsumed;
        root["trailerDamage"] = trailer.damage;
        root["cargoName"] = cargo.name;
        root["cargoMass"] = cargo.mass;
        root["sourceCity"] = source.city;
        root["sourceCompany"] = source.company;
        root["destinationCity"] = destination.city;
        root["destinationCompany"] = destination.company;
    }
#endif
};

static bool operator==(job_t const &lhs, job_t const &rhs) {
    return lhs.onJob == rhs.onJob;
}

#endif //ETS2_JOB_LOGGER_PLUGINDEFS_H
