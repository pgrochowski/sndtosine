#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <sndfile.h>
#include "samples.h"

int fft_out_to_freq_ampl (struct sample_cont *samples)
{
	int i;
	samples->max = 0.0;

	/* min value needs to be an actual sample */
	samples->min = sqrt(pow(samples->fft_out[0][0], 2.0)
			+ pow(samples->fft_out[0][1], 2.0));

	double freq_mult = (double)samples->samplerate / samples->total_samples;
	for (i = 0; i < (samples->total_samples / 2 + 1); i++) {
		samples->ampl[i] = pow(samples->fft_out[i][0], 2.0);
		samples->ampl[i] = samples->ampl[i] + pow(samples->fft_out[i][1], 2.0);
		samples->ampl[i] = sqrt(samples->ampl[i]);
		samples->freq[i] = i * freq_mult;
		if (samples->ampl[i] > samples->max)
			samples->max = samples->ampl[i];
		if (samples->ampl[i] < samples->min)
			samples->min = samples->ampl[i];
	}

	return 0;
}

int noise_gate_on_ampl (struct sample_cont *samples, double percent)
{
	int i;
	double limit = samples->max * percent;

	for (i = 0; i < (samples->total_samples / 2 + 1); i++) {
		if (samples->ampl[i] < limit)
			samples->ampl[i] = 0;
	}

	return 0;
}

int get_ampl_peaks (struct sample_cont *samples)
{
	int i;
	int num_peaks = 0;
	struct data_lln *first = NULL;
	struct data_lln *node = NULL;
	for (i = 1; i < (samples->total_samples / 2); i++) {
		if ((samples->ampl[i] > samples->ampl[i-1]) &
		    (samples->ampl[i] > samples->ampl[i+1])) {
			if (node == NULL) {
				/*
				 * First time through, save the pointer so we
				 * can attach it to samples
				 */
				first = malloc(sizeof(struct data_lln));
				if (first == NULL)
					return 1;
				node = first;
			} else {
				/*
				 * Not the first time, point current node to
				 * new node, and set the new node's next to null
				 */
				node->next = malloc(sizeof(struct data_lln));
				if (node->next == NULL)
					return 1;
				node = node->next;
				node->next = NULL;
			}
			node->ampl = samples->ampl[i];
			node->freq = samples->freq[i];
			num_peaks++;
		}
	}
	samples->peakll = first;
	return 0;
}
