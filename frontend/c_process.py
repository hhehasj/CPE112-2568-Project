import ctypes as cty


class Task(cty.Structure):
    _fields_ = [
        ("name", cty.c_char * 50),
        ("deadline", cty.c_int64),
        ("tag", cty.c_int),
    ]


class Task_queue(cty.Structure):
    _fields_ = [
        ("tasks", cty.POINTER(Task)),
        ("size", cty.c_int),
        ("capacity", cty.c_int),
    ]


class Stack(cty.Structure):
    _fields_ = [
        ("tasks", cty.POINTER(Task)),
        ("top", cty.c_int),
        ("capacity", cty.c_int),
    ]


backend: cty.CDLL = cty.CDLL("./Cbackend.so")

# --------------QUEUE-----------------------
backend.Initialize.argtypes = [cty.POINTER(Task_queue)]
backend.Initialize.restype = None

backend.Insert.argtypes = [Task, cty.POINTER(Task_queue)]
backend.Insert.restype = None

backend.Deletion.argtypes = [cty.POINTER(Task_queue), Task]
backend.Deletion.restype = cty.c_int

backend.Free_Queue.argtypes = [cty.POINTER(Task_queue)]
backend.Free_Queue.restype = None
# ------------------------------------------

# --------------STACK-----------------------
backend.Initialize_Stack.argtypes = [cty.POINTER(Stack)]
backend.Initialize_Stack.restype = None

backend.push.argtypes = [cty.POINTER(Stack), Task]
backend.push.restype = None

backend.pop.argtypes = [cty.POINTER(Stack)]
backend.pop.restype = Task

backend.Free_Stack.argtypes = [cty.POINTER(Stack)]
backend.Free_Stack.restype = None
# ------------------------------------------

# --------------TXT_UTILS-----------------------
backend.save_task.argtypes = [Task]
backend.save_task.restype = None

backend.remove_task.argtypes = [Task]
backend.remove_task.restype = None

backend.load_tasks.argtypes = [cty.POINTER(Task_queue)]
backend.load_tasks.restype = cty.c_int
# ------------------------------------------

queue = Task_queue()
backend.Initialize(cty.byref(queue))

stack = Stack()
backend.Initialize_Stack(cty.byref(stack))


def load_all_tasks_from_c():
    num_loaded = backend.load_tasks(cty.byref(queue))
    print(num_loaded)

    tasks_list: list[Task] = []
    if num_loaded > 0 and queue.tasks:
        for i in range(queue.size):
            tasks_list.append(queue.tasks[i])

    return tasks_list


def free_queue_stack():
    backend.Free_Queue(cty.byref(queue))
    backend.Free_Stack(cty.byref(stack))
