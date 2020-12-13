/*   
*   Author Johannes Tiefnig - johannes.tiefnig@student.tugraz.at
*   Date: November 2019
*
*   BASED ON:  
*   Copyright (c) 2015, Baser Kandehir, baser.kandehir@ieee.metu.edu.tr
*/


#ifndef MPU6050_H
#define MPU6050_H

#include "../mbed.h"



#include <math.h>
#include "MPU6050RegDef.h"

extern int16_t accelData[3],gyroData[3],tempData;
extern float accelBias[3], gyroBias[3];

// Acc Full Scale Range  +-2G 4G 8G 16G  
enum Ascale
{
    AFS_2G=0,  
    AFS_4G,
    AFS_8G,
    AFS_16G
};

// Gyro Full Scale Range +-250 500 1000 2000 Degrees per second
enum Gscale
{
    GFS_250DPS=0,   
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};


struct DataVector
{
    DataVector(int16_t x, int16_t y, int16_t z)
        : X(x), Y(y), Z(z)
    {}

    DataVector()
        : X(0), Y(0), Z(0)
    {}

    int16_t X, Y, Z;

    DataVector operator-(const DataVector& other)
    {
        return DataVector(X-other.X, Y-other.Y, Z-other.Z);
    }

    DataVector operator+(const DataVector& other)
    {
        return DataVector(X+other.X, Y+other.Y, Z+other.Z);
    }


    
};
 

/* Class  */
class MPU6050 
{
    DataVector GyroOffset;
    DataVector AccelOffset;

    int Ascale = AFS_2G;
    int Gscale = GFS_250DPS;

    

    I2C i2c; // Change later!

    //multiplyer for 
    float aRes, gRes; 
    
protected:

    void getAres();
    void getGres();
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    char readByte(uint8_t address, uint8_t subAddress); 
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t byteNum, uint8_t* dest);

public: 
    

    

    MPU6050(I2C interface_I2C)
    :i2c(interface_I2C)
    {
        
    }
    
    
    
    void whoAmI();
    void init();
    void reset();
   
    void readAccelData(int16_t* dest);
    DataVector readGyroData();

    DataVector GetGyroData();



    int16_t readTempData();
    
    void calibrate(float* dest1, float* dest2);
    //void complementaryFilter(float* pitch, float* roll);

    void Calibrate();

};

#endif