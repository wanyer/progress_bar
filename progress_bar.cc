//
// Author: Xu Peilin (xpeilin@gmail.com)
//

#include "progress_bar.h"

#include <sys/ioctl.h>

namespace utils {

ProgressBar::ProgressBar(DisplayMode mode,
                         uint64_t total_size,
                         uint32_t length,
                         std::string unit,
                         char ch1,
                         char ch2)
    : m_mode(mode),
      m_total_size(total_size),
      m_cur_size(0),
      m_bar_length(length),
      m_unit(unit),
      m_char_1(ch1),
      m_char_2(ch2),
      m_start_time(0) {
    if (m_bar_length <= 0) {
        if (m_mode == BRIEF) {
            m_bar_length = 80;
        } else {
            m_bar_length = GetScreenWidth() - 5;
        }
    }
    m_flush_buffer = new char[m_bar_length];
}

ProgressBar::~ProgressBar() {
    delete[] m_flush_buffer;
}

void ProgressBar::Refresh(int32_t cur_size) {
    if (cur_size > m_total_size) {
        cur_size = m_total_size;
    } else if (cur_size < 0) {
        cur_size = 0;
    }
    m_cur_size = cur_size;

    if (m_cur_time == time(NULL) && cur_size != m_total_size) {
        return;
    }
    m_cur_time = time(NULL);

    if (m_start_time == 0) {
        fflush(NULL);
        m_start_time = time(NULL);
        m_cur_time = m_start_time;
    }
    for (int32_t i = 0; i < m_bar_length; ++i) {
        putchar('\b');
    }
    if (m_mode == BRIEF) {
        FillFlushBufferBrief(cur_size);
    } else if (m_mode == ENHANCED) {
        FillFlushBufferEnhanced(cur_size);
    }
    fwrite(m_flush_buffer, 1, m_bar_length, stdout);
    fflush(stdout);
}

void ProgressBar::Add(int32_t size) {
    m_cur_size += size;
    Refresh(m_cur_size);
}

void ProgressBar::Done() {
    m_cur_size = 0;
    putchar('\n');
}

void ProgressBar::FillFlushBufferBrief(int64_t cur_size) {
    int32_t percent = cur_size * 100 / m_total_size;
    int32_t char_1_num = percent * (m_bar_length - 6) / 100;
    int32_t i = 0;
    while (i < char_1_num) {
        m_flush_buffer[i++] = m_char_1;
    }
    while (i < m_bar_length - 6) {
        m_flush_buffer[i++] = m_char_2;
    }
    snprintf(m_flush_buffer + i, 6,  " %3d%%", percent);
}

void ProgressBar::FillFlushBufferEnhanced(int64_t cur_size) {
    int32_t bar_remain = m_bar_length;
    int32_t disp_len;
    std::string unit;

    // fill time field
    int32_t time_s;
    time_s = GetTime();
    if (time_s == 0) {
        time_s = 1;
    }
    disp_len = 9;
    bar_remain -= disp_len;
    snprintf(m_flush_buffer + bar_remain, disp_len,  "%02d:%02d:%02d ",
             time_s / 3600,
             time_s % 3600 / 60,
             time_s % 60);
    m_flush_buffer[bar_remain + disp_len - 1] = ' ';

    // fill speed field
    double disp_size = (double)cur_size / time_s;
    if (disp_size >= 1024 * 1024) {
        unit = "M" + m_unit;
        disp_size = disp_size / (1024 * 1024);
    } else if (disp_size >= 1024) {
        unit = "K" + m_unit;
        disp_size = disp_size / 1024;
    } else {
        unit = m_unit;
    }
    disp_len = 9 + unit.length();
    bar_remain -= disp_len;
    snprintf(m_flush_buffer + bar_remain, 6, "%5.3f", disp_size);
    snprintf(m_flush_buffer + bar_remain + 5, disp_len - 5, "%s/s  ", unit.c_str());
    m_flush_buffer[bar_remain + disp_len - 1] = ' ';

    // fill cur_size field
    if (cur_size >= 1024 * 1024 * 1024) {
        unit = "G" + m_unit;
        disp_size = cur_size / (1024.0 * 1024 * 1024);
    } else if (cur_size >= 1024 * 1024) {
        unit = "M" + m_unit;
        disp_size = cur_size / (1024.0 * 1024);
    } else if (cur_size >= 1024) {
        unit = "K" + m_unit;
        disp_size = cur_size / 1024.0;
    } else {
        unit = m_unit;
        disp_size = cur_size;
    }
    disp_len = 7 + unit.length();
    bar_remain -= disp_len;
    snprintf(m_flush_buffer + bar_remain, 6,  "%5.3f",  disp_size);
    snprintf(m_flush_buffer + bar_remain + 5, disp_len - 5, "%s  ", unit.c_str());
    m_flush_buffer[bar_remain + disp_len - 1] = ' ';

    // fill percent field
    disp_len = 7;
    bar_remain -= disp_len;
    int32_t percent = cur_size * 100 / m_total_size;
    snprintf(m_flush_buffer + bar_remain, disp_len,  " %3d%%  ", percent);
    m_flush_buffer[bar_remain + disp_len - 1] = ' ';

    // file process bar
    int32_t char_1_num = percent * bar_remain / 100;
    int32_t i = 0;
    while (i < char_1_num) {
        m_flush_buffer[i++] = m_char_1;
    }
    while (i < bar_remain) {
        m_flush_buffer[i++] = m_char_2;
    }
}

int32_t ProgressBar::GetScreenWidth() {
    int32_t fd;
    struct winsize wsz;

    fd = fileno(stderr);
    if (ioctl(fd, TIOCGWINSZ, &wsz) < 0) {
        perror("fail to get screen width");
        return -1;
    }

    return wsz.ws_col;
}

int32_t ProgressBar::GetTime() {
    return (int32_t)(time(0) - m_start_time);
}
} // namespace utils
