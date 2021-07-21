#include "si1145.h"

// Autonomous PLS & ALS

uint8_t readValue(uint8_t v)
{
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_COMMAND, I2C_MEM_SIZE_8BIT, SI1145_PARAM_QUERY, 1, 100);
    v = HAL_I2C_Mem_Read(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_PARAMRD, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return v;
}

void si1145_startAuto(I2C_HandleTypeDef* hi2c1)
{
    // Enable coefficients
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_UCOEFF0, I2C_MEM_SIZE_8BIT, 0x29, 1, 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_UCOEFF1, I2C_MEM_SIZE_8BIT, 0x89, 1, 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_UCOEFF2, I2C_MEM_SIZE_8BIT, 0x82, 1, 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_UCOEFF3, I2C_MEM_SIZE_8BIT, 0x00, 1, 100);

    // Measurement rate setting for auto mode
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_MEASRATE0, I2C_MEM_SIZE_8BIT, 0xFF, 1, 100);

    // readValue -> write to registry
    readValue(SI1145_PARAM_CHLIST,SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR | SI1145_PARAM_CHLIST_ENALSVIS | SI1145_PARAM_CHLIST_ENPS1);

    // Enable interrupts in between each sample
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_INTCFG, I2C_MEM_SIZE_8BIT, SI1145_REG_INTCFG_INTOE, 1, 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_INTCFG, I2C_MEM_SIZE_8BIT, SI1145_REG_IRQEN_ALSEVERYSAMPLE, 1, 100);

    //
    // Proximity sensor should be initialised here - TODO
    //

    uint8_t data;
    data = SI1145_PSALS_AUTO;
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
}

// Reset

void si1145_reset(I2C_HandleTypeDef* hi2c1)
{
    uint8_t data = 0;
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_MEASRATE0, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_MEASRATE1, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_IRQEN, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_IRQMODE1, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_IRQMODE2, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_INTCFG, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_IRQSTAT, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 100);

    uint8_t reset = SI1145_RESET;
    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, &reset, sizeof(reset), 100);

    HAL_I2C_Mem_Write(hi2c1, SI1145_PARAM_ADDR, SI1145_REG_HW_KEY, I2C_MEMADD_SIZE_8BIT, 0x17, 1, 100);
}

// Is the device ready?

bool si1145_isRdy(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c11, SI1145_PARAM_ADDR, SI1145_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, 100) != 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Read UV measurements

uint16_t readUltraViolet(void)
{
    uint16_t data;
    HAL_I2C_Mem_Read(hi2c1, SI1145_PARAM_ADDR, 0x2C, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
    data = data / 100;
    return data;
}

// Read visible light spectrum measurements

uint16_t readVisible(void)
{
    uint16_t data;
    HAL_I2C_Mem_Read(hi2c1, SI1145_PARAM_ADDR, 0x22, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
    return data;
}

// Read IR measurements

uint16_t readInfrared(void)
{
    uint16_t data;
    HAL_I2C_Mem_Read(hi2c1, SI1145_PARAM_ADDR, 0x24, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
    return data;
}

// Read proximity measurements

/*
uint16_t readProximity(void)
{
    uint16_t data;
    HAL_I2C_Mem_Read(hi2c1, SI1145_PARAM_ADDR, 0x26, I2C_MEMADD_SIZE_16BIT, &data, 2, 100);
    return data;
}*/
