#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <curl/curl.h>

const int GAS_AMOUNT = 247368;

struct Memory{
  char *memory;
  size_t size;
};

static size_t write_func(void *data, size_t size, size_t nmemb, void *userData){
  size_t realSize = size * nmemb;
  struct Memory *memory = (struct Memory *)userData;

  char *ptr = (char*)realloc(memory->memory, memory->size + realSize + 1);

  if(ptr == NULL) return 0;
  

  memory->memory = ptr;
  memcpy(&memory->memory[memory->size], data, realSize);
  memory->size += realSize;
  memory->memory[memory->size] = 0;
  
  return realSize;
}

int main(void){
  CURL *curl;
  CURLcode res;

  struct Memory readBuffer;
  readBuffer.memory = NULL;
  readBuffer.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();
  if(curl){
    //Formula = GAS_AMOUNT * gas_price(fast) * 0.00038;
    curl_easy_setopt(curl, CURLOPT_URL, "https://ethgasstation.info/api/ethgasAPI.json?api-key=XXAPI_5066e2e6a0daf2d96488adb42823c0604e915ffc3ee9123fbe0066ba2cd7");

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    res = curl_easy_perform(curl);
    

    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(res));
    }
    printf("%s\n", readBuffer.memory);
    
    curl_easy_cleanup(curl);

    free(readBuffer.memory);
  }

  curl_global_cleanup();

  system("PAUSE");
  return 0;
}
