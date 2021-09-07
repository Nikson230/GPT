#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <curl/curl.h>

const int GAS_AMOUNT = 2600;
const float gwei_price = 0.00039;

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
    //curl_easy_setopt(curl, CURLOPT_URL, "https://ethgasstation.info/api/ethgasAPI.json?api-key=XXAPI_5066e2e6a0daf2d96488adb42823c0604e915ffc3ee9123fbe0066ba2cd7");
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.etherscan.io/api?module=gastracker&action=gasoracle&apikey=GAH5FN7VVG2365XEMCP5DUHG62BZF54D99");

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

    int index;

    for(int i = 0; i < readBuffer.size; i++){
      if(readBuffer.memory[i] == 'F'){
	index = i;
	break;
      }
    }

    index += 15;

    std::vector<char> charBuf;
    for(int j = index; j < readBuffer.size; j++){
      if(readBuffer.memory[j] != '"'){
	charBuf.push_back(readBuffer.memory[j]);
      } else {
	break;
      }
    }

    std::string fastPrice(charBuf.begin(), charBuf.end());
    std::cout << fastPrice << std::endl;
    int price = std::stoi(fastPrice) * GAS_AMOUNT * gwei_price;
    std::cout << price << "$" << std::endl;

    std::ofstream out("result.csv", std::ios::app);

    auto bn = std::chrono::system_clock::now();

    std::time_t time = std::chrono::system_clock::to_time_t(bn);
    
    out << std::ctime(&time) << "," << price << std::endl;

    out.close();
    
    curl_easy_cleanup(curl);

    free(readBuffer.memory);
  }

  curl_global_cleanup();

  system("PAUSE");
  return 0;
}
