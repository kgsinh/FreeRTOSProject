# 🔷 RTOS LED Pattern Sequencer – STM32G071RB

This project demonstrates how to structure and synchronize multiple **FreeRTOS tasks** on an STM32 microcontroller.  
It combines **periodic LED control**, **PWM fading**, and **button-triggered pattern switching** using **task notifications** and **queues**.

It’s a compact example of real-world embedded RTOS design — timing, task isolation, and ISR-safe communication — visualized through LED behavior.

---

## 🧩 Hardware Setup

| Peripheral | Pin | Function |
|-------------|-----|----------|
| Green LED | PC10 | Periodic blink (1Hz) |
| Blue LED | PC11 | PWM fading |
| Red LED | PC12 | Blink pattern |
| User Button | PC13 | Interrupt input (event trigger) |
| UART2 | PA2 / PA3 | Debug log (115200 baud) |

---

## ⚙️ System Overview

| Task | Purpose | Priority | RTOS Concept |
|------|----------|-----------|---------------|
| **Green LED Task** | Blink LED1 at 1Hz | 2 | Periodic timing using `vTaskDelayUntil()` |
| **Blue LED Task** | PWM fading control | 2 | Task timing and CPU sharing |
| **Red LED Task** | Blink LED3 | 2 | Cooperative multitasking |
| **Button Task** | Detect button press via ISR notification | 3 | ISR → Task communication |
| **Pattern Task** | Execute LED patterns based on button input | 1 | Queue-based coordination |

---

## 🔄 Control Flow

      +----------------------+
      |  Button Interrupt    |
      +----------------------+
                  |
                  v
      +----------------------+
      | Button Task          |
      | (Notification)       |
      +----------------------+
                  |
                  v
      +----------------------+
      | Pattern Generator    |
      | (Reads from Queue)   |
      +----------------------+
                  |
   +--------------+-----------------+
   | Green LED | Blue LED | Red LED |
   |   Task    |   Task   |   Task  |
   +--------------+-----------------+


---

## 🧠 Key RTOS Concepts Demonstrated

✅ **Task creation and priorities** – manage concurrency and preemption  
✅ **ISR to Task notifications** – signal from interrupts safely  
✅ **Queues** – send pattern data between tasks  
✅ **vTaskDelayUntil()** – precise periodic scheduling  
✅ **Task suspend/resume** – isolate LED control during patterns  
✅ **PWM integration** – link hardware timers to FreeRTOS  

---

## 🕹️ Operation Flow

1. System starts — all LED controller tasks begin running.  
2. A button press triggers an external interrupt (EXTI13).  
3. ISR sends a task notification to the Button Task.  
4. Button Task cycles through predefined pattern numbers (0, 1, 2).  
5. Pattern Generator Task:
   - Suspends the LED controller tasks.
   - Executes the selected LED pattern.
   - Resumes LED tasks after completion.  
6. UART prints debug information through every phase.

---

## 🧾 Example UART Log

```
=== BUTTON TASK STARTED ===
Pattern 0 sent to Pattern Generator Task
Executing Pattern 0: Blink Green LED 3 times
Resumed LED controller tasks after pattern execution
Pattern 1 sent to Pattern Generator Task
Executing Pattern 1: Fade Blue LED in and out
Resumed LED controller tasks after pattern execution
Pattern 2 sent to Pattern Generator Task
Executing Pattern 2: Blink Red LED 5 times
Resumed LED controller tasks after pattern execution
```


---

## 🔧 Technical Notes

- **Precise Timing:**  
  `vTaskDelayUntil()` keeps LED blink intervals stable even with varying execution times.  

## Queue Communication: 
 
```
xQueueSend(xPatternQueue, &pattern, portMAX_DELAY);
xQueueReceive(xPatternQueue, &receivedPattern, portMAX_DELAY);
```
## PWM Fade Control:  

```
for (int duty = 0; duty <= 100; duty += 10) {
    set_pwm_duty_cycle(duty);
    vTaskDelay(200);
}
for (int duty = 100; duty >= 0; duty -= 10) {
    set_pwm_duty_cycle(duty);
    vTaskDelay(200);
}
```

## 💡 Improvements to Explore

- Replace suspend/resume with Event Groups for cleaner synchronization.

- Add a UART command interface to select patterns.

- Use Software Timers for button debouncing or delayed pattern reset.

- Enable FreeRTOS trace hooks for runtime profiling.

- Expand LED patterns into modular routines (music-like rhythm sequencing).

## 🧰 Tools Used

- MCU: STM32G071RB

- RTOS: FreeRTOS (heap_4.c, 3072 bytes heap)

- Toolchain: STM32CubeIDE / arm-none-eabi-gcc

- Interface: UART + EXTI + TIMx PWM

## 🧩 Project Purpose

- This project bridges the gap between simple “LED blinking” and real firmware task management.
Each LED tells a story about scheduling, preemption, and coordination — the foundation of reliable embedded systems.