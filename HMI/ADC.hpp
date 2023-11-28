/**
 * @file        ADC.hpp
 * @author      CodeDog
 * @brief       ADC interface, header file.
 * @remarks     Monitors both hall sensor and pump current changes.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "c_bindings.h"
#include "hal_target.h"
EXTERN_C_BEGIN
#include HAL_HEADER
#include HAL_HEADER_(adc)
#include HAL_HEADER_(adc_ex)
EXTERN_C_END
#include "Action.hpp"
#include "IData.hpp"

/// @brief ADC observer API.
/// @tparam TSample Sample type.
template<typename TSample = uint16_t>
class ADCBase : public virtual IData<TSample>
{

public:

    using Callback = Ac2<double, double>;       // ARGS: Voltage [mV], Change [mV].

    static constexpr size_t channelsMax = 4;    // Maximum number of channels that can be set set.

    /// @brief Configures ADC observer.
    /// @param hadc ADC handle pointer.
    /// @param threshold A threshold value change to trigger the change event.
    ADCBase(ADC_HandleTypeDef* hadc, double threshold)
    {
        if (m_channels >= channelsMax) return; // This should never happen!
        m_instances[m_channels++] = this;
        m_hadc = hadc;
        m_threshold = threshold;
    }

    /// @brief Registers a function called on a value change that is greater than threshold.
    /// @param callback Function accepting the value and the value change.
    void registerCallback(Callback callback)
    {
        m_valueChanged = callback;
        HAL_ADC_RegisterCallback(m_hadc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, conversionComplete);
    }

    /// @brief Starts the conversion loop.
    void start() { HAL_ADC_Start_DMA(m_hadc, (uint32_t*)this->data(), this->size()); }

    /// @brief Calculates and returns the current instance value in millivolts.
    double value()
    {
        uint32_t s = 0;
        for (size_t i = 0, n = this->size(); i < n; ++i) s += this->at(i);
        return
            static_cast<double>(s) / static_cast<double>(this->size()) /
            static_cast<double>(maxValue) * static_cast<double>(supplyVoltage_mV);
    }

    /// @brief Returns the last calculated conversion value in millivolts.
    double lastValue() { return m_value; }

    /// @brief Stops the conversion loop.
    void stop() { HAL_ADC_Stop_DMA(m_hadc); }

protected:

    /// @brief Called on each conversion complete, triggers change events if applicable.
    static void conversionComplete(ADC_HandleTypeDef* hadc)
    {
        ADCBase* instance = nullptr;
        size_t i = 0;
        for (; i < m_channels; ++i) if (m_instances[i] && m_instances[i]->m_hadc == hadc)
        {
            instance = m_instances[i];
            break;
        }
        if (!instance) return;
        double p = instance->m_value;
        double v = instance->value();
        long double d = v - p;
        if (std::fabs(d) >= instance->m_threshold)
        {
            instance->m_value = v / instance->m_threshold * instance->m_threshold;
            if (instance->m_valueChanged) instance->m_valueChanged(instance->m_value, d);
        }
    }

    static inline size_t m_channels = 0;                    // Configured number of channels.
    static inline ADCBase* m_instances[channelsMax] = {};   // Configured instances.

    ADC_HandleTypeDef* m_hadc = nullptr;                    // ADC handle pointer.
    double m_value = 0;                                     // Current reading value.
    double m_threshold = 0;                                 // Current threshold value.
    Callback m_valueChanged = nullptr;                      // Value changed callback.

public:

    static constexpr double supplyVoltage_mV = 3300.0;  // ADC power supply voltage in millivolts.
    static constexpr uint16_t maxValue = 0xffff;        // ADC maximal reading corresponding to the measured voltage equal ADC power supply voltage.

};

/// @brief ADC observer with buffer.
/// @tparam TSize Number of samples.
/// @tparam TSample Sample type.
template<size_t TSize, typename TSample = uint16_t>
class ADCObserver final : public StaticData<TSize, TSample>, public ADCBase<TSample>
{

public:
    ADCObserver(ADC_HandleTypeDef* hadc, double threshold) : ADCBase<TSample>(hadc, threshold) { }

};
