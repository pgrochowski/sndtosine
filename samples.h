/* samples.h
 * Sample container struct definition and functions related to it.
 */

#ifndef _samples_h_
#define _samples_h_

#include <sndfile.h>
#include <fftw3.h>

/* Sample container
 * Used to store samples from a file as doubles and keeps related metadata.
 */
struct sample_cont {
	int num_channels;
	int samplerate;
	long long total_samples;
	double min;
	double max;
	struct data_lln *peakll;
	double **channels;
	double *ampl;
	double *freq;
	fftw_complex *fft_out;
};

/* Linked list for storing amplitude peaks. */
struct data_lln {
        double freq;
        double ampl;
        void *next;
};

/* allocate memory for a sample_cont struct and all of its attached
 * structs/arrays
 */
struct sample_cont *get_sample_cont (SF_INFO *wfinfo);

/* free a sample_cont struct and its attached structs/arrays. */
void free_sample_cont (struct sample_cont *samples);

#endif
