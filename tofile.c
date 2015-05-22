#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fftw3.h>
#include <math.h>
#include "samples.h"
#include "filters.h"

int dump_raw_results_to_file (const char* filepath, struct sample_cont *samples)
{
	errno = 0;
	int i = 0;

	FILE *outf = fopen(filepath, "w");

	if (errno != 0) {
		printf("Couldn't open csv file.\n");
		return 1;
	}

	double freq_mult = (double)samples->samplerate / samples->total_samples;
	double freq;
	double ampl;

	for (i = 0; i < (samples->total_samples / 2 + 1); i++) {
		ampl = pow(samples->fft_out[i][0], 2.0);
		ampl = ampl + pow(samples->fft_out[i][1], 2.0);
		ampl = sqrt(ampl);
		freq = i * freq_mult;
		fprintf(outf, "%f, %f\n", freq, ampl);
	}

	fclose(outf);
	return 0;
}

int dump_fft_results_to_file (const char* filepath, struct sample_cont *samples)
{
	errno = 0;
	int i = 0;

	FILE *outf = fopen(filepath, "w");

	if (errno != 0) {
		printf("Couldn't open csv file.\n");
		return 1;
	}

	for (i = 0; i < (samples->total_samples / 2 + 1); i++)
		fprintf(outf, "%f, %f\n", samples->freq[i], samples->ampl[i]);

	return 0;
}

int dump_peaks_to_file (const char* filepath, struct sample_cont *samples)
{
	struct data_lln *next = samples->peakll;
	FILE *outf = fopen(filepath, "w");
	if (outf == NULL)
		return 1;

	fprintf(outf, "Frequency, Amplitude\n");

	while (next != NULL) {
		fprintf(outf, "%f, %f\n", next->freq, next->ampl);
		next = next->next;
	}

	fclose(outf);
	return 0;
}
