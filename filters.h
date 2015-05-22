/* filters.h
 * All functions related to filtering data pulled from sound files
 */

#ifndef _filters_h_
#define _filters_h_

#include <stdio.h>
#include "samples.h"

/* Convert fft out to frequency/amplitude data points */
int fft_out_to_freq_ampl (struct sample_cont *samples);

/* Using the maximum amplitude of the data set, set any amplitudes below the
 * provided percentage to 0.
 *
 */
int noise_gate_on_ampl (struct sample_cont *samples, double percent);

/* Find peaks, put them into a linked list, and attach them to samples
 *
 *
 */
int get_ampl_peaks (struct sample_cont *samples);

#endif
