#include <iostream>
#include <iomanip> 
#include <cmath>
#include <fstream>
#include <cstdlib>

#include <SDL2/SDL.h>

#define RUN(CMD) if(!CMD) { \
                    printf("|\n| [%d] Error: %s\n|\n", __LINE__, SDL_GetError()); \
                    return; \
                 }

#define PI 3.14

struct AudioData{
    Uint8* pos;
    Uint32 length;
} data;

SDL_AudioSpec spec;

int phase = 0;

char* filepath = "./sample/sample.wav";
char* destination = "./sample/converted.wav";
double duration = 10.0;

bool play = false;
bool convert = false;

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

        phase = (phase+2)%(int)(spec.freq*duration);
        
        double angle = (phase*1.0/(spec.freq*duration))*2*PI;

        *s_left = (short) (*s_left * (1.0-cos(angle))/2.0);
        *s_right = (short) (*s_right * (1.0+cos(angle))/2.0);
    }

    SDL_memcpy(stream, data->pos, length);

    data->pos += length;
    data->length -= length;
}

void InitSource(){
    SDL_Init(SDL_INIT_AUDIO);

    RUN( SDL_LoadWAV(filepath, &spec, &data.pos, &data.length) )

    spec.callback = callbackFunc;
    spec.userdata = &data;
}

void PlayFile(){

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

void ConvertFile(){
    std::ifstream src(filepath, std::ios::in | std::ios::binary | std::ios::ate);
    if(!src.is_open())
        return;
    
    std::ofstream out(destination, std::ios::out | std::ios::binary | std::ios::app);
    if(!out.is_open())
        return;

    std::streampos file_size = src.tellg();
    
    src.seekg(0);

    char* header = (char*) calloc(44, sizeof(char));
    src.read(header, 44);
    out.write(header, 44);
    
    int data_size = file_size - 44;

    // Convert data frame by frame
    Sint16* frame = (Sint16*) calloc(2, sizeof(Sint16));

    for(int i=1; i<data_size/2; i+=2){
        src.read((char*)frame, 4);

        short* s_left = frame;
        short* s_right = frame+1;

        phase = (phase+2)%(int)(spec.freq*duration);
        
        double angle = (phase*1.0/(spec.freq*duration))*2*PI;

        *s_left = (short) (*s_left * (1.0-cos(angle))/2.0);
        *s_right = (short) (*s_right * (1.0+cos(angle))/2.0);

        out.write((char*) frame, 4);
    }

    src.close();
    out.close();
}

int main(int argc, char** argv){
    for(int i=1; i<argc; ++i){
        if(strcmp(argv[i], "--play") == 0){
            play = true;
        }
        if(strcmp(argv[i], "--convert") == 0){
            convert = true;
        }
        if(strcmp(argv[i], "--file") == 0){
            filepath = argv[++i];
        }
        if(strcmp(argv[i], "--out") == 0){
            destination = argv[++i];
        }
        if(strcmp(argv[i], "--duration") == 0){
            sscanf(argv[++i], "%lf", &duration);
        }
    }

    InitSource();

    if(play){
        std::cout << ">>======================================= \n";
        std::cout << "Playing:"<<std::endl;
        std::cout << "\t - file: " << filepath << std::endl;
        std::cout << "\t - duration: " << duration << "s" << std::endl;
        std::cout << ">>======================================= \n";
        PlayFile();
    }
    if(convert){
        std::cout << ">>======================================= \n";
        std::cout << "Converting:"<<std::endl;
        std::cout << "\t - from: " << filepath << std::endl;
        std::cout << "\t - to: " << destination << std::endl;
        std::cout << "\t - duration: " << duration << "s" << std::endl;
        std::cout << ">>======================================= \n";
        ConvertFile();
    }
    
    

    return 0;
}