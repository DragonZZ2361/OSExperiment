#include <iostream>
#include <algorithm>
#include <queue>
#include <matplotlibcpp.h>

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
    return {sum_round_time * 1.0, sum_weighted_time * 1.0 / n};
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
    return {sum_round_time * 1.0, sum_weighted_time * 1.0 / n};
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
    return {sum_round_time * 1.0, sum_weighted_time * 1.0 / n};
}

int main(){
    srand((unsigned int) time(NULL));
    int n;
    int slice_time;

    std::vector<double> x(17), y(17), z1(17), z2(17), z3(17), t(17);

    for(int i = 4 ; i < 21 ; i++){
        std::cout << "Total Process Number = " << i << std::endl;
        t.at(i - 4) = i;
        if(i == 4){
            for(int j = 0 ; j < i ; j++){
                process[j].id = j;
                process[j].arrive_time = rand() % 50 + 1;
                process[j].serve_time = rand() % 15 + 1;
            }
        }
        else{
            process[i-1].id = i - 1;
            process[i-1].arrive_time = rand() % 50 + 1;
            process[i-1].serve_time = rand() % 15 + 1;
        }

        x.at(i - 4) = FCFS(i).average_time;
        y.at(i - 4) = SPN(i).average_time;
        z1.at(i - 4) = RR(i, 1.0).average_time;
        z2.at(i - 4) = RR(i, 4.0).average_time;
        z3.at(i - 4) = RR(i, 9.0).average_time;
    }

    namespace plt = matplotlibcpp;
    plt::figure_size(1200, 780);
    plt::named_plot("FCFS", t, x);
    plt::named_plot("SPN", t, y);
    plt::named_plot("RR(1)", t, z1);
    plt::named_plot("RR(4)", t, z2);
    plt::named_plot("RR(9)", t, z3);
    plt::title("Experimet 5");
    plt::legend();
    plt::save("./figure.png");
    plt::show();

    return 0;
}