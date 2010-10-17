// File: lzham_timer.cpp
//
// Copyright (c) 2009-2010 Richard Geldreich, Jr. <richgel99@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include "lzham_core.h"
#include "lzham_timer.h"

namespace lzham
{
   unsigned long long lzham_timer::g_init_ticks;
   unsigned long long lzham_timer::g_freq;
   double lzham_timer::g_inv_freq;

   lzham_timer::lzham_timer() :
      m_start_time(0),
      m_stop_time(0),
      m_started(false),
      m_stopped(false)
   {
      if (!g_inv_freq) 
         init();
   }

   lzham_timer::lzham_timer(timer_ticks start_ticks)
   {
      if (!g_inv_freq) 
         init();
      
      m_start_time = start_ticks;
      
      m_started = true;
      m_stopped = false;
   }

   void lzham_timer::start(timer_ticks start_ticks)
   {
      m_start_time = start_ticks;
      
      m_started = true;
      m_stopped = false;
   }

   void lzham_timer::start()
   {
      QueryPerformanceCounter((LARGE_INTEGER*)&m_start_time);
      
      m_started = true;
      m_stopped = false;
   }

   void lzham_timer::stop()
   {
      LZHAM_ASSERT(m_started);
                  
      QueryPerformanceCounter((LARGE_INTEGER*)&m_stop_time);
      
      m_stopped = true;
   }

   double lzham_timer::get_elapsed_secs() const
   {
      LZHAM_ASSERT(m_started);
      if (!m_started)
         return 0;
      
      timer_ticks stop_time = m_stop_time;
      if (!m_stopped)
         QueryPerformanceCounter((LARGE_INTEGER*)&stop_time);
         
      timer_ticks delta = stop_time - m_start_time;
      return delta * g_inv_freq;
   }

   timer_ticks lzham_timer::get_elapsed_us() const
   {
      LZHAM_ASSERT(m_started);
      if (!m_started)
         return 0;
         
      timer_ticks stop_time = m_stop_time;
      if (!m_stopped)
         QueryPerformanceCounter((LARGE_INTEGER*)&stop_time);
      
      timer_ticks delta = stop_time - m_start_time;
      return (delta * 1000000ULL + (g_freq >> 1U)) / g_freq;      
   }

   void lzham_timer::init()
   {
      if (!g_inv_freq)
      {
         QueryPerformanceFrequency((LARGE_INTEGER*)&g_freq);
         g_inv_freq = 1.0f / g_freq;
         
         QueryPerformanceCounter((LARGE_INTEGER*)&g_init_ticks);
      }
   }

   timer_ticks lzham_timer::get_init_ticks()
   {
      if (!g_inv_freq) 
         init();
      
      return g_init_ticks;
   }

   timer_ticks lzham_timer::get_ticks()
   {
      if (!g_inv_freq) 
         init();
      
      timer_ticks ticks;
      QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
      return ticks - g_init_ticks;
   }

   double lzham_timer::ticks_to_secs(timer_ticks ticks)
   {
      if (!g_inv_freq) 
         init();
      
      return ticks * g_inv_freq;
   }
} // namespace lzham