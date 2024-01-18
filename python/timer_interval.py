import sched, time, functools, threading

__scheduler = sched.scheduler(time.time, time.sleep)

def Timer(seconds):
  def decorator(func):
    @functools.wraps(func)
    def wrapper(*argv, **kw):
      Timer(seconds)(func)
      func(*argv, **kw)
    __scheduler.enter(seconds, 1, wrapper, ())
    return wrapper
  __scheduler.run()
  return decorator

@Timer(seconds=1)
def testInterval():
  import datetime
  dt = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
  print ("timer  running", threading.current_thread().native_id, dt)

threading.Thread(target=testInterval, daemon=True).start()
print("thread running")

while True: time.sleep(1)