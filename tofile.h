/* tofile.h
 * Contains all file output functions.
 *
 * Some of these may be unused currently.
 */

#ifndef _tofile_h_
#define _tofile_h_

/* Dump raw data to a csv file
 *
 * This recalculates frequency and amplitude in the event the program has
 * already run some filtering on calculated values. Previously filtered values
 * will be erased so if you need both save this for last.
 */
int dump_raw_results_to_file (const char* filepath, struct sample_cont *samples);

/* Dump frequency/amplitude pairs out to csv
 *
 * Frequency and amplitude pairs need to be calculated already, otherwise this
 * will dump whatever is in the memory at the moment.
 */
int dump_fft_results_to_file (const char* filepath, struct sample_cont *samples);

/* Dump amplitude peaks to csv
 *
 * This only dumps frequency/amplitude pairs where the previous and next value
 * in the array is lower than the current value.
 */
int dump_peaks_to_file (const char* filepath, struct sample_cont *samples);

#endif
