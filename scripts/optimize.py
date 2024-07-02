import re
import subprocess
import time


def extract_requests_per_second(output):
    match = re.search(r"Requests per second:\s+([0-9.]+)", output)
    if match:
        return float(match.group(1))
    return -1


threads = [8, 16, 32, 64, 128, 256]
epoll_events = [64, 256, 1024, 4096]
file_path = "inc/common.h"

max_requests_per_second = 0
best_config = (0, 0)

for thread_count in threads:
    for event_length in epoll_events:
        with open(file_path, "r") as file:
            content = file.read()

        content = re.sub(r"#define NUM_THREADS \d+", f"#define NUM_THREADS {thread_count}", content)
        content = re.sub(r"#define MAX_EPOLL_EVENTS \d+", f"#define MAX_EPOLL_EVENTS {event_length}", content)

        with open(file_path, "w") as file:
            file.write(content)

        run_process = subprocess.Popen(["bash", "scripts/run.sh"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(0.5)

        test_process = subprocess.Popen(
            ["bash", "scripts/test.sh"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        test_output, _ = test_process.communicate(input="\n")

        requests_per_second = extract_requests_per_second(test_output)
        print(f"{requests_per_second} r/s with (t={thread_count}, e={event_length})")

        if requests_per_second and requests_per_second > max_requests_per_second:
            max_requests_per_second = requests_per_second
            best_config = (thread_count, event_length)

        run_process.terminate()
        run_process.kill()

print(f"best: {max_requests_per_second} r/s with (t={best_config[0]}, e={best_config[1]})")
