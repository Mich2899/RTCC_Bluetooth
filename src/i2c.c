/*
 * i2c.c
 * This file contains function definitions used for i2c operations.
 *  Created on: Sep 15, 2021
 *      Author: mich1576
 */

/*********************************************************INCLUDES****************************************************************/
#include "i2c.h"
// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

/*******************************************************GLOBAL VARIABLES*********************************************************/

  I2C_TransferReturn_TypeDef transferStatus;    // used to store the transfer status

  uint8_t cmd_data;                             // use this variable to send command to transfer buffer
  uint8_t read_data[2];                         // use for storing the 16 bit temperature data recieved from si7021 in array format
  uint16_t temp_data;                           // used for storing complete 16 bit data
  int Temperature;                              // Used to store the converted value


/*******************************************************FUNCTION DEFINITION******************************************************/
//Enables/Disables I2C sensor
void gpio_I2C(int on_off){
  GPIO_PinModeSet(SENSOR_PORT, SENSOR_ENABLE, gpioModePushPull, on_off);

}

//Initializes the I2CSPM_Init_TypeDef structure
void I2C_init(){

  I2CSPM_Init_TypeDef I2C_Config=
      {
          .port                 = I2C0,                   /* Peripheral port used for application is I2C0 */
          .sclPort              = I2C_port,               /* SCL pin port number */
          .sclPin               = SCL_pin,                /* SCL pin number */
          .sdaPort              = I2C_port,               /* SDA pin port number */
          .sdaPin               = SDA_pin,                /* SDA pin number */
          .portLocationScl      = SCL_PORT_LOCATION,      /* Port location of SCL signal */
          .portLocationSda      = SDA_PORT_LOCATION,      /* Port location of SDA signal */
          .i2cRefFreq           = 0,                      /* I2C reference clock */
          .i2cMaxFreq           = I2C_FREQ_STANDARD_MAX,  /* I2C max bus frequency to use */
          .i2cClhr              = i2cClockHLRStandard     /* Clock low/high ratio control */
        };

  I2CSPM_Init(&I2C_Config);                               /*Initalize I2C peripheral*/
 // uint32_t i2c_bus_frequency = I2C_BusFreqGet (I2C0);
}

//Used to perform I2C write. Sends start bit, slave address, send write command
//wait for acknowledgment, send measure command, wait for acknowledgment
void I2C_write(){

  I2C_TransferSeq_TypeDef transferSequence;
  // Send Measure Temperature command
  cmd_data = 0xF3;                                        //Provide command to perform measurement
  transferSequence.addr = SI7021_DEVICE_ADDR << 1;        //shift device address left
  transferSequence.flags = I2C_FLAG_WRITE;                //write command
  transferSequence.buf[0].data = &cmd_data;               //pointer to data to write
  transferSequence.buf[0].len = sizeof(cmd_data);
  transferStatus = I2CSPM_Transfer (I2C0, &transferSequence); //check the status of this operation

    if (transferStatus != i2cTransferDone) {
        LOG_ERROR("I2CSPM_Transfer: I2C bus write of cmd= %d failed", (int32_t)cmd_data );
    }
}

//Used to perform I2C read. Sends repeated start bit,
//slave address, send read command, read MS and LS byte
void I2C_read(){

  I2C_TransferSeq_TypeDef transferSequence;
  // Send Measure Temperature command

  //set the transfer sequence for read
  transferSequence.addr = SI7021_DEVICE_ADDR << 1;        //shift device address left
  transferSequence.flags = I2C_FLAG_READ;                 //read command
  transferSequence.buf[0].data = read_data;               //pointer to data to write
  transferSequence.buf[0].len = sizeof(read_data);
  transferStatus = I2CSPM_Transfer (I2C0, &transferSequence);//check the status of this operation

  temp_data = (read_data[0]<<8) + read_data[1];           //store the two 8-bit data into one 16-bit variable
  if (transferStatus != i2cTransferDone) {                //check status of this operation
      LOG_ERROR("Read Error");
  }
  else{
    Temperature = ((175.72*(temp_data))/65536)-46.85;     //convert 16-bit data in degree Celcius format
    LOG_INFO("Temperature: %d\n\r", Temperature);
  }
}

//Used to perform temperature measurement for si7021
void read_temp_from_si7021(){
  gpio_I2C(1);                                            //enable I2C sensor
  timerWaitUs(80000);                                     //wait for 80ms for power to stabilize+ boot time

  I2C_write();                                            //perform measure command write
  timerWaitUs(10000);                                     //conversion time
  I2C_read();                                             //perform read function to get the temperature data

  gpio_I2C(0);                                            //disable I2C sensor
}
