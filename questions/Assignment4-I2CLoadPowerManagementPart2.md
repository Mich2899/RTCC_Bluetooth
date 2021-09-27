Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements with logging disabled to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See the file Instructions to add screenshots in assignment.docx in the ECEN 5823 Student Public Folder.* 

1. What is the average current per period?
   Answer: **14.56 uA**
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/ecen5823-assignment4-Mich2899/blob/master/questions/screenshots/assignment4/avg_current_per_period.PNG)  

2. What is the average current when the Si7021 is Powered Off?
   Answer: **2.87 uA**
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-Mich2899/blob/master/questions/screenshots/assignment4/Avg_current_LPM_Off.PNG)  

3. What is the average current when the Si7021 is Powered On?
   Answer: **489.59 uA**
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-Mich2899/blob/master/questions/screenshots/assignment4/Avg_current_LPM_On.PNG)  

4. How long is the Si7021 Powered On for 1 temperature reading?
   Answer: **99.60 ms**
   <br>Screenshot:  
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/ecen5823-assignment4-Mich2899/blob/master/questions/screenshots/assignment4/duration_lpm_on.PNG)  

5. Compute what the total operating time of your design for assignment 4 would be in hours, assuming a 1000mAh battery power supply?
   Answer: Considering the average current per period for the device (period being 3 seconds), the current consumed for an hour would be 17.472 mA. We are provided a power supply of 1000 mAh and hence the total operting time for the device would be 57.23 hours. This is considering that the average current is almost similar for every period. 
   
6. How has the power consumption performance of your design changed since the previous assignment?
   Answer: As compared to the previous implementation method(polling), it has improved to a great extent. As we can observe in the table below the current consumption has drastically changed from the polling method.
   ```
   |Parameter              | Value using polling method | Value using interrupt method |
   -------------------------------------------------------------------------------------
   |Avg_current_per_period |           145.41 uA        |            14.56 uA          |
   -------------------------------------------------------------------------------------
   |Avg_current_LPM_On     |            4.37 mA         |            489.59 uA         |
   ```
7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.
   Answer: The code normally operates in EM3 mode. There are five states that the device operates in considering the state machine implemention. The states are Idle, Warmup, timerwait, write and read. Once the sensor is given time to power-up (LPM 80ms), we add an sleep power manager requirement for the device to sleep in EM1 and remove this requirement once the write transfer is completed. Similarly, the requirement is added after the 10.8 ms delay between write and read transfer is completed. Once the read transfer successfully finishes we remove the requirement. Hence the device successfully sleeps during I2C transfers.
   

