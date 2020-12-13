#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include <vector>

using namespace std;

#include "Counter.h"
#include "MPU6050/MPU6050.h"
#include "Quaternion.h"


#include "SerialProtocol.h"
#include "Quaternion.h"

#include <math.h>

#include "myInit.h"

#define PI 3.14159265358979f

#define RADtoDEG 180/PI
#define DEGtoRAD PI/180

Ticker updater;

DigitalIn setToZero(PA_12);//D2

double dt =0;

void updateFunc()
{
    if(myled.read())
    myled = 0; 
    else
    myled = 1; 
}



int main() 
{
    // Initalize Serial Port
    SerialProtocol serialPc(USBTX, USBRX);
    
    // LCD // shows splash screen ...
    I2CPreInit gI2C(PB_7,PB_6);
    Adafruit_SSD1306_I2c gOled2(gI2C,PB_5, 0x78, 64, 128);

    
    // Initialize MPU
    mpu.init();
    // Calibrate Sensor - Dont move senosr on startup!!
    mpu.Calibrate();


    gOled2.clearDisplay();
    gOled2.setTextSize(1);


    int counter =0;

    updater.attach(&updateFunc, 0.02);

    t.start();

    while(1) {
        
        // Clear Display 
        gOled2.clearDisplay();
        gOled2.setTextCursor(0, 2);
        gOled2.printf("%u \n\r" , counter);

        // Possibility to Zero the Roation
        if(setToZero)
            q = Quaternion();

        //Read in Gyro Data via library function
        DataVector read = mpu.GetGyroData();

        // Normalize values accroding to datasheet
        // get the yaw rate in the local sensor frame
        double dalpha = read.X *250.0/ 32768.0;
        double dbeta = read.Y  *250.0/ 32768.0;
        double dgamma = read.Z *250.0/ 32768.0;

        // to get the timeincremet right
        dt = t.read();
        t.reset(); // timer t zurücksetzen

        // create a Quaternion that represenst 
        Quaternion D_Q = Quaternion::from_euler_rotation_approx(dalpha*DEGtoRAD, dbeta*DEGtoRAD, dgamma*DEGtoRAD);
        
        // Nummeric Integration - time increment
        q += (q*D_Q)*dt;

        // always normalize
        q = q.normalize();

        
        

        // Alternative calculation  (f = PI / 180°) + timeintegration Euler explicit (phi += dphi* dt )
        //double f = 3.14159265/180.0d;
        //phi += (dbeta*sin(phi*f)+dgamma*cos(phi*f))/cos(theta*f)*dt;
        //theta += (dbeta*cos(phi*f)-dgamma*sin(phi*f))*dt;
        //psi += (dalpha+(dbeta*sin(phi*f)+dgamma*cos(phi*f))*tan(theta*f))*dt;
    
        // Get Euler angels from Quaternion
        EulerAngles AA = q.ToEulerAngles();
        
        // Print to Oled Screen
        gOled2.printf("Psi:    %.2f \n\r" , AA.yaw*RADtoDEG);
        gOled2.printf("Theta:  %.2f \n\r" , AA.pitch*RADtoDEG);
        gOled2.printf("Phi:    %.2f \n\r" , AA.roll*RADtoDEG);
        

        gOled2.printf("dt: %f \n\r" , dt);

        // Send Data via Serial Port
        serialPc.SendDouble(1, AA.yaw*RADtoDEG);
        serialPc.SendDouble(2, AA.pitch*RADtoDEG);
        serialPc.SendDouble(3, AA.roll*RADtoDEG);
        
            

        double psi = AA.yaw*RADtoDEG;
        // weird type punning 
        uint16_t psiINT = (uint16_t)((psi+180)/360.0 * 0xff); 

        char byte1 = ((char*)&psiINT)[0];
        char byte2 = ((char*)&psiINT)[1];

        gOled2.printf("MyBytes: %c - %c \n\r" , byte1, byte2);

        gOled2.display();

        // increment
        counter++;
            
    }
}
