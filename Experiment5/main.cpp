#include <iostream>
#include <algorithm>
#include <queue>

const int N = 100 + 10;

struct Process{
    long id;
    double arrive_time;
    double serve_time;
    double finish_time;
    double round_time;
    double weighted_time;
}process[N];

struct Result{
    double average_time, weighted_average_time;
}result_FCFS, result_SPN, result_RR;

struct Result FCFS(int n){
    std::sort(process, process + n, [&](Process a, Process b){return a.arrive_time < b.arrive_time;});
    double sum_round_time = 0, sum_weighted_time = 0;
    for(int i = 0 ; i < n ; i++){
        if(i == 0){
            process[i].finish_time = process[i].arrive_time + process[i].serve_time;
            process[i].round_time = process[i].finish_time - process[i].arrive_time;
            process[i].weighted_time = process[i].round_time / process[i].serve_time;
        }
        else{
            if(process[i].arrive_time < process[i-1].finish_time){
                process[i].finish_time = process[i-1].finish_time + process[i].serve_time;
                process[i].round_time = process[i].finish_time - process[i].arrive_time;
                process[i].weighted_time = process[i].round_time / process[i].serve_time;
            }
            else{
                process[i].finish_time = process[i].arrive_time + process[i].serve_time;
                process[i].round_time = process[i].finish_time - process[i].arrive_time;
                process[i].weighted_time = process[i].round_time / process[i].serve_time;
            }
        }
        sum_round_time += process[i].round_time;
        sum_weighted_time += process[i].weighted_time;
    }
    printf("--------------------------------------------FCFS---------------------------------------------\n");
    printf("ProcessID\tArrvieTime\tServeTime\tFinishTime\tRoundTime\tWeightedTime\n");
    for(int i = 0 ; i < n ; i++){
        printf("%ld\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\n", process[i].id, process[i].arrive_time, process[i].serve_time, process[i].finish_time, process[i].round_time, process[i].weighted_time);
    }
    printf("\nAverageRoundTime = %.2lf\t\tAverageWeightedTime = %.2lf\n", sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n);
    printf("---------------------------------------------------------------------------------------------\n\n");
    return {sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n};
}

struct Result SPN(int n){
    std::sort(process, process + n, [&](Process a, Process b){return a.arrive_time < b.arrive_time;});
    double sum_round_time = 0, sum_weighted_time = 0;
    for(int i = 0 ; i < n ; i++){
        if(i == 0){
            process[i].finish_time = process[i].arrive_time + process[i].serve_time;
            process[i].round_time = process[i].finish_time - process[i].arrive_time;
            process[i].weighted_time = process[i].round_time / process[i].serve_time;
        }
        else{
            for(int j = i ; j < n ; j++){
                if(process[j].arrive_time <= process[i-1].finish_time){
                    if(process[j].serve_time < process[i].serve_time){
                        std::swap(process[i], process[j]);
                    }
                }
                else{
                    break;
                }
            }
            if(process[i].arrive_time < process[i-1].finish_time){
                process[i].finish_time = process[i-1].finish_time + process[i].serve_time;
                process[i].round_time = process[i].finish_time - process[i].arrive_time;
                process[i].weighted_time = process[i].round_time / process[i].serve_time;
            }
            else{
                process[i].finish_time = process[i].arrive_time + process[i].serve_time;
                process[i].round_time = process[i].finish_time - process[i].arrive_time;
                process[i].weighted_time = process[i].round_time / process[i].serve_time;
            }
        }
        sum_round_time += process[i].round_time;
        sum_weighted_time += process[i].weighted_time;
    }
    printf("---------------------------------------------SPN---------------------------------------------\n");
    printf("ProcessID\tArrvieTime\tServeTime\tFinishTime\tRoundTime\tWeightedTime\n");
    for(int i = 0 ; i < n ; i++){
        printf("%ld\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\n", process[i].id, process[i].arrive_time, process[i].serve_time, process[i].finish_time, process[i].round_time, process[i].weighted_time);
    }
    printf("\nAverageRoundTime = %.2lf\t\tAverageWeightedTime = %.2lf\n", sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n);
    printf("---------------------------------------------------------------------------------------------\n\n");
    return {sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n};
}

