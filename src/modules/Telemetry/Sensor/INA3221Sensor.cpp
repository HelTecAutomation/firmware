#include "configuration.h"

#if HAS_TELEMETRY && !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "INA3221Sensor.h"
#include "TelemetrySensor.h"
#include <INA3221.h>

INA3221Sensor::INA3221Sensor() : TelemetrySensor(meshtastic_TelemetrySensorType_INA3221, "INA3221"){};

int32_t INA3221Sensor::runOnce()
{
    LOG_INFO("Init sensor: %s\n", sensorName);
    if (!hasSensor()) {
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;
    }
    if (!status) {
        ina3221.begin(nodeTelemetrySensorsMap[sensorType].second);
        ina3221.setShuntRes(100, 100, 100); // 0.1 Ohm shunt resistors
        status = true;
    } else {
        status = true;
    }
    return initI2CSensor();
};

void INA3221Sensor::setup() {}

bool INA3221Sensor::getMetrics(meshtastic_Telemetry *measurement)
{
    measurement->variant.environment_metrics.voltage = ina3221.getVoltage(INA3221_CH1);
    measurement->variant.environment_metrics.current = ina3221.getCurrent(INA3221_CH1);
    measurement->variant.power_metrics.ch1_voltage = ina3221.getVoltage(INA3221_CH1);
    measurement->variant.power_metrics.ch1_current = ina3221.getCurrent(INA3221_CH1);
    measurement->variant.power_metrics.ch2_voltage = ina3221.getVoltage(INA3221_CH2);
    measurement->variant.power_metrics.ch2_current = ina3221.getCurrent(INA3221_CH2);
    measurement->variant.power_metrics.ch3_voltage = ina3221.getVoltage(INA3221_CH3);
    measurement->variant.power_metrics.ch3_current = ina3221.getCurrent(INA3221_CH3);
    return true;
}

uint16_t INA3221Sensor::getBusVoltageMv()
{
    return lround(ina3221.getVoltage(INA3221_CH1) * 1000);
}

#endif