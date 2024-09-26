STM32SensorStation
==================

Educational project focused on learning I2C and SPI protocols and using external libraries. The MCU communicates with an LCD via SPI and with a BME680 sensor via I2C. The project also uses an RTC timer with time and calendar functions. Time and date setting has been implemented in the station using three switches that operate in an interrupt and have a debouncer implemented based on a timer.

Libraries used:<br />
---------------------

-hagl library (https://github.com/tuupola/hagl)<br />
-BME680x-STM32-HAL (https://github.com/Squieler/BME68x-STM32-HAL) in which I changed HAL to LL<br />
<br />

Device displays:<br />
----------------------
-Temperature<br />
-Air pressure<br />
-Humidity<br />
-Air quality<br />
-Date<br />
-Time<br />
<br />
![image](https://github.com/user-attachments/assets/5acbb2d1-2751-49a3-88cf-f59397ebb921)<br />
Image 1: Weather station display

Device setup:<br />
--------------------
![image](https://github.com/user-attachments/assets/5e449aee-dd01-4173-922c-8ca355a4857f)<br />
Image 2: Weather station during setup

After clicking the middle switch, the currently changed value is displayed in yellow. The left button decreases the value and the right button increases it. The middle switch moves to the next value.

Debouncing procedure:<br />
---------------------------
After a switch click, the MCU enters an interrupt handler (all three switches use EXTI9-15). In if statements, the program checks which button was clicked and sets the flag interrupt1, interrupt2, or interrupt3. It also sets the debouncer flag, which prevents the detection of another press, and starts timer 6. After 50 ms, in the interrupt from the timer, the debouncer flag is cleared, and the MCU can handle the next button press.
