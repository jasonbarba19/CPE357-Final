#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

void swap(int* min, int* curr){
    int temp = *min;
    *min = *curr;
    *curr = temp;
}

void print_array(int arr[], int size){
    printf("[");
    for (int i = 0; i < size; i ++){
        if (i + 1 == size){
            printf("%d", arr[i]);
        }
        else{
            printf("%d, ", arr[i]);
        }
    }
    printf("]\n");
}

int main(int argc, char *argv[]){
    struct timeval start, end;
    double time;
    gettimeofday(&start, NULL);
    int *nums = mmap(NULL, 13000 * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int *processes = mmap(NULL, 1 * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    processes = atoi(argv[1]);
    // process count cuts off at 4 according to project instructions
    if (processes > 4){
        processes = 4;
    }
    int value = 0;
    int index = 0;
    
    while (scanf("%d\n", &value) == 1){
        //printf("Number: %d\n", value);
        nums[index] = value;
        index ++;
    }
   
    printf("Initial Array: ");
    print_array(nums, index);
    
    int *sorted = mmap(NULL, 1 * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    sorted = 0; // true = 1, false = 0;
    fflush(0);
    if (processes == 1)
    {
        while (sorted == 0){
            sorted = 1;

            for (int i = 0; i <= index - 2; i = i + 2){
                if (nums[i] > nums[i + 1]){
                    swap(&nums[i], &nums[i + 1]);
                    sorted = 0;
                }
            }
            for (int i = 1; i <= index - 2; i = i + 2){
                if (nums[i] > nums[i + 1]){
                    swap(&nums[i], &nums[i + 1]);
                    sorted = 0;
                }
            }
        }
      
        printf("Sorted Array: ");
        print_array(nums, index);
        gettimeofday(&end, NULL);
        time = (((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
        printf("Time to Sort: %.1f microseconds\n", time);
        printf("Processes: %d\n", processes);
        munmap(nums, 13000 * sizeof(int));
        munmap(processes, 1 * sizeof(int));
        munmap(sorted, 1 * sizeof(int));
    }

    else if (processes == 2){
        int split = 0;
        if (index % 2 == 0){
            split = (index - 2) / 2;
        }
        else {
            split = (index - 1) / 2;
        }


        int pid = fork();
        if (pid == 0){
            // child
            //printf("Parent\n");
            while (sorted == 0){
                sorted = 1;

                for (int i = split; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
                for (int i = split+1; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                    }
                }
            }
            printf("Sorted Array: ");
            print_array(nums, index);
            gettimeofday(&end, NULL);
            time = (((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
            printf("Time to Sort: %.1f microseconds\n", time);
            printf("Processes: %d\n", processes);
            munmap(nums, 13000 * sizeof(int));
            munmap(processes, 1 * sizeof(int));
            munmap(sorted, 1 * sizeof(int));
        }
        else if (pid > 0){
            // parent

            while (sorted == 0){
                sorted = 1;

                for (int i = 0; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
                for (int i = 1; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
            }
        }
    }

    else if (processes == 3){
        int split1 = 0;
        int split2 = 0;
        if (index % 3 == 0){
            split1 = index / 3;
            split2 = index * (2/3);
        }
        else if (index % 3 == 1){
            split1 = (index + 1) / 3;
            split2 = split1 * 2;
        }
        else if (index % 3 == 2){
            split1 = (index + 2) / 3;
            split2 = split1 * 2;
        }
        int pid = fork();
        if (pid == 0){
            // child
            int pid2 = fork();
            if (pid2 == 0){
                // process 3

                while (sorted == 0){
                sorted = 1;

                for (int i = split2; i <= index -2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
                for (int i = split2 + 1; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
            }

                printf("Sorted Array: ");
                print_array(nums, index);
                gettimeofday(&end, NULL);
                time = (((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
                printf("Time to Sort: %.1f microseconds\n", time);
                printf("Processes: %d\n", processes);
                munmap(nums, 13000 * sizeof(int));
                munmap(processes, 1 * sizeof(int));
                munmap(sorted, 1 * sizeof(int));
                }
            else if (pid2 > 0){
                //process 2
                while (sorted == 0){
                sorted = 1;

                for (int i = split1; i <= split2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
                for (int i = split1+1; i <= split2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
            }
            }
        }
        else if (pid > 0){
            //process 1
            while (sorted == 0){
                sorted = 1;

                for (int i = 0; i <= index -2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
                for (int i = 1; i <= index - 2; i = i + 2){
                    if (nums[i] > nums[i + 1]){
                        swap(&nums[i], &nums[i + 1]);
                        sorted = 0;
                    }
                }
            }
        }

    }

    else if (processes == 4){
        int split1 = 0;
        int split2 = 0;
        int split3 = 0;
        if (index % 4 == 0){
            split1 = index / 4;
            split2 = split1 * 2;
            split3 = split1 * 3;
        }
        else if (index % 4 == 1){
            split1 = (index + 3) / 4;
            split2 = split1 * 2;
            split3 = split1 * 3;
        }
        else if (index % 4 == 2){
            split1 = (index + 2) / 4;
            split2 = split1 * 2;
            split3 = split1 * 3;
        }
        else if (index % 4 == 3){
            split1 = (index + 3) / 4;
            split2 = split1 * 2;
            split3 = split1 * 3;
        }
        int pid = fork();
        if (pid == 0){
            int pid1 = fork();
            if (pid1 == 0){
                //process 4
                while (sorted == 0){
                    sorted = 1;

                    for (int i = split3; i <= index - 2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                    for (int i = split3 + 1; i <= index - 2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                }
                printf("Sorted Array: ");
                print_array(nums, index);
                gettimeofday(&end, NULL);
                time = (((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
                printf("Time to Sort: %.1f microseconds\n", time);
                printf("Processes: %d\n", processes);
                munmap(nums, 13000 * sizeof(int));
                munmap(processes, 1 * sizeof(int));
                munmap(sorted, 1 * sizeof(int));
            }
            else if (pid1 > 0){
                //process 3
                while (sorted == 0){
                    sorted = 1;

                    for (int i = split2; i <= split3; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                    for (int i = split2 + 1; i <= split3; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                }
            }
        }
        else if (pid > 0){
            int pid2 = fork();
            if (pid2 == 0){
                //process 2
                while (sorted == 0){
                    sorted = 1;

                    for (int i = split1; i <= split2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                    for (int i = split1 + 1; i <= split2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                }
            }
            else if (pid2 > 0){
                //process 1
                while (sorted == 0){
                    sorted = 1;

                    for (int i = 0; i <= index -2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                    for (int i = 1; i <= index - 2; i = i + 2){
                        if (nums[i] > nums[i + 1]){
                            swap(&nums[i], &nums[i + 1]);
                            sorted = 0;
                        }
                    }
                }
            }
        }
    }
    
    // Numbers are now stored in integer array
    // Create shared memory array of number values
    // Split shared array into smaller shared arrays (number of smaller shared arrays is number of processes)
    // Each sorts their values
    // Check time
    // Print results
}