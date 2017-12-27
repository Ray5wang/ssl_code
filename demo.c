#include "dsplib/fft.h"
#define  FrameSize 4096
#define  MIC 2
#define RAW_BUFFER_SIZE (SAMPLES_PER_FRAME * NB_MICROPHONES)
#define SAMPLES_PER_FRAME 512
struct objFFT* myFFT;
struct ParametersStruct* parament;

struct mic_Array{
    float loc_x;
    float loc_y;
    float loc_z;
    float mic_signal[FrameSize];
    float mic_real[FrameSize];
    float mic_Imag[FrameSize];
};


float eps = 0;//zan shi shi 0
struct mic_Array mic[8];
struct mic_Array ss[8];
/* function  */
void showfft(float *real ,float *imag);
void caculate_gccphat(struct mic_Array  *mic);
void sprphat();

int main(int argc, char* argv[])
{
        
        short audio_raw_data[RAW_BUFFER_SIZE];
        float audio_float_data[NB_MICROPHONES][SAMPLES_PER_FRAME];
        const char* sepFileName = "separated*****.wav";
        const char* postFileName = "postfiltered*****.wav";

        mic[0].mic_signal[3]=1.4;
        mic[1].mic_signal[3]=1.4;
        myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
        fftInit(myFFT , FrameSize);
                fftComputeTwice(myFFT,  mic[0].mic_signal, mic[1].mic_signal
                                              , mic[0].mic_real,  mic[0].mic_Imag,
                                                   mic[1].mic_real,  mic[1].mic_Imag);
showfft(mic[1].mic_real ,mic[1].mic_Imag);
}




/* function  */

//*************************
/*cacullate the gcc-phat */
//************************
void caculate_gccphat(struct mic_Array *mic){
    int i,j;
    int first_data=0;
    float temp[FrameSize];
    printf("do fft ...");
    for(i=0;i<MIC/2;i++){                                               //do fft
        fftComputeTwice(myFFT,  mic[0].mic_signal, mic[1].mic_signal
                         , mic[0].mic_real,  mic[0].mic_Imag, 
                           mic[1].mic_real,  mic[1].mic_Imag);
    }
    //Calculate the cross-power spectrum: = fft(x1).*conj(fft(x2))
    for(i=0;i<MIC;i++){
        for(j=first_data+1;j<MIC;j++){
            point_multi(mic+first_data,mic+j,ss[j].mic_real,ss[j].mic_Imag); //  ss = MIC[first_data]*MIC[j];
            ifftComputeOnce(myFFT, ss[j].mic_real,ss[j].mic_Imag ,temp);

        }

        first_data++;
    }
}
//********************************
/*cacullate the point multi of */
//********************************
void point_multi(struct mic_Array data1,struct mic_Array data2,float *result_real,float *result_Imag,int N){
    int i;
    float phat;
    for(i=0;i<N;i++){
    result_real[i] =  data1.mic_real[i] * data2.mic_real[i] - data1.mic_Imag[i] * data2.mic_Imag[i];
    result_Imag[i] =  data1.mic_real[i] * data2.mic_Imag[i] + data1.mic_Imag[i] * data2.mic_real[i];
    phat = sqrt( result_real[i]* result_real[i]+result_Imag[i]*result_Imag[i] );
    result_real[i] = result_real[i]/phat;
    result_Imag[i] = result_Imag[i]/phat;
    }
}

//show the result of fft
void showfft(float *real ,float *imag){
    int i;
    for (i=0;i<FrameSize;i++){
        printf("%f+%fj \n",real[i],imag[i]);
    }
}

void Add_rawfile(){
    int channel;
    for (channel = 0; channel < NB_MICROPHONES; channel++){
        for (frame_index = 0; frame_index < SAMPLES_PER_FRAME; frame_index++)
        audio_float_data[channel][frame_index] = ((float) audio_raw_data[channel + (NB_MICROPHONES * frame_index)]) / 32768.0;
    }// Copy frames to the beamformer frames, will do 50% overlap internally
         preprocessorPushFrames(workspace.myPreprocessor, SAMPLES_PER_FRAME,channel);
         preprocessorAddFrame(workspace.myPreprocessor, &audio_float_data[channel][0], channel, SAMPLES_PER_FRAME);
}

//***********************
/*caculate the spr-phat*/
//***********************
//
void sprphat(){

}
