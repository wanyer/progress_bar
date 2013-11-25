#include "progress_bar.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    utils::ProgressBar progress_bar;
    for (int i = 0; i <= 100; ++i) {
        progress_bar.Refresh(i);
        sleep(1);
    }
    return 0;
}
