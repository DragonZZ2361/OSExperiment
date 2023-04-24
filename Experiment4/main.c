#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Page{
    int page_number;
    int block_number;
    int counter;
    int time;
    int exist_flag;
};

struct Block{
    int page_number;
    int block_number;
    struct Block *next;
};

struct Instruction{
    int value;
    int page_number;
    int page_offset;
};

struct Page page_table[32];
struct Block block[32], *free_block_head, *busy_block_head, *busy_block_tail;
struct Instruction instruction[320];

void initialize(int total_block);
void FIFO(int total_block);
void LRU(int total_block);
void OPT(int total_block);
void LFU(int total_block);
void CLOCK(int total_block);

int main(){
    srand(10 * getpid());
    int random_point = (int)(319.0 * rand()/RAND_MAX) + 1;
    for(int i = 0 ; i < 320 ; i += 4){
        instruction[i].value = random_point;
        instruction[i+1].value = random_point + 1;
        instruction[i+2].value = (int)(1.0 * instruction[i].value * rand() / RAND_MAX);
        instruction[i+3].value = instruction[i+2].value + 1;
        random_point = (int)(1.0 * rand() * (318 - instruction[i+2].value)/RAND_MAX) + instruction[i+2].value + 2;
    }

    for(int i = 0 ; i < 320 ; i++){
        instruction[i].page_number = instruction[i].value / 10;
        instruction[i].page_offset = instruction[i].value % 10;
    }

    LFU(4);

    return 0;
}

void initialize(int total_block){
    for(int i = 0 ; i < 32 ; i++){
        page_table[i].page_number = i;
        page_table[i].block_number = -1;
        page_table[i].counter = 0;
        page_table[i].time = -1;
        page_table[i].exist_flag = 0;
    }
    for(int i = 1 ; i < total_block ; i++){
        block[i-1].block_number = i - 1;
        block[i-1].next = &block[i];
    }
    block[31].block_number = 31;
    block[31].next = NULL;
    free_block_head = &block[0];
}

void FIFO(int total_block){
    int diseffect = 0;
    initialize(total_block);
    busy_block_head = busy_block_tail = NULL;
    for(int i = 0 ; i < 320 ; i++){
        if(page_table[instruction[i].page_number].block_number == -1){
            diseffect++;
            if(free_block_head == NULL){
                struct Block *tmp;
                tmp = busy_block_head->next;
                free_block_head = busy_block_head;
                free_block_head->next = NULL;
                busy_block_head = tmp;
                page_table[free_block_head->page_number].block_number = -1;
            }
            struct Block *tmp;
            tmp = free_block_head->next;
            free_block_head->page_number = instruction[i].page_number;
            free_block_head->next = NULL;
            page_table[instruction[i].page_number].block_number = free_block_head->block_number;
            if(busy_block_tail == NULL){
                busy_block_head = busy_block_tail = free_block_head;
            }
            else{
                busy_block_tail->next = free_block_head;
                busy_block_tail = free_block_head;
            }
            free_block_head = tmp;
        }
    }
    printf("FIFO: %6.4lf\n", 1 - (double)diseffect/320);
}

void LRU(int total_block){
    int diseffect = 0;
    initialize(total_block);
    busy_block_head = busy_block_tail = NULL;
    for(int i = 0 ; i < 320 ; i++){
        if(page_table[instruction[i].page_number].block_number == -1){
            diseffect++;
            if(free_block_head == NULL){
                int min_time = 0x3f3f3f3f;
                for(int i = 0 ; i < 32 ; i++){
                    if(page_table[i].exist_flag == 0) continue;
                    min_time = min_time < page_table[i].time ? min_time : page_table[i].time;
                }
                struct Block *pre,*now;
                now = busy_block_head;
                pre = NULL;
                while(now != NULL){
                    if(page_table[now->page_number].time == min_time){
                        if(pre == NULL){
                            struct Block *tmp;
                            tmp = busy_block_head->next;
                            free_block_head = busy_block_head;
                            free_block_head->next = NULL;
                            busy_block_head = tmp;
                            page_table[free_block_head->page_number].block_number = -1;
                            page_table[free_block_head->page_number].exist_flag = 0;
                        }
                        else{
                            pre->next = now->next;
                            free_block_head = now;
                            free_block_head->next = NULL;
                            page_table[free_block_head->page_number].block_number = -1;
                            page_table[free_block_head->page_number].exist_flag = 0;
                        }
                    }
                    pre = now;
                    now = now->next;
                }
            }
            struct Block *tmp;
            tmp = free_block_head->next;
            free_block_head->page_number = instruction[i].page_number;
            free_block_head->next = NULL;
            page_table[instruction[i].page_number].block_number = free_block_head->block_number;
            page_table[instruction[i].page_number].time = i;
            page_table[instruction[i].page_number].exist_flag = 1;
            if(busy_block_tail == NULL){
                busy_block_head = busy_block_tail = free_block_head;
            }
            else{
                busy_block_tail->next = free_block_head;
                busy_block_tail = free_block_head;
            }
            free_block_head = tmp;
        }
        else{
            page_table[instruction[i].page_number].time = i;
        }
    }
    printf("LRU: %6.4lf\n",1 - (double)diseffect/320);
}

void OPT(int total_block){

}

void LFU(int total_block){
    int diseffect = 0;
    initialize(total_block);
    busy_block_head = busy_block_tail = NULL;
    for(int i = 0 ; i < 320 ; i++){
        if(page_table[instruction[i].page_number].block_number == -1){
            diseffect++;
            if(free_block_head == NULL){
                int min_counter = 0x3f3f3f3f;
                for(int i = 0 ; i < 32 ; i++){
                    if(page_table[i].exist_flag == 0) continue;
                    min_counter = min_counter < page_table[i].counter ? min_counter : page_table[i].counter;
                }
                struct Block *pre,*now;
                now = busy_block_head;
                pre = NULL;
                while(now != NULL){
                    if(page_table[now->page_number].counter == min_counter){
                        if(pre == NULL){
                            struct Block *tmp;
                            tmp = busy_block_head->next;
                            free_block_head = busy_block_head;
                            free_block_head->next = NULL;
                            busy_block_head = tmp;
                            page_table[free_block_head->page_number].block_number = -1;
                            page_table[free_block_head->page_number].exist_flag = 0;
                        }
                        else{
                            pre->next = now->next;
                            free_block_head = now;
                            free_block_head->next = NULL;
                            page_table[free_block_head->page_number].block_number = -1;
                            page_table[free_block_head->page_number].exist_flag = 0;
                        }
                    }
                    pre = now;
                    now = now->next;
                }
            }
            struct Block *tmp;
            tmp = free_block_head->next;
            free_block_head->page_number = instruction[i].page_number;
            free_block_head->next = NULL;
            page_table[instruction[i].page_number].block_number = free_block_head->block_number;
            page_table[instruction[i].page_number].counter++;
            page_table[instruction[i].page_number].exist_flag = 1;
            if(busy_block_tail == NULL){
                busy_block_head = busy_block_tail = free_block_head;
            }
            else{
                busy_block_tail->next = free_block_head;
                busy_block_tail = free_block_head;
            }
            free_block_head = tmp;
        }
        else{
            page_table[instruction[i].page_number].counter++;
        }
    }
    printf("LFU: %6.4lf\n",1 - (double)diseffect/320);
}

void CLOCK(int total_block){

}