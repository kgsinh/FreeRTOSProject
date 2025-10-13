# FreeRTOS with STM32
## FreeRTOS LED Blink Sequencer
- Control multiple LEDs with different patterns, introduce inter-task communication, 
  synchronization, and timers to demonstrate all essential RTOS concepts. 
  
 1. Tasks and Their Roles 
  
  | Task         | Purpose                       | Priority | RTOS Concept Highlighted                      |
| ------------ | ----------------------------- | -------- | --------------------------------------------- |
| LED1 Task    | Blink LED1 at 1Hz             | Medium   | Periodic task with `vTaskDelayUntil`          |
| LED2 Task    | Blink LED2 at 2Hz             | Medium   | Task preemption vs blocking                   |
| LED3 Task    | PWM-style LED fading          | Low      | Fine-grained timing & CPU sharing             |
| Button Task  | Detect button presses         | High     | Event-driven task, ISR interaction            |
| Pattern Task | Change LED patterns on events | Medium   | Queue-driven inter-task communication         |
| Monitor Task | System health check or log    | Low      | Idle/low-priority task, background processing |


2. Task Diagram and Flow  
  
          +-------------------+
          |   Button ISR      |
          | (Button Pressed)  |
          +---------+---------+
                    |
                    v
           +--------+---------+
           | Pattern Task      |
           | (Queue/Notify)    |
           +--------+---------+
                    |
                    v
  +-----------------+-----------------+
  | LED1 Task        | LED2 Task       | LED3 Task
  | Blink 1Hz        | Blink 2Hz       | PWM Fading
  | Medium Priority  | Medium Priority | Low Priority
  +-----------------+-----------------+
                    |
                    v
             +------+------+
             | Monitor Task |
             | Idle/Logging |
             | Low Priority |
             +-------------+
             
### Flow Explanation:
1. Button press triggers an ISR.

2. ISR sends a message/event to Pattern Task (via queue or task notification).

3. Pattern Task updates LED sequence/patterns.

4. LED tasks run periodically based on their delay or vTaskDelayUntil.

5. Monitor Task runs in the background (idle) to log system status.

6. Mutex/semaphore protects LEDs if multiple tasks modify the same GPIO.

---
 
3. Hardware Connections

- Button --> GPIOC Pin 13 (User Button on many STM32 boards)
- LED1 --> GPIOC Pin 10 (Green LED)
- LED2 --> GPIOC Pin 11 (Blue LED)
- LED3 --> GPIOC Pin 12 (Red LED)

             
  