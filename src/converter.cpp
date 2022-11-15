#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#define RUN(CMD) if(!CMD) { \
                    printf("|\n| [%d] Error: %s\n|\n", __LINE__, SDL_GetError()); \
                    return; \
                 }

#define SAMPLE_PATH "./sample/sample.wav"
#define SECONDS 10

#define PI 3.14

struct AudioData{
    Uint8* pos;
    Uint32 length;
} data;

SDL_AudioSpec spec;

int phase = 0;

static void callbackFunc(void* userData, Uint8* stream, int streamLength){
    AudioData* data = (AudioData*) userData;

    if(data->length == 0)
        return;

    Uint32 length = (Uint32) streamLength;
    Sint16* samples = (Sint16*) data->pos;

    length = (length > data->length) ? data->length : length;

    for(int i=1; i<length/2; i+=2){
        short* s_left = samples+i-1;
        short* s_right = samples+i;

        phase = (phase+2)%(spec.freq*SECONDS);
        
        double angle = (phase*1.0/(spec.freq*SECONDS))*2*PI;

        *s_left = (short) (*s_left * (1.0-cos(angle))/2.0);
        *s_right = (short) (*s_right * (1.0+cos(angle))/2.0);
    }

    SDL_memcpy(stream, data->pos, length);

    data->pos += length;
    data->length -= length;
}

void PlayFile(const char* filePath){
    SDL_Init(SDL_INIT_AUDIO);

    RUN( SDL_LoadWAV(filePath, &spec, &data.pos, &data.length) )

    spec.callback = callbackFunc;
    spec.userdata = &data;

    SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(deviceID == 0){
        std::cerr << "Error loading a device...";
        return;
    }

    SDL_PauseAudioDevice(deviceID, 0);

    while (data.length > 0)
    {
        SDL_Delay(100);
    }
    
    SDL_CloseAudioDevice(deviceID);

    SDL_FreeWAV(data.pos);
    SDL_Quit();
}

int main(int argc, char** argv){
    std::cout << "Convert a WAV file to 8D \n";
    
    PlayFile(SAMPLE_PATH);

    return 0;
}