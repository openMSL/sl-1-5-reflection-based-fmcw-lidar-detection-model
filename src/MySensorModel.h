//
// Copyright 2016 -- 2018 PMSF IT Consulting Pierre R. Mai
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
// The MPL-2.0 is only an example here. You can choose any other open source license accepted by OpenMSL, or any other license if this template is used elsewhere.
//

#pragma once
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <random>

#include "OSMPConfig.h"
#include "fmi2Functions.h"
#include "osi_sensordata.pb.h"

using namespace std;

struct Beam
{
    float horizontal_angle;
    float vertical_angle;
    int beam_idx;
};

struct BeamDivergence
{
    float horizontal_angle;
    float vertical_angle;
};

struct ModelParameters
{
    // ray tracing parameter
    vector<Beam> beam_config;
    BeamDivergence beam_divergence;

    float emitted_power_per_beam_mW;
    float emitted_power_per_ray_w;

    uint32_t rays_per_beam_horizontal;
    uint32_t rays_per_beam_vertical;
    uint32_t rays_per_beam;

    float distance_noise_std;
    float velocity_noise_std;
};

struct SensorParameters
{
    float wavelength_m;
    float min_range;
    float max_range;

    double ramp_duration;
    double sample_frequency;
    double bandwidth;
    double f_res;
    double kappa;

    // fourier tracing parameter
    int window_data_per_bin;
    int bin_affect_range;
    std::vector<float> window_function;
    int fft_size;
    int num_fft_bins;
};

class MySensorModel
{
  public:
    void Init(double nominal_range_in, string theinstance_name, fmi2CallbackFunctions thefunctions, bool thelogging_on);

    osi3::SensorData Step(osi3::SensorView current_in, double time);


  protected:
    void SetParams();

    vector<Beam> CalculateBeamPattern() const;

    void AddNoise(double &measurand, double std, std::default_random_engine &generator);

    ModelParameters model_params;
    SensorParameters sensor_params;

  private:
    string instance_name_;
    bool logging_on_;
    set<string> logging_categories_;
    fmi2CallbackFunctions functions_;

    double nominal_range_;

    /* Private File-based Logging just for Debugging */
#ifdef PRIVATE_LOG_PATH
    static ofstream private_log_file;
#endif

    static void FmiVerboseLogGlobal(const char* format, ...)
    {
#ifdef VERBOSE_FMI_LOGGING
#ifdef PRIVATE_LOG_PATH
        va_list ap;
        va_start(ap, format);
        char buffer[1024];
        if (!private_log_file.is_open())
        {
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        }
        if (private_log_file.is_open())
        {
#ifdef _WIN32
            vsnprintf_s(buffer, 1024, format, ap);
#else
            vsnprintf(buffer, 1024, format, ap);
#endif
            private_log_file << "OSMPDummySensor"
                             << "::Global:FMI: " << buffer << endl;
            private_log_file.flush();
        }
#endif
#endif
    }

    void InternalLog(const char* category, const char* format, va_list arg)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        char buffer[1024];
#ifdef _WIN32
        vsnprintf_s(buffer, 1024, format, arg);
#else
        vsnprintf(buffer, 1024, format, arg);
#endif
#ifdef PRIVATE_LOG_PATH
        if (!private_log_file.is_open())
        {
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        }
        if (private_log_file.is_open())
        {
            private_log_file << "MySensorModel"
                             << "::"
                             << "template"
                             << "<" << ((void*)this) << ">:" << category << ": " << buffer << endl;
            private_log_file.flush();
        }
#endif
#ifdef PUBLIC_LOGGING
        if (logging_on_ && (logging_categories_.count(category) != 0U))
        {
            functions_.logger(functions_.componentEnvironment, instance_name_.c_str(), fmi2OK, category, buffer);
        }
#endif
#endif
    }

    void FmiVerboseLog(const char* format, ...)
    {
#if defined(VERBOSE_FMI_LOGGING) && (defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING))
        va_list ap;
        va_start(ap, format);
        internal_log("FMI", format, ap);
        va_end(ap);
#endif
    }

    /* Normal Logging */
    void NormalLog(const char* category, const char* format, ...)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        va_list ap;
        va_start(ap, format);
        InternalLog(category, format, ap);
        va_end(ap);
#endif
    }
};