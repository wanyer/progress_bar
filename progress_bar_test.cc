//
// Author: Xu Peilin (xpeilin@gmail.com)
//

#include "progress_bar.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    utils::ProgressBar progress_bar(utils::ENHANCED, 10000000);
    srand((uint32_t)time(NULL));
    int cur_size = 0;
    for (int i = 0; i <= 100; ++i) {
        progress_bar.Refresh(cur_size);
        cur_size += rand() % 1000000;
        sleep(1);
    }
    return 0;
}
