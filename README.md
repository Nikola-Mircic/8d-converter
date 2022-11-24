# 8D converter
Convert or play any song in 8D.

### Supported formats:
- WAV ( 16-bit, Stereo only )

### How to use:
  - #### Linux:
    To **play** a song with 8D effect, go to the project directory and type: <br>
    ` ./build/converter --play --file <filepath> -duration <duration> `
    
    Arguments [ Optional ] :
      - `--file <filepath>` -  Path to a file that you want to play. Default value is **"./sample/sample.wav"**.
      - `--duration <duration>` -  Duration of the transition in seconds. Default value is **10**.
     
    Example:
    
    ```
    $  ./build/converter --convert --file ./sample/sample.wav --out ./sample/converted.wav --duration 5
       >>======================================= 
       Playing:
            - file: ./sample/sample.wav
            - duration: 5s
       >>======================================= 
    ```
    
   
    To **convert** a song to 8D audio file, go to the project directory and type: <br>
    ` ./build/converter --convert --file <filepath> --out <outputfile> -duration <duration> `
    
    Arguments [ Optional ] :
      - `--file <filepath>` -  Path to a file that you want to convert. Default value is **"./sample/sample.wav"**.
      - `--out <outputfile>` - Path to a file where converted audio will be stored. Default value is **"./sample/converted.wav"**.
      - `--duration <duration>` -  Duration of the transition in seconds. Default value is **10**.
    
    Example:
    
    ```
    $  ./build/converter --convert --file ./sample/sample.wav --out ./sample/converted.wav --duration 5
       >>======================================= 
       Converting:
            - from: ./sample/sample.wav
            - to: ./sample/converted.wav
            - duration: 5s
       >>======================================= 
    ```
    
   
