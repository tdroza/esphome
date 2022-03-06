import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_NONE,
)


my_sensor_ns = cg.esphome_ns.namespace("my_sensor")

MySensor = my_sensor_ns.class_("MySensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        accuracy_decimals=2,
        state_class=STATE_CLASS_NONE,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(MySensor),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
