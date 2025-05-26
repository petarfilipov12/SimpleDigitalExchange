import threading
import time

class TimerUpdater:
    func = None
    cycle_time = None

    def __init__(self, func=None, cycle_time=1):
        self.func = func

        if(cycle_time < 0.0):
            raise Exception("Invalid Cycle Time: " + str(cycle_time))
        self.cycle_time = cycle_time
    
    def _Loop(self):
        while(True):
            time.sleep(self.cycle_time)
            self.func()
    
    def SetFunc(self, func):
        self.func = func
    
    def Run(self):
        if(None == self.func):
            raise Exception("TimerUpdated func not set.")
        
        t = threading.Thread(target=self._Loop)
        t.daemon = True
        t.start()
    

