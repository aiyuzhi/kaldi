#include "decoder_export.h"

#include "base/timer.h"
#include "feat/wave-reader.h"

using namespace kaldi;
using namespace fst;

void * CreateDecoder(const char * cfgPath)
{

//#ifdef __DEBUG
    printf("create decoder, config path: %s.\n", cfgPath);
//#endif
    std::string cfg = cfgPath;
    OnlineFstDecoder *decoder = new OnlineFstDecoder(cfg);
    decoder->InitDecoder();
    return (void *)decoder;
}

int DecoderFeedData(void * lpDecoder, void *data, int nbytes, int state)
{
    OnlineFstDecoder *decoder = (OnlineFstDecoder *)lpDecoder;  
    if (state==FEAT_START){
        decoder->Reset();
    }
    int numOfSamples = nbytes / sizeof(short);
    float * audio = new float[numOfSamples];
    for (int i=0; i<numOfSamples; i++)
        audio[i] = ((short *)data)[i];
    decoder->FeedData(audio, numOfSamples * sizeof(float), state);
    delete [] audio;
}

int GetResult(void * lpDecoder, int * words_id, int state)
{
    OnlineFstDecoder * decoder = (OnlineFstDecoder *)lpDecoder;
    Result *result;
    result = decoder->GetResult(state);
    int idx = 0;
    for (auto word_id : result->word_ids_){
        words_id[idx++] = word_id;
    }
    return idx;
}

void ResetDecoder(void * lpDecoder)
{
    OnlineFstDecoder * decoder = (OnlineFstDecoder *)lpDecoder;
    decoder->Reset();
}

void DisposeDecoder(void ** lpDecoder)
{   
#ifdef __DEBUG
	printf("dispose Decoder instance.\n");
#endif
    if (*lpDecoder!=nullptr){
        delete (OnlineFstDecoder *)*lpDecoder;
	*lpDecoder = nullptr;
    }
}

// int main(){
//     return 0;
// }
