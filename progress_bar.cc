// Copyright (C) 2013, Baidu Inc.
// Author: Xu Peilin (xupeilin@baidu.com)
//

#include "progress_bar.h"

namespace utils {

ProgressBar::ProgressBar(char ch1, char ch2, uint32_t length)
    : m_char_1(ch1),
      m_char_2(ch2),
      m_bar_length(length),
      m_percent(0) {
    m_flush_buffer = new char[m_bar_length];
}

ProgressBar::~ProgressBar() {
    if (m_percent != 100)
        putchar('\n');
    delete[] m_flush_buffer;
}

void ProgressBar::Refresh(int32_t percent) {
    if (percent > 100 || percent < 0 || percent == m_percent) {
        fflush(NULL);
        return;
    }
    m_percent = percent;
    for (int32_t i = 0; i < m_bar_length; ++i) {
        putchar('\b');
    }
    FillFlushBuffer(percent);
    for (int32_t i = 0; i < m_bar_length; ++i) {
        putchar(m_flush_buffer[i]);
    }
    fflush(stdout);
    if (percent == 100) {
        putchar('\n');
    }
}

void ProgressBar::FillFlushBuffer(int32_t percent) {
    int char_1_num = percent * (m_bar_length - 6) / 100;
    int i = 0;
    while (i < char_1_num) {
        m_flush_buffer[i++] = m_char_1;
    }
    while (i < m_bar_length - 6) {
        m_flush_buffer[i++] = m_char_2;
    }
    snprintf(m_flush_buffer + i, 6,  " %3d%%", percent);
}

} // namespace utils
