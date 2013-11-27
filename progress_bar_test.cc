//
// Author: Xu Peilin (xpeilin@gmail.com)
//

#include "progress_bar.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int cur_size = 0;
    int total_size = 100000000;

    utils::ProgressBar progress_bar(utils::ENHANCED, total_size);

    srand((uint32_t)time(NULL));
    timespec interval = {0, 1000};
    while (cur_size < total_size) {
        cur_size += rand() % 1000;
        progress_bar.Refresh(cur_size);
        nanosleep(&interval, &interval);
    }
    progress_bar.Done();
    return 0;
}
