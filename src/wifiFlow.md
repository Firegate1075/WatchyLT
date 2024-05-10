# Ideas for wifi

- wifi handler starts request
- when wifiHandler.process() is called, wifi handler updates request
- in controller: as long as wifiHandler has unfinished requests, esp32 does not sleep.
Instead waits in infinite loop while handling button inputs and processing wifi requests.

---

- use interrupts for buttons with button handling in isr
  - problems:
  1. large isr
  2. inputs need to be separated from displaying (handleScreen method)

--- 

- support for blocking *and* non blocking wifi transactions?

---

- wait to sleep for finished wifi request needed anyway (!?)