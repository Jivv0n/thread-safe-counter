# OS Environment
Linux Ubuntu 18.04

# Mutex & Semaphore

## Mutex
![Mutex](https://user-images.githubusercontent.com/84502236/121785894-7548d480-cbf7-11eb-9d81-75bfe6d7a73a.png)

## Semaphore
![Semaphore](https://user-images.githubusercontent.com/84502236/121785900-809c0000-cbf7-11eb-8deb-6ebc31960d71.png)

## Compare Performance
|Method|1|2|3|4|5|6|Average|
|:------:|:---:|:---:|:---:|:---:|:---:|:---:|:------:|
| Mutex    | 0.329 | 0.342 | 0.344 | 0.388 | 0.346 | 0.321  | 0.345 |
| Semaphore| 6.918 | 6.516 | 6.533 | 6.677 | 6.679 | 6.651  | 6.662 |

**Mutex is much faster than Semaphore**

# Analysis
Mutex has only two states: locked and unlocked, and only who locks can unlock.
In contrast, Semaphore has no lock owner. It means that anyone can unlock anyone. The thread which is having higher priority than current thread can also release binary semaphore and take lock. And multiple number of threads can acquire binary semaphore at a time concurrently.
In terms of calculating priority, checking Semaphore value, and etc, they create time delay.
So, when using single shared resource, Mutex would be better.