struct Result RR(int n, double slice_time){
    std::sort(process, process + n, [&](Process a, Process b){return a.arrive_time < b.arrive_time;});
    int unused_process_position = 0;
    double present_time = 0;
    double sum_round_time = 0, sum_weighted_time = 0;
    std::queue<std::pair<int, double>> queue;  // First element means Process ID, Second element means Rest Time for each process.
    while(true){
        if(!queue.empty()){
            auto tmp = queue.front();
            queue.pop();
            if(tmp.second <= slice_time){
                present_time += tmp.second;
                process[tmp.first].finish_time = present_time;
                process[tmp.first].round_time = process[tmp.first].finish_time - process[tmp.first].arrive_time;
                process[tmp.first].weighted_time = process[tmp.first].round_time / process[tmp.first].serve_time;
                sum_round_time += process[tmp.first].round_time;
                sum_weighted_time += process[tmp.first].weighted_time;
                for(int i = unused_process_position; i < n ; i++){
                    if(process[i].arrive_time <= present_time){
                        queue.push({i, process[i].serve_time});
                        if(i == n - 1){
                            unused_process_position = n;
                        }
                    }
                    else{
                        unused_process_position = i;
                        break;
                    }
                }
            }
            else{  //tmp.second(rest time for process) > slice_time, need another slice to work.
                present_time += slice_time;
                for(int i = unused_process_position; i < n ; i++){
                    if(process[i].arrive_time <= present_time){
                        queue.push({i, process[i].serve_time});
                        if(i == n - 1){
                            unused_process_position = n;
                        }
                    }
                    else{
                        unused_process_position = i;
                        break;
                    }
                }
                queue.push({tmp.first, tmp.second - slice_time});
            }
        }
        else{
            if(unused_process_position == n){
                break;
            }
            else{
                present_time = process[unused_process_position].arrive_time;
                queue.push({unused_process_position, process[unused_process_position].serve_time});
                unused_process_position++;
            }
        }
    }
    printf("---------------------------------------------RR----------------------------------------------\n");
    printf("ProcessID\tArrvieTime\tServeTime\tFinishTime\tRoundTime\tWeightedTime\n");
    for(int i = 0 ; i < n ; i++){
        printf("%ld\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%.1lf\n", process[i].id, process[i].arrive_time, process[i].serve_time, process[i].finish_time, process[i].round_time, process[i].weighted_time);
    }
    printf("\nAverageRoundTime = %.2lf\t\tAverageWeightedTime = %.2lf\n", sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n);
    printf("---------------------------------------------------------------------------------------------\n\n");
    return {sum_round_time * 1.0 / n, sum_weighted_time * 1.0 / n};
}

int main(){
    int n;
    int slice_time;
    std::cout << "The Number Of Process = ";
    std::cin >> n;
    if(n > N){
        std::cout << "Too Many Processes!\n";
        return 0;
    }
    std::cout << "Slice Time = ";
    std::cin >> slice_time;
    std::cout << std::endl;
    for(int i = 0 ; i < n ; i++){
        std::cout << "Process ID = " << i << std::endl;
        process[i].id = i;
        std::cout << "Process Arrive Time = ";
        std::cin >> process[i].arrive_time;
        std::cout << "Process Serve Time = ";
        std::cin >> process[i].serve_time;
        std::cout << std::endl;
    }

    printf("-----------------Input--------------------\n");
    printf("ProcessID\tArrvieTime\tServeTime\n");
    for(int i = 0 ; i < n ; i++){
        printf("%ld\t\t%.1lf\t\t%.1lf\n", process[i].id, process[i].arrive_time, process[i].serve_time);
    }
    printf("------------------------------------------\n\n");

    result_FCFS = FCFS(n);
    result_SPN = SPN(n);
    result_RR = RR(n, slice_time);

    std::cout << "FCFS : " << "Average Time : " << result_FCFS.average_time << "\t Weighted Average Time : " << result_FCFS.weighted_average_time << std::endl;
    std::cout << "SPN : " << " Average Time : " << result_SPN.average_time << "\t Weighted Average Time : " << result_SPN.weighted_average_time << std::endl;
    std::cout << "RR : " << "  Average Time : " << result_RR.average_time << "\t Weighted Average Time : " << result_RR.weighted_average_time << std::endl;

    return 0;
}