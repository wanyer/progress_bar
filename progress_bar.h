// Copyright (C) 2013, Baidu Inc.
// Author: Xu Peilin (xupeilin@baidu.com)
//
// Description:

#ifndef  PROGRESS_BAR_H
#define  PROGRESS_BAR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace utils {
enum {
    // brief mode, only a progress bar and easy to use
    BRIEF,
    // add some enhanced config
    ENHANCED
};

class ProgressBar {
public:
    ProgressBar(char ch1 = '>',
                char ch2 = '-',
                uint32_t length = 80);
    ~ProgressBar();

    void Refresh(int32_t percent);

private:
    void FillFlushBuffer(int32_t percent);

private:
    char m_char_1;
    char m_char_2;
    char *m_flush_buffer;
    int32_t m_bar_length;
    int32_t m_percent;
};

} // namespace utils
#endif  // PROGRESS_BAR_H
