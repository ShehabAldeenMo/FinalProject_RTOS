## This release depends on the sequence of 5 Tasks and 6 functions that work in synchronization to handle tasks
1. _**Init_Task**_ is used to initialize the system with clock and passing clock to specific peripherals that used in system. Initialize all GPIO pins with output push-pull.
   Initialize uart pins (TX-RX) with each specific purpose. Initialize LCD pins with each specific purpose. Delete initialization task at the end. Here is a sample for its sequence diagrame documentation.
![image](https://github.com/ShehabAldeenMo/FinalProject_RTOS/assets/144431914/86488088-5519-4398-bf12-47e583c47dcc)

3. _**ManagerTask**_ Transmits the list of options for the user on the putty screen and stops for input from the user then sends this option to queue and notify _**ActionTask**_ then
   suspends itself to exist from scheduling and gives permission for lower priority task to run.

4. _**ActionTask**_ stop to notification from _**ManagerTask**_ then receive option from queue. Depending on this option, It will notify **_LedUpdate_** or _**LcdUpdate**_ then suspends
   itself to exist from scheduling and give permission for lower priority tasks to run.

5. _**LedUpdate**_ asks about the shape of LEDs that user wants by text which send to putty screen and stop for any option from choosing. And call a function that meets the wanted option
   from 1:4. Then Resume every task that is suspended to enter the scheduling sequence again.

6. _**LcdUpdate**_ waits on characters that the user is given till he enters '#' to ensure that the statement that he wants to display is finished. Then Resume every task that is suspended to enter the scheduling sequence again.

7. _**vApplicationIdleHook**_ is call_back_task that calls in the background when the system is in sleep mode to refresh hardware resources.

LinkdIn Post : https://www.linkedin.com/posts/ali-mamdouh-11aa1a233_%D9%82%D8%A7%D9%84%D9%88%D8%A7-%D8%B3%D8%A8%D8%AD%D8%A7%D9%86%D9%83-%D9%84%D8%A7-%D8%B9%D9%84%D9%85-%D9%84%D9%86%D8%A7-%D8%A5%D9%84%D8%A7-activity-7123628448274653185-Tr2f?utm_source=share&utm_medium=member_desktop
