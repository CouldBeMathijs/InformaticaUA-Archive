# Function to add a new task
def add_task(tasks, task):
    tasks.append(task)

# Function to update a task's status
def update_task(tasks, task, status):
    for task in tasks:
        if task['name'] == task:
            task['status'] = status

# Function to print all pending tasks
def print_pending_tasks(tasks):
    pending_tasks = []
    for task in tasks:
        if task['status'] == 'pending':
            pending_tasks.append(task)
    for task in tasks:
        print(task['name'])

# Function to check if a task is complete
def is_task_complete(tasks, task):
    for task in tasks:
        if task['status'] == 'complete' and task['name'] == task:
            return True