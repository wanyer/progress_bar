//
// Author: Xu Peilin (xpeilin@gmail.com)
//

#ifndef  PROGRESS_BAR_H
#define  PROGRESS_BAR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <string>

namespace utils {
enum DisplayMode {
    // brief mode, only a progress bar and easy to use
    // >>>>>>>>>>>------------------------------- 28%
    BRIEF,

    // add some enhanced config
    // >>>>>>>>>-------------------------- 28% 30M 20KB/s 00:00:05
    ENHANCED
};

class ProgressBar {
public:
    ProgressBar(DisplayMode mode = BRIEF,
                uint64_t total_size = 100,
                uint32_t length = 0,
                std::string unit = "B",
                char ch1 = '>',
                char ch2 = '-');
    ~ProgressBar();

    void Refresh(int32_t cur_size);
    void Add(int32_t size);
    void Done();

    int32_t GetPercent() {
        return (int32_t)(m_cur_size * 100 / m_total_size);
    }

private:
    void FillFlushBufferBrief(int64_t cur_size);
    void FillFlushBufferEnhanced(int64_t cur_size);
    int32_t GetScreenWidth();
    int32_t GetTime();

private:
    DisplayMode m_mode;
    int64_t m_total_size;
    int64_t m_cur_size;
    int32_t m_bar_length;
    std::string m_unit;
    char m_char_1;
    char m_char_2;

    time_t m_start_time;
    time_t m_cur_time;
    char *m_flush_buffer;
};

} // namespace utils
#endif  // PROGRESS_BAR_H
