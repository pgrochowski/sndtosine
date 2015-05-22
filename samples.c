#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <fftw3.h>
#include <sndfile.h>
#include "samples.h"

struct sample_cont *get_sample_cont (SF_INFO *wfinfo)
{
	int i = 0;
	struct sample_cont *new_samples = malloc(sizeof(struct sample_cont));

	if (new_samples == NULL)
		return NULL;

	new_samples->channels = malloc(sizeof(double *) * wfinfo->channels);

	if (new_samples->channels == NULL)
		return NULL;

	/* allocate enough memory for every channel */
	for (i = 0; i < wfinfo->channels; i++) {
		/* fftw3 lib doc requests that their alloc is used for speed */
		new_samples->channels[i] = fftw_alloc_real(wfinfo->frames);
		if (new_samples->channels[i] == NULL)
			return NULL;
	}

	/* fftw3 lib doc requests that their alloc is used for speed */
	new_samples->fft_out = fftw_alloc_complex(wfinfo->frames / 2 + 1);

	if (new_samples->fft_out == NULL)
		return NULL;

	new_samples->ampl = malloc(sizeof(double) * wfinfo->frames / 2 + 1);
	new_samples->freq = malloc(sizeof(double) * wfinfo->frames / 2 + 1);

	if ((new_samples->ampl == NULL) || (new_samples->freq == NULL))
		return NULL;

	new_samples->num_channels = wfinfo->channels;
	new_samples->total_samples = (long long) wfinfo->frames;
	new_samples->samplerate = wfinfo->samplerate;

	/* set to null so when freeing memory later the system knows whether
	 * peaks were added.
	 */
	new_samples->peakll = NULL;
	return new_samples;
}

void free_sample_cont (struct sample_cont *samples)
{
	int i = 0;
	struct data_lln *next;
	struct data_lln *tmp;
	free(samples->ampl);
	free(samples->freq);
	fftw_free(samples->fft_out);

	for (i = 0; i < samples->num_channels; i++) {
		fftw_free(samples->channels[i]);
	}
	if (samples->peakll != NULL) {
		next = samples->peakll->next;
		while (next != NULL) {
			tmp = next->next;
			free(next);
			next = tmp;
		}
		free(samples->peakll);
	}
	free(samples->channels);
	free(samples);
	return;
}
